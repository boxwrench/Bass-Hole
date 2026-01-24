---
phase: 1
plan: 1
wave: 1
depends_on: []
files_modified:
  - include/sprites.h
  - src/sprites.cpp
autonomous: true

must_haves:
  truths:
    - "Sprite files can be loaded from SD card into RAM"
    - "RGB565 format sprites render correctly with BGR color order"
    - "Background can be rendered without performance degradation"
  artifacts:
    - "include/sprites.h exists with sprite loading API"
    - "src/sprites.cpp implements SD card sprite loading"
    - "Test sprite renders on screen"
  key_links:
    - "Sprite loader uses SD library and TFT_eSPI pushImage()"
    - "Memory budget considered for sprite caching"
---

# Plan 1.1: Sprite Loading Infrastructure

<objective>
Create the sprite loading system that reads RGB565 sprite files from SD card and renders them to the display. This provides the foundation for all sprite-based rendering in subsequent phases.

**Purpose:** Enable SD card sprite loading without reinventing what was learned in CYD tester project

**Output:** 
- `sprites.h` / `sprites.cpp` module with sprite loading API
- Working test demonstration (load + render a test sprite)
- Confirmed RGB565/BGR configuration works correctly
</objective>

<context>
Load for context:
- .gsd/SPEC.md (Phase 2 goals and constraints)
- .gsd/ARCHITECTURE.md (entity pool pattern, SPI initialization order)
- .gsd/STACK.md (TFT_eSPI configuration, SD library)
- .gsd/DECISIONS.md (ADR-004: RGB565 pre-conversion rationale)
- include/graphics.h (existing rendering functions)
- include/sdcard.h (SD card initialization)
- include/config.h (constants, colors, pin definitions)

**CYD Tester Learnings:**
- RGB565 raw files load faster than PNG decoding
- BGR color order (`TFT_RGB_ORDER=1`) required for correct colors
- Alpha blending done during PNG→RGB565 conversion (black background)
- `pushImage()` is the correct TFT_eSPI function for raw RGB565 data
</context>

<tasks>

<task type="auto">
  <name>Create sprite struct and loading API</name>
  <files>include/sprites.h, src/sprites.cpp</files>
  <action>
Create sprite management system:

**Header (include/sprites.h):**
- Define `Sprite` struct with fields: width, height, data pointer, filename
- Declare `spriteLoad()` function: loads .raw file from SD → RAM buffer
- Declare `spriteUnload()` function: frees RAM buffer
- Declare `spriteDraw()` function: renders sprite to display at (x, y)
- Declare `sprite Init()` function: sets up sprite system

**Implementation (src/sprites.cpp):**
- `spriteLoad()`: Use SD library to read .raw file, allocate buffer, store RGB565 data
- `spriteDraw()`: Use TFT_eSPI `pushImage(x, y, width, height, data)` to render
- Handle file-not-found errors gracefully (return null/false, log to serial if DEBUG_SERIAL)
- **Memory management:** Consider max sprite size limit (e.g., 64x64 = 8KB)

**AVOID:**
- PNG decoding (use pre-converted RGB565 .raw files per ADR-004)
- PROGMEM embedding (use SD card for flexibility)
- Transparency handling in this task (defer to later if needed)

**Reference:** CYD tester project validated this approach works
  </action>
  <verify>
Build succeeds with `pio run` (no compilation errors)
  </verify>
  <done>
Sprite loading API exists with clear function signatures
  </done>
</task>

<task type="auto">
  <name>Create test sprite and loading test</name>
  <files>src/main.cpp</files>
  <action>
Add temporary test code to validate sprite loading:

**In `setup()` after `gfxInit()`:**
1. Call `spriteInit()` 
2. Load a small test sprite (create `test_sprite.raw` 32x32 RGB565 file)
   - Can generate with Python: `tools/generate_test_png.py` → `tools/png_to_rgb565.py`
   - Use simple solid color or pattern for verification
3. Draw sprite to center of screen using `spriteDraw()`
4. Print success/failure to Serial

**AVOID:**
- Integrating with game entities yet (just test the system)
- Complex sprites (keep it simple: solid color square or checkerboard)

**Note:** This test code will be removed after Phase 1 verification
  </action>
  <verify>
Upload to hardware, observe test sprite renders correctly on screen at expected position
  </verify>
  <done>
Test sprite visible on display, Serial confirms successful load, colors are correct (not inverted/swapped)
  </done>
</task>

<task type="checkpoint:human-verify">
  <name>Verify sprite rendering on hardware</name>
  <files>N/A (hardware test)</files>
  <action>
**Upload firmware and visually verify:**
1. Test sprite appears on screen
2. Colors are correct (not washed out, not color-swapped)
3. Position is where expected (center of screen)
4. Serial output shows successful file load

**If colors are wrong:**
- Check `TFT_RGB_ORDER` in platformio.ini (should be 1 for BGR)
- Verify sprite conversion used correct byte order

**If sprite doesn't appear:**
- Check SD card has test_sprite.raw in root directory
- Verify SD init succeeded (Serial output)
- Check file size matches width * height * 2 bytes
  </action>
  <verify>
Visual confirmation on physical CYD hardware
  </verify>
  <done>
Test sprite renders correctly with proper colors, proving sprite loading system works
  </done>
</task>

</tasks>

<verification>
After all tasks, verify:
- [ ] spriteLoad() successfully reads RGB565 files from SD card
- [ ] spriteDraw() renders sprites to display
- [ ] Colors are correct (BGR order working)
- [ ] Build succeeds with no warnings
- [ ] Test sprite visible on hardware
</verification>

<success_criteria>
- [x] Sprite loading infrastructure exists (sprites.h/cpp)
- [x] Test sprite loads and renders correctly on hardware
- [x] RGB565/BGR configuration confirmed working
- [x] Memory allocation/deallocation handled safely
- [x] Foundation ready for Phase 2 (replacing game entity shapes)
</success_criteria>
