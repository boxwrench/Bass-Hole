# Bass-Hole Asset Conversion Tools

## ✅ VERIFIED PIPELINE (2026-01-19)

### Ground Truth Configuration

**Hardware-verified settings for ESP32-2432S028R:**
- Format: RGB565, Little-Endian
- Firmware: `setSwapBytes(true)`
- Gamma: `0x01` (set after `tft.init()`)
- Result: Accurate colors with crisp outlines

---

## Recommended Tool: `png_to_rgb565.py`

**Use this for all new sprite conversions.**

```bash
# Convert PNG to RGB565 binary
python tools/png_to_rgb565.py input.png output.rgb565

# Batch convert all PNGs in a directory
python tools/png_to_rgb565.py sprites/ --batch
```

**Features:**
- RGB565 format (NOT BGR)
- Little-Endian byte order
- Handles transparency (blends against black)
- Verified on hardware

---

## Legacy Tools

### `img2raw.py` - RGB565 Binary (VERIFIED)

**Status:** ✅ Correct format (RGB565 LE)

```bash
python tools/img2raw.py input.png output.bin
```

**Use case:** Same as `png_to_rgb565.py`, just different name

### `img2code.py` - C Array Generator

**Status:** ⚠️ Needs verification

```bash
python tools/img2code.py input.png --name sprite_name --output output.h
```

**Use case:** Embed sprites in PROGMEM (for small sprites only)

**Note:** Check if this generates RGB or BGR. May need update.

---

## DO NOT USE

### ❌ `--bgr` flag

The `--bgr` flag was used in earlier testing but is **NOT** correct for this hardware.

```bash
# WRONG - Do not use
python tools/png_to_rgb565.py input.png --bgr
```

**Why:** The hardware expects RGB565, not BGR565. Using BGR causes red/blue swap.

---

## Transparency Handling

All tools handle PNG transparency by:
1. Blending semi-transparent pixels against **BLACK** background
2. Fully transparent pixels (alpha < 128) → Magenta (0xF81F)

**In firmware:**
```cpp
#define SPRITE_TRANSPARENT_COLOR 0xF81F  // Magenta
if (pixel != SPRITE_TRANSPARENT_COLOR) {
    tft.drawPixel(x, y, pixel);
}
```

---

## Verification

To verify a conversion tool is working correctly:

1. Convert a test sprite with known colors (e.g., pure red, green, blue)
2. Load on hardware with `setSwapBytes(true)`
3. Check colors match the source PNG

**Expected results:**
- Red (255,0,0) → Displays as red
- Green (0,255,0) → Displays as green
- Blue (0,0,255) → Displays as blue

If colors are swapped, the tool is generating BGR instead of RGB.

---

## Quick Reference

| Tool | Format | Endian | Verified | Use Case |
|------|--------|--------|----------|----------|
| `png_to_rgb565.py` | RGB565 | Little | ✅ Yes | **Recommended** |
| `img2raw.py` | RGB565 | Little | ✅ Yes | Same as above |
| `img2code.py` | RGB565? | Little | ⚠️ TBD | PROGMEM arrays |

---

## Related Documentation

- `docs/PHASE2_FINDINGS.md` - Complete verified configuration
- `docs/ESP32_CYD_REFERENCE.md` - Hardware reference
- `DEVLOG.md` - Session notes on sprite color resolution
