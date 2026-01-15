#include "graphics.h"
#include "fish.h"
#include "food.h"
#include "coins.h"
#include "game_state.h"
#include "sprites/sprites.h"
#include "sprites/test_colors.h"
#include <TFT_eSPI.h>
#include <math.h>
#include <string.h>

// ============================================================================
// DISPLAY CONFIGURATION (TZT ESP32 CYD 2.4" - ILI9341)
// ============================================================================
//
// VERIFIED WORKING CONFIGURATION (2025-01-14):
// - Display rotation: 0 (portrait, USB at bottom)
// - invertDisplay: true (verified via color test)
// - Color constants: Normal RGB565 values (in config.h)
// - Sprite data: Normal RGB565 values (NO XOR inversion needed)
//
// IMPORTANT: Different display manufacturers may need invertDisplay(false).
// If colors look wrong, run gfxDrawColorTest() to determine correct setting:
//   - If "1. RAW" test shows correct colors → use invertDisplay(true)
//   - If "2. XOR" test shows correct colors → use invertDisplay(false)
//
// With invertDisplay(true):
// - Sprites use raw RGB565 data from PROGMEM
// - Text/UI use normal color constants
// - Display hardware inverts everything consistently
//
// ============================================================================

// Enable sprite rendering (set to 0 to use old geometric shapes)
#define USE_SPRITES 1

// Enable background sprite (set to 0 to use gradient)
#define USE_BACKGROUND_SPRITE 1

// Display color inversion - Try both true and false depending on your board
#define DISPLAY_INVERT true

// TFT display instance
static TFT_eSPI tft = TFT_eSPI();

// Sprite for double buffering (optional, uses more RAM)
// static TFT_eSprite sprite = TFT_eSprite(&tft);

void gfxInit()
{
    // Turn on backlight - try both common CYD pins
    pinMode(21, OUTPUT);
    digitalWrite(21, HIGH);
    pinMode(27, OUTPUT);
    digitalWrite(27, HIGH);

    // Initialize display
    tft.init();

    // Portrait mode for CYD (Rotation 3 is often Portrait with USB down)
    tft.setRotation(3);
    
    // CYD requires color inversion on some panels
#if DISPLAY_INVERT
    // tft.invertDisplay(true); // Handled by TFT_INVERSION_ON build flag
#endif

    // CRITICAL FIX: Enable byte swapping for PROGMEM sprites
    // This ensures colors render correctly (fixes endian mismatch)
    // tft.setSwapBytes(true); // DISABLED: Testing if this causes washed out colors

    // Aggressive clear in ALL rotations to remove ghost images
    for (int r = 0; r < 4; r++)
    {
        tft.setRotation(r);
        tft.fillScreen(TFT_BLACK);
    }

    // Set final rotation
    tft.setRotation(3);

#if DEBUG_SERIAL
    Serial.println("=== DISPLAY INIT v2025.01.13.A ==="); // Unique identifier for THIS version
    Serial.println("Display initialized");
    Serial.print("Rotation: ");
    Serial.println(tft.getRotation());
    Serial.print("Width: ");
    Serial.print(tft.width());
    Serial.print(" Height: ");
    Serial.println(tft.height());
    Serial.print("TANK_BOTTOM: ");
    Serial.println(TANK_BOTTOM);
    Serial.print("Footer starts at Y: ");
    Serial.println(TANK_BOTTOM);
    Serial.print("Screen ends at Y: ");
    Serial.println(SCREEN_HEIGHT);
    Serial.println("Ghost clear: 4 rotations");
#endif
}

void gfxClear(uint16_t color)
{
    tft.fillScreen(color);
}

void gfxBeginFrame()
{
    // If using sprite buffering, would create sprite here
}

void gfxEndFrame()
{
    // If using sprite buffering, would push sprite to display here
}

// ============================================================================
// GAME RENDERING
// ============================================================================

