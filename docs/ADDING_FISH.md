# Tutorial: Adding a New Fish Species

This step-by-step guide walks you through adding a new fish species to Bass Hole. By the end, you'll understand how the fish system works and have a template for adding any entity.

## Overview

Adding a fish involves:
1. Adding the species to the enum
2. Defining its stats
3. Adding its visual appearance
4. Testing

**Time:** ~15 minutes
**Difficulty:** Beginner

## Step 1: Add to Species Enum

Open `src/config.h` and find the `FishSpecies` enum:

```cpp
enum FishSpecies {
    FISH_RAINBOW_TROUT = 0,
    FISH_BLUEGILL,
    FISH_SMALLMOUTH_BASS,
    FISH_CHANNEL_CATFISH,
    FISH_LARGEMOUTH_BASS,
    // Add your new fish here:
    FISH_KOKANEE_SALMON,      // <-- NEW
    FISH_SPECIES_COUNT
};
```

**Important:** Add before `FISH_SPECIES_COUNT` - this auto-updates the count.

## Step 2: Define Fish Stats

In `src/fish.cpp`, find the `FISH_DATA` array:

```cpp
const FishStats FISH_DATA[] = {
    // name,              hunger, coin, speed, stages, cost
    {"Rainbow Trout",     2,      1,    1.0f,  3,      25},
    {"Bluegill",          3,      1,    1.3f,  2,      20},
    {"Smallmouth Bass",   2,      2,    1.0f,  3,      50},
    {"Channel Catfish",   1,      4,    0.6f,  3,      75},
    {"Largemouth Bass",   2,      5,    0.8f,  3,      150},
    // Add your new fish here:
    {"Kokanee Salmon",    2,      3,    1.2f,  3,      60},  // <-- NEW
};
```

### Understanding the Stats

| Stat | Description | Typical Range |
|------|-------------|---------------|
| `name` | Display name | String |
| `hungerRate` | How fast they get hungry (higher = hungrier) | 1-5 |
| `coinValue` | Base coin drop value | 1-10 |
| `speedMult` | Movement speed multiplier | 0.5-1.5 |
| `growthStages` | How many times they grow (affects final size/value) | 2-4 |
| `cost` | Shop price in coins | 10-200 |

### Balancing Tips

- **High hunger + high coins** = High risk, high reward
- **Low speed + high coins** = Easy to manage, premium price
- **Fast fish** = More chaotic, entertaining to watch
- Keep total coin value reasonable for progression

## Step 3: Add Visual Appearance

In `src/graphics.cpp`, find `gfxDrawFish()`:

```cpp
void gfxDrawFish(Fish* fish) {
    // ... existing code ...

    // Fish body color based on species
    uint16_t bodyColor;
    switch (fish->species) {
        case FISH_RAINBOW_TROUT:
            bodyColor = tft.color565(200, 150, 150);  // Pinkish
            break;
        case FISH_BLUEGILL:
            bodyColor = tft.color565(100, 100, 200);  // Blue
            break;
        // ... other fish ...

        // Add your new fish:
        case FISH_KOKANEE_SALMON:
            bodyColor = tft.color565(180, 80, 80);    // Reddish (spawning colors)
            break;

        default:
            bodyColor = COLOR_WHITE;
    }
    // ... rest of drawing code ...
}
```

### Color Tips

Use `tft.color565(R, G, B)` where R, G, B are 0-255.

Some fish color ideas:
- **Salmon:** Reddish-pink (180, 80, 80)
- **Catfish:** Brown-gray (100, 80, 60)
- **Bass:** Green-olive (80, 120, 60)
- **Trout:** Silver-pink (200, 180, 180)
- **Bluegill:** Blue-green (60, 100, 140)

## Step 4: Make It Unlockable (Optional)

Fish can be locked until the player progresses. In `src/config.h`:

```cpp
// In GameData struct or as defines
#define FISH_UNLOCK_KOKANEE  0x20  // Bit flag for 6th fish (2^5)
```

