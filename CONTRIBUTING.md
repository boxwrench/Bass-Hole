# Contributing to Bass Hole

Thanks for your interest in contributing! This project is meant to be a learning resource for ESP32 CYD game development, so contributions that improve code clarity, documentation, or add educational value are especially welcome.

## Ways to Contribute

### Code Contributions
- **Bug fixes** - Found something broken? Fix it!
- **New fish species** - See [docs/ADDING_FISH.md](docs/ADDING_FISH.md)
- **New features** - Enemies, bosses, effects, sounds
- **Optimizations** - Better performance, lower memory usage
- **Code quality** - Better comments, cleaner architecture

### Non-Code Contributions
- **Documentation** - Tutorials, explanations, diagrams
- **Art assets** - Fish sprites, UI elements, backgrounds
- **Ty Knotts dialogue** - Write grumpy fishing guide lines
- **Testing** - Try it on different CYD variants, report issues
- **Translations** - Localize strings for other languages

## Getting Started

1. **Fork the repository** on GitHub
2. **Clone your fork** locally
3. **Create a branch** for your changes: `git checkout -b feature/my-feature`
4. **Make your changes**
5. **Test on hardware** if possible (or Wokwi simulator)
6. **Commit** with clear messages
7. **Push** to your fork
8. **Open a Pull Request**

## Code Style

### General Guidelines

- **Keep it readable** - This is an educational project. Clear code > clever code.
- **Comment the "why"** - Code shows what, comments explain why.
- **Match existing style** - Look at surrounding code for patterns.
- **Small functions** - Each function should do one thing well.

### Naming Conventions

```cpp
// Functions: camelCase, verb prefix
void fishUpdate(unsigned long deltaTime);
bool coinCollect(int16_t x, int16_t y);

// Variables: camelCase
float targetX, targetY;
unsigned long lastFrameTime;

// Constants: SCREAMING_SNAKE_CASE
#define MAX_FISH 10
#define SCREEN_WIDTH 240

// Structs/Enums: PascalCase
struct Fish { ... };
enum GameState { STATE_PLAYING, STATE_PAUSED };

// Global instances: camelCase
GameData game;
Fish fishPool[MAX_FISH];
```

### File Organization

- **Headers (.h)** - Declarations, structs, constants
- **Implementation (.cpp)** - Function bodies, static helpers
- **One system per file pair** - fish.h/cpp, coins.h/cpp, etc.

### Comments

```cpp
// Good: Explains WHY
// Fish swim toward food when hungry (below 50%) to simulate natural behavior
if (fish->hunger < 50 && dist < 80) {
    fish->targetX = food->x;
}

// Bad: Just restates the code
// Check if hunger is less than 50
if (fish->hunger < 50) {
```

## Commit Messages

Use clear, descriptive commit messages:

```
Good:
- Add bluegill fish species with fast breeding behavior
- Fix touch calibration for 2.8" CYD variant
- Improve coin collection hit detection radius

Bad:
- Fixed stuff
- Update fish.cpp
- WIP
```

Format:
```
<type>: <short description>

<optional longer description>
```

Types: `add`, `fix`, `update`, `remove`, `refactor`, `docs`

## Pull Request Process

1. **Update documentation** if you changed behavior
2. **Test your changes** on hardware or Wokwi
3. **Describe what and why** in the PR description
4. **Link related issues** if applicable
5. **Be responsive** to review feedback

### PR Description Template

```markdown
## What does this PR do?
Brief description of changes

## Why?
Motivation for the change

## How to test
Steps to verify it works

## Screenshots/Video (if applicable)
Show visual changes

## Checklist
- [ ] Tested on hardware
- [ ] Updated documentation
- [ ] No compiler warnings
```

## Adding New Features

### Before Starting
- **Check existing issues** - Someone may already be working on it
- **Open an issue first** for large features - Let's discuss the approach
- **Start small** - Get feedback early

### New Fish Species
See the detailed tutorial: [docs/ADDING_FISH.md](docs/ADDING_FISH.md)

### New Enemies/Bosses
1. Add struct definition in `enemies.h`
2. Implement behavior in `enemies.cpp`
3. Add rendering in `graphics.cpp`
4. Update game loop to spawn/update enemies
5. **Document the enemy** in `DEVELOPMENT_STRATEGY.md`

### New Game States
1. Add state to `GameState` enum in `config.h`
2. Handle state in `gameStateUpdate()` in `game_state.cpp`
3. Add input handling in `handleInput()` in `BassHole.ino`
4. Add rendering in `render()` in `BassHole.ino`

## Hardware Testing

### Ideal
Test on actual CYD hardware before submitting.

### Alternative
Use [Wokwi ESP32 Simulator](https://wokwi.com/) with CYD configuration.

### Report Your Hardware
If testing on a CYD variant not listed in [docs/HARDWARE.md](docs/HARDWARE.md), please document it!

## Questions?

- **Open an issue** for bugs or feature discussions
- **Start a discussion** for general questions
- **Check existing issues** first - it may already be answered

## Code of Conduct

Be respectful and constructive. We're all here to learn and build something fun.

---

*"First contribution? Don't screw it up."* - Ty Knotts (he's kidding, you'll do great)
