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

#include "coins.h"
#include "config.h"
#include "fish.h"
#include "food.h"
#include "game_state.h"
#include "graphics.h"
#include "sdcard.h"
#include "touch.h"
#include "sd_sprites.h"
#include <Arduino.h>

// Forward declarations for ESP-IDF/C++ strictness
void handleInput();
void handlePlayingInput(TouchPoint tap);
void render();
void renderPlaying();
void renderGameOver();
void renderTitle();

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

void setup()
{
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

  // Initialize touch FIRST (before display claims SPI bus)
  touchInit();

#if DEBUG_SERIAL
  // Show calibration instructions
  touchCalibrate();
  delay(3000); // Give time to read instructions
#endif

  // Initialize display second (gives visual feedback)
  gfxInit();

  gfxClear(COLOR_BLACK);
  gfxDrawText("BASS HOLE", 60, 140, COLOR_WHITE, 3);
  gfxDrawText("Loading...", 80, 180, COLOR_WATER_LIGHT, 1);

  // Initialize SD card FIRST (before loading assets!)
  bool sdOk = sdInit();
  spriteInit();

  if (sdOk)
  {
    gfxDrawText("SD Card OK", 80, 200, COLOR_UI_GREEN, 1);

    // Load game assets NOW that SD is ready
    gfxLoadAssets();
  }
  else
  {
    gfxDrawText("No SD Card", 80, 200, COLOR_UI_RED, 1);
  }
  delay(1000);

  // Confirm touch initialized
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

void loop()
{
  unsigned long now = millis();
  unsigned long deltaTime = now - lastFrameTime;

  // Frame rate limiting
  if (deltaTime < FRAME_TIME_MS)
  {
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

  // State transition logic (detect entry to PLAYING)
  static GameState lastState = STATE_BOOT;
  if (game.state == STATE_PLAYING && lastState != STATE_PLAYING)
  {
    // Draw full background ONCE when entering playing state
    // (Also catches resume from pause)
    gfxDrawTank();
  }
  lastState = game.state;

  // Update game entities
  if (game.state == STATE_PLAYING && !game.isPaused)
  {
    // DIRTY RECT RENDERING:
    // 1. Clear OLD positions (restore background)
    gfxClearAllFish();
    gfxClearAllFood();
    gfxClearAllCoins();
    // 2. Update physics (positions change)
    fishUpdate(deltaTime);
    foodUpdate(deltaTime);
    coinsUpdate(deltaTime);

    // Check for game over (all fish dead)
    if (fishGetCount() == 0 && game.coins < FISH_COST_BASIC)
    {
      gameStateChange(STATE_GAMEOVER);
    }
  }

  // Render
  render();

  // FPS calculation
  frameCount++;
  if (now - fpsTimer >= 1000)
  {
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

void handleInput()
{
  if (!touchTapped())
    return;

  TouchPoint tap = touchGetTap();
  if (!tap.valid)
    return;

  // VISUAL DEBUG: Draw circle where tap occurred
  tft.fillCircle(tap.x, tap.y, 4, TFT_WHITE);

  switch (game.state)
  {
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

void handlePlayingInput(TouchPoint tap)
{
  // First, try to collect coins at tap location
  uint8_t collected = coinCollect(tap.x, tap.y);
  if (collected > 0)
  {
#if DEBUG_SERIAL
    Serial.print("Collected $");
    Serial.println(collected);
#endif
    return; // Don't drop food if we collected a coin
  }

  // Check if tap is in tank area
  if (tap.y >= TANK_TOP && tap.y <= TANK_BOTTOM)
  {
    // Drop food at tap location
    Food *food = foodDrop(tap.x, tap.y);
    if (food)
    {
#if DEBUG_SERIAL
      Serial.print("Dropped food at ");
      Serial.print(tap.x);
      Serial.print(", ");
      Serial.println(tap.y);
#endif
    }
  }

  // Check if tap is in bottom UI area (shop button area)
  if (tap.y > TANK_BOTTOM)
  {
    int16_t btnWidth = 100;
    int16_t btnHeight = 30;
    int16_t btnX = (SCREEN_WIDTH - btnWidth) / 2;
    int16_t btnY = TANK_BOTTOM + 5;

#if DEBUG_SERIAL
    Serial.print("Footer tap at Y=");
    Serial.print(tap.y);
    Serial.print(" (TANK_BOTTOM=");
    Serial.print(TANK_BOTTOM);
    Serial.print(") Button: ");
    Serial.print(btnX);
    Serial.print("-");
    Serial.print(btnX + btnWidth);
    Serial.print(", ");
    Serial.print(btnY);
    Serial.print("-");
    Serial.println(btnY + btnHeight);
#endif

    if (tap.x >= btnX && tap.x <= btnX + btnWidth && tap.y >= btnY &&
        tap.y <= btnY + btnHeight)
    {

#if DEBUG_SERIAL
      Serial.println("Buy button HIT!");
#endif

      if (game.coins >= FISH_COST_BASIC)
      {
        game.coins -= FISH_COST_BASIC;

        // Cycle through unlocked fish
        static uint8_t nextFishToSpawn = 0;

        // Simple loop to find next unlocked fish
        // (For Phase 2 we unlocked all 0x1F so this just cycles 0-4)
        nextFishToSpawn = (nextFishToSpawn + 1) % FISH_SPECIES_COUNT;

        // Spawn the selected fish
        fishSpawn((FishSpecies)nextFishToSpawn, SCREEN_WIDTH / 2, TANK_TOP + 20);

#if DEBUG_SERIAL
        Serial.print("Spawned Species ID: ");
        Serial.println(nextFishToSpawn);
#endif
#if DEBUG_SERIAL
        Serial.print("Bought fish! Remaining coins: $");
        Serial.println(game.coins);
#endif
      }
    }
  }
}

// ============================================================================
// RENDERING
// ============================================================================

void render()
{
  switch (game.state)
  {
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

void renderPlaying()
{
  // Background drawn via dirty rect restoration or state entry
  // gfxDrawTank();

  // Draw game entities (order matters for layering)
  gfxDrawAllFood();
  gfxDrawAllFish();
  gfxDrawAllCoins();

  // Draw UI
  gfxDrawUI();

// DEBUG: Show last tap location
#if DEBUG_TOUCH
  static TouchPoint lastDebugTap = {0, 0, 0, false};
  static unsigned long lastDebugTime = 0;
  if (touchTapped())
  {
    lastDebugTap = touchGetTap();
    lastDebugTime = millis();
  }
  // Show for 2 seconds
  if (millis() - lastDebugTime < 2000 && lastDebugTap.valid)
  {
    gfxDrawTouchDebug(lastDebugTap.x, lastDebugTap.y);
  }
#endif
}

void renderGameOver()
{
  gfxClear(COLOR_BLACK);

  gfxDrawText("GAME OVER", 50, 100, COLOR_UI_RED, 3);

  gfxDrawText("Final Score:", 70, 160, COLOR_TEXT, 1);

  char scoreText[16];
  snprintf(scoreText, sizeof(scoreText), "$%lu", game.coins);
  gfxDrawText(scoreText, 90, 180, COLOR_COIN_GOLD, 2);

  if (game.coins >= game.highScore)
  {
    gfxDrawText("NEW HIGH SCORE!", 50, 220, COLOR_UI_GREEN, 1);
  }

  gfxDrawText("Tap to restart", 65, 280, COLOR_WATER_LIGHT, 1);
}

void renderTitle()
{
  gfxClear(COLOR_WATER_DEEP);

  gfxDrawText("BASS HOLE", 45, 80, COLOR_WHITE, 3);
  gfxDrawText("Knot Your Average", 45, 130, COLOR_WATER_LIGHT, 1);
  gfxDrawText("Fishing Game", 70, 145, COLOR_WATER_LIGHT, 1);

  gfxDrawText("Tap to start", 70, 250, COLOR_TEXT, 1);
}
