// TFT_eSPI User Setup for ESP32 CYD 2.4" (Cheap Yellow Display)
// Working configuration for TZT ESP32 LVGL 2.4" from AliExpress
//
// IMPORTANT: Copy this file to your TFT_eSPI library folder:
//   Arduino/libraries/TFT_eSPI/User_Setup.h

#define USER_SETUP_INFO "ESP32 CYD 2.4"

// Display driver
#define ILI9341_DRIVER

// Display dimensions
#define TFT_WIDTH  240
#define TFT_HEIGHT 320

// ESP32 CYD pins for display
#define TFT_MISO 12
#define TFT_MOSI 13
#define TFT_SCLK 14
#define TFT_CS   15
#define TFT_DC   2
#define TFT_RST  -1
#define TFT_BL   21

// Touch CS pin
#define TOUCH_CS 33

// Fonts
#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_GFXFF
#define SMOOTH_FONT

// SPI speed
#define SPI_FREQUENCY       40000000
#define SPI_READ_FREQUENCY  20000000
#define SPI_TOUCH_FREQUENCY 2500000

// Color order - CYD uses BGR
#define TFT_RGB_ORDER TFT_BGR

// Required for SD card sharing
#define SUPPORT_TRANSACTIONS

// Runtime settings needed in graphics.cpp:
//   tft.setRotation(3);      // Portrait, USB at bottom
//   tft.invertDisplay(true); // Required for correct colors
