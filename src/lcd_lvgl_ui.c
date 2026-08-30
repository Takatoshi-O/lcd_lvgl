#include "lcd_lvgl_ui.h"

#include "esp_lvgl_port.h"
#include "lvgl.h"
#include "esp_log.h"
#include "esp_check.h"

#include "lcd_lvgl.h"
#include "lcd_lvgl_cfg.h"
#include "lcd_lvgl_ui_cfg.h"

static const char *TAG = "cam_lcd_ui";

/* LCDサイズ */
static uint16_t s_disp_w, s_disp_h;

static int s_cam_w, s_cam_h;
static int s_off_x, s_off_y;
static int s_cam_src_w, s_cam_src_h; 

/* UI Container */
static lv_obj_t *s_ui[LCD_UI_COUNT];

#define SQUARE_WIDTH 7
static lv_obj_t *s_rect[MAX_RECT_COUNT] = {NULL};

#define MAX_TEXT_COUNT 10
static lv_obj_t *s_text[LCD_UI_COUNT][MAX_TEXT_COUNT] = {NULL};


static lv_color_t display_color_to_lvgl(display_color_t color)
{
    switch (color) {
    case DISPLAY_COLOR_RED:
        return lv_color_hex(0xFF0000);
    case DISPLAY_COLOR_GREEN:
        return lv_color_hex(0x00FF00);
    case DISPLAY_COLOR_BLUE:
        return lv_color_hex(0x0000FF);
    case DISPLAY_COLOR_YELLOW:
        return lv_color_hex(0xFFFF00);
    case DISPLAY_COLOR_WHITE:
        return lv_color_hex(0xFFFFFF);
    case DISPLAY_COLOR_BLACK:
        return lv_color_hex(0x000000);
    default:
        return lv_color_hex(0x808080);
    }
}

static lv_obj_t *create_ui_container(lv_obj_t *parent)
{
    lv_obj_t *container = lv_obj_create(parent);

    /* 画面全体 */
    lv_obj_set_size(container, s_disp_w, s_disp_h);
    /* 左上 */
    lv_obj_set_pos(container, 0, 0);
    /* 背景透明 */
    lv_obj_set_style_bg_opa(container, LV_OPA_TRANSP, LV_PART_MAIN);
    /* 枠線なし */
    lv_obj_set_style_border_width(container, 0, LV_PART_MAIN);
    /* 内側余白なし */
    lv_obj_set_style_pad_all(container, 0, LV_PART_MAIN);
    /* スクロール無効 */
    lv_obj_remove_flag(container, LV_OBJ_FLAG_SCROLLABLE);

    return container;
}

static esp_err_t lcd_ui_rect_create(uint8_t index, lcd_ui_t ui)
{
    if (index >= MAX_RECT_COUNT) return ESP_ERR_INVALID_ARG;
    if (ui >= LCD_UI_COUNT) return ESP_ERR_INVALID_ARG;
    if (s_ui[ui] == NULL) return ESP_ERR_INVALID_STATE;
    if (s_rect[index] != NULL) return ESP_ERR_INVALID_STATE;

    /* LVGLロック */
    if (!lvgl_port_lock(0)) return ESP_FAIL;

    /* 長方形作成 */
    s_rect[index] = lv_obj_create(s_ui[ui]);
    /* 初期状態は非表示 */
    //lv_obj_add_flag(s_rect[index], LV_OBJ_FLAG_HIDDEN);
    /* 初期サイズ */
    lv_obj_set_size(s_rect[index], SQUARE_WIDTH, SQUARE_WIDTH);
    /* 背景透明 */
    lv_obj_set_style_bg_opa(s_rect[index], LV_OPA_TRANSP, LV_PART_MAIN);
    /* 枠線 */
    lv_obj_set_style_border_width(s_rect[index], 2, LV_PART_MAIN);
    /* 初期色 */
    lv_obj_set_style_border_color(s_rect[index], lv_color_hex(0x0000FF), LV_PART_MAIN);
    /* 角丸なし */
    lv_obj_set_style_radius(s_rect[index], 0, LV_PART_MAIN);

    lvgl_port_unlock();

    return ESP_OK;
}

