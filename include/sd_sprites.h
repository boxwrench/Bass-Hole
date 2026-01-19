#ifndef SD_SPRITES_H
#define SD_SPRITES_H

#include <Arduino.h>
#include <TFT_eSPI.h>

/**
 * @brief Sprite structure for SD card based RGB565 images
 */
struct Sprite
{
    uint16_t width;
    uint16_t height;
    uint16_t *data;
    char *name;
};

/**
 * @brief Initialize the sprite management system
 */
void spriteInit();

/**
 * @brief Load a raw RGB565 sprite from SD card into RAM
 *
 * @param path Path to the .raw file on SD card
 * @param width Width of the sprite
 * @param height Height of the sprite
 * @return Sprite* Pointer to the loaded sprite, or nullptr on failure
 */
Sprite *spriteLoad(const char *path, uint16_t width, uint16_t height);

/**
 * @brief Unload a sprite and free its RAM
 *
 * @param sprite Pointer to the sprite to unload
 */
void spriteUnload(Sprite *sprite);

/**
 * @brief Draw a sprite to the display
 *
 * @param sprite Pointer to the sprite to draw
 * @param x X coordinate (top-left)
 * @param y Y coordinate (top-left)
 */
void spriteDraw(Sprite *sprite, int16_t x, int16_t y);

/**
 * @brief Draw a sprite with transparency (skipping specific color)
 *
 * @param sprite Pointer to the sprite
 * @param x X coordinate
 * @param y Y coordinate
 * @param transparentColor RGB565 color to skip (default Magenta)
 */
void spriteDrawTransparent(Sprite *sprite, int16_t x, int16_t y, uint16_t transparentColor = 0xF81F);

#endif // SD_SPRITES_H
