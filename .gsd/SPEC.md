# SPEC.md — Project Specification

> **Status**: `FINALIZED`

## Vision

Transform Bass-Hole from a functional shape-based prototype into a visually polished game by implementing sprite-based graphics. Phase 1 proved the game logic works; Phase 2 makes it look like a real game by replacing all geometric shapes with proper art assets loaded from SD card.

## Goals

1. **Replace all shape-based rendering with sprite-based rendering** while maintaining identical gameplay
2. **Solve the background sprite size constraint** through tiling or optimization
3. **Leverage hardware learnings** from CYD tester project (verified SD card sprite loading)
4. **Maintain or improve performance** (target: ≥30 FPS with sprites)

## Non-Goals (Out of Scope for Phase 2)

- New gameplay features (enemies, bosses, combat, shop)
- Dialogue/narrator system (Ty Knotts)
- Sound effects or audio
- Particle effects
- Animations (single-frame sprites only for now)
- Network/WiFi features
- New fish species beyond existing 3

## Users

- **Primary:** Developers (yourself) validating sprite pipeline works in-game
- **Secondary:** Future players who will experience the visual upgrade
- **Tertiary:** ESP32 CYD community using this as a reference project

## Constraints

### Technical
- **File Size:** Background sprites at full resolution (240x320) are too large for SD card performance
- **Memory:** ESP32 SRAM limits sprite caching (must load from SD as needed)
- **Display:** RGB565 format, BGR color order (verified in CYD tester)
- **Storage:** SD card FAT32 filesystem

### Timeline
- No hard deadline, but Phase 2 should be achievable in focused development sessions

### Design
- Sprites must use 16-color dithering for authentic retro aesthetic
- NorCal fishing theme (realistic fish, not cartoon)
- Visual style: dithered realism for fish, clean pixel art for UI

## Success Criteria

Phase 2 is complete when:

- [ ] **Background renders** using either tiled textures OR optimized single sprite
- [ ] **All 3 fish species** render as sprites (Rainbow Trout, Bluegill, Largemouth Bass)
- [ ] **Food pellets** render as sprites
- [ ] **Coins** render as sprites
- [ ] **All Phase 1 gameplay** still works identically:
  - [ ] Touch input (drop food, collect coins, buy fish)
  - [ ] Fish AI (swim, hunger, seek food, drop coins)
  - [ ] Growth stages functional
  - [ ] Save/load to SD card works
  - [ ] Game over detection works
- [ ] **Performance is acceptable:** ≥25 FPS sustained (30 FPS target)
- [ ] **No visual glitches:** Sprites display with correct colors, no corruption
- [ ] **Documentation updated:** ASSETS.md reflects actual sprite workflow used

## Open Questions (To Resolve During Planning)

1. **Background approach:** Tiled textures vs optimized single sprite?
2. **Asset creation:** Generate with AI, hand-draw, or use existing art?
3. **Growth stages:** Scale single sprite or create separate sprites per stage?
4. **Sprite caching:** Which sprites stay in RAM vs load-on-demand from SD?
5. **UI sprites:** Keep text-based UI or create sprite-based buttons/overlays?

## Dependencies

- ✅ CYD tester findings (SD card PNG/RGB565 sprite loading verified)
- ✅ Asset conversion pipeline (tools/ directory scripts functional)
- 🔲 Art assets (sprites) - to be created/sourced during Phase 2
- ✅ Phase 1 game logic (working baseline)

## Risk Mitigation

| Risk | Impact | Mitigation |
|------|--------|------------|
| Sprite loading too slow | High | Pre-convert to RGB565, cache frequently used sprites |
| File size too large | Medium | Use texture tiling, aggressive dithering/optimization |
| Visual quality poor | Low | Iterate on dithering settings, adjust palette |
| Performance degradation | High | Profile early, fall back to simpler background if needed |

---

**Notes:**
- This spec focuses exclusively on sprite implementation (Phase 2)
- Future phases will add gameplay features (enemies, narrator, sound)
- CYD tester project validated technical approach; now apply to Bass-Hole
