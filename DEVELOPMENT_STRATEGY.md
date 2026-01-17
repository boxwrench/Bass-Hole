# Bass Hole - Development Strategy

## Game Identity

**Title:** Bass Hole  
**Tagline:** "Knot Your Average Fishing Game"

**Narrator/Guide:** Ty Knotts

- Species: Largemouth Bass (Micropterus salmoides)
- Personality: Terse, rude, impatient fishing veteran
- Role: Tutorial guide, between-level commentary, achievement roasts
- Voice: Think Ron Swanson meets a grumpy fishing guide
- Visual Style: Comic/cartoon with exaggerated expressions
- Sample dialogue:

- "You call that feeding? My grandma casts better."
- "Another guppy? We running a charity here?"
- "Congrats. You didn't screw up for once."
- "Clanker got your fish. You just gonna watch?"

---

## Core Gameplay (Insaniquarium Clone)

### Phase 1: Fish Management

- Tap to drop fish food pellets
- Fish eat → grow → drop coins
- Tap coins to collect
- Buy more fish with coins
- Buy upgrades (better food, auto-feeders, etc.)

### Phase 2: Threats - AI Fishermen

**Enemy Types:**

- **Clankers:** Robotic fishermen (low threat, slow)
- **Cogsuckers:** More aggressive AI bots (faster, steal multiple fish)
- Tap enemies to shoot/scare them away
- Lose fish = lose income
- Ty Knotts berates you when fish die

### Phase 3: Boss Battles (AI Meme Parodies)

#### **Boss 1: Cyberstuck**

- Parody of Cybertruck/Elon memes
- Glitchy angular robot that gets "stuck" in animations
- Attacks: Drops broken promises, overpromises catching all fish

#### **Boss 2: CatGPT**

- AI cat that "hallucinates" fake fish
- Attacks: Confuses your fish, generates phantom threats
- Occasionally gives helpful but wrong advice

#### **Boss 3: Ant-tropic**

- Parody of Claude/Anthropic
- Overly polite AI that "refuses" to steal fish for "safety reasons"
- Then does it anyway with long explanations
- Attacks: Constitutional decision-making delays, then sudden strikes

#### **Boss 4: Gaagle**

- Search engine turned evil
- Attacks: Ad-based distractions, sponsored fish kidnapping
- "Did you mean: bass whole?"

**Boss Design:** Comic/cartoon style, exaggerated features, animated personality

### Phase 4: Progression

- Unlock new NorCal fish species
- Each species has unique behaviors/coin values
- Prestige system: "Release and restock"

---

## NorCal Fish Species (Unlockable)

**Starter Fish:**

1. Rainbow Trout - Basic, reliable
2. Bluegill - Fast breeders, low value

**Mid-tier:**

1. Smallmouth Bass - Medium value
2. Channel Catfish - Slow, high value
3. Kokanee Salmon - Seasonal bonuses

**Premium:**

