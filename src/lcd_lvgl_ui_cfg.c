#include "lcd_lvgl_ui_cfg.h"
#include "lcd_lvgl_ui.h"

const static lcd_ui_text_config_t info_text_config[LCD_UI_INFO_TEXT_COUNT] = 
{
    [LCD_UI_INFO_TEXT_TITLE] = {
        .x = 0, .y = 0,
        .text = "INFO",
        .color = DISPLAY_COLOR_WHITE
    }
};

const static lcd_ui_text_config_t camera_calib_color_text_config[LCD_UI_CAMERA_CALIB_COLOR_TEXT_COUNT] =
{
    [LCD_UI_CAMERA_CALIB_COLOR_TEXT_TITLE] = {
        .x = 0, .y = 0,
        .text = "CAM COLOR CALIB",
        .color = DISPLAY_COLOR_WHITE
    },
    [LCD_UI_CAMERA_CALIB_COLOR_TEXT_LIST_LABEL] = {
        .x = 0, .y = 10,
        .text = "LIST",
        .color = DISPLAY_COLOR_WHITE
    },
    [LCD_UI_CAMERA_CALIB_COLOR_TEXT_LIST_VALUE] = {
        .x = 25, .y = 10,
        .text = "?",
        .color = DISPLAY_COLOR_WHITE
    },
    [LCD_UI_CAMERA_CALIB_COLOR_TEXT_POS_LABEL] = {
        .x = 64, .y = 10,
        .text = "POS",
        .color = DISPLAY_COLOR_WHITE
    },
    [LCD_UI_CAMERA_CALIB_COLOR_TEXT_POS_VALUE] = {
        .x = 89, .y = 10,
        .text = "?",
        .color = DISPLAY_COLOR_WHITE
    },
    [LCD_UI_CAMERA_CALIB_COLOR_TEXT_COLOR_LABEL] = {
        .x = 0, .y = 20,
        .text = "COLOR",
        .color = DISPLAY_COLOR_WHITE
    },
    [LCD_UI_CAMERA_CALIB_COLOR_TEXT_COLOR_VALUE] = {
        .x = 45, .y = 20,
        .text = "?",
        .color = DISPLAY_COLOR_WHITE
    },
    [LCD_UI_CAMERA_CALIB_COLOR_TEXT_CAMERA_EXIST] = {
        .x = 32, .y = 64,
        .text = "NO CAMERA",
        .color = DISPLAY_COLOR_RED
    },
};

const static lcd_ui_text_config_t camera_calib_pos_text_config[LCD_UI_CAMERA_CALIB_POS_TEXT_COUNT] =
{
    [LCD_UI_CAMERA_CALIB_POS_TEXT_TITLE] = {
        .x = 0, .y = 0,
        .text = "CAM POS CALIB",
        .color = DISPLAY_COLOR_WHITE
    },
    [LCD_UI_CAMERA_CALIB_POS_TEXT_LIST_LABEL] = {
        .x = 0, .y = 10,
        .text = "LIST",
        .color = DISPLAY_COLOR_WHITE
    },
    [LCD_UI_CAMERA_CALIB_POS_TEXT_LIST_VALUE] = {
        .x = 25, .y = 10,
        .text = "?",
        .color = DISPLAY_COLOR_WHITE
    },
    [LCD_UI_CAMERA_CALIB_POS_TEXT_POS_LABEL] = {
        .x = 64, .y = 10,
        .text = "POS",
        .color = DISPLAY_COLOR_WHITE
    },
    [LCD_UI_CAMERA_CALIB_POS_TEXT_POS_VALUE] = {
        .x = 89, .y = 10,
        .text = "?",
        .color = DISPLAY_COLOR_WHITE
    },
    [LCD_UI_CAMERA_CALIB_POS_TEXT_POS_X_LABEL] = {
        .x = 0, .y = 20,
        .text = "X",
        .color = DISPLAY_COLOR_WHITE
    },
    [LCD_UI_CAMERA_CALIB_POS_TEXT_POS_X_VALUE] = {
        .x = 9, .y = 20,
        .text = "?",
        .color = DISPLAY_COLOR_WHITE
    },
    [LCD_UI_CAMERA_CALIB_POS_TEXT_POS_Y_LABEL] = {
        .x = 64, .y = 20,
        .text = "Y",
        .color = DISPLAY_COLOR_WHITE
    },
    [LCD_UI_CAMERA_CALIB_POS_TEXT_POS_Y_VALUE] = {
        .x = 73, .y = 20,
        .text = "?",
        .color = DISPLAY_COLOR_WHITE
    },
    [LCD_UI_CAMERA_CALIB_POS_TEXT_CAMERA_EXIST] = {
        .x = 32, .y = 64,
        .text = "NO CAMERA",
        .color = DISPLAY_COLOR_RED
    },
};

