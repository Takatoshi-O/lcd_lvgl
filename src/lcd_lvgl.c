#include <string.h>
#include <math.h>

#include "lcd_lvgl.h"

#include "esp_log.h"
#include "esp_check.h"
#include "esp_heap_caps.h"
#include "esp_err.h"

#include "driver/gpio.h"

#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"

#include "esp_lvgl_port.h"
#include "lvgl.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "sdkconfig.h"
#include "lcd_lvgl_cfg.h"

#include "lcd_lvgl_ui.h"
#include "lcd_lvgl_cam.h"

#include "camera_manager.h"

static const char *TAG = "lcd_lvgl";

static esp_lcd_panel_handle_t s_panel = NULL;
static esp_lcd_panel_io_handle_t s_io = NULL;
static lv_display_t *s_disp = NULL;

static int s_disp_w, s_disp_h;      /* LCD 物理解像度 */
static int s_cam_w, s_cam_h;      /* アスペクト比維持した描画サイズ */
static int s_off_x, s_off_y;        /* レターボックスのオフセット */

/* ------------------------------------------------------------------ */
/* 初期化                                                              */
/* ------------------------------------------------------------------ */

static bool s_initialized = false;

esp_err_t lcd_lvgl_init(void)
{
    if (s_initialized) return ESP_OK;

    /* ============================================================
     * LCD設定
     * ============================================================ */

    s_disp_w = CONFIG_LCD_WIDTH;
    s_disp_h = CONFIG_LCD_HEIGHT;

    /* ============================================================
     * QVGA画像をLCDサイズへ変換するサイズを計算
     * ============================================================ */

    int cam_src_w, cam_src_h;
    camera_get_frame_size(&cam_src_w, &cam_src_h);

    float scale_x = (float)s_disp_w / cam_src_w;
    float scale_y = (float)s_disp_h / cam_src_h;
    float scale = scale_x < scale_y ? scale_x : scale_y;

    s_cam_w = (int)(cam_src_w * scale + 0.5f);
    s_cam_h = (int)(cam_src_h * scale + 0.5f);

    if (s_cam_w > s_disp_w) s_cam_w = s_disp_w;
    if (s_cam_h > s_disp_h) s_cam_h = s_disp_h;

    /* 横方向は常に中央 */
    s_off_x = (s_disp_w - s_cam_w) / 2;

    /* ============================================================
     * カメラ画像の縦位置
     * ============================================================ */

    #if CONFIG_LCD_V_ALIGN_TOP
        s_off_y = 0;
    #elif CONFIG_LCD_V_ALIGN_BOTTOM
        s_off_y = s_disp_h - s_cam_h;
    #else
        s_off_y = (s_disp_h - s_cam_h) / 2;
    #endif

    ESP_LOGI(
        TAG,
        "panel=%dx%d camera=%dx%d offset=(%d,%d)",
        s_disp_w, s_disp_h,
        s_cam_w,  s_cam_h,
        s_off_x,  s_off_y
    );


    /* ============================================================
     * バックライト
     * ============================================================ */

    #if LCD_PIN_BL >= 0

        gpio_config_t bl_cfg = {
            .pin_bit_mask = 1ULL << LCD_PIN_BL,
            .mode = GPIO_MODE_OUTPUT,
        };

        ESP_RETURN_ON_ERROR(
            gpio_config(&bl_cfg),
            TAG,
            "backlight gpio config failed"
        );

        gpio_set_level(
            LCD_PIN_BL,
            CONFIG_LCD_BL_ACTIVE_HIGH ? 1 : 0
        );

    #endif


    /* ============================================================
     * SPIバス
     * ============================================================ */

    spi_bus_config_t buscfg = {
        .sclk_io_num = LCD_PIN_CLK,
        .mosi_io_num = LCD_PIN_MOSI,
        .miso_io_num = -1,

        .quadwp_io_num = -1,
        .quadhd_io_num = -1,

        .max_transfer_sz = s_disp_w * s_disp_h * sizeof(uint16_t),
    };

    ESP_RETURN_ON_ERROR(
        spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO),
        TAG,
        "spi_bus_initialize failed"
    );


    /* ============================================================
     * LCD SPI IO
     * ============================================================ */

    esp_lcd_panel_io_spi_config_t io_cfg = {
        .cs_gpio_num = LCD_PIN_CS,
        .dc_gpio_num = LCD_PIN_DC,

        .spi_mode = 0,

        .pclk_hz = CONFIG_LCD_SPI_CLOCK_HZ,

        .trans_queue_depth = 10,

        .lcd_cmd_bits = 8,
        .lcd_param_bits = 8,
    };

    ESP_RETURN_ON_ERROR(
        esp_lcd_new_panel_io_spi(SPI2_HOST, &io_cfg, &s_io),
        TAG,
        "esp_lcd_new_panel_io_spi failed"
    );


    /* ============================================================
     * LCDパネル
     * ============================================================ */

    esp_lcd_panel_dev_config_t panel_cfg = {
        .reset_gpio_num = LCD_PIN_RST,
        .bits_per_pixel = 16,

    #if CONFIG_LCD_COLOR_ORDER_BGR
        .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_BGR,
    #else
        .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB,
    #endif
    };


    ESP_RETURN_ON_ERROR(
        esp_lcd_new_panel_st7789(s_io, &panel_cfg, &s_panel),
        TAG,
        "esp_lcd_new_panel_st7789 failed"
    );


    /* ============================================================
     * LCD初期化
     * ============================================================ */

    ESP_RETURN_ON_ERROR(
        esp_lcd_panel_reset(s_panel),
        TAG,
        "panel reset failed"
    );

    ESP_RETURN_ON_ERROR(
        esp_lcd_panel_init(s_panel),
        TAG,
        "panel init failed"
    );

    ESP_RETURN_ON_ERROR(
        esp_lcd_panel_invert_color(s_panel, LCD_INVERT_COLOR),
        TAG,
        "invert color failed"
    );


    if (CONFIG_LCD_X_GAP != 0 || CONFIG_LCD_Y_GAP != 0) 
    {
        ESP_RETURN_ON_ERROR(
            esp_lcd_panel_set_gap(s_panel, CONFIG_LCD_X_GAP, CONFIG_LCD_Y_GAP),
            TAG,
            "set gap failed"
        );
    }


    ESP_RETURN_ON_ERROR(
        esp_lcd_panel_disp_on_off(s_panel, true),
        TAG,
        "panel on failed"
    );


    /* ============================================================
     * LVGL
     * ============================================================ */

    lvgl_port_cfg_t lvgl_cfg = ESP_LVGL_PORT_INIT_CONFIG();

    ESP_RETURN_ON_ERROR(lvgl_port_init(&lvgl_cfg), TAG, "lvgl_port_init failed");


    lvgl_port_display_cfg_t disp_cfg = {
        .io_handle = s_io,
        .panel_handle = s_panel,

        .buffer_size = (size_t)s_disp_w * s_disp_h,

        .double_buffer = true,

        .hres = s_disp_w,
        .vres = s_disp_h,

        .monochrome = false,

        .color_format = LV_COLOR_FORMAT_RGB565,

        .rotation = {
            .swap_xy  = LCD_SWAP_XY,
            .mirror_x = LCD_MIRROR_X,
            .mirror_y = LCD_MIRROR_Y,
        },

        .flags = {
            .buff_dma   = true,
            .swap_bytes = true,
        },
    };

    s_disp = lvgl_port_add_disp(&disp_cfg);

    ESP_RETURN_ON_FALSE(s_disp != NULL, ESP_FAIL, TAG, "lvgl_port_add_disp failed");

    camera_config_t cam_cfg;
    camera_get_default_config(&cam_cfg);

    ESP_RETURN_ON_ERROR(camera_init(&cam_cfg), TAG, "camera_init failed");

    /* ============================================================
     * カメラ画像表示
     * ============================================================ */

    lcd_lvgl_camera_config_t camera_config = {
        .parent = lv_scr_act(),

        .lcd_width = s_disp_w,
        .lcd_height = s_disp_h,

        .cam_width = s_cam_w,
        .cam_height = s_cam_h,

        .off_x = s_off_x,
        .off_y = s_off_y,

        /*
         * LCDをLVGL側でミラーする場合、
         * カメラバッファ側は逆方向に補正
         */
        .mirror_x = !LCD_MIRROR_X,
        .mirror_y = !LCD_MIRROR_Y,
    };

    ESP_RETURN_ON_ERROR(lcd_lvgl_cam_init(&camera_config), TAG, "camera display init failed");

    /* ============================================================
     * UI
     * ============================================================ */

    ESP_RETURN_ON_ERROR(
        lcd_ui_init(
            s_disp_w,  s_disp_h,
            s_cam_w,   s_cam_h,
            s_off_x,   s_off_y,
            cam_src_w, cam_src_h
        ),
        TAG,
        "UI init failed"
    );

    s_initialized = true;
    return ESP_OK;
}

esp_err_t lcd_lvgl_start(void)
{
    return camera_start();
}

esp_err_t lcd_lvgl_stop(void)
{
    return camera_stop();
}