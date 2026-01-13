#ifndef FISH_H
#define FISH_H

#include <Arduino.h>
#include "config.h"

// ============================================================================
// FISH DATA STRUCTURE
// ============================================================================

struct Fish {
    // Position and movement
    float x, y;
    float vx, vy;           // Velocity
    float targetX, targetY; // Where fish wants to go

    // State
    bool active;
    FishSpecies species;
    uint8_t growthStage;    // 0=small, 1=medium, 2=large

    // Hunger system
    uint8_t hunger;         // 0=starving, 100=full
    unsigned long lastFed;  // Timestamp of last feeding

    // Animation
    uint8_t frame;          // Current animation frame
    bool facingRight;       // Direction fish is facing
    unsigned long lastFrameTime;

    // Coin generation
    unsigned long lastCoinDrop;

    // Visual
    uint16_t tint;          // Color tint (for variety)
};

// Fish pool
extern Fish fishPool[MAX_FISH];
extern uint8_t fishCount;

// ============================================================================
// FISH FUNCTIONS
// ============================================================================

// Initialize fish system
void fishInit();

// Update all fish (movement, hunger, coins)
void fishUpdate(unsigned long deltaTime);

// Spawn a new fish
Fish* fishSpawn(FishSpecies species, float x, float y);

// Remove a fish (died or sold)
void fishRemove(Fish* fish);

// Feed fish - called when food is near
void fishFeed(Fish* fish);

// Check if fish is hungry (for visual indicator)
bool fishIsHungry(Fish* fish);

// Get fish at screen position (for tap selection)
Fish* fishGetAt(int16_t screenX, int16_t screenY);

// Get count of active fish
uint8_t fishGetCount();

#endif // FISH_H
