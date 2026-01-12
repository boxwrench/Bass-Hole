/*
 * Bass Hole - "Knot Your Average Fishing Game"
 *
 * An Insaniquarium-style game for ESP32 CYD (Cheap Yellow Display)
 * Featuring Ty Knotts, the grumpiest largemouth bass guide
 *
 * Hardware: TZT ESP32 2.4" CYD (240x320, ILI9341, XPT2046 touch)
 *
 * Core gameplay:
 * - Tap to drop food
 * - Fish eat, grow, drop coins
 * - Tap coins to collect
 * - Don't let fish starve!
 */

#include "config.h"
#include "game_state.h"
#include "fish.h"
#include "food.h"
#include "coins.h"
#include "touch.h"
#include "graphics.h"
#include "sdcard.h"

// ============================================================================
// TIMING
// ============================================================================

unsigned long lastFrameTime = 0;
unsigned long frameCount = 0;
unsigned long fpsTimer = 0;
uint16_t currentFPS = 0;

// ============================================================================
// SETUP
// ============================================================================

void setup() {
#if DEBUG_SERIAL
    Serial.begin(115200);
    delay(100);
    Serial.println();
    Serial.println("================================");
    Serial.println("  BASS HOLE");
    Serial.println("  Knot Your Average Fishing Game");
    Serial.println("================================");
    Serial.println();
#endif

    // Initialize display first (gives visual feedback)
    gfxInit();
    gfxClear(COLOR_BLACK);
    gfxDrawText("BASS HOLE", 60, 140, COLOR_WHITE, 3);
    gfxDrawText("Loading...", 80, 180, COLOR_WATER_LIGHT, 1);

    // Initialize SD card
    bool sdOk = sdInit();
    if (sdOk) {
        gfxDrawText("SD Card OK", 80, 200, COLOR_UI_GREEN, 1);
    } else {
        gfxDrawText("No SD Card", 80, 200, COLOR_UI_RED, 1);
    }
    delay(500);

    // Initialize touch
    touchInit();
    gfxDrawText("Touch OK", 80, 220, COLOR_UI_GREEN, 1);
    delay(500);

    // Initialize game systems
    fishInit();
    foodInit();
    coinsInit();
    gameStateInit();

    // Clear splash screen before starting game
    gfxClear(COLOR_BLACK);

    // Start new game
    gameStateReset();

#if DEBUG_SERIAL
    Serial.println("Setup complete!");
    Serial.print("Free heap: ");
    Serial.println(ESP.getFreeHeap());
#endif

    lastFrameTime = millis();
    fpsTimer = millis();
}

// ============================================================================
// MAIN LOOP
// ============================================================================

void loop() {
    unsigned long now = millis();
    unsigned long deltaTime = now - lastFrameTime;

    // Frame rate limiting
    if (deltaTime < FRAME_TIME_MS) {
        delay(FRAME_TIME_MS - deltaTime);
        now = millis();
        deltaTime = now - lastFrameTime;
    }
    lastFrameTime = now;

    // Update game state timing
    gameStateUpdate();

    // Handle input
    touchUpdate();
    handleInput();

    // Update game entities
    if (game.state == STATE_PLAYING && !game.isPaused) {
        fishUpdate(deltaTime);
        foodUpdate(deltaTime);
        coinsUpdate(deltaTime);

        // Check for game over (all fish dead)
        if (fishGetCount() == 0 && game.coins < FISH_COST_BASIC) {
            gameStateChange(STATE_GAMEOVER);
        }
    }

    // Render
    render();

    // FPS calculation
    frameCount++;
    if (now - fpsTimer >= 1000) {
        currentFPS = frameCount;
        frameCount = 0;
        fpsTimer = now;

#if DEBUG_SERIAL && DEBUG_FPS
        Serial.print("FPS: ");
        Serial.print(currentFPS);
        Serial.print(" | Fish: ");
        Serial.print(fishGetCount());
        Serial.print(" | Coins: $");
        Serial.print(game.coins);
        Serial.print(" | Heap: ");
        Serial.println(ESP.getFreeHeap());
#endif
    }
}

// ============================================================================
// INPUT HANDLING
// ============================================================================

void handleInput() {
    if (!touchTapped()) return;

    TouchPoint tap = touchGetTap();
    if (!tap.valid) return;

    switch (game.state) {
        case STATE_PLAYING:
            handlePlayingInput(tap);
            break;

        case STATE_GAMEOVER:
            // Tap anywhere to restart
            gameStateReset();
            break;

        case STATE_TITLE:
            gameStateChange(STATE_PLAYING);
            break;

        default:
            break;
    }
}

void handlePlayingInput(TouchPoint tap) {
    // First, try to collect coins at tap location
    uint8_t collected = coinCollect(tap.x, tap.y);
    if (collected > 0) {
#if DEBUG_SERIAL
        Serial.print("Collected $");
        Serial.println(collected);
#endif
        return;  // Don't drop food if we collected a coin
    }

    // Check if tap is in tank area
    if (tap.y >= TANK_TOP && tap.y <= TANK_BOTTOM) {
        // Drop food at tap location
        Food* food = foodDrop(tap.x, tap.y);
        if (food) {
#if DEBUG_SERIAL
            Serial.print("Dropped food at ");
            Serial.print(tap.x);
            Serial.print(", ");
            Serial.println(tap.y);
#endif
        }
    }

    // Check if tap is in bottom UI area (shop button area - future)
    if (tap.y > TANK_BOTTOM) {
        // TODO: Open shop when implemented
    }
}

// ============================================================================
// RENDERING
// ============================================================================

void render() {
    switch (game.state) {
        case STATE_PLAYING:
            renderPlaying();
            break;

        case STATE_GAMEOVER:
            renderGameOver();
            break;

        case STATE_TITLE:
            renderTitle();
            break;

        default:
            renderPlaying();
            break;
    }

#if DEBUG_FPS
    gfxDrawFPS(currentFPS);
#endif
}

void renderPlaying() {
    // Draw background
    gfxDrawTank();

    // Draw game entities (order matters for layering)
    gfxDrawAllFood();
    gfxDrawAllFish();
    gfxDrawAllCoins();

    // Draw UI
    gfxDrawUI();
}

void renderGameOver() {
    gfxClear(COLOR_BLACK);

    gfxDrawText("GAME OVER", 50, 100, COLOR_UI_RED, 3);

    gfxDrawText("Final Score:", 70, 160, COLOR_TEXT, 1);

    char scoreText[16];
    snprintf(scoreText, sizeof(scoreText), "$%lu", game.coins);
    gfxDrawText(scoreText, 90, 180, COLOR_COIN_GOLD, 2);

    if (game.coins >= game.highScore) {
        gfxDrawText("NEW HIGH SCORE!", 50, 220, COLOR_UI_GREEN, 1);
    }

    gfxDrawText("Tap to restart", 65, 280, COLOR_WATER_LIGHT, 1);
}

void renderTitle() {
    gfxClear(COLOR_WATER_DEEP);

    gfxDrawText("BASS HOLE", 45, 80, COLOR_WHITE, 3);
    gfxDrawText("Knot Your Average", 45, 130, COLOR_WATER_LIGHT, 1);
    gfxDrawText("Fishing Game", 70, 145, COLOR_WATER_LIGHT, 1);

    gfxDrawText("Tap to start", 70, 250, COLOR_TEXT, 1);
}
