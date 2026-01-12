# CYD Variant Configurations

This file tracks working configurations for different ESP32 CYD (Cheap Yellow Display) board variants. If your board isn't listed, follow the debugging steps and contribute your configuration!

## How to Use This File

1. **Find your board** in the table below
2. **Copy the settings** to your `User_Setup.h` and code
3. **If not listed**, see [Debugging a New Variant](#debugging-a-new-variant)

---

## Tested Configurations

### Variant 1: TZT ESP32 2.4" CYD (2432S028)

| Property | Value |
|----------|-------|
| **Seller** | TZT Official Store (AliExpress) |
| **Model** | ESP32-2432S028 |
| **Display** | 2.4" ILI9341 240x320 |
| **Touch** | XPT2046 resistive |
| **Tested By** | @boxwrench |
| **Status** | In progress |

**TFT_eSPI User_Setup.h:**
```cpp
#define ILI9341_DRIVER
#define TFT_WIDTH  240
#define TFT_HEIGHT 320
#define TFT_RGB_ORDER TFT_BGR  // Required - colors inverted without this

#define TFT_MISO  12
#define TFT_MOSI  13
#define TFT_SCLK  14
#define TFT_CS    15
#define TFT_DC    2
#define TFT_RST   -1

#define TOUCH_CS  33

#define SPI_FREQUENCY       40000000
#define SPI_READ_FREQUENCY  20000000
#define SPI_TOUCH_FREQUENCY 2500000

#define SUPPORT_TRANSACTIONS
```

**Backlight:**
```cpp
// In gfxInit() - enable both common backlight pins
pinMode(21, OUTPUT);
digitalWrite(21, HIGH);
pinMode(27, OUTPUT);
digitalWrite(27, HIGH);
```

**Touch SPI:**
```cpp
// Touch shares HSPI with display
SPIClass hspi(HSPI);
hspi.begin(14, 12, 13, TOUCH_CS);  // SCK, MISO, MOSI, CS
touch.begin(hspi);
```

**Touch Calibration:**
```cpp
// TODO: Add calibrated values after testing
#define TOUCH_MIN_X  200
#define TOUCH_MAX_X  3800
#define TOUCH_MIN_Y  200
#define TOUCH_MAX_Y  3800
```

**Notes:**
- Display required BGR color order (reds showed as blues without it)
- Backlight not on by default - must set GPIO HIGH
- Touch calibration TBD

---

### Variant 2: [Template - Copy for New Variants]

| Property | Value |
|----------|-------|
| **Seller** | (where you bought it) |
| **Model** | (any model numbers on board) |
| **Display** | (size, driver, resolution) |
| **Touch** | (XPT2046 or other) |
| **Tested By** | @your_github |
| **Status** | Working / Partial / In Progress |

**TFT_eSPI User_Setup.h:**
```cpp
// Your working configuration here
```

**Backlight:**
```cpp
// Which GPIO pin(s) control backlight
```

**Touch Calibration:**
```cpp
// Your calibrated touch values
```

**Notes:**
- Any quirks or issues encountered

---

## Debugging a New Variant

### Step 1: Identify Your Board

Look for:
- Model number printed on PCB (often ESP32-XXXXSXXX format)
- Seller listing title/description
- Any version markings (V1, V2, Rev, etc.)

### Step 2: Display Not Working

**Black screen:**
1. Try different backlight pins: 21, 27, 4, 16
   ```cpp
   pinMode(PIN, OUTPUT);
   digitalWrite(PIN, HIGH);
   ```
2. Check Serial Monitor - is code running?

**White screen:**
- Wrong display driver or SPI pins
- Try `ST7789_DRIVER` instead of `ILI9341_DRIVER`

**Wrong colors (red↔blue swapped):**
- Add `#define TFT_RGB_ORDER TFT_BGR`
- Or try `#define TFT_RGB_ORDER TFT_RGB`

**Display garbled/noisy:**
- Reduce SPI_FREQUENCY to 27000000 or 20000000

**Image offset or wrapped:**
- Try different rotation: `tft.setRotation(0)` through `3`
- Check TFT_WIDTH and TFT_HEIGHT match your display

### Step 3: Touch Not Working

**No response:**
1. Enable DEBUG_TOUCH in config.h
2. Check Serial Monitor for any touch output
3. Verify TOUCH_CS pin (common: 33)
4. Try initializing touch on HSPI:
   ```cpp
   SPIClass hspi(HSPI);
   hspi.begin(14, 12, 13, TOUCH_CS);
   touch.begin(hspi);
   ```

**Touch coordinates wrong:**
1. Enable DEBUG_TOUCH to see raw values
2. Tap corners, note raw X/Y ranges
3. Update TOUCH_MIN/MAX values
4. May need to swap X↔Y or invert axes

### Step 4: SD Card Not Working

**Not detected:**
- Check SD_CS pin (common: 5)
- Try different SD card (FAT32 formatted)
- Some boards need card inserted before power-on

---

## Common CYD Pin Configurations

### 2.4" Variants (240x320)

| Function | Variant A | Variant B | Notes |
|----------|-----------|-----------|-------|
| TFT_MOSI | 13 | 13 | |
| TFT_MISO | 12 | 12 | |
| TFT_SCLK | 14 | 14 | |
| TFT_CS | 15 | 15 | |
| TFT_DC | 2 | 2 | |
| TFT_RST | -1 | -1 | Usually tied to EN |
| TFT_BL | 21 | 27 | Backlight varies! |
| TOUCH_CS | 33 | 33 | |
| TOUCH_IRQ | 36 | 36 | |
| SD_CS | 5 | 5 | |
| SD_MOSI | 23 | 23 | VSPI |
| SD_MISO | 19 | 19 | VSPI |
| SD_SCK | 18 | 18 | VSPI |

### 2.8" Variants (320x480)

| Function | Common | Notes |
|----------|--------|-------|
| Resolution | 320x480 | Different from 2.4" |
| Driver | ILI9341 or ST7789 | Check your display |

---

## Contributing Your Configuration

Found a working config for a new board? Please contribute!

1. Fork the repository
2. Copy the template in this file
3. Fill in your working configuration
4. Submit a Pull Request

Include:
- Where you bought the board
- Any model numbers visible
- Your complete working settings
- Any issues you encountered and solutions

---

## Resources

- [TFT_eSPI Setup Guide](https://github.com/Bodmer/TFT_eSPI)
- [ESP32 GPIO Reference](https://randomnerdtutorials.com/esp32-pinout-reference-gpios/)
- [CYD Reddit Community](https://www.reddit.com/r/esp32/)
