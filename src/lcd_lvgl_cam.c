#include <string.h>

#include "lcd_lvgl_cam.h"

#include "esp_log.h"
#include "esp_heap_caps.h"
#include "esp_check.h"

#include "esp_camera.h"

#include "esp_lvgl_port.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "lcd_lvgl.h"
#include "lcd_lvgl_cfg.h"

#include "camera_manager.h"

static const char *TAG = "lcd_lvgl_camera";

static lv_obj_t *s_canvas = NULL;
static uint16_t *s_cam_buf = NULL;

static volatile bool s_running = false;

/* LCDサイズ */
static int s_disp_w,  s_disp_h;

/* カメラ描画サイズ */
static int s_cam_w, s_cam_h;

/* レターボックス位置 */
static int s_off_x, s_off_y;

/* カメラミラー */
static bool s_mirror_x, s_mirror_y;

/* カメラ画像サイズ */
static int s_cam_src_w, s_cam_src_h; 

/* ------------------------------------------------------------------ */
/* YUV422(YUYV packed) -> RGB565 変換                                  */
/* ------------------------------------------------------------------ */
static inline uint16_t yuv_to_rgb565(uint8_t y, uint8_t u, uint8_t v)
{
    int d = (int)u - 128;
    int e = (int)v - 128;

    int r = y + ((359 * e) >> 8);
    int g = y - ((88 * d + 183 * e) >> 8);
    int b = y + ((454 * d) >> 8);

    if (r < 0) r = 0; else if (r > 255) r = 255;
    if (g < 0) g = 0; else if (g > 255) g = 255;
    if (b < 0) b = 0; else if (b > 255) b = 255;

    return (uint16_t)(((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3));
}

/**
 * src: OV3660 から得た YUV422(YUYV) の生バッファ (src_w x src_h)
 * dst: LCD全体分のRGB565バッファ (dst_w x dst_h)
 * draw_w/draw_h: アスペクト比を保ったまま描画する実サイズ
 * off_x/off_y: dst 内でのレターボックス開始位置
 *
 * ニアレストネイバーで縮小しつつ、余白は黒で埋める。
 */
static void convert_yuv422_letterboxed(const uint8_t *src, int src_w, int src_h,
                                        uint16_t *dst, int dst_w, int dst_h,
                                        int draw_w, int draw_h, int off_x, int off_y)
{
    if (draw_w < dst_w || draw_h < dst_h) {
        /* 余白帯(位置によらず)を毎フレーム黒でクリア */
        memset(dst, 0, (size_t)dst_w * dst_h * sizeof(uint16_t));
    }

    int sx,sy;
    for (int dy = 0; dy < draw_h; dy++) {
        if (!s_mirror_y) sy = (dy * src_h) / draw_h;
        else sy = ((draw_h - 1 - dy) * src_h) / draw_h;
        
        const uint8_t *row = src + (size_t)sy * src_w * 2; /* 1px=2byte(YUV422) */
        uint16_t *out_row = dst + (size_t)(off_y + dy) * dst_w + off_x;

        for (int dx = 0; dx < draw_w; dx++) {
            if (s_mirror_x) sx = (dx * src_w) / draw_w;
            else sx = ((draw_w - 1 - dx) * src_w) / draw_w;
            int pair_x = sx & ~1;                 /* YUYV は2px単位 */
            const uint8_t *p = row + (size_t)pair_x * 2;

            uint8_t y, u, v;
            if ((sx & 1) == 0) {
                y = p[0]; u = p[1]; v = p[3];
            } else {
                y = p[2]; u = p[1]; v = p[3];
            }
            out_row[dx] = yuv_to_rgb565(y, u, v);
        }
    }
}

/* ------------------------------------------------------------------ */
/* カメラ描画                                                          */
/* ------------------------------------------------------------------ */

static void on_camera_frame(camera_fb_t *fb, void *user_ctx)
{
    if (fb->format == PIXFORMAT_YUV422 && fb->width == s_cam_src_w && fb->height == s_cam_src_h)
    {
        if (lvgl_port_lock(0))
        {
            convert_yuv422_letterboxed(fb->buf, fb->width, fb->height,
                                        s_cam_buf, s_disp_w, s_disp_h,
                                        s_cam_w, s_cam_h, s_off_x, s_off_y);
            lv_obj_invalidate(s_canvas);
            lvgl_port_unlock();
        }
    }
    else
    {
        ESP_LOGW(TAG, "unexpected frame: fmt=%d %dx%d", fb->format, fb->width, fb->height);
    }
}

esp_err_t lcd_lvgl_cam_init(const lcd_lvgl_camera_config_t *config)
{
    ESP_RETURN_ON_FALSE(config != NULL, ESP_ERR_INVALID_ARG, TAG, "config is NULL");
    ESP_RETURN_ON_FALSE(config->parent != NULL, ESP_ERR_INVALID_ARG, TAG, "parent is NULL");

    /* LCDサイズ */
    s_disp_w = config->lcd_width;
    s_disp_h = config->lcd_height;
    /* 描画サイズ */
    s_cam_w = config->cam_width;
    s_cam_h = config->cam_height;
    /* オフセット */
    s_off_x = config->off_x;
    s_off_y = config->off_y;
    /* ミラー */
    s_mirror_x = config->mirror_x;
    s_mirror_y = config->mirror_y;

    /* カメラ描画バッファ */
    s_cam_buf = heap_caps_malloc((size_t)s_disp_w * s_disp_h * sizeof(uint16_t), MALLOC_CAP_DMA | MALLOC_CAP_8BIT);

    ESP_RETURN_ON_FALSE(s_cam_buf != NULL, ESP_ERR_NO_MEM, TAG, "camera buffer alloc failed");

    memset(s_cam_buf, 0, (size_t)s_disp_w * s_disp_h * sizeof(uint16_t));

    if (!lvgl_port_lock(0)) {
        heap_caps_free(s_cam_buf);
        s_cam_buf = NULL;
        return ESP_FAIL;
    }

    /* Camera Canvas */
    s_canvas = lv_canvas_create(config->parent);
    lv_canvas_set_buffer(s_canvas, s_cam_buf, s_disp_w, s_disp_h, LV_COLOR_FORMAT_RGB565);
    lv_obj_set_pos(s_canvas, 0, 0);

    lvgl_port_unlock();

    ESP_LOGI(
        TAG,
        "camera initialized: "
        "lcd=%dx%d "
        "camera=%dx%d "
        "offset=(%d,%d)",

        s_disp_w, s_disp_h,
        s_cam_w,  s_cam_h,
        s_off_x,  s_off_y
    );

    camera_get_frame_size(&s_cam_src_w, &s_cam_src_h); 

    ESP_RETURN_ON_ERROR(
        camera_register_frame_cb(on_camera_frame, NULL),
        TAG, "camera_register_frame_cb failed"
    );

    return ESP_OK;
}
