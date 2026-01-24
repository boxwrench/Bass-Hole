# Bass-Hole Ground Truth Configuration
**Date:** 2026-01-20  
**Purpose:** Establish verified working configuration to prevent circular debugging

---

## Hardware
- **Board:** ESP32 CYD 2.4" (TZT variant, ESP32-2432S028R)
- **Display:** ILI9341 240x320
- **Touch:** XPT2046

---

## VERIFIED Display Configuration (from DEVLOG.md 2026-01-19)

```cpp
// Display Init
tft.init();
tft.setRotation(1);           // LANDSCAPE, USB down (NOT 0!)
tft.invertDisplay(true);       // Required with ILI9341_2_DRIVER

// Gamma 0x01 - CRITICAL for vibrant sprites
tft.writecommand(0x26);
tft.writedata(0x01);

// Byte swapping for RGB565 sprites
tft.setSwapBytes(true);
```

**Expected Screen Dimensions:**
- Width: 320
- Height: 240
- Orientation: Landscape (USB at bottom)

---

## VERIFIED Touch Configuration (from DEVLOG.md)

```cpp
touch.begin();
touch.setRotation(1);  // DIFFERENT from display rotation!

// Touch mapping (from DEVLOG lines 188-190)
// X = INVERTED
mappedX = map(p.x, TOUCH_MAX_X, TOUCH_MIN_X, 0, SCREEN_WIDTH);
// Y = Normal  
mappedY = map(p.y, TOUCH_MIN_Y, TOUCH_MAX_Y, 0, SCREEN_HEIGHT);
```

**Touch Calibration Values:**
```cpp
#define TOUCH_MIN_X  600
#define TOUCH_MAX_X  3600
#define TOUCH_MIN_Y  500
#define TOUCH_MAX_Y  3600
```

---

## Asset Pipeline

**Conversion:**
```bash
python tools/png_to_rgb565.py input.png output.raw
# NO --bgr flag! Use RGB mode with setSwapBytes(true)
```

**Format:** RGB565 Little-Endian

---

## Current Problem

**What we changed:**
- Set rotation to `0` (portrait) instead of `1` (landscape)
- This conflicts with DEVLOG verified settings

**What config.h expects:**
```cpp
#define SCREEN_WIDTH  240
#define SCREEN_HEIGHT 320
```
This is PORTRAIT dimensions, but DEVLOG says rotation `1` (landscape)!

---

## Resolution Required

**Option 1:** Use DEVLOG settings (rotation 1 = landscape)
- Change `config.h` to `SCREEN_WIDTH 320, SCREEN_HEIGHT 240`
- Update all game logic for landscape layout

**Option 2:** Verify if DEVLOG rotation `1` was for CYD-Tester, not Bass-Hole
- Check if Bass-Hole Phase 1 actually used rotation `0` (portrait)
- Reconcile the discrepancy

**QUESTION FOR USER:** Was Phase 1 Bass-Hole running in **landscape** or **portrait** mode?
