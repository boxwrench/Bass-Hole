# Bass Hole - Setup Guide

## Hardware Required

- **TZT ESP32 2.4" CYD** (Cheap Yellow Display)
  - ILI9341 240x320 LCD
  - XPT2046 Touch controller
  - ESP32-WROOM module
- **MicroSD card** (any size, FAT32 formatted)

## Arduino IDE Setup

### 1. Install ESP32 Board Support

1. Open Arduino IDE → File → Preferences
2. Add to "Additional Board Manager URLs":
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
3. Tools → Board → Boards Manager → Search "esp32" → Install "esp32 by Espressif"

### 2. Install Required Libraries

Tools → Manage Libraries → Install:

- **TFT_eSPI** by Bodmer
- **XPT2046_Touchscreen** by Paul Stoffregen

### 3. Configure TFT_eSPI

**Option A: Replace User_Setup.h**

1. Find your Arduino libraries folder:
   - Windows: `C:\Users\<YOU>\Documents\Arduino\libraries\TFT_eSPI\`
   - Mac: `~/Documents/Arduino/libraries/TFT_eSPI/`
2. Backup the existing `User_Setup.h`
3. Copy `BassHole/User_Setup.h` from this project to that folder

**Option B: Use User_Setup_Select.h**

1. In `TFT_eSPI/User_Setup_Select.h`, comment out the default include
2. Add: `#include <User_Setups/Setup_CYD_2.4.h>`
3. Copy `BassHole/User_Setup.h` to `TFT_eSPI/User_Setups/Setup_CYD_2.4.h`

### 4. Select Board Settings

In Arduino IDE → Tools:

- **Board:** "ESP32 Dev Module"
- **Upload Speed:** 921600
- **CPU Frequency:** 240MHz
- **Flash Frequency:** 80MHz
- **Flash Mode:** QIO
- **Flash Size:** 4MB
- **Partition Scheme:** Default 4MB with spiffs
- **PSRAM:** Disabled
- **Port:** (select your COM port)

## Building & Uploading

1. Open `BassHole/BassHole.ino` in Arduino IDE
2. Click Verify (checkmark) to compile
3. Click Upload (arrow) to flash to device

## SD Card Setup

Format your SD card as FAT32, then create this folder structure:

```
/save/          (game saves go here - created automatically)
/sprites/       (future: sprite assets)
```

## Touch Calibration

If touch input seems off (taps don't register where you tap):

1. Enable `#define DEBUG_TOUCH 1` in `config.h`
2. Upload and open Serial Monitor (115200 baud)
3. Tap the corners of the screen
4. Note the raw X,Y values shown
5. Update `TOUCH_MIN_X/Y` and `TOUCH_MAX_X/Y` in `touch.cpp`

## Troubleshooting

**White/blank screen:**
- Check TFT_eSPI User_Setup.h is correctly configured
- Try reducing SPI_FREQUENCY to 27000000

**Touch not responding:**
- Check TOUCH_CS pin (33) matches your board
- Verify XPT2046_Touchscreen library is installed

**SD card not detected:**
- Ensure card is FAT32 formatted
- Check SD_CS pin (5) matches your board
- Try a different SD card

**Build errors about missing files:**
- Make sure all .cpp and .h files are in the `BassHole/` folder
- Arduino IDE should auto-detect them when opening BassHole.ino

## Project Structure

```
Bass-Hole/
├── BassHole/             # Arduino sketch folder
│   ├── BassHole.ino      # Main sketch
│   ├── config.h          # Hardware & game constants
│   ├── game_state.h/cpp  # Game state machine
│   ├── fish.h/cpp        # Fish behavior
│   ├── food.h/cpp        # Food pellets
│   ├── coins.h/cpp       # Coin collection
│   ├── touch.h/cpp       # Touch input
│   ├── graphics.h/cpp    # Rendering
│   ├── sdcard.h/cpp      # SD card access
│   └── User_Setup.h      # TFT_eSPI config (copy to library)
├── assets/               # Art assets (future)
├── sdcard/               # Files to copy to SD card
├── DEVELOPMENT_STRATEGY.md
└── SETUP.md              # This file
```