In shop/unlock logic (when implemented):
```cpp
// Check if unlocked
bool isUnlocked = (game.fishUnlocked & FISH_UNLOCK_KOKANEE) != 0;

// Unlock the fish
game.fishUnlocked |= FISH_UNLOCK_KOKANEE;
```

## Step 5: Test Your Fish

1. **Quick test:** Modify `gameStateReset()` in `game_state.cpp`:
   ```cpp
   // Temporarily spawn your new fish for testing
   fishSpawn(FISH_KOKANEE_SALMON, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
   ```

2. **Upload and observe:**
   - Does it appear with the right color?
   - Does movement speed feel right?
   - Are coin drops balanced?
   - Does hunger work correctly?

3. **Serial debug:** Enable in `config.h`:
   ```cpp
   #define DEBUG_FISH 1
   ```

## Complete Example: Steelhead Trout

Here's a full example adding Steelhead Trout:

### config.h
```cpp
enum FishSpecies {
    FISH_RAINBOW_TROUT = 0,
    FISH_BLUEGILL,
    FISH_SMALLMOUTH_BASS,
    FISH_CHANNEL_CATFISH,
    FISH_LARGEMOUTH_BASS,
    FISH_STEELHEAD_TROUT,    // <-- NEW
    FISH_SPECIES_COUNT
};
```

### fish.cpp
```cpp
const FishStats FISH_DATA[] = {
    {"Rainbow Trout",     2,      1,    1.0f,  3,      25},
    {"Bluegill",          3,      1,    1.3f,  2,      20},
    {"Smallmouth Bass",   2,      2,    1.0f,  3,      50},
    {"Channel Catfish",   1,      4,    0.6f,  3,      75},
    {"Largemouth Bass",   2,      5,    0.8f,  3,      150},
    {"Steelhead Trout",   2,      4,    1.4f,  3,      100},  // Fast & valuable
};
```

### graphics.cpp
```cpp
case FISH_STEELHEAD_TROUT:
    bodyColor = tft.color565(160, 170, 180);  // Silver-blue
    break;
```

## Adding Special Behaviors (Advanced)

Want your fish to do something unique? Modify `fishUpdate()` in `fish.cpp`:

```cpp
// Example: Kokanee Salmon gives bonus coins seasonally
if (fish->species == FISH_KOKANEE_SALMON) {
    // Check "season" based on play time
    uint32_t minute = (game.playTime / 60000) % 4;
    if (minute == 0) {  // "Spawning season" every 4 minutes
        // Double coin drops during this minute
        value *= 2;
    }
}
```

Or add a helper function:
```cpp
// In fish.cpp
static bool fishHasSpecialBehavior(Fish* fish) {
    switch (fish->species) {
        case FISH_KOKANEE_SALMON:
            return true;  // Has seasonal bonus
        default:
            return false;
    }
}
```

## Checklist

Before submitting a PR with a new fish:

- [ ] Added to `FishSpecies` enum
- [ ] Added stats to `FISH_DATA` array
- [ ] Added color in `gfxDrawFish()`
- [ ] Stats are balanced with existing fish
- [ ] Tested on hardware or simulator
- [ ] Updated `DEVELOPMENT_STRATEGY.md` fish list (optional)

## Ideas for New Fish

From the design doc's NorCal theme:

| Fish | Suggested Stats | Notes |
|------|-----------------|-------|
| Kokanee Salmon | Med hunger, high coins, fast | Seasonal bonus mechanic |
| Steelhead Trout | Low hunger, high coins, fast | Premium fish |
| Striped Bass | Low hunger, very high coins, slow | Tank/defender role |
| Sacramento Pikeminnow | High hunger, negative coins | Invasive - must remove! |
| Sturgeon | Very low hunger, massive coins, very slow | Ultra rare |

---

*"You added a new fish? Don't let it die on day one."* - Ty Knotts
