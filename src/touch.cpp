#include "touch.h"
#include <XPT2046_Touchscreen.h>
#include <SPI.h>

// Touch controller shares SPI with display (HSPI)
// Use HSPI explicitly to match TFT pins (12, 13, 14)
static SPIClass touchSpi(HSPI);
static XPT2046_Touchscreen touch(TOUCH_CS, TOUCH_IRQ);

// Touch state
static TouchPoint currentTouch;
static TouchPoint lastTap;
static bool wasTouched = false;
static bool tapOccurred = false;
static unsigned long lastTouchTime = 0;

// Calibration values for TZT ESP32 CYD 2.4" (verified 2025-01-14)
// Raw touch ranges: X 600-3600, Y 500-3600
//
// MAPPING (for portrait mode, rotation 0, USB at bottom):
// - Touch rotation: 1
// - X-axis: INVERTED (map from MAX→MIN to 0→WIDTH)
// - Y-axis: NORMAL (map from MIN→MAX to 0→HEIGHT)
//
// This produces accurate touch registration across the entire screen.
//
#define TOUCH_MIN_X 600
#define TOUCH_MAX_X 3600
#define TOUCH_MIN_Y 500
#define TOUCH_MAX_Y 3600

void touchInit()
{
#if DEBUG_SERIAL
    Serial.println("Initializing XPT2046 touch on shared SPI...");
    Serial.print("Touch IRQ pin: ");
    Serial.println(TOUCH_IRQ);
#endif

    // Initialize custom SPI for HSPI bus (shared with TFT)
    touchSpi.begin(14, 12, 13, TOUCH_CS); // CLK=14, MISO=12, MOSI=13, SS=33

    pinMode(TOUCH_IRQ, INPUT_PULLUP);
    pinMode(TOUCH_CS, OUTPUT);
    digitalWrite(TOUCH_CS, HIGH); // Start with touch disabled

    // Pass custom SPI instance to library
    touch.begin(touchSpi);
    touch.setRotation(1); // Matches Display Landscape

    currentTouch.valid = false;
    lastTap.valid = false;
    wasTouched = false;
    tapOccurred = false;

#if DEBUG_SERIAL
    Serial.println("Touch configuration applied (Rotation 0, IRQ PULLUP)");

    // Test touch controller communication
    Serial.println("Testing touch controller...");
    TS_Point testPoint = touch.getPoint();
    Serial.print("Initial read -> X:");
    Serial.print(testPoint.x);
    Serial.print(" Y:");
    Serial.print(testPoint.y);
    Serial.print(" Z:");
    Serial.println(testPoint.z);

    if (testPoint.x == -4096 || testPoint.y == -4096)
    {
        Serial.println("WARNING: Touch controller not responding properly!");
        Serial.println("Check SPI wiring and shared bus configuration.");
    }
    else
    {
        Serial.println("Touch controller communication OK");
    }
#endif
}

void touchUpdate()
{
    tapOccurred = false;

    // On CYD, TOUCH_IRQ (36) is LOW when the screen is touched.
    bool irqTouched = (digitalRead(TOUCH_IRQ) == LOW);

    if (!irqTouched)
    {
        if (wasTouched)
        {
            // Touch released - this is a tap
            unsigned long now = millis();
            if (now - lastTouchTime > TOUCH_DEBOUNCE_MS)
            {
                tapOccurred = true;
                lastTap = currentTouch;
                lastTouchTime = now;
#if DEBUG_TOUCH
                Serial.println("Touch Released (Tap)");
#endif
            }
        }
        currentTouch.valid = false;
        wasTouched = false;
        return;
    }

    // IRQ says touched, read SPI coordinates
    TS_Point p = touch.getPoint();

    // Filter out obvious noise
    bool pressureOk = (p.z >= TOUCH_MIN_PRESSURE && p.z < 5000);

#if DEBUG_TOUCH
    if (pressureOk || p.z > 50)
    {
        Serial.print("Raw touch -> X:");
        Serial.print(p.x);
        Serial.print(" Y:");
        Serial.print(p.y);
        Serial.print(" Z:");
        Serial.println(p.z);
    }
#endif

    if (pressureOk)
    {
        // Map raw coordinates (Landscape 320x240)
        // AXES SWAPPED for Landscape (Fixes Anti-Diagonal inversion)
        // map(p.y, MIN_Y, MAX_Y) -> Screen X
        // map(p.x, MIN_X, MAX_X) -> Screen Y

        int16_t mappedX = map(p.y, TOUCH_MIN_Y, TOUCH_MAX_Y, 0, SCREEN_WIDTH);
        // Note: Check if Y needs inversion (MAX->MIN) depending on visual test
        // Previously TR->BL implies both swapped.
        int16_t mappedY = map(p.x, TOUCH_MIN_X, TOUCH_MAX_X, 0, SCREEN_HEIGHT);

        // Clamp to screen
        mappedX = constrain(mappedX, 0, SCREEN_WIDTH - 1);
        mappedY = constrain(mappedY, 0, SCREEN_HEIGHT - 1);

        currentTouch.x = mappedX;
        currentTouch.y = mappedY;
        currentTouch.pressure = p.z;
        currentTouch.valid = true;
#if DEBUG_TOUCH
        Serial.printf("Touch Mapped: X:%d Y:%d (Raw: %d,%d, Z:%d)\n", mappedX, mappedY, p.x, p.y, p.z);
#endif
        wasTouched = true;
    }
    else
    {
        currentTouch.valid = false;
        wasTouched = false;
    }
}

TouchPoint touchGet()
{
    return currentTouch;
}

bool touchIsPressed()
{
    return currentTouch.valid;
}

bool touchTapped()
{
    return tapOccurred;
}

TouchPoint touchGetTap()
{
    return lastTap;
}

void touchCalibrate()
{
    // Simple calibration routine - prints instructions for manual calibration

#if DEBUG_SERIAL
    Serial.println();
    Serial.println("========================================");
    Serial.println("       TOUCH CALIBRATION MODE");
    Serial.println("========================================");
    Serial.println("Touch each corner firmly for 1 second:");
    Serial.println("1. TOP-LEFT corner");
    Serial.println("2. TOP-RIGHT corner");
    Serial.println("3. BOTTOM-LEFT corner");
    Serial.println("4. BOTTOM-RIGHT corner");
    Serial.println("5. CENTER of screen");
    Serial.println();
    Serial.println("Record the RAW X,Y values for each.");
    Serial.println("Then update TOUCH_MIN/MAX in touch.cpp");
    Serial.println("========================================");
#endif
}
