#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <Arduino.h>
#include "config.h"

// Forward declarations
struct Fish;
struct Food;
struct Coin;

// ============================================================================
// GAME DATA STRUCTURES
// ============================================================================

struct GameData {
    // Current state
    GameState state;
    GameState previousState;

    // Economy
    uint32_t coins;
    uint32_t highScore;

    // Level/progression
    uint8_t currentLevel;
    uint8_t fishUnlocked;  // Bitmask of unlocked species

    // Stats
    uint32_t totalCoinsEarned;
    uint32_t fishFed;
    uint32_t fishLost;
    uint32_t enemiesDefeated;
    uint32_t bossesDefeated;

    // Timing
    unsigned long lastUpdate;
    unsigned long playTime;  // Total play time in ms

    // Flags
    bool isPaused;
    bool soundEnabled;
    bool tutorialComplete;
};

// Global game data
extern GameData game;

// ============================================================================
// GAME STATE FUNCTIONS
// ============================================================================

// Initialize game to default state
void gameStateInit();

// Change game state with transition
void gameStateChange(GameState newState);

// Update current state (called each frame)
void gameStateUpdate();

// Reset game (new game)
void gameStateReset();

// ============================================================================
// SAVE/LOAD (SD Card)
// ============================================================================

// Save game to SD card
bool gameSave();

// Load game from SD card
bool gameLoad();

// Check if save exists
bool gameSaveExists();

#endif // GAME_STATE_H
