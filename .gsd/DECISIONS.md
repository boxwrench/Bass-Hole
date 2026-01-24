# Decisions

> Architectural Decision Records (ADRs) for Bass-Hole

## Format

Each decision follows this structure:
- **Date:** When the decision was made
- **Status:** Proposed | Accepted | Deprecated | Superseded
- **Context:** What prompted this decision
- **Decision:** What was decided
- **Consequences:** Impact and trade-offs

---

## ADR-001: Use Entity Pool Pattern for Game Objects
**Date:** Pre-GSD (Phase 1 implementation)  
**Status:** Accepted  

**Context:** ESP32 has limited SRAM and dynamic allocation can cause heap fragmentation on embedded systems.

**Decision:** Use fixed-size arrays (pools) for all game entities (fish, food, coins) with active/inactive flags instead of dynamic allocation.

**Consequences:**
- ✅ Predictable memory usage
- ✅ No heap fragmentation
- ✅ Cache-friendly iteration
- ❌ Hard limits on entity counts (MAX_FISH=10, MAX_FOOD=15, MAX_COINS=20)
- ❌ Wasted memory if pools not fully utilized

---

## ADR-002: Initialize Touch Before Display
**Date:** Pre-GSD (Phase 1 hardware testing)  
**Status:** Accepted  

**Context:** SPI bus conflicts caused touch initialization failures when display initialized first.

**Decision:** Always initialize touch (`touchInit()`) before display (`gfxInit()`) in `setup()`.

**Consequences:**
- ✅ Reliable touch initialization
- ✅ No SPI bus conflicts
- ❌ Must document this order for contributors
- ℹ️ Found through hardware debugging

---

## ADR-003: Use BGR Color Order for Display
**Date:** Pre-GSD (Phase 1 display config)  
**Status:** Accepted  

**Context:** Initial display showed incorrect colors (gold appeared blue).

**Decision:** Set `TFT_RGB_ORDER=1` in build flags to use BGR instead of RGB.

**Consequences:**
- ✅ Correct color rendering on CYD hardware
- ✅ Matches most CYD board configurations
- ℹ️ Documented in CYD_VARIANTS.md

---

## ADR-004: Asset Pipeline - RGB565 Pre-conversion
**Date:** Pre-GSD (CYD tester validation)  
**Status:** Accepted  

**Context:** PNG decoding on ESP32 is slower than raw RGB565 loading. File size is critical for SD card performance.

**Decision:** Pre-convert all sprites to RGB565 raw format (`.raw` files) using Python tools. Load directly via `pushImage()`.

**Consequences:**
- ✅ Faster sprite loading (no decompression)
- ✅ Simpler rendering code
- ❌ Larger file sizes than PNG (no compression)
- ❌ Requires asset conversion step in workflow
- ℹ️ Tools: `png_to_rgb565.py` in `tools/` directory

---

---

## ADR-005: Background Rendering Strategy - Texture Tiling
**Date:** 2026-01-19
**Status:** Accepted

**Context:** Full-resolution background sprites (240x320) in RGB565 format take up 150KB. Loading these from the SD card is relatively slow and creates a performance bottleneck during state transitions and dirty rect restoration.

**Decision:** We will use **Texture Tiling** for the background environment.

**Consequences:**
- ✅ **Performance:** A single 32x32 water tile (2KB) is loaded into RAM once when entering the playing state. Tiling this across the 240x240 tank area is extremely fast and allows for efficient dirty rect restoration without SD card reads during the game loop.
- ✅ **Visuals:** Uses the "Grass-Sand-Water TileSet" by TheWaffle (OpenGameArt).
- ✅ **RAM usage:** Consumes only 2KB of RAM for the tile data.
- ✅ **Scalability:** Future background variety (reeds, rocks, sand) can be added by loading additional small tiles.
- ℹ️ Implementation uses `sd_sprites.h` loader.

---

## Future Decisions
