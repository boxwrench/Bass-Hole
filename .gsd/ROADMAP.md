# ROADMAP.md

> **Current Phase**: Phase 1 (Sprite Infrastructure)
> **Milestone**: Phase 2 - Sprite-Based Rendering

## Must-Haves (from SPEC)

- [ ] Background rendered (tiled or optimized sprite)
- [ ] All 3 fish species as sprites
- [ ] Food pellet sprites
- [ ] Coin sprites
- [ ] Phase 1 gameplay parity maintained
- [ ] ≥25 FPS performance

## Phases

### Phase 1: Sprite Infrastructure
**Status**: ✅ Complete  
**Objective**: Set up sprite loading system and decide background approach

**Tasks:**
- Decide background strategy (tiled vs single optimized sprite)
- Implement sprite loader wrapper (SD card → display)
- Add sprite struct/registry for asset management
- Test background rendering performance
- Profile memory usage with sprite caching

**Deliverable:** Sprite loading system functional, background displays

**Requirements:** Built on CYD tester learnings (RGB565, BGR color order)

---

### Phase 2: Core Game Sprites
**Status**: ✅ Complete
**Objective**: Replace shapes with sprites for main game entities

**Tasks:**
- Create/source fish sprites (3 species, possibly 3 growth stages each)
- Create food pellet sprite
- Create coin sprite
- Integrate sprites into existing render functions (replace `gfxDrawFish`, etc.)
- Handle sprite positioning/centering (sprites have dimensions vs point-based shapes)
- Test collision detection still works with sprite bounds

**Deliverable:** Fish, food, coins all render as sprites

**Dependencies:** Phase 1 sprite loader must be working

---

### Phase 3: UI Enhancement (Optional)
**Status**: ⬜ Not Started  
**Objective**: Polish UI with sprite-based elements

**Tasks:**
- Create button sprites (buy fish button)
- Create UI overlay/panel sprites  
- Create coin counter icon
- Replace text-based UI where appropriate
- Ensure touch hit detection works with sprite-based buttons

**Deliverable:** Polished sprite-based UI

**Dependencies:** Phase 2 complete

**Note:** This phase is optional; can ship Phase 2 with text-based UI intact

---

### Phase 4: Asset Pipeline Documentation
**Status**: ⬜ Not Started  
**Objective**: Document the actual sprite workflow used

**Tasks:**
- Update ASSETS.md with real workflow (PNG → dithering → RGB565 → SD card)
- Document sprite dimensions and naming conventions
- Create asset generation script (if needed)
- Add sprite troubleshooting section
- Update CYD_VARIANTS.md if any hardware-specific sprite issues found

**Deliverable:** Complete sprite documentation for contributors

**Dependencies:** Phases 1-2 complete (know what actually worked)

---

### Phase 5: Performance Optimization & Polish
**Status**: ⬜ Not Started  
**Objective**: Hit 30 FPS target and fix any visual issues

**Tasks:**
- Profile frame timing with all sprites loaded
- Optimize sprite caching strategy (which sprites stay in RAM)
- Fix any sprite color/transparency issues
- Smooth any rendering glitches (flicker, tearing)
- Test on multiple CYD boards if available
- Final save/load testing with sprite-based game

**Deliverable:** Stable 30 FPS, no visual artifacts, hardware-verified

**Success Criteria:** All Phase 2 SPEC.md criteria met

---

## Future Milestones (Post-Phase 2)

### Milestone: Phase 3 - Gameplay Expansion
- Enemy system (Clankers)
- Boss battles
- Shop/upgrades
- New fish species

### Milestone: Phase 4 - Polish & Narrative
- Ty Knotts dialogue system
- Sound effects (ESP32 DAC)
- Particle effects
- Animated sprites

### Milestone: Phase 5 - Release
- WiFi features (leaderboards, OTA updates)
- Multiple levels/tanks
- Achievement system
- Full documentation for contributors

---

## Progress Tracking

**Current Phase:** Phase 1  
**Completed Phases:** 0 / 5  
**Overall Progress:** 0%  

**Next Action:** Run `/plan 1` to break down Phase 1 into detailed tasks

---

## Notes

- Each phase builds on the previous (sequential dependencies)
- Phase 3 (UI Enhancement) is optional and can be skipped
- Hardware validation via CYD tester gives high confidence in approach
- Asset creation (art) will happen in parallel with implementation
