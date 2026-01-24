---
phase: 2
plan: 2
wave: 2
depends_on: [2.1]
files_modified:
  - src/graphics.cpp
  - include/graphics.h
autonomous: true

must_haves:
  truths:
    - "Fish rendering uses sprites"
    - "Food rendering uses sprites"
    - "Coin rendering uses sprites"
    - "Geometric drawing code is removed/disabled"
  artifacts:
    - "Updated graphics.cpp"
  key_links:
    - "Uses spriteDrawTransparent from sd_sprites.h"
---

# Plan 2.2: Sprite Rendering Integration

<objective>
Update the graphics engine to render the newly sourced sprites instead of geometric shapes.

**Purpose:** Implement the core visual upgrade.

**Output:** 
- `gfxDrawFish` rendering sprite.
- `gfxDrawFood` rendering sprite.
- `gfxDrawCoin` rendering sprite.
</objective>

<context>
Load for context:
- src/graphics.cpp
- include/sd_sprites.h
</context>

<tasks>

<task type="auto">
  <name>Implement Fish Sprite Rendering</name>
  <files>src/graphics.cpp</files>
  <action>
1. Function `gfxDrawFish`:
   - Load specific sprite based on `fish->species`.
   - Use `spriteDrawTransparentFlip` based on `fish->facingRight`.
   - Center sprite on `fish->x, fish->y`.
2. Cache sprites?
   - Decision: Load on demand or cache all 5 on boot?
   - Strategy: Cache all 5 species small sprites on `gfxInit` or `fishInit`. They are small enough (~2KB each).
  </action>
  <verify>
Code compiles.
  </verify>
  <done>
Fish rendering uses sprites.
  </done>
</task>

<task type="auto">
  <name>Implement Food and Coin Rendering</name>
  <files>src/graphics.cpp</files>
  <action>
1. `gfxDrawFood`: Render pellet sprite.
2. `gfxDrawCoin`: Render coin sprite.
3. Remove old geometric code (guarded by `#if USE_SPRITES`).
  </action>
  <verify>
Code compiles.
  </verify>
  <done>
Entities render with sprites.
  </done>
</task>

</tasks>

<verification>
After all tasks, verify:
- [ ] Build succeeds
- [ ] No compilation errors
</verification>
