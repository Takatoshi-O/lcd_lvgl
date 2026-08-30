# lcd_lvgl

`lcd_lvgl` は、SPI接続LCD、LVGL、`camera_manager` を組み合わせて、カメラ映像とオーバーレイUIをLCDに表示するESP-IDFコンポーネントです。

## 役割

| ファイル | 役割 |
|---|---|
| `include/lcd_lvgl.h` | LCD/LVGLの初期化とカメラ表示の開始・停止 |
| `include/lcd_lvgl_cam.h` | カメラ映像をLVGLへ表示するための設定と初期化 |
| `include/lcd_lvgl_ui.h` | UI画面、矩形オーバーレイ、テキスト制御 |
| `include/lcd_lvgl_ui_cfg.h` | 各UI画面で使用する定義済みテキスト項目ID |
| `lcd_lvgl_cfg.h` | 内部用のボードGPIO・表示方向設定 |
| `src/*.c` | 実装 |

## 対応ボード

現在の設定では以下に対応しています。

- XIAO ESP32-S3 Sense
- Freenove ESP32-S3 WROOM1

実装ではSPI接続のST7789パネルを使用し、LVGLの表示形式はRGB565として構成しています。

## Kconfig設定

`idf.py menuconfig` の **Component config -> LCD LVGL Configuration** から設定します。

| 項目 | デフォルト | 説明 |
|---|---:|---|
| LCD board | XIAO ESP32-S3 Sense | ボードごとのGPIO選択 |
| `LCD_WIDTH` | 128 | LCD横幅 |
| `LCD_HEIGHT` | 128 | LCD高さ |
| `LCD_SPI_CLOCK_HZ` | 20000000 | SPIクロック(Hz) |
| `LCD_INVERT_COLOR` | off | LCD色反転 |
| `LCD_BL_ACTIVE_HIGH` | on | バックライト極性 |
| `LCD_MIRROR_X` | off | LVGL X方向ミラー |
| `LCD_MIRROR_Y` | off | LVGL Y方向ミラー |
| `LCD_SWAP_XY` | off | LVGLのX/Y入れ替え |
| LCD color order | BGR | RGB/BGR選択 |
| `LCD_X_GAP` | 2 | LCD X方向オフセット |
| `LCD_Y_GAP` | 1 | LCD Y方向オフセット |
| Vertical alignment | Center | カメラ映像の縦位置 |

## 初期化

基本的な使用順序は次のとおりです。

```c
ESP_ERROR_CHECK(lcd_lvgl_init());
ESP_ERROR_CHECK(lcd_lvgl_start());
```

`lcd_lvgl_init()` はSPIバス、LCDパネル、LVGL表示、カメラマネージャー、カメラ表示オブジェクト、UI層を初期化します。また、カメラのアスペクト比を維持したままLCD内に収める描画サイズを計算します。

`lcd_lvgl_start()` はカメラマネージャーを開始し、`lcd_lvgl_stop()` はカメラフレーム取得を停止します。

## カメラ表示

カメラ表示処理は `camera_manager` のフレームコールバックを利用してフレームを受け取ります。YUV422をRGB565へ変換し、アスペクト比を維持してLCDサイズへリサイズし、必要に応じてレターボックス表示します。

カメラ表示設定には次の値があります。

- LCD幅・高さ
- カメラ表示幅・高さ
- X/Yオフセット
- 変換後カメラ画像のX/Yミラー

トップレベルでは、LVGLの表示方向設定とカメラバッファ側のミラー方向が整合するよう補正値を設定しています。

## UI層

`lcd_ui_init()` は定義済みの各UI画面についてLVGLコンテナを作成します。現在のUI IDは次のとおりです。

- `LCD_UI_DISPLAY_POS`
- `LCD_UI_INFO`
- `LCD_UI_CAMERA_CALIB_COLOR`
- `LCD_UI_CAMERA_CALIB_POS`
- `LCD_UI_COLOR_CALIB_COLOR`
- `LCD_UI_COLOR_RGBC`

矩形オーバーレイは現在 `MAX_RECT_COUNT` (=12) 個まで管理できます。`lcd_lvgl_set_rect()` はカメラ座標をLCD座標へ変換し、色を変更して前面へ移動します。`DISPLAY_COLOR_CLEAR` を指定すると非表示になります。

テキストは `lcd_lvgl_ui_cfg.h` / `lcd_lvgl_ui_cfg.c` の設定を使います。`lcd_ui_text_set()` は文字列と色を変更し、`lcd_ui_text_display()` は表示・非表示だけを変更します。

## 依存コンポーネント

- `driver`
- `esp_lcd`
- `esp32-camera`
- `esp_lvgl_port`
- `lvgl`
- `camera_manager`

コンポーネントメタデータではESP-IDF 6.0.1を対象とし、`esp32-camera`、`esp_lvgl_port`、LVGL 9.3の依存バージョンを定義しています。

## 公開ヘッダー

- `lcd_lvgl.h`
- `lcd_lvgl_cam.h`
- `lcd_lvgl_ui.h`
- `lcd_lvgl_ui_cfg.h`
