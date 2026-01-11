#ifndef SDCARD_H
#define SDCARD_H

#include <Arduino.h>
#include "config.h"

// ============================================================================
// SD CARD FUNCTIONS
// ============================================================================

// Initialize SD card
bool sdInit();

// Check if SD card is available
bool sdIsReady();

// Get SD card info
uint64_t sdGetTotalBytes();
uint64_t sdGetUsedBytes();

// ============================================================================
// FILE OPERATIONS
// ============================================================================

// Check if file exists
bool sdFileExists(const char* path);

// Read file into buffer (returns bytes read, -1 on error)
int32_t sdReadFile(const char* path, uint8_t* buffer, size_t maxLen);

// Write buffer to file (returns bytes written, -1 on error)
int32_t sdWriteFile(const char* path, const uint8_t* data, size_t len);

// Append to file
int32_t sdAppendFile(const char* path, const uint8_t* data, size_t len);

// Delete file
bool sdDeleteFile(const char* path);

// ============================================================================
// GAME DATA
// ============================================================================

// Save game state to /save/game.dat
bool sdSaveGame(const void* data, size_t len);

// Load game state from /save/game.dat
bool sdLoadGame(void* data, size_t len);

// ============================================================================
// SPRITE LOADING (for later phases)
// ============================================================================

// Load raw RGB565 sprite data
// bool sdLoadSprite(const char* path, uint16_t* buffer, uint16_t width, uint16_t height);

#endif // SDCARD_H
