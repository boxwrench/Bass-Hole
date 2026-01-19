#include "sprites.h"
#include "sdcard.h"
#include "graphics.h"
#include "config.h"
#include <Arduino.h>
#include <string.h>

void spriteInit()
{
#if DEBUG_SERIAL
    Serial.println("Sprite system initialized");
#endif
}

Sprite *spriteLoad(const char *path, uint16_t width, uint16_t height)
{
    size_t size = (size_t)width * height * 2; // RGB565 = 2 bytes per pixel

    // Allocate RAM for the image data
    uint16_t *buffer = (uint16_t *)malloc(size);
    if (!buffer)
    {
#if DEBUG_SERIAL
        Serial.print("Failed to allocate RAM for sprite data: ");
        Serial.println(path);
#endif
        return nullptr;
    }

    // Read from SD using the existing sdcard utility
    int32_t bytesRead = sdReadFile(path, (uint8_t *)buffer, size);
    if (bytesRead < 0 || (size_t)bytesRead != size)
    {
#if DEBUG_SERIAL
        Serial.print("Failed to read sprite from SD: ");
        Serial.print(path);
        Serial.print(" (read ");
        Serial.print(bytesRead);
        Serial.print("/");
        Serial.print(size);
        Serial.println(" bytes)");
#endif
        free(buffer);
        return nullptr;
    }

    // Create the Sprite structure
    Sprite *sprite = (Sprite *)malloc(sizeof(Sprite));
    if (!sprite)
    {
#if DEBUG_SERIAL
        Serial.println("Failed to allocate Sprite structure");
#endif
        free(buffer);
        return nullptr;
    }

    sprite->width = width;
    sprite->height = height;
    sprite->data = buffer;
    sprite->name = strdup(path);

#if DEBUG_SERIAL
    Serial.print("Loaded sprite: ");
    Serial.print(path);
    Serial.print(" (");
    Serial.print(width);
    Serial.print("x");
    Serial.print(height);
    Serial.println(")");
#endif

    return sprite;
}

void spriteUnload(Sprite *sprite)
{
    if (!sprite)
        return;

#if DEBUG_SERIAL
    Serial.print("Unloading sprite: ");
    Serial.println(sprite->name ? sprite->name : "unknown");
#endif

    if (sprite->data)
        free(sprite->data);
    if (sprite->name)
        free(sprite->name);
    free(sprite);
}

void spriteDraw(Sprite *sprite, int16_t x, int16_t y)
{
    if (!sprite || !sprite->data)
        return;

    // Enable byte swapping for raw RGB565 data
    // (verified in CYD tester project as necessary for correct colors)
    tft.setSwapBytes(true);
    tft.pushImage(x, y, sprite->width, sprite->height, sprite->data);
}

void spriteDrawTransparent(Sprite *sprite, int16_t x, int16_t y, uint16_t transparentColor)
{
    if (!sprite || !sprite->data)
        return;

    // Pixel-by-pixel rendering for transparency support
    // Note: This is slower than pushImage, use sparingly
    for (int16_t py = 0; py < sprite->height; py++)
    {
        for (int16_t px = 0; px < sprite->width; px++)
        {
            uint16_t pixel = sprite->data[py * sprite->width + px];
            if (pixel != transparentColor)
            {
                // Byte swap for drawPixel (since setSwapBytes only affects pushImage)
                uint16_t swapped = (pixel >> 8) | (pixel << 8);
                tft.drawPixel(x + px, y + py, swapped);
            }
        }
    }
}
