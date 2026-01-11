# Asset Creation Guide

This guide covers creating, converting, and integrating visual assets for Bass Hole. The game uses two visual styles: **16-color dithered realism** for fish and environments, and **bold cartoon style** for characters.

## Visual Style Overview

### Fish & Environments: 16-Color Dithered

**Goal:** Realistic fish sprites with a retro aesthetic.

- Based on real NorCal fish species
- Dithered to 16-color palette
- Natural colors: greens, blues, browns
- CRT scanline effects (optional)

### Characters (Ty & Bosses): Cartoon

**Goal:** Expressive, exaggerated characters.

- Bold outlines
- Vibrant colors (not palette-limited)
- Exaggerated expressions
- Comic book style

### UI: Clean Pixel Art

**Goal:** Readable, functional interface.

- Simple shapes
- Clear contrast
- Consistent icon style

## The 16-Color Palette

For dithered assets, use this NorCal-inspired palette:

```
Natural Palette (hex):
#001F3F - Deep Blue (deep water)
#0074D9 - Ocean Blue (mid water)
#7FDBFF - Aqua (shallow water)
#39CCCC - Teal (vegetation)
#3D9970 - Olive (plants)
#2ECC40 - Green (algae)
#01FF70 - Lime (highlights)
#8B4513 - Brown (wood, rocks)
#D2B48C - Tan (sand)
#AAAAAA - Gray (stones)
#DDDDDD - Silver (fish scales)
#FFFFFF - White (highlights)
#000000 - Black (outlines)
#333333 - Dark Gray (shadows)
#FF851B - Orange (accent)
#FF4136 - Red (warnings)
```

Save this as `assets/palette.hex` for use with dithering tools.

## Creating Fish Sprites

### Method 1: AI Generation + Dithering

**Step 1: Generate base image**

Use an AI image generator (Google AI Studio, Midjourney, etc.):

```
Prompt for realistic fish:
"Photorealistic [Fish Name] ([Scientific Name]), side view profile,
swimming pose, isolated on white background, high detail scales,
natural lighting, reference photo style"

Example:
"Photorealistic Rainbow Trout (Oncorhynchus mykiss), side view profile,
swimming pose, isolated on white background, high detail scales,
natural lighting, reference photo style"
```

**Step 2: Clean up in image editor**
- Remove background (make transparent or solid color)
- Crop to fish bounds
- Scale to target size (e.g., 48x32 pixels for large fish)

**Step 3: Apply dithering**

Using ImageMagick:
```bash
convert input.png -colors 16 -dither FloydSteinberg \
    -remap palette.png output.png
```

Or use the project's dither tool (when implemented):
```bash
python tools/dither_tool.py input.png --palette assets/palette.hex
```

**Step 4: Manual cleanup**
- Open in pixel editor (Aseprite, Piskel, GIMP)
- Clean up dithering artifacts
- Ensure key features (eye, fins) are clear
- Check at actual display size

### Method 2: Pixel Art from Scratch

For more control, draw directly:

1. Use a pixel art tool (Aseprite recommended)
2. Set canvas to target size (24x16 for small fish)
3. Import the 16-color palette
4. Use reference photos for accuracy
5. Start with silhouette, then add detail

### Sprite Sizes

| Fish Size | Sprite Dimensions | Growth Stage |
|-----------|-------------------|--------------|
| Small | 24 x 16 px | 0 |
| Medium | 32 x 20 px | 1 |
| Large | 48 x 32 px | 2 |

## Creating Character Art

### Ty Knotts Character Sheet

Ty needs multiple expressions for dialogue:

| Expression | Usage |
|------------|-------|
| Neutral | Default, instructions |
| Angry | Fish died, mistakes |
| Smug | Player achievement |
| Facepalm | Repeated failures |
| Surprised | Boss appears |

**Prompt template:**
```
"Cartoon largemouth bass character, [expression] expression,
anthropomorphic, wearing fishing vest, thick black outlines,
comic book style, vibrant colors, upper body portrait"
```

**Size:** 64x64 or 80x80 pixels for dialogue box.

### Boss Characters

Each boss has a distinct parody design:

| Boss | Visual Style |
|------|--------------|
| Cyberstuck | Angular robot truck, glitch effects |
| CatGPT | Smug AI cat, holographic elements |
| Ant-tropic | Friendly robot with bowtie, nervous |
| Gaagle | Search bar with tentacles, ad banners |

**Size:** 80x80 to 120x120 pixels (bosses are larger than fish).

## Converting to C Arrays

ESP32 can't load PNG files directly. Sprites must be converted to C arrays of RGB565 values.

### Using img2code.py