static esp_err_t lcd_ui_text_create(uint8_t index, lcd_ui_t ui, const lcd_ui_text_config_t *config)
{
    if (index >= MAX_TEXT_COUNT) return ESP_ERR_INVALID_ARG;
    if (ui >= LCD_UI_COUNT) return ESP_ERR_INVALID_ARG;
    if (config == NULL) return ESP_ERR_INVALID_ARG;
    if (s_ui[ui] == NULL) return ESP_ERR_INVALID_STATE;
    if (s_text[ui][index] != NULL) return ESP_ERR_INVALID_STATE;

    if (!lvgl_port_lock(0)) return ESP_FAIL;

    /* 指定UIのTextを作成 */
    s_text[ui][index] = lv_label_create(s_ui[ui]);
    /* 初期状態は非表示 */
    //lv_obj_add_flag(s_text[ui][index], LV_OBJ_FLAG_HIDDEN);

    lv_obj_set_pos(s_text[ui][index], config->x, config->y);
    lv_obj_set_style_text_font(s_text[ui][index], &lv_font_montserrat_10, LV_PART_MAIN);
    lv_obj_set_style_text_color(s_text[ui][index], 
                                display_color_to_lvgl(config->color), LV_PART_MAIN);
    lv_label_set_text(s_text[ui][index], config->text);

    lvgl_port_unlock();

    return ESP_OK;
}

esp_err_t lcd_ui_init(uint16_t width, uint16_t height, int cam_w, int cam_h, 
                      int off_x, int off_y, int cam_src_w, int cam_src_h)
{
    s_disp_w = width; s_disp_h = height;
    s_cam_w  = cam_w; s_cam_h  = cam_h;
    s_off_x  = off_x; s_off_y  = off_y;
    s_cam_src_w = cam_src_w; 
    s_cam_src_h = cam_src_h;

    if (!lvgl_port_lock(0)) {
        ESP_LOGE(TAG, "failed to lock LVGL");
        return ESP_FAIL;
    }

    /* Display position UI */
    s_ui[LCD_UI_DISPLAY_POS] = create_ui_container(lv_scr_act());
    /* Camera calib color UI */
    s_ui[LCD_UI_CAMERA_CALIB_COLOR] = create_ui_container(lv_scr_act());
    /* Camera calib position UI */
    s_ui[LCD_UI_CAMERA_CALIB_POS] = create_ui_container(lv_scr_act());
    /* Color sensor calib color UI */
    s_ui[LCD_UI_COLOR_CALIB_COLOR] = create_ui_container(lv_scr_act());
    /* Color sensor RGBC UI */
    s_ui[LCD_UI_COLOR_RGBC] = create_ui_container(lv_scr_act());
    /* Info UI */
    s_ui[LCD_UI_INFO] = create_ui_container(lv_scr_act());

    /* Camera UI以外を非表示 */
    for (int i = 0; i < LCD_UI_COUNT; i++) {
        if (i != LCD_UI_INFO) lv_obj_add_flag(s_ui[i], LV_OBJ_FLAG_HIDDEN);
    }

    lvgl_port_unlock();

    ESP_LOGI(TAG, "UI initialized: %dx%d", s_disp_w, s_disp_h);

    for (int i = 0; i < MAX_RECT_COUNT; i++)
    {
        lcd_ui_rect_create(i, LCD_UI_DISPLAY_POS);
    }

    for (int ui = 0; ui < LCD_UI_COUNT; ui++) 
    {
        size_t text_count;
        const lcd_ui_text_config_t *text_config = lcd_ui_get_text_config((lcd_ui_t)ui, &text_count);

        if (text_config == NULL) continue;

        for (size_t i = 0; i < text_count; i++) 
        {
            ESP_RETURN_ON_ERROR(lcd_ui_text_create(i, (lcd_ui_t)ui, &text_config[i]), TAG, "failed to create text");
        }
    }

    return ESP_OK;
}

esp_err_t lcd_ui_set(lcd_ui_t ui)
{
    if (ui >= LCD_UI_COUNT) return ESP_ERR_INVALID_ARG;

    if (!lvgl_port_lock(0)) return ESP_FAIL;

    /* 全UIを隠す */
    for (int i = 0; i < LCD_UI_COUNT; i++) {
        if (i == LCD_UI_DISPLAY_POS) continue;
        if (i == LCD_UI_COLOR_RGBC)  continue;

        lv_obj_add_flag(s_ui[i], LV_OBJ_FLAG_HIDDEN);
    }

    /* 指定したUIを表示 */
    lv_obj_clear_flag(s_ui[ui], LV_OBJ_FLAG_HIDDEN);

    lvgl_port_unlock();

    return ESP_OK;
}

