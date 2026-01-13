# Changelog

All notable changes to Bass Hole will be documented in this file.

## [Unreleased]

### Phase 1: Core Gameplay - In Progress

#### Added
- Project structure and documentation
- Core game loop (fish, food, coins)
- Fish AI with hunger, growth, coin drops
- Touch input handling
- SD card save/load system
- TFT_eSPI display integration
- CYD variant documentation for community contributions
- Python tools for sprite conversion (img2code.py, dither_tool.py)

#### Hardware Testing
- TZT ESP32 LVGL 2.4" CYD (AliExpress)
  - Display: Working (debugging rotation/colors)
  - Touch: Working (debugging phantom touches)
  - SD Card: Working
  - See `docs/CYD_VARIANTS.md` for configuration details

### Planned Features

#### Phase 2: Art & Character
- [ ] 16-color dithered fish sprites
- [ ] Ty Knotts narrator with dialogue system
- [ ] Tank background art
- [ ] UI sprite elements

#### Phase 3: Enemies
- [ ] Clanker enemy type
- [ ] Cogsucker enemy type
- [ ] Tap-to-defeat mechanic

#### Phase 4: Bosses
- [ ] Boss battle framework
- [ ] Cyberstuck (tap rapidly when stuck)
- [ ] CatGPT (Simon-says pattern)
- [ ] Ant-tropic (timing windows)
- [ ] Gaagle (whack-a-mole)

#### Phase 5: Polish
- [ ] Particle effects
- [ ] Sound effects (ESP32 DAC)
- [ ] Achievement system
- [ ] High score leaderboard

---

## Version History

### 0.1.0 - Initial Development
- Initial commit with project structure
- Phase 1 gameplay code (unverified on hardware)

### 0.1.1 - Hardware Debugging
- Added backlight control for CYD variants
- Added BGR color order support
- Debugging display rotation and touch input
- Created CYD_VARIANTS.md for community hardware testing

### 0.1.2 - Touch Calibration Fix
- Fixed XPT2046 touch controller returning -4096 error values
- Added universal touch calibration for ESP32 CYD 2.4" (TZT variant)
- Fixed Y-axis inversion (touches now map correctly to screen coordinates)
- Calibration values: X: 600-3600, Y: 500-3600 (inverted)
- Touch input now accurate across all screen areas
- Updated CYD_VARIANTS.md with verified calibration data
