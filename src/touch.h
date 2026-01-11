#ifndef TOUCH_H
#define TOUCH_H

#include <Arduino.h>
#include "config.h"

// ============================================================================
// TOUCH DATA
// ============================================================================

struct TouchPoint {
    int16_t x;
    int16_t y;
    uint16_t pressure;
    bool valid;
};

// ============================================================================
// TOUCH FUNCTIONS
// ============================================================================

// Initialize touch controller
void touchInit();

// Read current touch state (call once per frame)
void touchUpdate();

// Get current touch point (valid only if touched)
TouchPoint touchGet();

// Check if screen is currently being touched
bool touchIsPressed();

// Check if a new tap occurred this frame
bool touchTapped();

// Get the tap position (only valid if touchTapped() is true)
TouchPoint touchGetTap();

// Calibration (for debug/setup)
void touchCalibrate();

#endif // TOUCH_H
