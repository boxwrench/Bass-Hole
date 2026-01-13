# PlatformIO Setup & Build

This project utilizes [PlatformIO](https://platformio.org/) for development and building. This provides a more robust environment than the standard Arduino IDE, with better dependency management and build speeds.

## Prerequisites

1.  **VS Code**: Install Visual Studio Code.
2.  **PlatformIO Extension**: Install the [PlatformIO IDE](https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide) extension for VS Code.

## Project Structure

- **`src/`**: Contains the source code (`.cpp` files). `main.cpp` is the entry point (formerly `BassHole.ino`).
- **`include/`**: Contains header files (`.h`).
- **`platformio.ini`**: The project configuration file. It defines the board (ESP32 Dev Module), framework (Arduino), and dependencies (TFT_eSPI).

## How to Build

1.  Open this folder in VS Code.
2.  Click the PlatformIO Alien icon in the left sidebar.
3.  Under `Project Tasks` -> `esp32dev`, click **Build**.
4.  To upload to the device, click **Upload**.

## Dependencies

Library dependencies are automatically managed by PlatformIO via `platformio.ini`:
- `TFT_eSPI` by Bodmer
- `XPT2046_Touchscreen` by Paul Stoffregen

No manual library installation is required.
