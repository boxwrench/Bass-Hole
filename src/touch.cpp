#include "touch.h"
#include <XPT2046_Touchscreen.h>
#include <SPI.h>

// Touch controller
static XPT2046_Touchscreen touch(TOUCH_CS, TOUCH_IRQ);

// Touch state
static TouchPoint currentTouch;
static TouchPoint lastTap;
static bool wasTouched = false;
static bool tapOccurred = false;
static unsigned long lastTouchTime = 0;

// Calibration values (adjust for your specific screen)
// These map raw touch coordinates to screen coordinates
#define TOUCH_MIN_X  200
#define TOUCH_MAX_X  3800
#define TOUCH_MIN_Y  200
#define TOUCH_MAX_Y  3800

void touchInit() {
    touch.begin();
    touch.setRotation(0);  // Match display rotation

    currentTouch.valid = false;
    lastTap.valid = false;
    wasTouched = false;
    tapOccurred = false;

#if DEBUG_SERIAL
    Serial.println("Touch initialized");
#endif
}

void touchUpdate() {
    tapOccurred = false;

    bool touched = touch.touched();

    if (touched) {
        TS_Point p = touch.getPoint();

        // Check minimum pressure
        if (p.z < TOUCH_MIN_PRESSURE) {
            touched = false;
        } else {
            // Map raw coordinates to screen coordinates
            // Note: You may need to swap X/Y or invert based on your screen orientation
            int16_t mappedX = map(p.x, TOUCH_MIN_X, TOUCH_MAX_X, 0, SCREEN_WIDTH);
            int16_t mappedY = map(p.y, TOUCH_MIN_Y, TOUCH_MAX_Y, 0, SCREEN_HEIGHT);

            // Clamp to screen bounds
            mappedX = constrain(mappedX, 0, SCREEN_WIDTH - 1);
            mappedY = constrain(mappedY, 0, SCREEN_HEIGHT - 1);

            currentTouch.x = mappedX;
            currentTouch.y = mappedY;
            currentTouch.pressure = p.z;
            currentTouch.valid = true;

#if DEBUG_TOUCH
            Serial.print("Touch: ");
            Serial.print(mappedX);
            Serial.print(", ");
            Serial.print(mappedY);
            Serial.print(" (raw: ");
            Serial.print(p.x);
            Serial.print(", ");
            Serial.print(p.y);
            Serial.println(")");
#endif
        }
    }

    // Detect tap (touch released)
    unsigned long now = millis();
    if (!touched && wasTouched) {
        // Touch was released - this is a tap
        if (now - lastTouchTime > TOUCH_DEBOUNCE_MS) {
            tapOccurred = true;
            lastTap = currentTouch;
            lastTouchTime = now;
        }
    }

    if (!touched) {
        currentTouch.valid = false;
    }

    wasTouched = touched;
}

TouchPoint touchGet() {
    return currentTouch;
}

bool touchIsPressed() {
    return currentTouch.valid;
}

bool touchTapped() {
    return tapOccurred;
}

TouchPoint touchGetTap() {
    return lastTap;
}

void touchCalibrate() {
    // Simple calibration routine
    // Touch the corners when prompted and record raw values

#if DEBUG_SERIAL
    Serial.println("=== TOUCH CALIBRATION ===");
    Serial.println("Touch corners when prompted");
    Serial.println("Record the raw X,Y values shown");
    Serial.println("Update TOUCH_MIN/MAX values in touch.cpp");
    Serial.println("=========================");
#endif

    // In a full implementation, this would:
    // 1. Show "Touch top-left" on screen
    // 2. Wait for touch, record raw values
    // 3. Repeat for other corners
    // 4. Calculate and save calibration
}
