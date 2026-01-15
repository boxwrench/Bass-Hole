# Phase 2: Sprite Integration - Findings & Configuration

**Date:** 2025-01-14
**Hardware:** TZT ESP32 CYD 2.4" (ILI9341 + XPT2046)

## Summary

Successfully integrated sprite rendering for Phase 2. Fish now display as actual sprite artwork instead of geometric shapes. Background sprite added. All touch and display issues resolved through calibration.

## Display Configuration (CRITICAL)

### Working Settings

```cpp
// graphics.cpp
tft.setRotation(0);              // Portrait, USB at bottom
tft.invertDisplay(false);        // MUST be false for sprites
```

### Color Handling Solution

With `invertDisplay(false)`:
- ✓ **Sprites render correctly** (RGB565 data embedded in arrays)
- ✗ **UI/text render inverted** (TFT primitives affected by inversion setting)

**Solution:** Invert all color constants in `config.h`

```cpp
// config.h - All colors pre-inverted
#define COLOR_BLACK   0xFFFF  // XOR with 0xFFFF inverts RGB565
#define COLOR_WHITE   0x0000
#define COLOR_UI_GREEN 0xF81F  // Note: This is magenta value, displays as green!
```

**Why this works:**
- Sprite data contains absolute RGB565 values → renders correctly
- TFT functions use inverted constants → cancels out to correct display

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

## Sprite Pipeline

### 1. Resize (All Done)
```bash
python tools/resize_sprites.py
```
- 42 sprites resized
- Fish: 32×32px
- Backgrounds: 240×240px
- UI: 16×16px

### 2. Convert to C Arrays
```bash
python tools/img2code.py input.png --name sprite_name --output output.h
```
- Outputs RGB565 const arrays in PROGMEM
- Magenta (0xF81F) = transparent

### 3. Integration
```cpp
#include "sprites/fish_sprites.h"
gfxDrawSpriteTransparent(sprite_fish_r_trout, x, y, width, height);
```

## Memory Usage

| Component | Size | Location |
|-----------|------|----------|
| Rainbow Trout sprite | 2KB | PROGMEM |
| Sierra background | 115KB | PROGMEM |
| **Total Flash** | **38.1%** (498KB / 1.3MB) | Good headroom |
| **Total RAM** | **7.4%** (24KB / 327KB) | Excellent |

## Performance

- **FPS:** Stable 30 FPS with background + sprite fish
- **Touch latency:** < 50ms
- **Sprite rendering:** ~1ms per fish (pixel-by-pixel)

## Issues Resolved

### 1. Fish "Ghosty" Appearance
**Problem:** Semi-transparent fish, visible box
**Cause:** TFT_eSPI's transparent pushImage() unreliable
**Fix:** Manual pixel-by-pixel transparency check

### 2. Fish Box When Facing Left
**Problem:** Opaque rectangle when flipped
**Cause:** Flip function not checking transparency
**Fix:** Added transparency check to flip function

### 3. Inverted UI Colors
**Problem:** Text/buttons wrong colors, sprites correct
**Cause:** `invertDisplay(false)` needed for sprites, inverts primitives
**Fix:** Pre-inverted color constants in config.h

### 4. Touch Inverted Up/Down
**Problem:** Crosshairs on opposite vertical position
**Cause:** Y-axis mapping backwards
**Fix:** Swapped TOUCH_MIN_Y and TOUCH_MAX_Y in map()

### 5. Touch Inverted Left/Right (Initially)
**Problem:** Crosshairs on opposite horizontal position
**Cause:** X-axis not inverted
**Fix:** Changed map() to use TOUCH_MAX_X → TOUCH_MIN_X

## Next Steps (Phase 2B)

1. **Convert remaining fish sprites** (9 more species)
2. **Add UI sprites** (coin, pellet already resized)
3. **Ty Knotts dialogue system**
4. **Species-specific sprite rendering**

## Lessons Learned

1. **Display inversion is finicky** - Sprites and primitives behave differently
2. **Touch rotation ≠ display rotation** - Need independent calibration
3. **Manual transparency more reliable** - Hardware acceleration inconsistent
4. **Document everything** - Small config changes have big impacts
5. **Test on hardware early** - Simulators don't catch display quirks

## Files Modified

- `config.h` - Inverted color constants + documentation
- `graphics.cpp` - Display config documentation + sprite rendering
- `touch.cpp` - Calibration values + mapping documentation
- `docs/HARDWARE.md` - Added verified configuration section
- `tools/resize_sprites.py` - Batch sprite resizing (auto-crop feature)
- `include/sprites/*` - New sprite header structure

## Configuration Summary (Copy-Paste Reference)

```cpp
// Display (graphics.cpp)
#define DISPLAY_INVERT false
tft.setRotation(0);

// Touch (touch.cpp)
touch.setRotation(1);
#define TOUCH_MIN_X  600
#define TOUCH_MAX_X  3600
#define TOUCH_MIN_Y  500
#define TOUCH_MAX_Y  3600
int16_t mappedX = map(p.x, 3600, 600, 0, 240);   // Inverted
int16_t mappedY = map(p.y, 500, 3600, 0, 320);   // Normal

// Colors (config.h) - All inverted via XOR 0xFFFF
#define COLOR_BLACK  0xFFFF
#define COLOR_WHITE  0x0000
```

---

**Status:** Ready for Phase 2B (remaining sprites + dialogue system)
**Hardware:** Fully calibrated and documented
**Performance:** Excellent (30 FPS, 38% flash, 7% RAM)
