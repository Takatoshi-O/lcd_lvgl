#pragma once

#include <stddef.h>

#include "lcd_lvgl_ui.h"


/* =========================================================
 * INFO UI
 * ========================================================= */

typedef enum {
    LCD_UI_INFO_TEXT_TITLE,

    LCD_UI_INFO_TEXT_COUNT
} lcd_ui_info_text_t;


/* =========================================================
 * CALIB COLOR UI
 * ========================================================= */

typedef enum {
    LCD_UI_CAMERA_CALIB_COLOR_TEXT_TITLE,

    LCD_UI_CAMERA_CALIB_COLOR_TEXT_LIST_LABEL,
    LCD_UI_CAMERA_CALIB_COLOR_TEXT_LIST_VALUE,

    LCD_UI_CAMERA_CALIB_COLOR_TEXT_POS_LABEL,
    LCD_UI_CAMERA_CALIB_COLOR_TEXT_POS_VALUE,

    LCD_UI_CAMERA_CALIB_COLOR_TEXT_COLOR_LABEL,
    LCD_UI_CAMERA_CALIB_COLOR_TEXT_COLOR_VALUE,

    LCD_UI_CAMERA_CALIB_COLOR_TEXT_CAMERA_EXIST,

    LCD_UI_CAMERA_CALIB_COLOR_TEXT_COUNT
} lcd_ui_camera_calib_color_text_t;


/* =========================================================
 * CALIB POSITION UI
 * ========================================================= */

typedef enum {
    LCD_UI_CAMERA_CALIB_POS_TEXT_TITLE,

    LCD_UI_CAMERA_CALIB_POS_TEXT_LIST_LABEL,
    LCD_UI_CAMERA_CALIB_POS_TEXT_LIST_VALUE,

    LCD_UI_CAMERA_CALIB_POS_TEXT_POS_LABEL,
    LCD_UI_CAMERA_CALIB_POS_TEXT_POS_VALUE,

    LCD_UI_CAMERA_CALIB_POS_TEXT_POS_X_LABEL,
    LCD_UI_CAMERA_CALIB_POS_TEXT_POS_X_VALUE,

    LCD_UI_CAMERA_CALIB_POS_TEXT_POS_Y_LABEL,
    LCD_UI_CAMERA_CALIB_POS_TEXT_POS_Y_VALUE,

    LCD_UI_CAMERA_CALIB_POS_TEXT_CAMERA_EXIST,

    LCD_UI_CAMERA_CALIB_POS_TEXT_COUNT
} lcd_ui_camera_calib_pos_text_t;

/* =========================================================
 * CALIB COLOR UI
 * ========================================================= */

typedef enum {
    LCD_UI_COLOR_CALIB_COLOR_TEXT_TITLE,

    LCD_UI_COLOR_CALIB_COLOR_TEXT_ID_LABEL,
    LCD_UI_COLOR_CALIB_COLOR_TEXT_ID_VALUE,

    LCD_UI_COLOR_CALIB_COLOR_TEXT_COLOR_LABEL,
    LCD_UI_COLOR_CALIB_COLOR_TEXT_COLOR_VALUE,

    LCD_UI_COLOR_CALIB_COLOR_TEXT_SENSOR_EXIST,

    LCD_UI_COLOR_CALIB_COLOR_TEXT_COUNT
} lcd_ui_color_calib_color_text_t;

/* =========================================================
 * CALIB RGBC UI
 * ========================================================= */

typedef enum {
    LCD_UI_COLOR_RGBC_TEXT_R_LAVEL,
    LCD_UI_COLOR_RGBC_TEXT_R_VALUE,

    LCD_UI_COLOR_RGBC_TEXT_G_LAVEL,
    LCD_UI_COLOR_RGBC_TEXT_G_VALUE,

    LCD_UI_COLOR_RGBC_TEXT_B_LAVEL,
    LCD_UI_COLOR_RGBC_TEXT_B_VALUE,

    LCD_UI_COLOR_RGBC_TEXT_C_LAVEL,
    LCD_UI_COLOR_RGBC_TEXT_C_VALUE,

    LCD_UI_COLOR_RGBC_TEXT_COUNT
} lcd_ui_color_rgbc_text_t;

/* =========================================================
 * Text Config取得
 * ========================================================= */

const lcd_ui_text_config_t *lcd_ui_get_text_config(lcd_ui_t ui, size_t *count);