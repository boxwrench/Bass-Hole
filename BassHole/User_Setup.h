// ============================================================================
// TFT_eSPI User Setup for TZT ESP32 2.4" CYD (Cheap Yellow Display)
// ============================================================================
//
// IMPORTANT: Copy this file to your Arduino libraries folder:
//   Arduino/libraries/TFT_eSPI/User_Setup.h
//
// Or create User_Setup_Select.h that includes this file
// ============================================================================

#define USER_SETUP_INFO "CYD 2.4 inch 240x320"

// ============================================================================
// DISPLAY DRIVER
// ============================================================================

#define ILI9341_DRIVER

// ============================================================================
// DISPLAY PINS (CYD 2.4" standard pinout)
// ============================================================================

#define TFT_MISO  12
#define TFT_MOSI  13
#define TFT_SCLK  14
#define TFT_CS    15
#define TFT_DC    2
#define TFT_RST   -1  // Connected to ESP32 EN (reset)
#define TFT_BL    21  // Backlight control (optional)

// Touch pins (XPT2046)
#define TOUCH_CS  33

// ============================================================================
// DISPLAY SETTINGS
// ============================================================================

#define TFT_WIDTH  240
#define TFT_HEIGHT 320

// ============================================================================
// FONTS
// ============================================================================

#define LOAD_GLCD   // Original Adafruit 8 pixel font
#define LOAD_FONT2  // Small 16 pixel font
#define LOAD_FONT4  // Medium 26 pixel font
// #define LOAD_FONT6  // Large 48 pixel font (disabled to save space)
// #define LOAD_FONT7  // 7 segment 48 pixel font
// #define LOAD_FONT8  // Large 75 pixel font
#define LOAD_GFXFF  // FreeFonts

#define SMOOTH_FONT

// ============================================================================
// SPI SETTINGS
// ============================================================================

#define SPI_FREQUENCY       40000000  // 40 MHz (stable for most CYDs)
// #define SPI_FREQUENCY    55000000  // 55 MHz (faster, may need testing)
// #define SPI_FREQUENCY    80000000  // 80 MHz (maximum, test stability)

#define SPI_READ_FREQUENCY  20000000

#define SPI_TOUCH_FREQUENCY  2500000

// ============================================================================
// OPTIONAL FEATURES
// ============================================================================

// Uncomment for DMA support (faster rendering)
// #define USE_HSPI_PORT

// Transaction support for SD card sharing
#define SUPPORT_TRANSACTIONS
