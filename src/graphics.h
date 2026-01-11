#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <Arduino.h>
#include "config.h"

// Forward declarations
struct Fish;
struct Food;
struct Coin;

// ============================================================================
// GRAPHICS FUNCTIONS
// ============================================================================

// Initialize display
void gfxInit();

// Clear screen / fill with color
void gfxClear(uint16_t color = COLOR_BLACK);

// Begin/end frame (for double buffering if implemented)
void gfxBeginFrame();
void gfxEndFrame();

// ============================================================================
// GAME RENDERING
// ============================================================================

// Draw the water tank background
void gfxDrawTank();

// Draw a fish
void gfxDrawFish(Fish* fish);

// Draw all fish
void gfxDrawAllFish();

// Draw a food pellet
void gfxDrawFood(Food* food);

// Draw all food
void gfxDrawAllFood();

// Draw a coin
void gfxDrawCoin(Coin* coin);

// Draw all coins
void gfxDrawAllCoins();

// Draw the UI (coins, level, etc)
void gfxDrawUI();

// Draw FPS counter (debug)
void gfxDrawFPS(uint16_t fps);

// ============================================================================
// PRIMITIVES (simple shapes for Phase 1)
// ============================================================================

// These wrap TFT_eSPI functions for consistency
void gfxFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void gfxDrawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void gfxFillCircle(int16_t x, int16_t y, int16_t r, uint16_t color);
void gfxDrawCircle(int16_t x, int16_t y, int16_t r, uint16_t color);
void gfxDrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void gfxDrawText(const char* text, int16_t x, int16_t y, uint16_t color, uint8_t size = 1);

// ============================================================================
// SPRITE RENDERING (for later phases with SD card sprites)
// ============================================================================

// Draw sprite from SD card (loads on demand)
// void gfxDrawSprite(const char* filename, int16_t x, int16_t y);

#endif // GRAPHICS_H
