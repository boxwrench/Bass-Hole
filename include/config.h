#ifndef CONFIG_H
#define CONFIG_H

// ============================================================================
// HARDWARE CONFIGURATION - TZT ESP32 2.4" CYD (Cheap Yellow Display)
// ============================================================================

// Display: ILI9341 320x240 (Landscape)
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

// TFT_eSPI pins are configured in User_Setup.h of the library
// For CYD 2.4": TFT_MOSI=13, TFT_SCLK=14, TFT_CS=15, TFT_DC=2, TFT_RST=-1

// Touch: XPT2046
#define TOUCH_CS 33
#define TOUCH_IRQ 36

// SD Card (directly on board)
#define SD_CS 5
#define SD_MOSI 23
#define SD_MISO 19
#define SD_SCK 18

// Onboard LED (active LOW on most CYDs)
#define LED_PIN 4

// LDR (light sensor) - some boards have this
#define LDR_PIN 34

// ============================================================================
// DISPLAY COLORS (RGB565 format)
// ============================================================================
//
// VERIFIED: TZT ESP32 CYD 2.4" uses invertDisplay(true) with normal RGB565 values
//
// If colors look wrong on your board:
// 1. Call gfxDrawColorTest() in main.cpp setup() to run color diagnostic
// 2. If "1. RAW" test is correct → use invertDisplay(true) + these normal values
// 3. If "2. XOR" test is correct → use invertDisplay(false) + XOR these values
//
// Different display manufacturers may require different inversion settings.
//
#define COLOR_BLACK 0x0000       // Black
#define COLOR_WHITE 0xFFFF       // White
#define COLOR_WATER_DEEP 0x0019  // Deep blue
#define COLOR_WATER_MID 0x03BF   // Ocean blue
#define COLOR_WATER_LIGHT 0x7F3F // Aqua
#define COLOR_SAND 0xD5AA        // Tan/sand bottom
#define COLOR_COIN_GOLD 0xFE60   // Gold for coins
#define COLOR_FOOD_BROWN 0x8A22  // Fish food pellets
#define COLOR_UI_GREEN 0x07E0    // UI elements (green)
#define COLOR_UI_RED 0xF800      // Warnings/damage (red)
#define COLOR_TEXT 0xFFFF        // Default text (white)

// ============================================================================
// GAME CONSTANTS
// ============================================================================

// Timing
#define TARGET_FPS 30
#define FRAME_TIME_MS (1000 / TARGET_FPS)

// Tank dimensions (play area within screen)
#define TANK_LEFT 0
#define TANK_TOP 40 // Leave room for UI at top
#define TANK_RIGHT SCREEN_WIDTH
#define TANK_BOTTOM (SCREEN_HEIGHT - 40) // Leave room for UI at bottom
#define TANK_WIDTH (TANK_RIGHT - TANK_LEFT)
#define TANK_HEIGHT (TANK_BOTTOM - TANK_TOP)

// Fish settings
#define MAX_FISH 10 // Max fish on screen at once
#define FISH_WIDTH 24
#define FISH_HEIGHT 16
#define FISH_SPEED_MIN 0.5f
#define FISH_SPEED_MAX 1.5f
#define FISH_HUNGER_MAX 100    // Hunger meter max value
#define FISH_HUNGER_RATE 1     // Hunger decrease per second
#define FISH_STARVE_TIME 30000 // ms until fish dies if not fed

// Food settings
#define MAX_FOOD 15          // Max food pellets on screen
#define FOOD_SIZE 4          // Pellet radius
#define FOOD_FALL_SPEED 1.0f // Pixels per frame
#define FOOD_COST 5          // Coins per pellet (later upgrade)

// Coin settings
#define MAX_COINS 20          // Max coins on screen
#define COIN_SIZE 8           // Coin radius
#define COIN_FLOAT_SPEED 0.3f // How fast coins float up
#define COIN_LIFETIME 5000    // ms before coin despawns

// Economy
#define STARTING_COINS 50
#define FISH_COST_BASIC 25  // Cost to buy a rainbow trout
#define COIN_VALUE_SMALL 1  // Small fish coin drop
#define COIN_VALUE_MEDIUM 3 // Medium fish coin drop
#define COIN_VALUE_LARGE 5  // Large fish coin drop

// Touch settings
#define TOUCH_DEBOUNCE_MS 100  // Minimum ms between touch events
#define TOUCH_MIN_PRESSURE 200 // Minimum pressure to register touch (increase if phantom touches)

// ============================================================================
// FISH SPECIES DATA
// ============================================================================

enum FishSpecies
{
    FISH_RAINBOW_TROUT = 0, // Starter fish
    FISH_BLUEGILL,          // Fast breeder, low value
    FISH_SMALLMOUTH_BASS,   // Mid-tier
    FISH_CHANNEL_CATFISH,   // Slow, high value
    FISH_LARGEMOUTH_BASS,   // Ty's species - premium
    FISH_SPECIES_COUNT
};

// Fish stats: {hunger_rate, coin_value, speed_mult, growth_stages}
struct FishStats
{
    const char *name;
    uint8_t hungerRate;   // How fast they get hungry
    uint8_t coinValue;    // Base coin drop value
    float speedMult;      // Speed multiplier
    uint8_t growthStages; // How many times they grow
    uint16_t cost;        // Shop cost
};

// Defined in fish.cpp
extern const FishStats FISH_DATA[];

// ============================================================================
// GAME STATES
// ============================================================================

enum GameState
{
    STATE_BOOT,     // Initial boot/splash
    STATE_TITLE,    // Title screen
    STATE_PLAYING,  // Main gameplay
    STATE_SHOP,     // Buy fish/upgrades
    STATE_BOSS,     // Boss battle
    STATE_GAMEOVER, // Game over
    STATE_PAUSED    // Paused
};

// ============================================================================
// DEBUG FLAGS
// ============================================================================

#define DEBUG_SERIAL 1 // Enable serial debug output
#define DEBUG_FPS 1    // Show FPS counter
#define DEBUG_TOUCH 1  // Show touch coordinates
#define DEBUG_FISH 0   // Show fish state info

#endif // CONFIG_H
