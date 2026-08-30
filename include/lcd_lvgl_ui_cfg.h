#pragma once
/**
 * @file lcd_lvgl_ui_cfg.h
 * @brief 各UI画面で使用するテキスト項目のIDを定義し、画面別のテキスト設定を取得するAPIを提供します。
 */

#include <stddef.h>

#include "lcd_lvgl_ui.h"


/* =========================================================
 * INFO UI
 * ========================================================= */

/** @brief INFO画面で参照するテキスト項目IDです。 */
typedef enum {
    LCD_UI_INFO_TEXT_TITLE,

    LCD_UI_INFO_TEXT_COUNT
} lcd_ui_info_text_t;


/* =========================================================
 * CALIB COLOR UI
 * ========================================================= */

/** @brief カメラ色キャリブレーション画面で使用するテキスト項目IDです。 */
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

/** @brief カメラ位置キャリブレーション画面で使用するテキスト項目IDです。 */
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

/** @brief カラーセンサー色キャリブレーション画面で使用するテキスト項目IDです。 */
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

/** @brief カラーセンサーRGBC表示画面で使用するテキスト項目IDです。 */
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

/**
 * @brief 指定UIの静的テキスト設定配列を取得します。
 *
 * @param ui 設定を取得するUIです。
 * @param count テキスト項目数の格納先です。
 * @return テキスト設定配列への読み取り専用ポインタです。該当UIがない場合の扱いは実装に従います。
 */
const lcd_ui_text_config_t *lcd_ui_get_text_config(lcd_ui_t ui, size_t *count);