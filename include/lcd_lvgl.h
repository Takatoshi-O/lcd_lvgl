#pragma once

#include <stdbool.h>
#include "esp_err.h"
#include "driver/spi_master.h"

#ifdef __cplusplus
extern "C" {
#endif

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
esp_err_t lcd_lvgl_init();

/** カメラ→LCD 表示更新タスクを開始 */
esp_err_t lcd_lvgl_start(void);

/** 更新タスクを停止 */
esp_err_t lcd_lvgl_stop(void);

#ifdef __cplusplus
}
#endif