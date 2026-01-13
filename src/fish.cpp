#include "fish.h"
#include "food.h"
#include "coins.h"
#include "game_state.h"
#include <math.h>

// Fish stats table
const FishStats FISH_DATA[] = {
    // name,              hunger, coin, speed, stages, cost
    {"Rainbow Trout",     2,      1,    1.0f,  3,      25},
    {"Bluegill",          3,      1,    1.3f,  2,      20},
    {"Smallmouth Bass",   2,      2,    1.0f,  3,      50},
    {"Channel Catfish",   1,      4,    0.6f,  3,      75},
    {"Largemouth Bass",   2,      5,    0.8f,  3,      150}
};

// Fish pool
Fish fishPool[MAX_FISH];
uint8_t fishCount = 0;

// Internal helpers
static void fishUpdateMovement(Fish* fish, unsigned long deltaTime);
static void fishUpdateHunger(Fish* fish, unsigned long deltaTime);
static void fishCheckFood(Fish* fish);
static void fishDropCoin(Fish* fish);
static void fishPickNewTarget(Fish* fish);

void fishInit() {
    for (int i = 0; i < MAX_FISH; i++) {
        fishPool[i].active = false;
    }
    fishCount = 0;
}

void fishUpdate(unsigned long deltaTime) {
    unsigned long now = millis();

    for (int i = 0; i < MAX_FISH; i++) {
        Fish* fish = &fishPool[i];
        if (!fish->active) continue;

        // Update hunger
        fishUpdateHunger(fish, deltaTime);

        // Check if fish dies from starvation
        if (fish->hunger == 0) {
            unsigned long timeSinceLastFed = now - fish->lastFed;
            if (timeSinceLastFed > FISH_STARVE_TIME) {
                fishRemove(fish);
                game.fishLost++;
#if DEBUG_SERIAL
                Serial.println("Fish starved!");
#endif
                continue;
            }
        }

        // Check for nearby food
        fishCheckFood(fish);

        // Update movement
        fishUpdateMovement(fish, deltaTime);

        // Drop coins periodically (based on growth stage)
        if (fish->hunger > 50) {  // Only drop coins if not too hungry
            unsigned long coinInterval = 5000 - (fish->growthStage * 1000);  // Faster as they grow
            if (now - fish->lastCoinDrop > coinInterval) {
                fishDropCoin(fish);
                fish->lastCoinDrop = now;
            }
        }

        // Update animation frame
        if (now - fish->lastFrameTime > 200) {
            fish->frame = (fish->frame + 1) % 4;
            fish->lastFrameTime = now;
        }
    }
}

Fish* fishSpawn(FishSpecies species, float x, float y) {
    // Find inactive slot
    Fish* fish = nullptr;
    for (int i = 0; i < MAX_FISH; i++) {
        if (!fishPool[i].active) {
            fish = &fishPool[i];
            break;
        }
    }

    if (!fish) {
#if DEBUG_SERIAL
        Serial.println("Fish pool full!");
#endif
        return nullptr;
    }

    // Initialize fish
    fish->active = true;
    fish->species = species;
    fish->x = x;
    fish->y = y;
    fish->vx = 0;
    fish->vy = 0;
    fishPickNewTarget(fish);

    fish->growthStage = 0;
    fish->hunger = FISH_HUNGER_MAX;
    fish->lastFed = millis();
    fish->lastCoinDrop = millis();

    fish->frame = 0;
    fish->facingRight = (random(2) == 0);
    fish->lastFrameTime = millis();

    fish->tint = 0;  // No tint for now

    fishCount++;

#if DEBUG_SERIAL
    Serial.print("Spawned: ");
    Serial.println(FISH_DATA[species].name);
#endif

    return fish;
}

void fishRemove(Fish* fish) {
    if (fish && fish->active) {
        fish->active = false;
        fishCount--;
    }
}

void fishFeed(Fish* fish) {
    if (!fish || !fish->active) return;

    fish->hunger = FISH_HUNGER_MAX;
    fish->lastFed = millis();
    game.fishFed++;

    // Check for growth
    const FishStats* stats = &FISH_DATA[fish->species];
    if (fish->growthStage < stats->growthStages - 1) {
        // Random chance to grow when fed
        if (random(100) < 20) {  // 20% chance
            fish->growthStage++;
#if DEBUG_SERIAL
            Serial.print(stats->name);
            Serial.print(" grew to stage ");
            Serial.println(fish->growthStage);
#endif
        }
    }
}

