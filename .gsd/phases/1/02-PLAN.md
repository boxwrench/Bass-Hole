---
phase: 1
plan: 2
wave: 2
depends_on: [1.1]
files_modified:
  - include/config.h
  - include/graphics.h
  - src/graphics.cpp
  - sdcard/backgrounds/
autonomous: false

must_haves:
  truths:
    - "Background renders at acceptable FPS (≥25 FPS target)"
    - "Background approach is decided and implemented"
    - "File size constraint is solved"
  artifacts:
    - "Background assets exist on SD card"
    - "gfxDrawTank() uses sprite-based background"
  key_links:
    - "Background strategy balances visual quality and performance"
---

# Plan 1.2: Background Strategy & Implementation

<objective>
Solve the background sprite size constraint and implement the chosen approach. CYD tester found full-resolution backgrounds (240x320) are too large for good SD performance. This plan evaluates and implements either texture tiling or optimized single sprite.

**Purpose:** Establish background rendering that meets performance targets

**Output:**
- Decision documented (tiled vs optimized single sprite)
- Background assets created and on SD card  
- `gfxDrawTank()` updated to use sprite-based background
- Performance profiled (FPS measurement)
</objective>

<context>
Load for context:
- .gsd/SPEC.md (≥25 FPS target, background constraint)
- .gsd/phases/1/01-PLAN.md (sprite loading system from Plan 1.1)
- include/graphics.h (gfxDrawTank function)
- src/graphics.cpp (current background rendering)  
- include/config.h (SCREEN_WIDTH, SCREEN_HEIGHT, tank dimensions)

**Constraint:** Full 240x320 RGB565 sprite = 150KB, too slow to load frequently
**Options:** 
A) Texture tiling (smaller repeating tiles)
B) Optimized single sprite (aggressive dithering, smaller dimensions with scaling)
</context>

<tasks>

<task type="checkpoint:decision">
  <name>Choose background approach</name>
  <files>N/A (decision point)</files>
  <action>
**Decision Point:** Choose background rendering strategy:

**Option A: Texture Tiling**
- Create 64x64 water texture tile (~8KB)
- Render by repeating tile across screen  
- Add land/vegetation tiles for variety
- **Pros:** Very small file size, flexible composition, retro aesthetic
- **Cons:** More complex rendering logic, potential visible seams

**Option B: Optimized Single Sprite**
- Create 240x320 background, aggressively dither to 4-8 colors
- Or create smaller (e.g., 120x160) and scale 2x at render time
- **Pros:** Simple rendering (one pushImage call), no seams
- **Cons:** Less flexible, still larger file than tiles

**Option C: Hybrid**
- Small optimized background (e.g., 240x200) for water area only
- Texture tiles for tank edges/borders
- **Pros:** Balance of quality and size
- **Cons:** Medium complexity

**Recommended:** Start with Option A (tiling) - most retro-authentic, smallest files, proven in many embedded games.

**USER DECISION REQUIRED:** Which option do you want to pursue?
  </action>
  <verify>
User selects Option A, B, or C
  </verify>
  <done>
Background approach decided and documented in JOURNAL.md
  </done>
</task>

<task type="auto">
  <name>Create background assets</name>
  <files>sdcard/backgrounds/, assets/backgrounds/ (source art)</files>
  <action>
Based on decision from previous task:

**If Option A (Tiling):**
1. Create water texture (64x64 PNG) - blue gradient with subtle noise
2. Convert to RGB565: `python tools/png_to_rgb565.py assets/backgrounds/water_tile.png sdcard/backgrounds/water.raw`
3. Optional: Create land tile, vegetation tile
4. Test file size (should be ~8KB per tile)

**If Option B (Single Sprite):**
1. Create/source pond background art (240x320 PNG or smaller)
2. Apply aggressive dithering: `python tools/dither_tool.py --colors 8 input.png output.png`
3. Convert to RGB565: `python tools/png_to_rgb565.py output.png sdcard/backgrounds/tank_bg.raw`
4. Test file size (target <30KB if possible)

**If Option C (Hybrid):**
1. Create optimized water area sprite (240x200)
2. Create border/edge tiles (64x64)
3. Convert all to RGB565

**AVOID:**
- Full-color, unoptimized images (file size too large)
- Transparency (blend to solid color during conversion)

**Tools:** Use existing Python scripts in `tools/` directory
  </action>
  <verify>
Background asset files exist in `sdcard/backgrounds/` directory, file sizes are within target
  </verify>
  <done>
Background assets created, converted to RGB565, ready to load
  </done>
</task>

<task type="auto">
  <name>Update gfxDrawTank to use sprite background</name>
  <files>include/graphics.h, src/graphics.cpp, include/config.h</files>
  <action>
Modify `gfxDrawTank()` to render sprite-based background:

**If Option A (Tiling):**
```cpp
// In gfxDrawTank():
Sprite* waterTile = spriteLoad("backgrounds/water.raw", 64, 64);
for (int y = 0; y < SCREEN_HEIGHT; y += 64) {
    for (int x = 0; x < SCREEN_WIDTH; x += 64) {
        spriteDraw(waterTile, x, y);
    }
}
spriteUnload(waterTile); // Or cache if performance needs it
```

**If Option B (Single Sprite):**
```cpp
// In gfxDrawTank():
Sprite* tankBg = spriteLoad("backgrounds/tank_bg.raw", SCREEN_WIDTH, SCREEN_HEIGHT);
spriteDraw(tankBg, 0, 0);
spriteUnload(tankBg);
```

**Add to config.h:**
- Constants for background sprite dimensions
- Path defines for sprite filenames

**AVOID:**
- Loading background every frame (only on state entry or when needed)
- Keeping large sprites in RAM permanently (load/unload or cache strategically)

**Note:** Current code draws background once on state entry - maintain this pattern
  </action>
  <verify>
Build succeeds, upload to hardware, background renders
  </verify>
  <done>
gfxDrawTank() uses sprite-based background, no more gradient/fill code
  </done>
</task>

<task type="checkpoint:human-verify">
  <name>Profile FPS and verify visual quality</name>
  <files>N/A (hardware testing)</files>
  <action>
**Upload firmware and measure:**
1. Enable DEBUG_FPS in config.h
2. Observe FPS counter during gameplay
3. Note if FPS ≥25 sustained (target met)
4. Visual check: Does background look good? Any seams/artifacts?

**If FPS <25:**
- Consider caching background sprite in RAM (trade memory for speed)
- Try smaller tiles (32x32 instead of 64x64)
- Reduce color depth further (4-color dither)

**If visual quality poor:**
- Adjust dithering settings
- Try different tile size
- Consider hybrid approach

**Decision point:** Is performance acceptable to proceed?
  </action>
  <verify>
User confirms ≥25 FPS and acceptable visual quality
  </verify>
  <done>
Background rendering meets performance target, visual quality approved
  </done>
</task>

</tasks>

<verification>
After all tasks, verify:
- [ ] Background approach decided and implemented
- [ ] Background assets on SD card (optimized file size)
- [ ] gfxDrawTank() renders sprite background
- [ ] FPS ≥25 with background rendering
- [ ] Visual quality acceptable
- [ ] Ready for Phase 2 (adding game entity sprites)
</verification>

<success_criteria>
- [ ] Background renders sprite-based (no more simple gradient)
- [ ] Performance target met (≥25 FPS)
- [ ] File size constraint solved  
- [ ] Approach documented in DECISIONS.md
- [ ] Foundation ready for entity sprites in Phase 2
</success_criteria>
