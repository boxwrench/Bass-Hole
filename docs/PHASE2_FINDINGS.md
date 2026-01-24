# Phase 2: Sprite Integration - Findings & Configuration

**Last Updated:** 2026-01-19  
**Hardware:** ESP32-2432S028R (ILI9341_2_DRIVER + XPT2046)

---

## ✅ VERIFIED GROUND TRUTH (2026-01-19)

### Critical Discovery: Gamma Correction

**The washed-out sprite issue was caused by the default gamma curve, NOT byte-swapping or RGB order.**

### Verified Display Configuration

```cpp
// graphics.cpp - VERIFIED WORKING
void gfxInit() {
    pinMode(21, OUTPUT); digitalWrite(21, HIGH);  // Backlight
    pinMode(27, OUTPUT); digitalWrite(27, HIGH);  // Alternate backlight
    
    tft.init();
    
    // CRITICAL: Set gamma for vibrant sprite colors
    tft.writecommand(0x26);  // Gamma Set command
    tft.writedata(0x01);     // Gamma curve 1 (more saturated)
    
    // Required for ILI9341_2_DRIVER variant
    tft.invertDisplay(true);
    
    // Landscape mode, USB down
    tft.setRotation(1);
    
    // Required for RGB565 sprites
    tft.setSwapBytes(true);
}
```

### Verified PlatformIO Configuration

```ini
[env:bass-hole]
platform = espressif32@6.5.0
board = esp32dev
framework = arduino

lib_deps = 
    bodmer/TFT_eSPI@^2.5.43
    https://github.com/PaulStoffregen/XPT2046_Touchscreen.git

build_flags = 
    -DUSER_SETUP_LOADED=1
    -DILI9341_2_DRIVER=1        # Fixes static/corruption
    -DTFT_WIDTH=240
    -DTFT_HEIGHT=320
    -DTFT_MISO=12
    -DTFT_MOSI=13
    -DTFT_SCLK=14
    -DTFT_CS=15
    -DTFT_DC=2
    -DTFT_RST=-1
    -DTFT_BL=21
    -DTFT_BACKLIGHT_ON=HIGH
    # DO NOT USE: -DTFT_RGB_ORDER=1 (causes red/blue swap)
    # DO NOT USE: -DUSE_HSPI_PORT=1 (unnecessary)
    -DLOAD_GLCD=1
    -DLOAD_FONT2=1
    -DLOAD_FONT4=1
    -DLOAD_FONT6=1
    -DLOAD_FONT7=1
    -DLOAD_FONT8=1
    -DLOAD_GFXFF=1
    -DSMOOTH_FONT=1
    -DSPI_FREQUENCY=40000000
    -DSPI_READ_FREQUENCY=20000000
    -DSPI_TOUCH_FREQUENCY=2500000
```

### Verified Sprite Asset Pipeline

**Format:** RGB565, Little-Endian  
**Tool:** `tools/png_to_rgb565.py` (default RGB mode, NOT --bgr)  
**Firmware:** `setSwapBytes(true)`

```bash
# Generate verified RGB565 sprite
python tools/png_to_rgb565.py input.png output.rgb565
```

**Result:** Accurate colors with crisp outlines

---

## Touch Calibration (VERIFIED)

### Working Settings

```cpp
// touch.cpp
touch.setRotation(1);  // Different from display rotation!

// Raw ranges
#define TOUCH_MIN_X  600
#define TOUCH_MAX_X  3600
#define TOUCH_MIN_Y  500
#define TOUCH_MAX_Y  3600

// Mapping
int16_t mappedX = map(p.x, TOUCH_MAX_X, TOUCH_MIN_X, 0, SCREEN_WIDTH);  // X inverted
int16_t mappedY = map(p.y, TOUCH_MIN_Y, TOUCH_MAX_Y, 0, SCREEN_HEIGHT); // Y normal
```

### Result
- Touch crosshairs appear exactly where tapped
- Coins collectible
- Buttons functional

---

## Sprite Rendering

### Transparency Implementation

```cpp
void gfxDrawSpriteTransparent() {
    // Pixel-by-pixel rendering
    // Skip pixels with value 0xF81F (magenta)
    for (py/px loop) {
        if (pixel != SPRITE_TRANSPARENT_COLOR) {
            tft.drawPixel(x + px, y + py, pixel);
        }
    }
}
```

**Why pixel-by-pixel?**
- TFT_eSPI's `pushImage()` with transparency was unreliable
- Pixel-by-pixel gives consistent results across different displays
- Performance acceptable for 32×32 sprites (~1024 pixels)

