/*
 * sprites.h - Master sprite include file for Bass Hole
 *
 * Includes all sprite data and provides helper macros for sprite rendering.
 * Sprites are stored in PROGMEM (flash) to conserve SRAM.
 *
 * Transparency: Magenta (0xF81F) is used as the transparent color.
 * Use gfxDrawSpriteTransparent() to render with transparency support.
 */

#ifndef SPRITES_H
#define SPRITES_H

#include <Arduino.h>

// ============================================================================
// TRANSPARENCY
// ============================================================================

// Magenta is used as the transparent color in all sprites
#define SPRITE_TRANSPARENT_COLOR 0xF81F

// ============================================================================
// SPRITE INCLUDES
// ============================================================================

// Fish sprites (10 species)
#include "fish_sprites.h"

// Background sprites
#include "bg_sierra_data.h"
#include "bg_delta_data.h"

// UI elements (coins, pellets, hearts, buttons)
#include "ui_sprites.h"

// Visual effects (bubbles, sparkles, splash)
// TODO: Uncomment when effect sprite data is converted
// #include "effect_sprites.h"

// ============================================================================
// SPRITE HELPER MACROS
// ============================================================================

// Get sprite dimensions
#define SPRITE_WIDTH(name)  name##_WIDTH
#define SPRITE_HEIGHT(name) name##_HEIGHT

// Check if a pixel is transparent
#define IS_TRANSPARENT(pixel) ((pixel) == SPRITE_TRANSPARENT_COLOR)

#endif // SPRITES_H
