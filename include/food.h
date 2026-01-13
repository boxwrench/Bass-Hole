#ifndef FOOD_H
#define FOOD_H

#include <Arduino.h>
#include "config.h"

// ============================================================================
// FOOD DATA STRUCTURE
// ============================================================================

struct Food {
    float x, y;
    bool active;
    unsigned long spawnTime;
};

// Food pool
extern Food foodPool[MAX_FOOD];

// ============================================================================
// FOOD FUNCTIONS
// ============================================================================

// Initialize food system
void foodInit();

// Update all food (falling, being eaten)
void foodUpdate(unsigned long deltaTime);

// Drop food at position
Food* foodDrop(float x, float y);

// Remove food pellet
void foodRemove(Food* food);

// Get food count
uint8_t foodGetCount();

#endif // FOOD_H
