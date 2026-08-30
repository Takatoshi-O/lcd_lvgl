#pragma once
/**
 * @file lcd_lvgl_cam.h
 * @brief カメラフレームをLVGLのLCD表示へ変換するための設定構造体と初期化APIを定義します。
 */

#include <stdint.h>
#include <stdbool.h>

#include "esp_err.h"
#include "lvgl.h"

/**
 * @brief カメラ画像をLCD上へ表示するための配置・反転設定です。
 */
typedef struct {
    lv_obj_t *parent;

    uint16_t lcd_width;
    uint16_t lcd_height;

    int cam_width;
    int cam_height;

    int off_x;
    int off_y;

    bool mirror_x;
    bool mirror_y;
} lcd_lvgl_camera_config_t;


/* カメラ表示初期化 */
/**
 * @brief カメラ表示用のLVGLオブジェクトとフレームコールバックを初期化します。
 *
 * @param config LCDサイズ、カメラサイズ、オフセット、ミラー設定などを指定します。
 * @return 初期化結果です。
 */
esp_err_t lcd_lvgl_cam_init(const lcd_lvgl_camera_config_t *config);
