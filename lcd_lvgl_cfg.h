#ifndef LCD_LVGL_CFG_H
#define LCD_LVGL_CFG_H

/**
 * @file lcd_lvgl_cfg.h
 * @brief LCD関連の設定(カメラ設定は camera コンポーネントへ移動)
 */

/* ============================================================
 * LCDボード選択
 * ============================================================ */

#if defined(CONFIG_LCD_BOARD_XIAO_ESP32_S3_SENSE)

    #define LCD_BOARD_SELECTED 1

#elif defined(CONFIG_LCD_BOARD_FREENOVE_ESP32_S3_WROOM1)

    #define LCD_BOARD_SELECTED 2

#else

    #error "No supported LCD board selected in menuconfig."

#endif


/* ============================================================
 * XIAO ESP32-S3 Sense
 * ============================================================ */

#if LCD_BOARD_SELECTED == 1

    #define LCD_BOARD_NAME "XIAO ESP32-S3 Sense"

    #define LCD_PIN_MOSI     9
    #define LCD_PIN_CLK      7

    #define LCD_PIN_CS       4
    #define LCD_PIN_DC       3
    #define LCD_PIN_RST      2
    #define LCD_PIN_BL       -1


/* ============================================================
 * Freenove ESP32-S3 WROOM1
 * ============================================================ */

#elif LCD_BOARD_SELECTED == 2

    #define LCD_BOARD_NAME "Freenove ESP32-S3 WROOM1"

    #define LCD_PIN_MOSI     41
    #define LCD_PIN_CLK      40

    #define LCD_PIN_CS       1
    #define LCD_PIN_DC       42
    #define LCD_PIN_RST      2
    #define LCD_PIN_BL       -1

#else

    #error "Invalid LCD board selection."

#endif

#ifdef CONFIG_LCD_INVERT_COLOR
#define LCD_INVERT_COLOR true
#else
#define LCD_INVERT_COLOR false
#endif

#ifdef CONFIG_LCD_SWAP_XY
#define LCD_SWAP_XY true
#else
#define LCD_SWAP_XY false
#endif

#ifdef CONFIG_LCD_MIRROR_X
#define LCD_MIRROR_X true
#else
#define LCD_MIRROR_X false
#endif

#ifdef CONFIG_LCD_MIRROR_Y
#define LCD_MIRROR_Y true
#else
#define LCD_MIRROR_Y false
#endif

#endif /* LCD_LVGL_CFG_H */