void gfxDrawTank()
{
#if USE_BACKGROUND_SPRITE
    // Draw background sprite (240x240, fits tank area perfectly)
    // Using delta - has clear blue sky, green vegetation, blue water
    gfxDrawSprite(sprite_bg_delta, TANK_LEFT, TANK_TOP,
                  SPRITE_BG_DELTA_WIDTH, SPRITE_BG_DELTA_HEIGHT);
#else
    // Draw water gradient (simplified - just solid colors for Phase 1)
    // Top of tank - lighter blue
    tft.fillRect(TANK_LEFT, TANK_TOP, TANK_WIDTH, TANK_HEIGHT / 3, COLOR_WATER_LIGHT);
    // Middle - medium blue
    tft.fillRect(TANK_LEFT, TANK_TOP + TANK_HEIGHT / 3, TANK_WIDTH, TANK_HEIGHT / 3, COLOR_WATER_MID);
    // Bottom - darker blue
    tft.fillRect(TANK_LEFT, TANK_TOP + 2 * TANK_HEIGHT / 3, TANK_WIDTH, TANK_HEIGHT / 3, COLOR_WATER_DEEP);

    // Sand at very bottom
    tft.fillRect(TANK_LEFT, TANK_BOTTOM - 10, TANK_WIDTH, 10, COLOR_SAND);
#endif
}

void gfxDrawFish(Fish *fish)
{
    if (!fish || !fish->active)
        return;

#if USE_SPRITES
    // Sprite-based rendering
    // Sprite-based rendering
    const uint16_t *sprite = sprite_fish_r_trout;
    int16_t spriteW = SPRITE_FISH_R_TROUT_WIDTH;
    int16_t spriteH = SPRITE_FISH_R_TROUT_HEIGHT;

    // Calculate position (center sprite on fish position)
    int16_t x = (int16_t)fish->x - spriteW / 2;
    int16_t y = (int16_t)fish->y - spriteH / 2;

    // Draw with transparency, flip if facing right (source sprite faces left)
    if (fish->facingRight)
    {
        gfxDrawSpriteTransparentFlip(sprite, x, y, spriteW, spriteH);
    }
    else
    {
        gfxDrawSpriteTransparent(sprite, x, y, spriteW, spriteH);
    }

    // Hunger indicator (red outline when hungry)
    if (fishIsHungry(fish))
    {
        tft.drawRect(x - 1, y - 1, spriteW + 2, spriteH + 2, COLOR_UI_RED);
    }

#else
    // Legacy geometric rendering (fallback)
    float scale = 1.0f + fish->growthStage * 0.3f;
    int16_t w = (int16_t)(FISH_WIDTH * scale);
    int16_t h = (int16_t)(FISH_HEIGHT * scale);

    int16_t x = (int16_t)fish->x - w / 2;
    int16_t y = (int16_t)fish->y - h / 2;

    // Fish body color based on species
    uint16_t bodyColor;
    switch (fish->species)
    {
    case FISH_RAINBOW_TROUT:
        bodyColor = tft.color565(200, 150, 150); // Pinkish
        break;
    case FISH_BLUEGILL:
        bodyColor = tft.color565(100, 100, 200); // Blue
        break;
    case FISH_SMALLMOUTH_BASS:
        bodyColor = tft.color565(100, 150, 100); // Green
        break;
    case FISH_CHANNEL_CATFISH:
        bodyColor = tft.color565(100, 80, 60); // Brown
        break;
    case FISH_LARGEMOUTH_BASS:
        bodyColor = tft.color565(50, 120, 50); // Dark green
        break;
    default:
        bodyColor = COLOR_WHITE;
    }

    // Simple fish shape (ellipse body + triangle tail)
    tft.fillEllipse(fish->x, fish->y, w / 2, h / 2, bodyColor);

    // Tail (triangle)
    int16_t tailDir = fish->facingRight ? -1 : 1;
    int16_t tailX = fish->x + tailDir * (w / 2);
    tft.fillTriangle(
        tailX, fish->y,
        tailX + tailDir * (w / 3), fish->y - h / 3,
        tailX + tailDir * (w / 3), fish->y + h / 3,
        bodyColor);

    // Eye
    int16_t eyeX = fish->x + (fish->facingRight ? w / 4 : -w / 4);
    tft.fillCircle(eyeX, fish->y - h / 6, 2, COLOR_BLACK);

    // Hunger indicator (red tint when hungry)
    if (fishIsHungry(fish))
    {
        tft.drawEllipse(fish->x, fish->y, w / 2 + 1, h / 2 + 1, COLOR_UI_RED);
    }
#endif
}

void gfxDrawAllFish()
{
    for (int i = 0; i < MAX_FISH; i++)
    {
        gfxDrawFish(&fishPool[i]);
    }
}

