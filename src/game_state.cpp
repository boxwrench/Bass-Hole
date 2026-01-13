#include "game_state.h"
#include "fish.h"
#include "food.h"
#include "coins.h"
#include "sdcard.h"

// Global game data
GameData game;

void gameStateInit() {
    game.state = STATE_BOOT;
    game.previousState = STATE_BOOT;
    game.coins = STARTING_COINS;
    game.highScore = 0;
    game.currentLevel = 1;
    game.fishUnlocked = 0x01;  // Only rainbow trout unlocked initially

    game.totalCoinsEarned = 0;
    game.fishFed = 0;
    game.fishLost = 0;
    game.enemiesDefeated = 0;
    game.bossesDefeated = 0;

    game.lastUpdate = millis();
    game.playTime = 0;

    game.isPaused = false;
    game.soundEnabled = true;
    game.tutorialComplete = false;

    // Try to load saved game
    if (sdIsReady() && gameSaveExists()) {
        gameLoad();
    }
}

void gameStateChange(GameState newState) {
    game.previousState = game.state;
    game.state = newState;

#if DEBUG_SERIAL
    Serial.print("State change: ");
    Serial.print(game.previousState);
    Serial.print(" -> ");
    Serial.println(game.state);
#endif
}

void gameStateUpdate() {
    unsigned long now = millis();
    unsigned long deltaTime = now - game.lastUpdate;
    game.lastUpdate = now;

    if (!game.isPaused && game.state == STATE_PLAYING) {
        game.playTime += deltaTime;
    }
}

void gameStateReset() {
    game.coins = STARTING_COINS;
    game.currentLevel = 1;
    game.playTime = 0;
    game.isPaused = false;

    // Reset fish, food, coins
    fishInit();
    foodInit();
    coinsInit();

    // Spawn starting fish
    fishSpawn(FISH_RAINBOW_TROUT, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

    gameStateChange(STATE_PLAYING);
}

// ============================================================================
// SAVE/LOAD
// ============================================================================

#define SAVE_MAGIC 0xBA55  // "BASS"

struct SaveData {
    uint16_t magic;
    uint32_t coins;
    uint32_t highScore;
    uint8_t currentLevel;
    uint8_t fishUnlocked;
    uint32_t totalCoinsEarned;
    uint32_t playTime;
    bool tutorialComplete;
    uint8_t checksum;
};

uint8_t calculateChecksum(const SaveData* data) {
    uint8_t sum = 0;
    const uint8_t* bytes = (const uint8_t*)data;
    // Sum all bytes except checksum itself
    for (size_t i = 0; i < sizeof(SaveData) - 1; i++) {
        sum ^= bytes[i];
    }
    return sum;
}

bool gameSave() {
    if (!sdIsReady()) return false;

    SaveData data;
    data.magic = SAVE_MAGIC;
    data.coins = game.coins;
    data.highScore = game.highScore;
    data.currentLevel = game.currentLevel;
    data.fishUnlocked = game.fishUnlocked;
    data.totalCoinsEarned = game.totalCoinsEarned;
    data.playTime = game.playTime;
    data.tutorialComplete = game.tutorialComplete;
    data.checksum = calculateChecksum(&data);

    return sdSaveGame(&data, sizeof(SaveData));
}

bool gameLoad() {
    if (!sdIsReady()) return false;

    SaveData data;
    if (!sdLoadGame(&data, sizeof(SaveData))) {
        return false;
    }

    // Validate save
    if (data.magic != SAVE_MAGIC) {
#if DEBUG_SERIAL
        Serial.println("Save: Invalid magic number");
#endif
        return false;
    }

    if (data.checksum != calculateChecksum(&data)) {
#if DEBUG_SERIAL
        Serial.println("Save: Checksum mismatch");
#endif
        return false;
    }

    // Load data
    game.coins = data.coins;
    game.highScore = data.highScore;
    game.currentLevel = data.currentLevel;
    game.fishUnlocked = data.fishUnlocked;
    game.totalCoinsEarned = data.totalCoinsEarned;
    game.playTime = data.playTime;
    game.tutorialComplete = data.tutorialComplete;

#if DEBUG_SERIAL
    Serial.println("Game loaded successfully");
#endif
    return true;
}

bool gameSaveExists() {
    return sdFileExists("/save/game.dat");
}
