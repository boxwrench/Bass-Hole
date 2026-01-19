#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <TFT_eSPI.h>
#include "config.h"

// TFT instance exposed for other modules (e.g. sprites)
extern TFT_eSPI tft;

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
void gfxDrawFish(Fish *fish);

// Clear a fish (restore background at its position)
void gfxClearFish(Fish *fish);

// Clear all fish (call before updating positions)
void gfxClearAllFish();

// Draw all fish
void gfxDrawAllFish();

// Restore background at specific area (dirty rect)
void gfxRestoreBackground(int16_t x, int16_t y, int16_t w, int16_t h);

// Draw a food pellet
void gfxDrawFood(Food *food);

// Clear a food pellet
void gfxClearFood(Food *food);

// Draw all food
void gfxDrawAllFood();

// Clear all food
void gfxClearAllFood();

// Draw a coin
void gfxDrawCoin(Coin *coin);

// Clear a coin
void gfxClearCoin(Coin *coin);

// Draw all coins
void gfxDrawAllCoins();

// Clear all coins
void gfxClearAllCoins();

// Draw the UI (coins, level, etc)
void gfxDrawUI();

// Draw FPS counter (debug)
void gfxDrawFPS(uint16_t fps);

// Draw touch debug crosshair
void gfxDrawTouchDebug(int16_t x, int16_t y);

// Draw color test pattern (debug)
void gfxDrawColorTest();

// ============================================================================
// PRIMITIVES (simple shapes for Phase 1)
// ============================================================================

// These wrap TFT_eSPI functions for consistency
void gfxFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void gfxDrawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void gfxFillCircle(int16_t x, int16_t y, int16_t r, uint16_t color);
void gfxDrawCircle(int16_t x, int16_t y, int16_t r, uint16_t color);
void gfxDrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void gfxDrawText(const char *text, int16_t x, int16_t y, uint16_t color, uint8_t size = 1);

// ============================================================================
// SPRITE RENDERING
// ============================================================================

// Draw sprite from PROGMEM (no transparency)
void gfxDrawSprite(const uint16_t *sprite, int16_t x, int16_t y,
                   int16_t width, int16_t height);

// Draw sprite with transparency (skips magenta 0xF81F pixels)
void gfxDrawSpriteTransparent(const uint16_t *sprite, int16_t x, int16_t y,
                              int16_t width, int16_t height);

// Draw sprite with transparency and horizontal flip
void gfxDrawSpriteTransparentFlip(const uint16_t *sprite, int16_t x, int16_t y,
                                  int16_t width, int16_t height);

#endif // GRAPHICS_H
