#include "graphics.h"
#include "fish.h"
#include "food.h"
#include "coins.h"
#include "game_state.h"
#include <TFT_eSPI.h>
#include <math.h>
#include <string.h>

// TFT display instance
static TFT_eSPI tft = TFT_eSPI();

// Sprite for double buffering (optional, uses more RAM)
// static TFT_eSprite sprite = TFT_eSprite(&tft);

void gfxInit() {
    // Turn on backlight - try both common CYD pins
    pinMode(21, OUTPUT);
    digitalWrite(21, HIGH);
    pinMode(27, OUTPUT);
    digitalWrite(27, HIGH);

    tft.init();

    // Portrait mode with USB at bottom
    // Rotation 0 should give portrait with USB at bottom (180° from rotation 2)
    tft.setRotation(0);
    tft.invertDisplay(true);  // Required for correct colors

    // Aggressive clear in ALL rotations to remove ghost images
    for (int r = 0; r < 4; r++) {
        tft.setRotation(r);
        tft.fillScreen(TFT_BLACK);
    }

    // Set final rotation
    tft.setRotation(0);

#if DEBUG_SERIAL
    Serial.println("=== DISPLAY INIT v2025.01.13.A ===");  // Unique identifier for THIS version
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

void gfxClear(uint16_t color) {
    tft.fillScreen(color);
}

void gfxBeginFrame() {
    // If using sprite buffering, would create sprite here
}

void gfxEndFrame() {
    // If using sprite buffering, would push sprite to display here
}

// ============================================================================
// GAME RENDERING
// ============================================================================

void gfxDrawTank() {
    // Draw water gradient (simplified - just solid colors for Phase 1)
    // Top of tank - lighter blue
    tft.fillRect(TANK_LEFT, TANK_TOP, TANK_WIDTH, TANK_HEIGHT / 3, COLOR_WATER_LIGHT);
    // Middle - medium blue
    tft.fillRect(TANK_LEFT, TANK_TOP + TANK_HEIGHT / 3, TANK_WIDTH, TANK_HEIGHT / 3, COLOR_WATER_MID);
    // Bottom - darker blue
    tft.fillRect(TANK_LEFT, TANK_TOP + 2 * TANK_HEIGHT / 3, TANK_WIDTH, TANK_HEIGHT / 3, COLOR_WATER_DEEP);

    // Sand at very bottom
    tft.fillRect(TANK_LEFT, TANK_BOTTOM - 10, TANK_WIDTH, 10, COLOR_SAND);
}

void gfxDrawFish(Fish* fish) {
    if (!fish || !fish->active) return;

    // Scale based on growth stage
    float scale = 1.0f + fish->growthStage * 0.3f;
    int16_t w = (int16_t)(FISH_WIDTH * scale);
    int16_t h = (int16_t)(FISH_HEIGHT * scale);

    int16_t x = (int16_t)fish->x - w / 2;
    int16_t y = (int16_t)fish->y - h / 2;

    // Fish body color based on species
    uint16_t bodyColor;
    switch (fish->species) {
        case FISH_RAINBOW_TROUT:
            bodyColor = tft.color565(200, 150, 150);  // Pinkish
            break;
        case FISH_BLUEGILL:
            bodyColor = tft.color565(100, 100, 200);  // Blue
            break;
        case FISH_SMALLMOUTH_BASS:
            bodyColor = tft.color565(100, 150, 100);  // Green
            break;
        case FISH_CHANNEL_CATFISH:
            bodyColor = tft.color565(100, 80, 60);    // Brown
            break;
        case FISH_LARGEMOUTH_BASS:
            bodyColor = tft.color565(50, 120, 50);    // Dark green
            break;
        default:
            bodyColor = COLOR_WHITE;
    }

    // Simple fish shape (ellipse body + triangle tail)
    // Body
    tft.fillEllipse(fish->x, fish->y, w / 2, h / 2, bodyColor);

    // Tail (triangle)
    int16_t tailDir = fish->facingRight ? -1 : 1;
    int16_t tailX = fish->x + tailDir * (w / 2);
    tft.fillTriangle(
        tailX, fish->y,
        tailX + tailDir * (w / 3), fish->y - h / 3,
        tailX + tailDir * (w / 3), fish->y + h / 3,
        bodyColor
    );

    // Eye
    int16_t eyeX = fish->x + (fish->facingRight ? w / 4 : -w / 4);
    tft.fillCircle(eyeX, fish->y - h / 6, 2, COLOR_BLACK);

    // Hunger indicator (red tint when hungry)
    if (fishIsHungry(fish)) {
        tft.drawEllipse(fish->x, fish->y, w / 2 + 1, h / 2 + 1, COLOR_UI_RED);
    }
}

void gfxDrawAllFish() {
    for (int i = 0; i < MAX_FISH; i++) {
        gfxDrawFish(&fishPool[i]);
    }
}

void gfxDrawFood(Food* food) {
    if (!food || !food->active) return;

    // Simple brown circle for food pellet
    tft.fillCircle((int16_t)food->x, (int16_t)food->y, FOOD_SIZE, COLOR_FOOD_BROWN);
}

void gfxDrawAllFood() {
    for (int i = 0; i < MAX_FOOD; i++) {
        gfxDrawFood(&foodPool[i]);
    }
}

void gfxDrawCoin(Coin* coin) {
    if (!coin || !coin->active) return;

    // Bob animation
    float displayX = coin->x + sinf(coin->floatOffset) * 3;

    // Coin size based on value
    int16_t size = COIN_SIZE + (coin->value > 3 ? 2 : 0);

    // Gold circle with darker outline
    tft.fillCircle((int16_t)displayX, (int16_t)coin->y, size, COLOR_COIN_GOLD);
    tft.drawCircle((int16_t)displayX, (int16_t)coin->y, size, tft.color565(180, 130, 0));

    // $ symbol for larger coins
    if (coin->value > 2) {
        tft.setTextColor(tft.color565(180, 130, 0));
        tft.setTextSize(1);
        tft.setCursor((int16_t)displayX - 2, (int16_t)coin->y - 3);
        tft.print("$");
    }
}

void gfxDrawAllCoins() {
    for (int i = 0; i < MAX_COINS; i++) {
        gfxDrawCoin(&coinPool[i]);
    }
}

void gfxDrawUI() {
    // Top bar - black background
    tft.fillRect(0, 0, SCREEN_WIDTH, TANK_TOP, COLOR_BLACK);

    // Coin display
    tft.setTextColor(COLOR_COIN_GOLD);
    tft.setTextSize(2);
    tft.setCursor(5, 10);
    tft.print("$");
    tft.print(game.coins);

    // Fish count
    tft.setTextColor(COLOR_TEXT);
    tft.setCursor(SCREEN_WIDTH - 60, 10);
    tft.print("Fish:");
    tft.print(fishGetCount());

    // Bottom bar - bright color to verify it renders
    tft.fillRect(0, TANK_BOTTOM, SCREEN_WIDTH, SCREEN_HEIGHT - TANK_BOTTOM, COLOR_UI_GREEN);

    // DEBUG: Simple text using same approach as header (which works)
    tft.setTextColor(COLOR_BLACK);  // Black on green
    tft.setTextSize(2);
    tft.setCursor(10, TANK_BOTTOM + 10);
    tft.print("FOOTER TEST");

    // BUTTON CODE TEMPORARILY DISABLED FOR DEBUGGING
    /*
    // Buy Fish Button - centered, highly visible
    int16_t btnWidth = 100;
    int16_t btnHeight = 30;
    int16_t btnX = (SCREEN_WIDTH - btnWidth) / 2;
    int16_t btnY = TANK_BOTTOM + 5;

    // Button background - bright green when affordable, dark when not
    uint16_t btnColor = game.coins >= FISH_COST_BASIC ? COLOR_UI_GREEN : tft.color565(80, 80, 80);
    tft.fillRect(btnX, btnY, btnWidth, btnHeight, btnColor);
    tft.drawRect(btnX, btnY, btnWidth, btnHeight, COLOR_COIN_GOLD);

    // Button text - FIXED: Use opaque text rendering without background parameter
    // This works better with inverted displays
    uint16_t textColor = game.coins >= FISH_COST_BASIC ? COLOR_BLACK : COLOR_WHITE;

    // Draw a small background rect for the text first (ensures proper contrast)
    tft.fillRect(btnX + 2, btnY + 2, btnWidth - 4, btnHeight - 4, btnColor);

    // Now draw text with single-parameter setTextColor (no background)
    tft.setTextColor(textColor);
    tft.setTextSize(2);

    // Format complete string first, then print once
    char btnText[16];
    snprintf(btnText, sizeof(btnText), "BUY $%d", FISH_COST_BASIC);

    // Center the text (7 chars * 12px/char at size 2 = 84px)
    int16_t textWidth = strlen(btnText) * 12;
    tft.setCursor(btnX + (btnWidth - textWidth) / 2, btnY + 7);
    tft.print(btnText);

    // High score - right side, explicit background
    tft.setTextColor(COLOR_TEXT, COLOR_WATER_DEEP);
    tft.setTextSize(1);
    tft.setCursor(SCREEN_WIDTH - 45, TANK_BOTTOM + 5);
    tft.print("Hi:");
    tft.print(game.highScore);
    */
}

void gfxDrawFPS(uint16_t fps) {
    tft.setTextColor(COLOR_UI_GREEN, COLOR_BLACK);
    tft.setTextSize(1);
    tft.setCursor(SCREEN_WIDTH - 35, 2);
    tft.print(fps);
    tft.print("fps");
}

// ============================================================================
// PRIMITIVES
// ============================================================================

void gfxFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    tft.fillRect(x, y, w, h, color);
}

void gfxDrawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    tft.drawRect(x, y, w, h, color);
}

void gfxFillCircle(int16_t x, int16_t y, int16_t r, uint16_t color) {
    tft.fillCircle(x, y, r, color);
}

void gfxDrawCircle(int16_t x, int16_t y, int16_t r, uint16_t color) {
    tft.drawCircle(x, y, r, color);
}

void gfxDrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
    tft.drawLine(x0, y0, x1, y1, color);
}

void gfxDrawText(const char* text, int16_t x, int16_t y, uint16_t color, uint8_t size) {
    tft.setTextColor(color);
    tft.setTextSize(size);
    tft.setCursor(x, y);
    tft.print(text);
}
