# Bass Hole

**"Knot Your Average Fishing Game"**

An open-source Insaniquarium-style game for the ESP32 CYD (Cheap Yellow Display). Feed fish, collect coins, and survive the sass of Ty Knotts - the grumpiest largemouth bass guide in Northern California.

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![Platform](https://img.shields.io/badge/platform-ESP32-green.svg)
![Display](https://img.shields.io/badge/display-CYD%202.4%22-yellow.svg)

<!-- TODO: Add gameplay gif here -->
<!-- ![Gameplay](docs/images/gameplay.gif) -->

## About This Project

Bass Hole is designed as both a playable game and an **educational reference** for ESP32 CYD game development. The codebase demonstrates:

- Touch input handling on CYD hardware
- Game state management
- Entity systems (fish, food, coins)
- SD card save/load functionality
- Simple 2D rendering with TFT_eSPI
- Modular code architecture for embedded games

Whether you want to play the game, learn ESP32 game development, or fork it to build your own - welcome aboard.

## Features

### Current (Phase 1)
- [x] Core gameplay loop (feed fish → collect coins)
- [x] Fish AI with hunger, growth, and coin generation
- [x] Touch input for food drops and coin collection
- [x] Multiple fish species with unique stats
- [x] Game state save/load to SD card
- [x] Simple shape-based graphics

### Planned
- [ ] Sprite-based graphics (16-color dithered fish)
- [ ] Ty Knotts narrator with dialogue system
- [ ] Enemy fishermen (Clankers, Cogsuckers)
- [ ] Boss battles with AI parody characters
- [ ] Shop system for upgrades
- [ ] Sound effects via ESP32 DAC
- [ ] Particle effects

## Hardware Requirements

| Component | Specification |
|-----------|---------------|
| Board | ESP32 CYD 2.4" (TZT/Sunton) |
| Display | ILI9341 240x320 LCD |
| Touch | XPT2046 resistive |
| Storage | MicroSD card (FAT32) |

**Tested on:** TZT ESP32 LVGL WiFi & Bluetooth 2.4" LCD TFT Module

Other 2.4" CYD variants should work with minor pin adjustments. See [docs/HARDWARE.md](docs/HARDWARE.md) for pinout details.

## Quick Start

### 1. Clone the Repository

```bash
git clone https://github.com/boxwrench/Bass-Hole.git
cd Bass-Hole
```

### 2. Install Dependencies

**Arduino IDE:**
1. Install [ESP32 board support](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html)
2. Install libraries via Library Manager:
   - `TFT_eSPI` by Bodmer
   - `XPT2046_Touchscreen` by Paul Stoffregen

### 3. Configure TFT_eSPI

Copy `BassHole/User_Setup.h` to your TFT_eSPI library folder:
- Windows: `Documents\Arduino\libraries\TFT_eSPI\`
- Mac/Linux: `~/Arduino/libraries/TFT_eSPI/`

This configures the correct pins for the CYD 2.4" display.

### 4. Upload

1. Open `BassHole/BassHole.ino` in Arduino IDE
2. Select **Board:** "ESP32 Dev Module"
3. Select your COM port
4. Click Upload

### 5. Play!

- **Tap the tank** to drop food
- **Tap coins** to collect them
- **Don't let your fish starve!**

## Project Structure

```
Bass-Hole/
├── BassHole/               # Arduino sketch folder
│   ├── BassHole.ino        # Main entry point
│   ├── config.h            # Hardware pins & game constants
│   ├── game_state.h/cpp    # State machine, save/load
│   ├── fish.h/cpp          # Fish entity system
│   ├── food.h/cpp          # Food pellet system
│   ├── coins.h/cpp         # Coin system
│   ├── touch.h/cpp         # Touch input handling
│   ├── graphics.h/cpp      # Rendering layer
│   ├── sdcard.h/cpp        # SD card operations
│   └── User_Setup.h        # TFT_eSPI configuration
├── assets/                 # Art assets (sprites, backgrounds)
├── tools/                  # Asset conversion scripts
├── docs/                   # Documentation
│   ├── ARCHITECTURE.md     # Code structure explained
│   ├── HARDWARE.md         # CYD variants & pinouts
│   ├── ADDING_FISH.md      # Tutorial: add new fish species
│   └── ASSETS.md           # Sprite creation guide
├── sdcard/                 # Files to copy to SD card
├── SETUP.md                # Detailed setup instructions
├── CONTRIBUTING.md         # How to contribute
├── LICENSE                 # MIT License
└── README.md               # You are here
```

## Documentation

| Document | Description |
|----------|-------------|
| [SETUP.md](SETUP.md) | Detailed installation & troubleshooting |
| [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md) | How the code is organized |
| [docs/HARDWARE.md](docs/HARDWARE.md) | CYD variants, pinouts, mods |
| [docs/ADDING_FISH.md](docs/ADDING_FISH.md) | Tutorial: Add a new fish species |
| [docs/ASSETS.md](docs/ASSETS.md) | Creating & converting sprites |
| [DEVELOPMENT_STRATEGY.md](DEVELOPMENT_STRATEGY.md) | Full game design document |

## Contributing

Contributions are welcome! See [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

**Good first issues:**
- Add new fish species
- Improve touch calibration
- Add screen shake effects
- Write Ty Knotts dialogue

## Game Design

**Narrator:** Ty Knotts - A grumpy largemouth bass who guides players with terse, sarcastic commentary. Think Ron Swanson as a fishing guide.

**Visual Style:**
- Fish: 16-color dithered realistic sprites
- Characters: Bold cartoon style
- UI: Clean pixel art

**NorCal Theme:** All fish species are native to Northern California waters.

See [DEVELOPMENT_STRATEGY.md](DEVELOPMENT_STRATEGY.md) for the complete design document.

## Tech Stack

- **MCU:** ESP32-WROOM-32
- **Display:** ILI9341 via TFT_eSPI
- **Touch:** XPT2046 resistive touchscreen
- **Storage:** SD card via SPI
- **Language:** C++ (Arduino framework)

## License

This project is licensed under the MIT License - see [LICENSE](LICENSE) for details.

## Acknowledgments

- Inspired by [Insaniquarium](https://en.wikipedia.org/wiki/Insaniquarium) by PopCap Games
- Built for the ESP32 CYD community
- TFT_eSPI library by [Bodmer](https://github.com/Bodmer/TFT_eSPI)

---

*"You call that feeding? My grandma casts better."* - Ty Knotts
