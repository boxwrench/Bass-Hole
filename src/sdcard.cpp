#include "sdcard.h"
#include <SD.h>
#include <SPI.h>

static bool sdReady = false;

bool sdInit() {
    // Initialize SPI for SD card
    SPI.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);

    if (!SD.begin(SD_CS)) {
#if DEBUG_SERIAL
        Serial.println("SD Card: Mount failed!");
#endif
        sdReady = false;
        return false;
    }

    uint8_t cardType = SD.cardType();
    if (cardType == CARD_NONE) {
#if DEBUG_SERIAL
        Serial.println("SD Card: No card attached");
#endif
        sdReady = false;
        return false;
    }

#if DEBUG_SERIAL
    Serial.print("SD Card: ");
    switch (cardType) {
        case CARD_MMC:  Serial.print("MMC");  break;
        case CARD_SD:   Serial.print("SDSC"); break;
        case CARD_SDHC: Serial.print("SDHC"); break;
        default:        Serial.print("Unknown"); break;
    }
    Serial.print(" - Size: ");
    Serial.print(SD.cardSize() / (1024 * 1024));
    Serial.println(" MB");
#endif

    // Create save directory if it doesn't exist
    if (!SD.exists("/save")) {
        SD.mkdir("/save");
    }

    sdReady = true;
    return true;
}

bool sdIsReady() {
    return sdReady;
}

uint64_t sdGetTotalBytes() {
    if (!sdReady) return 0;
    return SD.totalBytes();
}

uint64_t sdGetUsedBytes() {
    if (!sdReady) return 0;
    return SD.usedBytes();
}

// ============================================================================
// FILE OPERATIONS
// ============================================================================

bool sdFileExists(const char* path) {
    if (!sdReady) return false;
    return SD.exists(path);
}

int32_t sdReadFile(const char* path, uint8_t* buffer, size_t maxLen) {
    if (!sdReady) return -1;

    File file = SD.open(path, FILE_READ);
    if (!file) {
#if DEBUG_SERIAL
        Serial.print("SD: Failed to open ");
        Serial.println(path);
#endif
        return -1;
    }

    size_t bytesRead = file.read(buffer, maxLen);
    file.close();

    return bytesRead;
}

int32_t sdWriteFile(const char* path, const uint8_t* data, size_t len) {
    if (!sdReady) return -1;

    File file = SD.open(path, FILE_WRITE);
    if (!file) {
#if DEBUG_SERIAL
        Serial.print("SD: Failed to create ");
        Serial.println(path);
#endif
        return -1;
    }

    size_t bytesWritten = file.write(data, len);
    file.close();

    return bytesWritten;
}

int32_t sdAppendFile(const char* path, const uint8_t* data, size_t len) {
    if (!sdReady) return -1;

    File file = SD.open(path, FILE_APPEND);
    if (!file) {
        return -1;
    }

    size_t bytesWritten = file.write(data, len);
    file.close();

    return bytesWritten;
}

bool sdDeleteFile(const char* path) {
    if (!sdReady) return false;
    return SD.remove(path);
}

// ============================================================================
// GAME DATA
// ============================================================================

bool sdSaveGame(const void* data, size_t len) {
    return sdWriteFile("/save/game.dat", (const uint8_t*)data, len) == (int32_t)len;
}

bool sdLoadGame(void* data, size_t len) {
    return sdReadFile("/save/game.dat", (uint8_t*)data, len) == (int32_t)len;
}
