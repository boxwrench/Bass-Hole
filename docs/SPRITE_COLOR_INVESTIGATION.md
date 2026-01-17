# Sprite Color Investigation Notes

## Current Status (v0.2.1-color-fix)

### ✅ What's Working

- **UI colors**: Perfect (gold, green, white all correct)
- **Sprite hue**: Correct (not inverted, right color family)
- **Performance**: 30 FPS maintained
- **Ghosting**: Completely eliminated

### ⚠️ Current Issue

- **Sprites appear washed out** - correct colors but low saturation/vibrancy

## Possible Causes of Washed-Out Appearance

### 1. RGB565 Bit Depth Reduction

**What it is:** RGB565 reduces 24-bit color (16M colors) to 16-bit (64K colors)

- Red: 8-bit → 5-bit (loses 3 bits)
- Green: 8-bit → 6-bit (loses 2 bits)
- Blue: 8-bit → 5-bit (loses 3 bits)

**Impact:** Subtle gradients and saturated colors get quantized

**Check:** Compare source PNG to displayed sprite visually

**Fix:** Boost saturation in source images before conversion

### 2. PNG Source Images

**What it is:** Original sprite PNGs may already be low saturation

**Check:** Open source PNGs in image editor and check HSL values

**Fix:** Increase saturation in Photoshop/GIMP before conversion

### 3. Gamma Correction

**What it is:** Display gamma may not match image gamma

**Check:** Test with known-saturated test image

**Fix:** Add gamma correction in `img2code.py` or display init

### 4. Color Quantization in Conversion

**What it is:** `img2code.py` may be reducing color range during RGB565 conversion

**Current conversion (img2code.py:32):**

```python
def rgb_to_rgb565(r, g, b):
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)
```

**Fix options:**

- Pre-process with saturation boost before conversion
- Add dithering to smooth color transitions
- Test alternate RGB565 conversion formulas

### 5. Display Settings

**What it is:** Physical display may have low contrast/saturation

**Check:** Test with known vibrant test pattern

**Fix:** Hardware limitation (no software fix)

## Next Steps to Investigate

### Quick Tests (in order)

1. **View source PNG** - Check if sprites are already washed out
2. **Create test pattern** - Generate highly saturated RGB test sprite
3. **Compare UI vs sprites** - UI is vibrant, why aren't sprites?
4. **Test with boosted saturation** - Increase source PNG saturation +30%

### If Problem Persists

- Add saturation boost to `img2code.py`
- Implement color multiplication/adjustment in sprite rendering
- Consider switching to different color space for conversion

## Reference: Working Configuration

```cpp
// graphics.cpp - Current working settings
#define DISPLAY_INVERT false       // UI correct
tft.setSwapBytes(true);            // PROGMEM byte order
// No manual byte swap in drawPixel

// platformio.ini - Display config
-DILI9341_DRIVER=1                 // Correct driver
-DTFT_RGB_ORDER=1                  // RGB (not BGR)
```

## Color Test Results

| Component         | Hue        | Saturation    | Brightness | Status    |
| ----------------- | ---------- | ------------- | ---------- | --------- |
| UI (primitives)   | ✅ Correct | ✅ Vibrant    | ✅ Good    | Perfect   |
| Sprites (PROGMEM) | ✅ Correct | ⚠️ Washed out | ⚠️ Dim     | Needs fix |

## Documentation

- Full analysis in DEVLOG.md (2026-01-16 session)
- Git tag: `v0.2.1-color-fix`
- Commit: `c6b75ca`
