# Architecture Overview

This document explains how the Bass Hole codebase is organized and how the different systems interact. Use this as a guide when exploring the code or adding new features.

## High-Level Structure

```
┌─────────────────────────────────────────────────────────────┐
│                      BassHole.ino                           │
│                    (Main Entry Point)                       │
│  - setup(): Initialize all systems                          │
│  - loop(): Game loop (input → update → render)              │
└─────────────────────────────────────────────────────────────┘
          │              │              │              │
          ▼              ▼              ▼              ▼
    ┌──────────┐  ┌──────────┐  ┌──────────┐  ┌──────────┐
    │  touch   │  │   fish   │  │ graphics │  │  sdcard  │
    │  .h/.cpp │  │  .h/.cpp │  │  .h/.cpp │  │  .h/.cpp │
    └──────────┘  └──────────┘  └──────────┘  └──────────┘
          │              │              │              │
          └──────────────┴──────────────┴──────────────┘
                                │
                         ┌──────────────┐
                         │   config.h   │
                         │  (Constants) │
                         └──────────────┘
```

## File Responsibilities

### config.h
**Purpose:** Single source of truth for all configuration.

Contains:
- Hardware pin definitions
- Display colors (RGB565)
- Game constants (speeds, sizes, limits)
- Fish species data structure
- Game state enum
- Debug flags

**Why it matters:** Changing a value here affects the entire game. Want bigger fish? Change `FISH_WIDTH`. Want more coins? Change `MAX_COINS`.

### game_state.h / game_state.cpp
**Purpose:** Manages overall game state and progression.

Key structures:
```cpp
struct GameData {
    GameState state;      // Current game state (playing, paused, etc.)
    uint32_t coins;       // Player's money
    uint8_t currentLevel; // Progression
    // ... stats, flags, timing
};
```

Key functions:
- `gameStateInit()` - Set initial values, load save
- `gameStateChange()` - Transition between states
- `gameStateReset()` - Start new game
- `gameSave()` / `gameLoad()` - SD card persistence

### fish.h / fish.cpp
**Purpose:** Fish entity management and AI behavior.

Key structures:
```cpp
struct Fish {
    float x, y;           // Position
    float vx, vy;         // Velocity
    FishSpecies species;  // Type of fish
    uint8_t hunger;       // 0-100 hunger meter
    uint8_t growthStage;  // Size/value progression
    // ...
};
```

Key functions:
- `fishSpawn()` - Create new fish
- `fishUpdate()` - Movement, hunger, coin drops
- `fishFeed()` - Called when fish eats food
- `fishGetAt()` - Hit detection for taps

**AI Behavior:**
1. Fish pick random target points in tank
2. Swim toward target with smooth acceleration
3. When hungry (<50%), prioritize nearby food
4. Drop coins periodically (faster when well-fed)
5. Die if starved for too long

### food.h / food.cpp
**Purpose:** Food pellet management.

Simple system:
- Pellets spawn at tap location
- Fall downward at constant speed
- Removed when eaten by fish or hits bottom

### coins.h / coins.cpp
**Purpose:** Coin spawning and collection.

Features:
- Coins float upward slowly
- Bob side-to-side for visual interest
- Expire after timeout if not collected
- Collection adds to player's coin count

### touch.h / touch.cpp
**Purpose:** Touch input handling.

Features:
- Debouncing to prevent accidental double-taps
- Coordinate mapping from raw touch to screen pixels
- Tap detection (touch released = tap)
- Calibration support

**Important:** Touch calibration values may need adjustment per-device. See `TOUCH_MIN_X/Y` and `TOUCH_MAX_X/Y` constants.

### graphics.h / graphics.cpp
**Purpose:** All rendering operations.

Layers (bottom to top):
1. Tank background (water gradient)
2. Food pellets
3. Fish
4. Coins
5. UI overlay

**Current implementation:** Simple shapes (ellipses, circles, triangles).
**Future:** SD card sprite loading.

### sdcard.h / sdcard.cpp
**Purpose:** SD card file operations.

Features:
- Initialize SD card on SPI bus
- Read/write arbitrary files
- Game save/load with checksum validation