const static lcd_ui_text_config_t color_calib_color_text_config[LCD_UI_COLOR_CALIB_COLOR_TEXT_COUNT] =
{
    [LCD_UI_COLOR_CALIB_COLOR_TEXT_TITLE] = {
        .x = 0, .y = 0,
        .text = "COLOR SENSOR CALIB",
        .color = DISPLAY_COLOR_WHITE
    },
    [LCD_UI_COLOR_CALIB_COLOR_TEXT_ID_LABEL] = {
        .x = 0, .y = 10,
        .text = "ID",
        .color = DISPLAY_COLOR_WHITE
    },
    [LCD_UI_COLOR_CALIB_COLOR_TEXT_ID_VALUE] = {
        .x = 13, .y = 10,
        .text = "?",
        .color = DISPLAY_COLOR_WHITE
    },
    [LCD_UI_COLOR_CALIB_COLOR_TEXT_COLOR_LABEL] = {
        .x = 32, .y = 10,
        .text = "COLOR",
        .color = DISPLAY_COLOR_WHITE
    },
    [LCD_UI_COLOR_CALIB_COLOR_TEXT_COLOR_VALUE] = {
        .x = 75, .y = 10,
        .text = "?",
        .color = DISPLAY_COLOR_WHITE
    },
    [LCD_UI_COLOR_CALIB_COLOR_TEXT_SENSOR_EXIST] = {
        .x = 0, .y = 20,
        .text = "NO SENSOR",
        .color = DISPLAY_COLOR_RED
    }
};

const static lcd_ui_text_config_t color_rgbc_text_config[LCD_UI_COLOR_RGBC_TEXT_COUNT] =
{
    [LCD_UI_COLOR_RGBC_TEXT_R_LAVEL] = {
        .x = 0, .y = 20,
        .text = "R",
        .color = DISPLAY_COLOR_RED
    },
    [LCD_UI_COLOR_RGBC_TEXT_R_VALUE] = {
        .x = 9, .y = 20,
        .text = "?",
        .color = DISPLAY_COLOR_RED
    },
    [LCD_UI_COLOR_RGBC_TEXT_G_LAVEL] = {
        .x = 32, .y = 20,
        .text = "G",
        .color = DISPLAY_COLOR_GREEN
    },
    [LCD_UI_COLOR_RGBC_TEXT_G_VALUE] = {
        .x = 41, .y = 20,
        .text = "?",
        .color = DISPLAY_COLOR_GREEN
    },
    [LCD_UI_COLOR_RGBC_TEXT_B_LAVEL] = {
        .x = 64, .y = 20,
        .text = "B",
        .color = DISPLAY_COLOR_BLUE
    },
    [LCD_UI_COLOR_RGBC_TEXT_B_VALUE] = {
        .x = 73, .y = 20,
        .text = "?",
        .color = DISPLAY_COLOR_BLUE
    },
    [LCD_UI_COLOR_RGBC_TEXT_C_LAVEL] = {
        .x = 96, .y = 20,
        .text = "C",
        .color = DISPLAY_COLOR_WHITE
    },
    [LCD_UI_COLOR_RGBC_TEXT_C_VALUE] = {
        .x = 105, .y = 20,
        .text = "?",
        .color = DISPLAY_COLOR_WHITE
    },
};



const lcd_ui_text_config_t *lcd_ui_get_text_config(lcd_ui_t ui, size_t *count)
{
    if (count == NULL) return NULL;

    switch (ui) 
    {
    case LCD_UI_INFO:
        *count = sizeof(info_text_config) / sizeof(info_text_config[0]);
        return info_text_config;
    case LCD_UI_CAMERA_CALIB_COLOR:
        *count = sizeof(camera_calib_color_text_config) / sizeof(camera_calib_color_text_config[0]);
        return camera_calib_color_text_config;
    case LCD_UI_CAMERA_CALIB_POS:
        *count = sizeof(camera_calib_pos_text_config) /sizeof(camera_calib_pos_text_config[0]);
        return camera_calib_pos_text_config;
    case LCD_UI_COLOR_CALIB_COLOR:
        *count = sizeof(color_calib_color_text_config) /sizeof(color_calib_color_text_config[0]);
        return color_calib_color_text_config;
    case LCD_UI_COLOR_RGBC:
        *count = sizeof(color_rgbc_text_config) /sizeof(color_rgbc_text_config[0]);
        return color_rgbc_text_config;
    default:
        *count = 0;
        return NULL;
    }
}