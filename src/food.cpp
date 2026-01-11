#include "food.h"

// Food pool
Food foodPool[MAX_FOOD];

void foodInit() {
    for (int i = 0; i < MAX_FOOD; i++) {
        foodPool[i].active = false;
    }
}

void foodUpdate(unsigned long deltaTime) {
    for (int i = 0; i < MAX_FOOD; i++) {
        Food* food = &foodPool[i];
        if (!food->active) continue;

        // Fall down
        food->y += FOOD_FALL_SPEED;

        // Remove if reached bottom
        if (food->y >= TANK_BOTTOM - FOOD_SIZE) {
            foodRemove(food);
#if DEBUG_SERIAL
            // Serial.println("Food hit bottom");
#endif
        }
    }
}

Food* foodDrop(float x, float y) {
    // Find inactive slot
    Food* food = nullptr;
    for (int i = 0; i < MAX_FOOD; i++) {
        if (!foodPool[i].active) {
            food = &foodPool[i];
            break;
        }
    }

    if (!food) {
#if DEBUG_SERIAL
        Serial.println("Food pool full!");
#endif
        return nullptr;
    }

    food->active = true;
    food->x = x;
    food->y = y;
    food->spawnTime = millis();

    return food;
}

void foodRemove(Food* food) {
    if (food) {
        food->active = false;
    }
}

uint8_t foodGetCount() {
    uint8_t count = 0;
    for (int i = 0; i < MAX_FOOD; i++) {
        if (foodPool[i].active) count++;
    }
    return count;
}