### Horizontal Flip (for fish direction)

```cpp
void gfxDrawSpriteTransparentFlip() {
    // Read sprite right-to-left
    int16_t srcIdx = py * width + (width - 1 - px);
    // Same transparency logic
}
```

---

## Sprite Pipeline

### 1. Resize (All Done)
```bash
python tools/resize_sprites.py
```
- 42 sprites resized
- Fish: 32×32px
- Backgrounds: 240×240px
- UI: 16×16px

### 2. Convert to RGB565
```bash
python tools/png_to_rgb565.py input.png output.rgb565
```
- Outputs RGB565 Little-Endian binary
- Use with `setSwapBytes(true)` in firmware

### 3. Integration
```cpp
#include "sprites/fish_sprites.h"
gfxDrawSpriteTransparent(sprite_fish_r_trout, x, y, width, height);
```

---

## Memory Usage

| Component | Size | Location |
|-----------|------|----------|
| Rainbow Trout sprite | 2KB | PROGMEM |
| Sierra background | 115KB | PROGMEM |
| **Total Flash** | **38.1%** (498KB / 1.3MB) | Good headroom |
| **Total RAM** | **7.4%** (24KB / 327KB) | Excellent |

---

## Performance

- **FPS:** Stable 30 FPS with background + sprite fish
- **Touch latency:** < 50ms
- **Sprite rendering:** ~1ms per fish (pixel-by-pixel)

---

## Issues Resolved

### 1. Washed Out Sprite Colors (CRITICAL)
**Problem:** Sprites appeared desaturated/muted  
**Cause:** Default gamma curve incompatible with panel variant  
**Fix:** Set Gamma 0x01 after `tft.init()`

### 2. Display Static/Corruption
**Problem:** Random pixels, shifted display  
**Cause:** Using `ILI9341_DRIVER` instead of `ILI9341_2_DRIVER`  
**Fix:** Changed to `ILI9341_2_DRIVER` in build flags

### 3. Inverted Colors
**Problem:** All colors appeared as opposites  
**Cause:** `ILI9341_2_DRIVER` defaults to opposite inversion  
**Fix:** Added `invertDisplay(true)` after init

### 4. Red/Blue Swap
**Problem:** Red appeared blue, blue appeared red  
**Cause:** `TFT_RGB_ORDER=1` flag in build config  
**Fix:** Removed flag, use default RGB order

### 5. Fish "Ghosty" Appearance
**Problem:** Semi-transparent fish, visible box  
**Cause:** TFT_eSPI's transparent pushImage() unreliable  
**Fix:** Manual pixel-by-pixel transparency check

### 6. Touch Inverted Up/Down
**Problem:** Crosshairs on opposite vertical position  
**Cause:** Y-axis mapping backwards  
**Fix:** Swapped TOUCH_MIN_Y and TOUCH_MAX_Y in map()

---

## Next Steps (Phase 2B)

1. **Update graphics.cpp** with gamma setting
2. **Regenerate all sprite assets** using verified RGB565 pipeline
3. **Convert remaining fish sprites** (9 more species)
4. **Add UI sprites** (coin, pellet already resized)
5. **Ty Knotts dialogue system**

---

## Lessons Learned

1. **Gamma is critical** - Default gamma causes washed-out sprites
2. **Driver variant matters** - ILI9341_2_DRIVER fixes static but changes defaults
3. **Test on hardware early** - Simulators don't catch gamma/color issues
4. **Document everything** - Small config changes have big impacts
5. **RGB order is hardware-specific** - Don't assume TFT_RGB_ORDER is needed

---

## Configuration Summary (Copy-Paste Reference)

```cpp
// Display (graphics.cpp)
void gfxInit() {
    tft.init();
    tft.writecommand(0x26); tft.writedata(0x01);  // Gamma 0x01
    tft.invertDisplay(true);                       // Required for _2_DRIVER
    tft.setRotation(1);                            // Landscape, USB down
    tft.setSwapBytes(true);                        // Required for RGB565
}

// Touch (touch.cpp)
touch.setRotation(1);
#define TOUCH_MIN_X  600
#define TOUCH_MAX_X  3600
#define TOUCH_MIN_Y  500
#define TOUCH_MAX_Y  3600
int16_t mappedX = map(p.x, 3600, 600, 0, 320);   // Inverted
int16_t mappedY = map(p.y, 500, 3600, 0, 240);   // Normal
```

---

**Status:** Ready for Phase 2B (remaining sprites + dialogue system)  
**Hardware:** Fully calibrated and documented  
**Performance:** Excellent (30 FPS, 38% flash, 7% RAM)
