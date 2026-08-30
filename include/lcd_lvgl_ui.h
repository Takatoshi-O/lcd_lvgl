#pragma once
/**
 * @file lcd_lvgl_ui.h
 * @brief カメラ画像上に重ねて表示する矩形・テキストを含むUIの初期化と表示制御APIを定義します。
 */

#include <stdint.h>
#include "esp_err.h"
#include "lvgl.h"
#include "lcd_lvgl.h"

/* UIの種類 */
/**
 * @brief コンポーネントが提供するUI画面の種類です。
 */
typedef enum {
    LCD_UI_DISPLAY_POS,
    LCD_UI_INFO,
    LCD_UI_CAMERA_CALIB_COLOR,
    LCD_UI_CAMERA_CALIB_POS,
    LCD_UI_COLOR_CALIB_COLOR,
    LCD_UI_COLOR_RGBC,
    
    LCD_UI_COUNT
} lcd_ui_t;

/** @brief UIで同時に管理できる矩形オブジェクト数の最大値です。 */
#define MAX_RECT_COUNT 12

/**
 * @brief UIテキスト1件の位置、文字列、表示色を指定する設定です。
 */
typedef struct {
    int x,y;
    const char *text;
    display_color_t color;
} lcd_ui_text_config_t;


/* UI初期化 */
/**
 * @brief UIレイヤーを初期化し、画面別の矩形・テキストオブジェクトを生成します。
 *
 * @param width LCDの横幅です。
 * @param height LCDの高さです。
 * @param cam_w 表示領域として使用するカメラ幅です。
 * @param cam_h 表示領域として使用するカメラ高さです。
 * @param off_x カメラ画像のXオフセットです。
 * @param off_y カメラ画像のYオフセットです。
 * @param cam_src_w 元カメラフレームの横幅です。
 * @param cam_src_h 元カメラフレームの高さです。
 * @return 初期化結果です。
 */
esp_err_t lcd_ui_init(uint16_t width, uint16_t height, int cam_w, int cam_h, 
                      int off_x, int off_y, int cam_src_w, int cam_src_h);

/* UI切り替え */
/**
 * @brief 現在のUIとして指定画面を選択します。
 *
 * @param ui 選択するUIです。
 * @return 切り替え結果です。
 */
esp_err_t lcd_ui_set(lcd_ui_t ui);

/* UI表示切り替え */
/**
 * @brief 指定UI全体の表示・非表示を切り替えます。
 *
 * @param ui 対象UIです。
 * @param is_display trueなら表示、falseなら非表示です。
 * @return 操作結果です。
 */
esp_err_t lcd_ui_display(lcd_ui_t ui, bool is_display);

/**
 * @brief カメラ座標をLCD座標へ変換して矩形表示位置と色を設定します。
 *
 * @param index 変更対象の矩形インデックスです。
 * @param cam_x カメラ座標系のX位置です。
 * @param cam_y カメラ座標系のY位置です。
 * @param color 矩形の表示色です。
 * @return 操作結果です。
 */
esp_err_t lcd_lvgl_set_rect(int index, int cam_x, int cam_y, display_color_t color);

/**
 * @brief 指定UIのテキスト内容と色を変更します。
 *
 * @param ui 対象UIです。
 * @param index テキスト項目のインデックスです。
 * @param text 表示する文字列です。
 * @param color 表示色です。
 */
void lcd_ui_text_set(lcd_ui_t ui, uint8_t index, const char *text, display_color_t color);

/**
 * @brief 指定UIの特定テキスト項目を表示・非表示にします。
 *
 * @param ui 対象UIです。
 * @param index テキスト項目のインデックスです。
 * @param is_display trueなら表示、falseなら非表示です。
 */
void lcd_ui_text_display(lcd_ui_t ui, uint8_t index, bool is_display);