```bash
python tools/img2code.py assets/sprites/fish/trout.png \
    --name sprite_trout \
    --output src/sprites/trout.h
```

**Output format:**
```cpp
// trout.h - Auto-generated sprite data
#ifndef SPRITE_TROUT_H
#define SPRITE_TROUT_H

#include <Arduino.h>

#define SPRITE_TROUT_WIDTH  24
#define SPRITE_TROUT_HEIGHT 16

const uint16_t sprite_trout[384] PROGMEM = {
    0x0000, 0x0000, 0x4A69, 0x4A69, // ... pixel data
};

#endif
```

### Using in Code

```cpp
#include "sprites/trout.h"

// Draw sprite
tft.pushImage(x, y, SPRITE_TROUT_WIDTH, SPRITE_TROUT_HEIGHT, sprite_trout);
```

## SD Card Sprites (Alternative)

For larger projects, store sprites on SD card instead of flash:

### File Format: Raw RGB565

```bash
python tools/img2raw.py input.png output.raw
```

### Loading at Runtime

```cpp
// In sprites.cpp (future implementation)
bool loadSprite(const char* filename, uint16_t* buffer, int w, int h) {
    File file = SD.open(filename);
    if (!file) return false;

    file.read((uint8_t*)buffer, w * h * 2);  // 2 bytes per pixel
    file.close();
    return true;
}
```

### Trade-offs

| Method | Pros | Cons |
|--------|------|------|
| Flash (C arrays) | Fast, no SD needed | Limited space, recompile to change |
| SD Card | More space, easy updates | Slower load, requires SD card |

## Animation

### Sprite Sheets

For animated fish, create a horizontal sprite sheet:

```
[Frame 0][Frame 1][Frame 2][Frame 3]
```

**Convention:** 4 frames per animation, same dimensions per frame.

### Animation Code

```cpp
// In fish struct
uint8_t animFrame;
unsigned long lastFrameTime;

// In update
if (millis() - fish->lastFrameTime > 150) {  // 150ms per frame
    fish->animFrame = (fish->animFrame + 1) % 4;
    fish->lastFrameTime = millis();
}

// In render
int frameX = fish->animFrame * SPRITE_WIDTH;
tft.pushImage(x, y, SPRITE_WIDTH, SPRITE_HEIGHT,
              &spriteSheet[frameX * SPRITE_HEIGHT]);
```

## Background Art

### Tank Backgrounds

**Size:** 240 x 240 pixels (tank area)

**Layers:**
1. Water gradient (can be code-generated)
2. Background elements (rocks, plants) - sprites
3. Foreground decorations - sprites

**Prompt for AI generation:**
```
"Underwater scene, Sierra Nevada mountain lake floor, rocks and
aquatic plants, natural lighting filtering through water,
no fish, game background art style"
```

Then dither to 16 colors and tile if needed.

## Tool Setup

### Required Software

- **Image Editor:** GIMP (free), Aseprite ($20, worth it), or Piskel (free, web-based)
- **Python 3:** For conversion scripts
- **ImageMagick:** For batch processing (optional)

### Project Tools (tools/ folder)

| Script | Purpose |
|--------|---------|
| `img2code.py` | Convert PNG to C array |
| `img2raw.py` | Convert PNG to raw RGB565 |
| `dither_tool.py` | Apply palette dithering |
| `spritesheet.py` | Combine frames into sheet |

## Asset Checklist

### Phase 1 (Minimum Viable)
- [ ] Rainbow Trout sprite (3 sizes)
- [ ] Food pellet sprite
- [ ] Coin sprite (with animation)
- [ ] Basic UI elements

### Phase 2 (Characters)
- [ ] Ty Knotts expressions (5 poses)
- [ ] Dialogue box frame
- [ ] Speech bubble

### Phase 3 (Enemies)
- [ ] Clanker sprite
- [ ] Cogsucker sprite

### Phase 4 (Bosses)
- [ ] Cyberstuck sprite + animations
- [ ] CatGPT sprite + animations
- [ ] Ant-tropic sprite + animations
- [ ] Gaagle sprite + animations

### Phase 5 (Polish)
- [ ] All fish species (10 total, 3 sizes each)
- [ ] Particle effects (bubbles, sparkles)
- [ ] Background variations

## Resources

- [Lospec Palette List](https://lospec.com/palette-list) - Color palette inspiration
- [OpenGameArt](https://opengameart.org/) - Free game assets
- [Aseprite](https://www.aseprite.org/) - Best pixel art tool
- [Piskel](https://www.piskelapp.com/) - Free web-based pixel editor

---

*"That fish looks like it was drawn by a five-year-old. I love it."* - Ty Knotts (on early placeholder art)
