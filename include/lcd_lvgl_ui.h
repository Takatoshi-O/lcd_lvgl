#pragma once

#include <stdint.h>
#include "esp_err.h"
#include "lvgl.h"
#include "lcd_lvgl.h"

/* UIの種類 */
typedef enum {
    LCD_UI_DISPLAY_POS,
    LCD_UI_INFO,
    LCD_UI_CAMERA_CALIB_COLOR,
    LCD_UI_CAMERA_CALIB_POS,
    LCD_UI_COLOR_CALIB_COLOR,
    LCD_UI_COLOR_RGBC,
    
    LCD_UI_COUNT
} lcd_ui_t;

#define MAX_RECT_COUNT 12

typedef struct {
    int x,y;
    const char *text;
    display_color_t color;
} lcd_ui_text_config_t;


/* UI初期化 */
esp_err_t lcd_ui_init(uint16_t width, uint16_t height, int cam_w, int cam_h, 
                      int off_x, int off_y, int cam_src_w, int cam_src_h);

/* UI切り替え */
esp_err_t lcd_ui_set(lcd_ui_t ui);

/* UI表示切り替え */
esp_err_t lcd_ui_display(lcd_ui_t ui, bool is_display);

esp_err_t lcd_lvgl_set_rect(int index, int cam_x, int cam_y, display_color_t color);

void lcd_ui_text_set(lcd_ui_t ui, uint8_t index, const char *text, display_color_t color);

void lcd_ui_text_display(lcd_ui_t ui, uint8_t index, bool is_display);