// Restore background at specific area (dirty rect)
void gfxRestoreBackground(int16_t x, int16_t y, int16_t w, int16_t h) {
#if USE_BACKGROUND_SPRITE
    // Clip to tank area
    if (x < TANK_LEFT) { w -= (TANK_LEFT - x); x = TANK_LEFT; }
    if (y < TANK_TOP) { h -= (TANK_TOP - y); y = TANK_TOP; }
    
    // Bounds check
    if (w <= 0 || h <= 0) return;
    
    // Calculate offsets into background sprite
    int16_t bgX = x - TANK_LEFT;
    int16_t bgY = y - TANK_TOP;
    
    // Bounds check for sprite source
    if (bgX + w > SPRITE_BG_DELTA_WIDTH) w = SPRITE_BG_DELTA_WIDTH - bgX;
    if (bgY + h > SPRITE_BG_DELTA_HEIGHT) h = SPRITE_BG_DELTA_HEIGHT - bgY;

    if (w <= 0 || h <= 0) return;

    // Draw background chunk line-by-line
    // We cannot push a rect from the large sprite directly as it's not contiguous
    for (int16_t row = 0; row < h; row++) {
        int32_t offset = (bgY + row) * SPRITE_BG_DELTA_WIDTH + bgX;
        tft.pushImage(x, y + row, w, 1, &sprite_bg_delta[offset]);
    }
#else
    // Fallback: fill rect with blue
    tft.fillRect(x, y, w, h, COLOR_WATER_MID);
#endif
}

// Clear a fish by redrawing the background behind it
void gfxClearFish(Fish* fish) {
    if (!fish || !fish->active) return;

    int16_t spriteW, spriteH;

#if USE_SPRITES
    spriteW = SPRITE_FISH_R_TROUT_WIDTH;
    spriteH = SPRITE_FISH_R_TROUT_HEIGHT;
#else
    float scale = 1.0f + fish->growthStage * 0.3f;
    spriteW = (int16_t)(FISH_WIDTH * scale);
    spriteH = (int16_t)(FISH_HEIGHT * scale);
#endif

    // Calculate position (top-left)
    int16_t x = (int16_t)fish->x - spriteW / 2;
    int16_t y = (int16_t)fish->y - spriteH / 2;

    // Add padding to ensure full clear (rounding errors etc)
    gfxRestoreBackground(x - 2, y - 2, spriteW + 4, spriteH + 4);
}

void gfxClearAllFish() {
    for (int i = 0; i < MAX_FISH; i++) {
        gfxClearFish(&fishPool[i]);
    }
}

void gfxDrawFood(Food *food)
{
    if (!food || !food->active)
        return;

    // Simple brown circle for food pellet
    tft.fillCircle((int16_t)food->x, (int16_t)food->y, FOOD_SIZE, COLOR_FOOD_BROWN);
}

void gfxClearFood(Food* food) {
    if (!food || !food->active) return;
    
    // Clear rect around circle
    int16_t r = FOOD_SIZE + 2; // Padding
    gfxRestoreBackground((int16_t)food->x - r, (int16_t)food->y - r, r*2, r*2);
}

void gfxDrawAllFood()
{
    for (int i = 0; i < MAX_FOOD; i++)
    {
        gfxDrawFood(&foodPool[i]);
    }
}

void gfxClearAllFood() {
    for (int i = 0; i < MAX_FOOD; i++) {
        gfxClearFood(&foodPool[i]);
    }
}

void gfxDrawCoin(Coin *coin)
{
    if (!coin || !coin->active)
        return;

    // Bob animation
    float displayX = coin->x + sinf(coin->floatOffset) * 3;

    // Coin size based on value
    int16_t size = COIN_SIZE + (coin->value > 3 ? 2 : 0);

    // Gold circle with darker outline
    tft.fillCircle((int16_t)displayX, (int16_t)coin->y, size, COLOR_COIN_GOLD);
    tft.drawCircle((int16_t)displayX, (int16_t)coin->y, size, tft.color565(180, 130, 0));

    // $ symbol for larger coins
    if (coin->value > 2)
    {
        tft.setTextColor(tft.color565(180, 130, 0));
        tft.setTextSize(1);
        tft.setCursor((int16_t)displayX - 2, (int16_t)coin->y - 3);
        tft.print("$");
    }
}

void gfxDrawAllCoins()
{
    for (int i = 0; i < MAX_COINS; i++)
    {
        gfxDrawCoin(&coinPool[i]);
    }
}