1. Largemouth Bass (Ty's species) - Highest base value, rare
2. Steelhead Trout - Speed + value hybrid
3. Striped Bass - Tank fish, scares small Clankers

**Special/Easter Eggs:**

1. Sacramento Pikeminnow (invasive) - Negative value, must remove
2. Sturgeon - Ultra rare, massive coin drops

---

## Visual Style

### **Fish & Environment:** 16-Color Dithered Realism

- Fish sprites based on real NorCal species photos
- Dithered to 16-color palette for retro aesthetic
- Tank backgrounds: Dithered Sierra Nevada underwater scenes
- Natural colors: greens, browns, blues, grays
- CRT scanline effects optional

### **Characters (Ty & Bosses):** Comic/Cartoon

- Ty Knotts: Exaggerated cartoon bass with attitude
- Speech bubbles for sass
- Expressive animations (eye rolls, smirks, facepalms)
- Boss characters: Over-the-top parody designs
- Vibrant colors, bold outlines

### **UI Elements:** Mixed

- Coins, food pellets: Simple pixel art
- Buttons: Clean, readable
- Text: Retro terminal font for Ty's dialogue

---

## Repository Structure

```text
bass-hole-cyd/
├── src/
│   ├── main.ino
│   ├── config.h           # Game constants, fish stats
│   ├── fish_ai.cpp        # Fish behavior per species
│   ├── ty_knotts.cpp      # Dialogue system
│   ├── enemies.cpp        # Clankers, Cogsuckers logic
│   ├── bosses.cpp         # Boss battle mechanics
│   ├── graphics.cpp       # Sprite rendering
│   └── touch.cpp          # Input handling
├── assets/
│   ├── sprites/
│   │   ├── ty_knotts/     # Cartoon: various expressions
│   │   ├── fish/          # Dithered: all species, realistic
│   │   ├── enemies/       # Clankers, Cogsuckers
│   │   ├── bosses/        # Cartoon: Cyberstuck, CatGPT, etc.
│   │   ├── ui/            # Buttons, coins, food
│   │   └── backgrounds/   # Dithered: underwater scenes
│   ├── dialogue/
│   │   ├── ty_lines.txt       # All Ty Knotts quotes
│   │   └── boss_dialogue.txt  # Boss intro/defeat lines
│   └── converted/         # C arrays
├── tools/
│   ├── img2code.py
│   └── dither_tool.py     # 16-color dithering script
├── docs/
│   ├── fish_database.md   # NorCal species reference
│   ├── ty_personality.md  # Character guide
│   ├── boss_designs.md    # Boss mechanics & parodies
│   └── palette.md         # 16-color palette reference
└── README.md
```

---

## Development Phases (No Pressure!)

### **Phase 1: Proof of Concept [COMPLETE]**

**Goals:**

- [x] Basic fish + food + coins loop working
- [x] Basic Ty Knotts tutorial foundation
- [x] Single fish type (Rainbow Trout)
- [x] Hardware verified on TZT CYD 2.4"
- [x] Save/load game state to SD card

### **Phase 2: Art Assets [IN PROGRESS]**

**Goals:**

- Generate initial 16-color palette
- Generate Ty Knotts character sheet (neutral, angry, smug expressions)
- Generate Rainbow Trout sprite (dithered)
- Generate Bluegill sprite (dithered)
- Generate Tank background (dithered Sierra Nevada underwater)
- Generate Clanker enemy design
- Generate UI elements (coins, food pellets, buttons)

**Process:**

1. Generate in Google AI Studio
2. Run through dithering tool → 16 colors (for fish/environment)
3. Convert to C arrays
4. Integrate into game

### **Phase 2.5: Sprite Display Pipeline Fix [PLANNED]**

**Problem Identified (2026-01-16):**
Current sprite rendering has inconsistent byte swapping causing color issues:
- `gfxDrawSprite()` uses pushImage WITHOUT setSwapBytes
- `gfxDrawSpriteTransparent()` uses manual byte swap with drawPixel
- `gfxRestoreBackground()` uses pushImage WITHOUT setSwapBytes
- platformio.ini missing `TFT_INVERSION_ON` despite code comments saying it's there

**Research Findings:**
THREE separate settings control sprite colors (often conflated):
1. **RGB Order** (TFT_RGB vs TFT_BGR) - channel position
2. **Byte Swap** (setSwapBytes) - endianness for pushImage
3. **Inversion** (TFT_INVERSION_ON) - panel-level inversion

**Goals:**

- [ ] Run sprite test plan from DIST_ESP32-CYD-Tester (requires hardware)
- [ ] Determine correct RGB_ORDER for our board
- [ ] Determine correct INVERSION setting
- [ ] Determine correct setSwapBytes setting
- [ ] Update platformio.ini with verified settings
- [ ] Refactor gfxInit() to set swap bytes ONCE
- [ ] Refactor gfxDrawSpriteTransparent() to use pushImage with transparency
- [ ] Remove manual byte swap code
- [ ] Verify all sprite paths use consistent settings
- [ ] Test background restoration with correct settings

**Code Changes Required:**

```cpp
// platformio.ini - add after determining correct values:
-DTFT_RGB_ORDER=TFT_BGR   ; or TFT_RGB based on test
-DTFT_INVERSION_ON        ; if needed based on test

// graphics.cpp gfxInit() - add once:
tft.setSwapBytes(true);   // based on test result

// graphics.cpp gfxDrawSpriteTransparent() - replace pixel loop:
// OLD: for loop with drawPixel and manual swap
// NEW: tft.pushImage(x, y, w, h, sprite, SPRITE_TRANSPARENT_COLOR);
```

**Blocked Until:** Hardware available for testing

**Reference:** See `DIST_ESP32-CYD-Tester/docs/SPRITE_TEST_PLAN.md`

### **Phase 3: Core Loop**

**Goals:**

- 3 fish species unlockable
- Shop system (buy fish, upgrades)
- Clankers appear and steal fish
- Tap-to-shoot defense mechanic
- Save/load game state (EEPROM)

### **Phase 4: Boss Battle System**

**Goals:**

- Boss encounter framework
- First boss: Cyberstuck
- Boss intro dialogue
- Unique attack patterns
- Victory rewards

### **Phase 5: Content Expansion**

**Goals:**

- All 4 bosses implemented
- Full fish species roster
- Cogsuckers (advanced enemies)
- More Ty dialogue variety
- Achievement system with roasts

### **Phase 6: Polish**

**Goals:**

- Particle effects (bubbles, coin sparkles, dithered explosions)
- Sound effects (ESP32 buzzer - 8-bit style)
- Screen shake on attacks
- High score leaderboard
- Ty's contextual commentary

### **Phase 7: Release**

**Goals:**

- Wokwi demo link
- Flash .bin file for CYD
- Video showcase (boss fights, Ty sass)
- GitHub release
- README with fishing puns
- Boss parody disclaimer

---

## Ty Knotts Dialogue Categories

### **Tutorial**

- "Tap the screen. Drop food. It's not rocket surgery."
- "Fish eat. Fish poop coins. You collect. Got it?"
- "See that Clanker? Tap it before it steals your fish. Do I gotta do everything?"

### **Achievements**

- "10 fish alive? Don't get cocky."
- "First Clanker kill. Took you long enough."
- "Beat Cyberstuck. Even a broken clock's right twice a day."

### **Failures**

- "And there goes your best fish. Nice job, genius."
- "Out of coins already? What are you, a tourist?"
- "Cogsuckers ate three fish while you were napping."

### **Idle/Waiting**

- "You gonna do something or just stare?"
- "Fish don't feed themselves, pal."

### **Milestones**

- "Level 5. You're less terrible than I thought."
- "Unlocked Largemouth Bass. That's MY family. Don't screw this up."

### **Boss Encounters**

- "Oh great, Cyberstuck's here. This oughta be good."
- "CatGPT thinks it's so smart. Show it what real intelligence looks like."
- "Ant-tropic's being 'helpful' again. Watch your back."
- "Gaagle wants to 'organize' your tank. Over my dead body."

---

## Boss Battle Mechanics

### **Cyberstuck**

- **Appearance:** Angular robot truck, glitchy animations
- **Attack Pattern:** 
  - Charges forward, gets stuck mid-screen
  - Drops "broken promises" that slow fish
  - Announces grand plans, delivers nothing
- **Defeat:** Tap rapidly while stuck to overheat
- **Ty's Commentary:** "Shocking. Another delay."

### **CatGPT**

- **Appearance:** Smug AI cat with holographic whiskers
- **Attack Pattern:**
  - "Hallucinates" phantom fish that distract you
  - Gives confidently wrong feeding advice
  - Occasional helpful tip (10% chance)
- **Defeat:** Feed real fish while ignoring phantoms
- **Ty's Commentary:** "At least it's not boring."

### **Ant-tropic**

- **Appearance:** Friendly-looking AI with bowtie, nervous smile
- **Attack Pattern:**
  - Pauses to "consider ethical implications"
  - Refuses to attack, then does anyway with apology
  - Long explanations that freeze gameplay briefly
- **Defeat:** Tap during explanations to interrupt
- **Ty's Commentary:** "Just pick a lane already."

### **Gaagle**

- **Appearance:** Search bar with tentacles, ad banners everywhere
- **Attack Pattern:**
  - Spawns sponsored fish that steal coins
  - Auto-complete suggestions block your view
  - "Did you mean [wrong thing]?" confuses controls
- **Defeat:** Close all ads, tap the real search button
- **Ty's Commentary:** "Monetizing fish now? Unbelievable."

---

## Color Palette (16-Color Dithered)

**For Fish & Environments:**

```text
Natural Palette:
- Deep Blue (#001f3f)
- Ocean Blue (#0074D9)
- Aqua (#7FDBFF)
- Teal (#39CCCC)
- Olive (#3D9970)
- Green (#2ECC40)
- Lime (#01FF70)
- Brown (#8B4513)
- Tan (#D2B48C)
- Gray (#AAAAAA)
- Silver (#DDDDDD)
- White (#FFFFFF)
- Black (#000000)
- Dark Gray (#333333)
- Orange (#FF851B) (for accent)
- Red (#FF4136) (for warnings)
```

**For Cartoon Characters (Ty & Bosses):**

- Full color, not dithered
- Bold outlines
- Vibrant, exaggerated

---

## Technical Considerations

### **Performance Targets**

- 30+ FPS for smooth fish movement
- Touch response < 50ms
- Max 15 fish on screen simultaneously
- Boss battles may drop to 20 FPS (acceptable)

### **Memory Management**

- Sprite sheets for fish animations
- Compressed dialogue strings
- EEPROM for save data:
  - High score
  - Unlocked species
  - Current tank state
  - Boss defeats

### **Wokwi Testing Strategy**

1. Test each fish species behavior individually
2. Verify touch calibration
3. Boss battle frame rate checks
4. Memory usage monitoring
5. Flash to hardware for final validation

---

## Asset Generation Workflow

### **Step 1: Google AI Prompts**

(Will be provided when ready)

Example for fish:

```text
"Photorealistic Rainbow Trout (Oncorhynchus mykiss), side view, 
swimming pose, white background, high detail for 16-color dithering"
```

Example for Ty Knotts:

```text
"Cartoon largemouth bass character, grumpy expression, 
arms crossed, wearing fishing vest, thick outlines, 
comic book style"
```

### **Step 2: Dithering (Fish Only)**

```bash
python tools/dither_tool.py assets/sprites/fish/trout.png --palette palette.txt
```

### **Step 3: Conversion**

```bash
python tools/img2code.py assets/sprites/fish/trout_dithered.png > assets/converted/trout.h
```

### **Step 4: Integration**

```cpp
#include "assets/converted/trout.h"
// Use in sprite rendering
```

---

## Next Steps (When Ready!)

1. **Create GitHub repo:** `bass-hole-cyd`
2. **Generate initial assets:**
   - 16-color palette file
   - Google AI prompts for initial assets
   - Dithering tool script
3. **First sprites:**
   - Ty Knotts character
   - Rainbow Trout sprite
   - Tank background
4. **Claude Code setup:**
   - Project initialization
   - Basic game loop
   - Touch input system
5. **Iterate at your own pace!**

---

## Development Resources

- **Wokwi:** Browser-based CYD emulator (wokwi.com)
- **TFT_eSPI Library:** ESP32 graphics optimization
- **XPT2046_Touchscreen:** Touch input handling
- **Google AI Studio:** Asset generation
- **Claude Code:** Agentic coding assistance

---

**Let's make Ty proud (or at least less disappointed)!**
