#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "esp_err.h"
#include "lvgl.h"

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
esp_err_t lcd_lvgl_cam_init(const lcd_lvgl_camera_config_t *config);