esp_err_t lcd_ui_display(lcd_ui_t ui, bool is_display)
{
    if (ui >= LCD_UI_COUNT) return ESP_ERR_INVALID_ARG;

    if (!lvgl_port_lock(0)) return ESP_FAIL;

    if (is_display) lv_obj_remove_flag(s_ui[ui], LV_OBJ_FLAG_HIDDEN);
    else            lv_obj_add_flag(s_ui[ui], LV_OBJ_FLAG_HIDDEN);

    lvgl_port_unlock();

    return ESP_OK;
}

static void camera_to_lcd(int cam_x, int cam_y, int *lcd_x, int *lcd_y)
{
    *lcd_x = s_off_x + (cam_x * s_cam_w) / s_cam_src_w;
    *lcd_y = s_off_y + (cam_y * s_cam_h) / s_cam_src_h;
}

esp_err_t lcd_lvgl_set_rect(int index, int cam_x, int cam_y, display_color_t color)
{
    if (index >= MAX_RECT_COUNT) return ESP_ERR_INVALID_ARG;
    if (s_rect[index] == NULL) return ESP_ERR_INVALID_STATE;

    if (color == DISPLAY_COLOR_CLEAR)
    {
        if (!lvgl_port_lock(0)) return ESP_FAIL;
        
        lv_obj_add_flag(s_rect[index], LV_OBJ_FLAG_HIDDEN);
        lvgl_port_unlock();
        
        return ESP_OK;
    }

    int lcd_x;
    int lcd_y;

    /* 左上 */
    camera_to_lcd(cam_x, cam_y, &lcd_x, &lcd_y);

    lcd_x = lcd_x - (SQUARE_WIDTH / 2) - 1;
    lcd_y = lcd_y - (SQUARE_WIDTH / 2) - 1;

    if (!lvgl_port_lock(0)) return ESP_FAIL;

    /* 長方形の位置 */
    lv_obj_set_pos(s_rect[index], lcd_x, lcd_y);
    lv_obj_set_style_border_color(s_rect[index], display_color_to_lvgl(color), LV_PART_MAIN);
    lv_obj_move_foreground(s_rect[index]);
    /* 表示 */
    lv_obj_remove_flag(s_rect[index], LV_OBJ_FLAG_HIDDEN);

    lvgl_port_unlock();

    return ESP_OK;
}

void lcd_ui_text_set(lcd_ui_t ui, uint8_t index, const char *text, display_color_t color)
{
    if (ui >= LCD_UI_COUNT) return;
    if (index >= MAX_TEXT_COUNT) return;
    if (s_text[ui][index] == NULL) return;

    if (color == DISPLAY_COLOR_CLEAR)
    {
        if (!lvgl_port_lock(0)) return;
        
        lv_obj_add_flag(s_text[ui][index], LV_OBJ_FLAG_HIDDEN);
        lvgl_port_unlock();
        
        return;
    }

    if (!lvgl_port_lock(0)) return;

    lv_label_set_text(s_text[ui][index], text);
    lv_obj_set_style_text_color(s_text[ui][index], display_color_to_lvgl(color), LV_PART_MAIN);
    lv_obj_remove_flag(s_text[ui][index], LV_OBJ_FLAG_HIDDEN);

    lvgl_port_unlock();
}

void lcd_ui_text_display(lcd_ui_t ui, uint8_t index, bool is_display)
{
    if (ui >= LCD_UI_COUNT) return;
    if (index >= MAX_TEXT_COUNT) return;
    if (s_text[ui][index] == NULL) return;

    if (!lvgl_port_lock(0)) return;

    if (is_display) lv_obj_remove_flag(s_text[ui][index], LV_OBJ_FLAG_HIDDEN);
    else            lv_obj_add_flag(s_text[ui][index], LV_OBJ_FLAG_HIDDEN);

    lvgl_port_unlock();

    return;
}