void gfxClearCoin(Coin* coin) {
    if (!coin || !coin->active) return;
    
    // Coins animate (bob left/right) and have variable size
    // Max size is roughly 10px radius + 3px bob + padding
    int16_t r = 16; 
    gfxRestoreBackground((int16_t)coin->x - r, (int16_t)coin->y - r, r*2, r*2);
}

void gfxClearAllCoins() {
    for (int i = 0; i < MAX_COINS; i++) {
        gfxClearCoin(&coinPool[i]);
    }
}

void gfxDrawUI()
{
    // Top bar - black background
    tft.fillRect(0, 0, SCREEN_WIDTH, TANK_TOP, COLOR_BLACK);

    // Coin display
    tft.setTextColor(COLOR_COIN_GOLD);
    tft.setTextSize(2);
    tft.setCursor(5, 10);
    tft.print("$");
    tft.print(game.coins);

    // Fish count - label at top right, count below
    tft.setTextColor(COLOR_TEXT);
    tft.setTextSize(1);
    tft.setCursor(SCREEN_WIDTH - 35, 5);
    tft.print("FISH");
    tft.setTextSize(2);
    tft.setCursor(SCREEN_WIDTH - 30, 18);
    tft.print(fishGetCount());

    // Bottom bar - footer background
    tft.fillRect(0, TANK_BOTTOM, SCREEN_WIDTH, SCREEN_HEIGHT - TANK_BOTTOM, COLOR_BLACK);

    // Buy Fish Button - centered
    int16_t btnWidth = 100;
    int16_t btnHeight = 30;
    int16_t btnX = (SCREEN_WIDTH - btnWidth) / 2;
    int16_t btnY = TANK_BOTTOM + 5;

    // Button background - bright green when affordable, dark when not
    uint16_t btnColor = game.coins >= FISH_COST_BASIC ? COLOR_UI_GREEN : tft.color565(80, 80, 80);
    tft.fillRect(btnX, btnY, btnWidth, btnHeight, btnColor);
    tft.drawRect(btnX, btnY, btnWidth, btnHeight, COLOR_COIN_GOLD);

    // Button text
    uint16_t textColor = game.coins >= FISH_COST_BASIC ? COLOR_BLACK : COLOR_WHITE;
    tft.setTextColor(textColor);
    tft.setTextSize(2);

    char btnText[16];
    snprintf(btnText, sizeof(btnText), "BUY $%d", FISH_COST_BASIC);
    int16_t textWidth = strlen(btnText) * 12;
    tft.setCursor(btnX + (btnWidth - textWidth) / 2, btnY + 7);
    tft.print(btnText);
}

void gfxDrawFPS(uint16_t fps)
{
    tft.setTextColor(COLOR_UI_GREEN, COLOR_BLACK);
    tft.setTextSize(1);
    tft.setCursor(SCREEN_WIDTH - 35, 2);
    tft.print(fps);
    tft.print("fps");
}

// DEBUG: Draw touch crosshair
void gfxDrawTouchDebug(int16_t x, int16_t y) {
    if (x < 0 || y < 0) return;

    // Draw crosshair
    tft.drawLine(x - 10, y, x + 10, y, COLOR_UI_RED);
    tft.drawLine(x, y - 10, x, y + 10, COLOR_UI_RED);
    tft.drawCircle(x, y, 5, COLOR_UI_RED);

    // Draw coordinates
    tft.setTextColor(COLOR_UI_RED, COLOR_BLACK);
    tft.setTextSize(1);
    char buf[16];
    snprintf(buf, sizeof(buf), "%d,%d", x, y);
    tft.setCursor(x + 12, y - 4);
    tft.print(buf);
}

// ============================================================================
// PRIMITIVES
// ============================================================================

void gfxFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
    tft.fillRect(x, y, w, h, color);
}

void gfxDrawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
    tft.drawRect(x, y, w, h, color);
}

void gfxFillCircle(int16_t x, int16_t y, int16_t r, uint16_t color)
{
    tft.fillCircle(x, y, r, color);
}

void gfxDrawCircle(int16_t x, int16_t y, int16_t r, uint16_t color)
{
    tft.drawCircle(x, y, r, color);
}

void gfxDrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
    tft.drawLine(x0, y0, x1, y1, color);
}