## Game Loop

The main loop in `BassHole.ino` follows this pattern:

```cpp
void loop() {
    // 1. TIMING
    // Calculate delta time, limit frame rate to ~30 FPS

    // 2. INPUT
    touchUpdate();        // Read touch hardware
    handleInput();        // Process taps based on game state

    // 3. UPDATE
    if (state == PLAYING) {
        fishUpdate();     // Move fish, check hunger, drop coins
        foodUpdate();     // Move food downward
        coinsUpdate();    // Float coins, check expiration
    }

    // 4. RENDER
    render();             // Draw everything based on state

    // 5. DEBUG
    // FPS counter, serial output
}
```

## Entity Pool Pattern

Fish, food, and coins all use the same pattern - a fixed-size array (pool) with active/inactive slots:

```cpp
Fish fishPool[MAX_FISH];  // Pre-allocated array
uint8_t fishCount = 0;    // Active count

Fish* fishSpawn(...) {
    // Find inactive slot
    for (int i = 0; i < MAX_FISH; i++) {
        if (!fishPool[i].active) {
            fishPool[i].active = true;
            // Initialize...
            fishCount++;
            return &fishPool[i];
        }
    }
    return nullptr;  // Pool full
}

void fishRemove(Fish* fish) {
    fish->active = false;
    fishCount--;
}
```

**Why this pattern?**
- No dynamic memory allocation (heap fragmentation is bad on embedded)
- Predictable memory usage
- Cache-friendly iteration
- Simple and fast

## State Machine

Game states control what's updated and rendered:

```
         ┌─────────┐
         │  BOOT   │ (splash screen)
         └────┬────┘
              ▼
         ┌─────────┐
    ┌────│  TITLE  │ (tap to start)
    │    └────┬────┘
    │         ▼
    │    ┌─────────┐
    │    │ PLAYING │◄──────────────┐
    │    └────┬────┘               │
    │         │                    │
    │    ┌────┴────┐         ┌─────┴─────┐
    │    ▼         ▼         │           │
    │ ┌──────┐ ┌──────┐      │           │
    │ │ SHOP │ │ BOSS │──────┘           │
    │ └──┬───┘ └──────┘                  │
    │    └───────────────────────────────┘
    │
    │    ┌──────────┐
    └───►│ GAMEOVER │ (tap to restart)
         └──────────┘
```

## Memory Considerations

ESP32 has ~520KB SRAM. Current usage estimate:

| System | Approximate Size |
|--------|------------------|
| Fish pool (10 fish) | ~600 bytes |
| Food pool (15 pellets) | ~200 bytes |
| Coin pool (20 coins) | ~400 bytes |
| Game state | ~100 bytes |
| Display buffer | ~2KB (TFT_eSPI internal) |
| **Total** | **~3-4KB** |

Plenty of headroom for sprites and additional features.

## Adding New Systems

To add a new entity type (e.g., enemies):

1. **Create header** (`enemies.h`):
   - Define data structure
   - Declare pool and functions

2. **Create implementation** (`enemies.cpp`):
   - Initialize pool
   - Update logic
   - Spawn/remove functions

3. **Add to config.h**:
   - Constants (`MAX_ENEMIES`, etc.)
   - Any new enums or stats

4. **Integrate in BassHole.ino**:
   - Call `enemiesInit()` in setup
   - Call `enemiesUpdate()` in game loop
   - Add rendering call

5. **Add to graphics.cpp**:
   - `gfxDrawEnemy()` function
   - `gfxDrawAllEnemies()` helper

## Future Architecture Plans

### Phase 2: Sprite System
- Add `sprites.h/cpp` for SD card sprite loading
- Cache frequently used sprites in RAM
- Support for animated sprite sheets

### Phase 3: Dialogue System
- Add `ty_knotts.h/cpp` for narrator
- Text rendering with word wrap
- Triggered lines based on events

### Phase 4: Audio
- Add `audio.h/cpp` for ESP32 DAC output
- Simple tone generation for 8-bit style sounds
- SD card WAV playback (optional)
