#pragma once
/**
 * @file lcd_lvgl.h
 * @brief SPI LCDとLVGL、カメラ表示機能を初期化・開始・停止するトップレベルAPIを定義します。
 */

#include <stdbool.h>
#include "esp_err.h"
#include "driver/spi_master.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief UIやテキストで使用する論理色を表します。
 */
typedef enum {
    DISPLAY_COLOR_CLEAR = 0,
    DISPLAY_COLOR_BLACK,
    DISPLAY_COLOR_WHITE,
    DISPLAY_COLOR_RED,
    DISPLAY_COLOR_GREEN,
    DISPLAY_COLOR_BLUE,
    DISPLAY_COLOR_YELLOW,
} display_color_t;

/**
 * SPIバス・LCDパネル・LVGL・カメラの初期化を行う。
 * まだキャプチャは開始しない (cam_lcd_lvgl_start() を呼ぶこと)。
 */
/**
 * @brief SPIバス、LCDパネル、LVGL、カメラ表示機能を初期化します。
 *
 * カメラキャプチャ自体は開始せず、開始にはlcd_lvgl_start()を使用します。
 *
 * @return 初期化結果です。
 */
esp_err_t lcd_lvgl_init();

/** カメラ→LCD 表示更新タスクを開始 */
/**
 * @brief カメラからLCDへの表示更新を開始します。
 *
 * @return 開始結果です。
 */
esp_err_t lcd_lvgl_start(void);

/** 更新タスクを停止 */
/**
 * @brief カメラからLCDへの表示更新を停止します。
 *
 * @return 停止結果です。
 */
esp_err_t lcd_lvgl_stop(void);

#ifdef __cplusplus
}
#endif