void gfxDrawText(const char *text, int16_t x, int16_t y, uint16_t color, uint8_t size)
{
    tft.setTextColor(color);
    tft.setTextSize(size);
    tft.setCursor(x, y);
    tft.print(text);
}

// ============================================================================
// SPRITE RENDERING
// ============================================================================

void gfxDrawSprite(const uint16_t *sprite, int16_t x, int16_t y,
                   int16_t width, int16_t height)
{
    // TFT_INVERSION_ON is set in platformio.ini, so pushImage now works correctly
    tft.pushImage(x, y, width, height, sprite);
}

void gfxDrawSpriteTransparent(const uint16_t *sprite, int16_t x, int16_t y,
                              int16_t width, int16_t height)
{
    // Pixel-by-pixel with transparency check (no XOR needed with TFT_INVERSION_ON)
    for (int16_t py = 0; py < height; py++) {
        for (int16_t px = 0; px < width; px++) {
            uint16_t pixel = pgm_read_word(&sprite[py * width + px]);
            if (pixel != SPRITE_TRANSPARENT_COLOR) {
                // Manual byte swap needed for drawPixel because setSwapBytes only affects pushImage
                // This is critical for ILI9341_DRIVER to avoid "washed out" colors
                tft.drawPixel(x + px, y + py, (pixel >> 8) | (pixel << 8));
            }
        }
    }
}

void gfxDrawSpriteTransparentFlip(const uint16_t *sprite, int16_t x, int16_t y,
                                  int16_t width, int16_t height)
{
    // Horizontally flipped with transparency (no XOR needed with TFT_INVERSION_ON)
    for (int16_t py = 0; py < height; py++) {
        for (int16_t px = 0; px < width; px++) {
            uint16_t pixel = pgm_read_word(&sprite[py * width + (width - 1 - px)]);
            if (pixel != SPRITE_TRANSPARENT_COLOR) {
                // Manual byte swap needed for drawPixel
                tft.drawPixel(x + px, y + py, (pixel >> 8) | (pixel << 8));
            }
        }
    }
}

// ============================================================================
// COLOR TEST
// ============================================================================

void gfxDrawColorTest() {
    tft.fillScreen(COLOR_BLACK);

    // Draw labels
    tft.setTextColor(COLOR_WHITE);
    tft.setTextSize(2);
    int16_t yPos = 5;

    // Test: Hardware pushImage with TFT_INVERSION_ON
    tft.setCursor(5, yPos);
    tft.print("Color Test (R G B W K)");
    yPos += 30;

    // REFERENCE (fillRect - known good)
    tft.setCursor(5, yPos); 
    tft.print("Ref (fillRect):");
    yPos += 20;
    // Draw 10px wide blocks to match 50x50 sprite columns
    tft.fillRect(10, yPos, 10, 50, 0xF800); // Red
    tft.fillRect(20, yPos, 10, 50, 0x07E0); // Green
    tft.fillRect(30, yPos, 10, 50, 0x001F); // Blue
    tft.fillRect(40, yPos, 10, 50, 0xFFFF); // White
    tft.fillRect(50, yPos, 10, 50, 0x0000); // Black (Invisible on black bg, but consistent)
    
    // Draw outline around reference
    tft.drawRect(9, yPos-1, 52, 52, COLOR_WHITE);
    
    yPos += 60;

    // TEST (pushImage - what we are testing)
    tft.setCursor(5, yPos);
    tft.print("Test (pushImage):");
    yPos += 20;
    
    // IMPORTANT: Enable byte swapping for PROGMEM images
    // This solves the issue where colors are garbled (endian mismatch)
    tft.setSwapBytes(true); 
    
    // Push the 50x50 sprite directly
    tft.pushImage(10, yPos, 
                  SPRITE_TEST_COLORS_WIDTH, 
                  SPRITE_TEST_COLORS_HEIGHT, 
                  sprite_test_colors);
                  
    // Reset swap bytes to default just in case (though we likely want it true everywhere)
    // tft.setSwapBytes(false); 

    // Draw outline around test
    tft.drawRect(9, yPos-1, 52, 52, COLOR_WHITE);
    
    yPos += 60;
    
    // Legend
    tft.setTextSize(1);
    tft.setCursor(5, yPos);
    tft.print("1. Order: R G B W K");
    yPos += 15;
    tft.setCursor(5, yPos);
    tft.print("2. Colors MATCH Ref");
}
