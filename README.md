# lcd_lvgl

`lcd_lvgl` is an ESP-IDF component that combines an SPI LCD, LVGL, and the `camera_manager` component to display a camera stream with overlay UI elements.

## Responsibilities

| File | Responsibility |
|---|---|
| `include/lcd_lvgl.h` | Top-level LCD/LVGL initialization and camera display start/stop |
| `include/lcd_lvgl_cam.h` | Camera-to-LVGL display configuration and initialization |
| `include/lcd_lvgl_ui.h` | UI screens, rectangle overlays, and text control |
| `include/lcd_lvgl_ui_cfg.h` | IDs for predefined text items on each UI screen |
| `lcd_lvgl_cfg.h` | Internal board GPIO and display-orientation configuration |
| `src/*.c` | Implementation |

## Supported boards

The current board configuration supports:

- XIAO ESP32-S3 Sense
- Freenove ESP32-S3 WROOM1

The implementation uses an ST7789 panel over SPI and configures LVGL with RGB565 display format.

## Kconfig

Select **Component config -> LCD LVGL Configuration** in `idf.py menuconfig`.

| Option | Default | Description |
|---|---:|---|
| LCD board | XIAO ESP32-S3 Sense | Board-specific GPIO selection |
| `LCD_WIDTH` | 128 | LCD width |
| `LCD_HEIGHT` | 128 | LCD height |
| `LCD_SPI_CLOCK_HZ` | 20000000 | SPI clock in Hz |
| `LCD_INVERT_COLOR` | off | LCD color inversion |
| `LCD_BL_ACTIVE_HIGH` | on | Backlight polarity |
| `LCD_MIRROR_X` | off | LVGL X-axis mirroring |
| `LCD_MIRROR_Y` | off | LVGL Y-axis mirroring |
| `LCD_SWAP_XY` | off | LVGL X/Y swap |
| LCD color order | BGR | RGB/BGR selection |
| `LCD_X_GAP` | 2 | LCD X offset/gap |
| `LCD_Y_GAP` | 1 | LCD Y offset/gap |
| Vertical alignment | Center | Camera image vertical alignment |

## Initialization

The high-level sequence is:

```c
ESP_ERROR_CHECK(lcd_lvgl_init());
ESP_ERROR_CHECK(lcd_lvgl_start());
```

`lcd_lvgl_init()` initializes the SPI bus, LCD panel, LVGL display, camera manager, camera display object, and UI layer. It calculates the camera image size so that the source aspect ratio is preserved inside the configured LCD area.

`lcd_lvgl_start()` starts the camera manager. `lcd_lvgl_stop()` stops camera frame acquisition.

## Camera display

The camera display path receives frames through `camera_manager` callbacks. The camera frame is converted from YUV422 to RGB565 and resized to fit the LCD area with letterboxing.

The camera display configuration contains:

- LCD width/height
- Camera display width/height
- X/Y offset
- X/Y mirroring for the converted camera image

The top-level component compensates the camera buffer mirroring against the LVGL display rotation settings.

## UI layer

`lcd_ui_init()` creates one LVGL container for each predefined UI screen. Current screen IDs are:

- `LCD_UI_DISPLAY_POS`
- `LCD_UI_INFO`
- `LCD_UI_CAMERA_CALIB_COLOR`
- `LCD_UI_CAMERA_CALIB_POS`
- `LCD_UI_COLOR_CALIB_COLOR`
- `LCD_UI_COLOR_RGBC`

The rectangle API currently manages up to `MAX_RECT_COUNT` (=12) square overlays. `lcd_lvgl_set_rect()` accepts camera coordinates, maps them into LCD coordinates, changes the rectangle color, and moves it to the foreground. `DISPLAY_COLOR_CLEAR` hides the rectangle.

Text items are configured through `lcd_lvgl_ui_cfg.h` and `lcd_lvgl_ui_cfg.c`. `lcd_ui_text_set()` changes a label's text and color, while `lcd_ui_text_display()` changes only its visibility.

## Dependencies

- `driver`
- `esp_lcd`
- `esp32-camera`
- `esp_lvgl_port`
- `lvgl`
- `camera_manager`

The component metadata currently targets ESP-IDF 6.0.1 and lists compatible versions for `esp32-camera`, `esp_lvgl_port`, and LVGL 9.3.

## Public headers

- `lcd_lvgl.h`
- `lcd_lvgl_cam.h`
- `lcd_lvgl_ui.h`
- `lcd_lvgl_ui_cfg.h`
