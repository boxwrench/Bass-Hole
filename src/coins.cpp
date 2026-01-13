#include "coins.h"
#include "game_state.h"
#include <math.h>

// Coin pool
Coin coinPool[MAX_COINS];

void coinsInit() {
    for (int i = 0; i < MAX_COINS; i++) {
        coinPool[i].active = false;
    }
}

void coinsUpdate(unsigned long deltaTime) {
    unsigned long now = millis();

    for (int i = 0; i < MAX_COINS; i++) {
        Coin* coin = &coinPool[i];
        if (!coin->active) continue;

        // Float upward slowly
        coin->y -= COIN_FLOAT_SPEED;

        // Bob side to side
        coin->floatOffset += 0.1f;

        // Check expiration
        if (now - coin->spawnTime > COIN_LIFETIME) {
            coin->active = false;
#if DEBUG_SERIAL
            // Serial.println("Coin expired");
#endif
        }

        // Stop at surface
        if (coin->y < TANK_TOP + COIN_SIZE) {
            coin->y = TANK_TOP + COIN_SIZE;
        }
    }
}

Coin* coinSpawn(float x, float y, uint8_t value) {
    // Find inactive slot
    Coin* coin = nullptr;
    for (int i = 0; i < MAX_COINS; i++) {
        if (!coinPool[i].active) {
            coin = &coinPool[i];
            break;
        }
    }

    if (!coin) {
#if DEBUG_SERIAL
        Serial.println("Coin pool full!");
#endif
        return nullptr;
    }

    coin->active = true;
    coin->x = x;
    coin->y = y;
    coin->value = value;
    coin->spawnTime = millis();
    coin->floatOffset = random(100) / 100.0f * 6.28f;  // Random phase

    return coin;
}

uint8_t coinCollect(int16_t screenX, int16_t screenY) {
    // Check each coin
    for (int i = 0; i < MAX_COINS; i++) {
        Coin* coin = &coinPool[i];
        if (!coin->active) continue;

        // Calculate display position (with bob offset)
        float displayX = coin->x + sinf(coin->floatOffset) * 3;
        float displayY = coin->y;

        // Check if tap is within coin radius
        float dx = screenX - displayX;
        float dy = screenY - displayY;
        float dist = sqrtf(dx * dx + dy * dy);

        if (dist < COIN_SIZE * 2) {  // Generous tap target
            uint8_t value = coin->value;
            coin->active = false;

            // Add to game coins
            game.coins += value;
            game.totalCoinsEarned += value;

            // Update high score
            if (game.coins > game.highScore) {
                game.highScore = game.coins;
            }

#if DEBUG_SERIAL
            Serial.print("Collected coin: +");
            Serial.println(value);
#endif
            return value;
        }
    }

    return 0;  // No coin collected
}

uint8_t coinCollectRadius(int16_t screenX, int16_t screenY, int16_t radius) {
    uint8_t totalValue = 0;

    for (int i = 0; i < MAX_COINS; i++) {
        Coin* coin = &coinPool[i];
        if (!coin->active) continue;

        float dx = screenX - coin->x;
        float dy = screenY - coin->y;
        float dist = sqrtf(dx * dx + dy * dy);

        if (dist < radius) {
            totalValue += coin->value;
            game.coins += coin->value;
            game.totalCoinsEarned += coin->value;
            coin->active = false;
        }
    }

    if (totalValue > 0 && game.coins > game.highScore) {
        game.highScore = game.coins;
    }

    return totalValue;
}

uint8_t coinGetCount() {
    uint8_t count = 0;
    for (int i = 0; i < MAX_COINS; i++) {
        if (coinPool[i].active) count++;
    }
    return count;
}
