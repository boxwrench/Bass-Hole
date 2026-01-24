# TODO

> Backlog items and quick captures

## Format
- `[ ]` = Not started
- `[x]` = Complete
- `[~]` = In progress
- `[!]` = Blocked

---

## Phase 2 - Sprite Implementation

### Immediate (Before Planning Phase 1)
- [ ] Decide background approach (tiled textures vs optimized single sprite)
- [ ] Determine if existing art assets exist or need creation
- [ ] Check if CYD tester has reusable sprite test code

### Asset Creation
- [ ] Create/source fish sprites (Rainbow Trout, Bluegill, Largemouth Bass)
- [ ] Create/source food pellet sprite
- [ ] Create/source coin sprite
- [ ] Create background tiles OR optimized background sprite
- [ ] Test sprites with dithering tool for 16-color reduction

### Technical Debt (From .gsd/ARCHITECTURE.md)
- [ ] Add gameplay GIF to README.md (line 13 placeholder)
- [ ] Create per-device touch calibration guide
- [ ] Implement full dirty rect optimization (currently partial)
- [ ] Add automated testing framework
- [ ] Clean up unused `managed_components/` (2440 files, mostly unused)

### Documentation
- [ ] Update ASSETS.md with actual sprite workflow used in Phase 2
- [ ] Document sprite dimensions and naming conventions
- [ ] Add sprite troubleshooting section

### Future Features (Post-Phase 2)
- [ ] Audio system (ESP32 DAC)
- [ ] Ty Knotts dialogue system
- [ ] Enemy system (Clankers)
- [ ] Boss battles
- [ ] Shop/upgrades
- [ ] Particle effects
- [ ] Animated sprites

---

## Quick Captures

*Use this section for ideas, questions, or items that don't fit elsewhere*

- Consider creating asset generation script to batch-process sprites
- Test sprite loading performance with different file sizes
- Investigate if LovyanGFX is faster than TFT_eSPI for sprite rendering
- Document color palette standards for consistent sprite appearance

---

*Use `/add-todo` to quickly add items, `/check-todos` to review*