bool fishIsHungry(Fish* fish) {
    return fish && fish->active && fish->hunger < 30;
}

Fish* fishGetAt(int16_t screenX, int16_t screenY) {
    // Check from front to back (last drawn = front)
    for (int i = MAX_FISH - 1; i >= 0; i--) {
        Fish* fish = &fishPool[i];
        if (!fish->active) continue;

        // Simple bounding box check
        float halfW = FISH_WIDTH / 2.0f * (1.0f + fish->growthStage * 0.3f);
        float halfH = FISH_HEIGHT / 2.0f * (1.0f + fish->growthStage * 0.3f);

        if (screenX >= fish->x - halfW && screenX <= fish->x + halfW &&
            screenY >= fish->y - halfH && screenY <= fish->y + halfH) {
            return fish;
        }
    }
    return nullptr;
}

uint8_t fishGetCount() {
    return fishCount;
}

// ============================================================================
// INTERNAL FUNCTIONS
// ============================================================================

static void fishPickNewTarget(Fish* fish) {
    // Pick random point in tank
    fish->targetX = random(TANK_LEFT + FISH_WIDTH, TANK_RIGHT - FISH_WIDTH);
    fish->targetY = random(TANK_TOP + FISH_HEIGHT, TANK_BOTTOM - FISH_HEIGHT);
}

static void fishUpdateMovement(Fish* fish, unsigned long deltaTime) {
    const FishStats* stats = &FISH_DATA[fish->species];
    float speed = FISH_SPEED_MIN + (FISH_SPEED_MAX - FISH_SPEED_MIN) * stats->speedMult;

    // Calculate direction to target
    float dx = fish->targetX - fish->x;
    float dy = fish->targetY - fish->y;
    float dist = sqrtf(dx * dx + dy * dy);

    // Pick new target if close enough
    if (dist < 10) {
        fishPickNewTarget(fish);
        return;
    }

    // Normalize and apply speed
    float moveX = (dx / dist) * speed;
    float moveY = (dy / dist) * speed;

    // Apply some smoothing/inertia
    fish->vx = fish->vx * 0.9f + moveX * 0.1f;
    fish->vy = fish->vy * 0.9f + moveY * 0.1f;

    // Update position
    fish->x += fish->vx;
    fish->y += fish->vy;

    // Update facing direction
    if (fabsf(fish->vx) > 0.1f) {
        fish->facingRight = (fish->vx > 0);
    }

    // Clamp to tank bounds
    float halfW = FISH_WIDTH / 2.0f;
    float halfH = FISH_HEIGHT / 2.0f;
    fish->x = constrain(fish->x, TANK_LEFT + halfW, TANK_RIGHT - halfW);
    fish->y = constrain(fish->y, TANK_TOP + halfH, TANK_BOTTOM - halfH);
}

static void fishUpdateHunger(Fish* fish, unsigned long deltaTime) {
    const FishStats* stats = &FISH_DATA[fish->species];

    // Decrease hunger over time
    static unsigned long hungerTimer = 0;
    hungerTimer += deltaTime;

    if (hungerTimer >= 1000) {  // Every second
        hungerTimer = 0;
        if (fish->hunger > 0) {
            fish->hunger = max(0, fish->hunger - stats->hungerRate);
        }
    }
}

static void fishCheckFood(Fish* fish) {
    extern Food foodPool[];

    float eatRadius = FISH_WIDTH / 2.0f + FOOD_SIZE;

    for (int i = 0; i < MAX_FOOD; i++) {
        Food* food = &foodPool[i];
        if (!food->active) continue;

        float dx = food->x - fish->x;
        float dy = food->y - fish->y;
        float dist = sqrtf(dx * dx + dy * dy);

        if (dist < eatRadius) {
            // Eat the food!
            fishFeed(fish);
            foodRemove(food);

#if DEBUG_FISH
            Serial.println("Fish ate food!");
#endif
            break;  // Only eat one per frame
        }

        // If hungry, swim towards nearest food
        if (fish->hunger < 50 && dist < 80) {
            fish->targetX = food->x;
            fish->targetY = food->y;
        }
    }
}

static void fishDropCoin(Fish* fish) {
    const FishStats* stats = &FISH_DATA[fish->species];
    uint8_t value = stats->coinValue + fish->growthStage;

    coinSpawn(fish->x, fish->y, value);
}
