---
phase: 2
plan: 1
wave: 1
depends_on: []
files_modified:
  - sdcard/sprites/
  - assets/sprites/
autonomous: true

must_haves:
  truths:
    - "All 5 fish species converted to RGB565"
    - "Food and Coin converted to RGB565"
  artifacts:
    - "Assets on SD card in RGB565 format"
  key_links:
    - "Uses png_to_rgb565.py tool"
---

# Plan 2.1: Conversion of Existing Assets

<objective>
Convert the existing pixel art assets (provided by user in `assets/resized/`) to the RGB565 `.raw` format required by the `sd_sprites` loader.

**Purpose:** Prepare the verified assets for the rendering engine.

**Output:** 
- RGB565 `.raw` files in `sdcard/sprites/`.
</objective>

<tasks>

<task type="auto">
  <name>Convert Fish Assets</name>
  <files>sdcard/sprites/fish/</files>
  <action>
1. Create directory `sdcard/sprites/fish`.
2. Run `python tools/png_to_rgb565.py --bgr assets/resized/fish/ sdcard/sprites/fish/`
  </action>
</task>

<task type="auto">
  <name>Convert UI Assets</name>
  <files>sdcard/sprites/ui/</files>
  <action>
1. Create directory `sdcard/sprites/ui`.
2. Run `python tools/png_to_rgb565.py --bgr assets/resized/ui/ sdcard/sprites/ui/`
  </action>
</task>

</tasks>
