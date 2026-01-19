# Journal

> Development journal for Bass-Hole

## Format
Each entry records:
- Date
- What was accomplished
- Key insights
- Blockers encountered
- Next steps

---

## 2026-01-19 - GSD Project Initialization

**Session Goal:** Initialize GSD structure for Phase 2 sprite implementation

**Accomplished:**
- ✅ Ran `/map` to analyze existing codebase
- ✅ Created `.gsd/ARCHITECTURE.md` (8 core components documented)
- ✅ Created `.gsd/STACK.md` (ESP32 + PlatformIO + TFT_eSPI)
- ✅ Created `.gsd/SPEC.md` (Phase 2 sprite implementation scope)
- ✅ Created `.gsd/ROADMAP.md` (5 phases: Infrastructure → Optimization)
- ✅ Initialized GSD documentation structure

**Key Insights:**
- Phase 1 is complete and hardware-verified
- CYD tester project validated sprite loading approach (RGB565, SD card)
- Background sprite size is a constraint (need tiling or optimization)
- Entity pool pattern from Phase 1 is solid foundation
- Asset pipeline tools exist but need to be applied to real game

**Blockers:**
- None (clear path forward)

**Decisions Made:**
- Focus Phase 2 exclusively on sprite implementation (no new gameplay)
- Defer UI sprites to optional Phase 3
- Target ≥25 FPS (30 FPS ideal)

**Next Steps:**
- Run `/plan 1` to break down Phase 1 (Sprite Infrastructure) into detailed tasks
- Decide background approach (tiled vs single sprite)
- Create/source initial sprite assets (or plan asset generation)

---

## Historical Context (Pre-GSD)

### Phase 1 Development
- Implemented core gameplay with shape-based graphics
- Fish AI, hunger system, coin generation working
- Touch input calibrated and functional
- SD card save/load implemented
- Achieved ~31 FPS on TZT CYD 2.4"

### Display Configuration Research
- Resolved color issues (BGR vs RGB)
- Fixed SPI bus initialization order
- Documented gamma settings and rotation
- Created separate CYD tester project for hardware validation

### Asset Pipeline Exploration
- Built Python tools for PNG → RGB565 conversion
- Tested dithering and color reduction
- Validated sprite loading performance on hardware
- Discovered background sprite size constraints

---

*Future entries will be added as development progresses*
