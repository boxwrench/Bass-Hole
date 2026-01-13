#ifndef COINS_H
#define COINS_H

#include <Arduino.h>
#include "config.h"

// ============================================================================
// COIN DATA STRUCTURE
// ============================================================================

struct Coin {
    float x, y;
    uint8_t value;
    bool active;
    unsigned long spawnTime;
    float floatOffset;  // For bobbing animation
};

// Coin pool
extern Coin coinPool[MAX_COINS];

// ============================================================================
// COIN FUNCTIONS
// ============================================================================

// Initialize coin system
void coinsInit();

// Update all coins (floating, expiring)
void coinsUpdate(unsigned long deltaTime);

// Spawn a coin at position
Coin* coinSpawn(float x, float y, uint8_t value);

// Collect coin at screen position (returns value, 0 if none)
uint8_t coinCollect(int16_t screenX, int16_t screenY);

// Collect all coins in radius
uint8_t coinCollectRadius(int16_t screenX, int16_t screenY, int16_t radius);

// Get coin count
uint8_t coinGetCount();

#endif // COINS_H
