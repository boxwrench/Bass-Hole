---
phase: 2
plan: 3
wave: 3
depends_on: [2.2]
files_modified:
  - src/main.cpp
  - src/game_state.cpp
  - include/game_state.h
autonomous: false

must_haves:
  truths:
    - "User can spawn all 5 fish species for verification"
    - "Progression logic allows switching fish"
  artifacts:
    - "Updated Shop Logic in main.cpp"
  key_links:
    - "Addresses user concern about progression"
---

# Plan 2.3: Progression Logic & Verification

<objective>
Update the game logic to allow spawning/buying of all 5 fish species, enabling verification of the new sprites. Current logic only allows Rainbow Trout.

**Purpose:** Fix progression blocker and enable full testing.

**Output:** 
- Updated `main.cpp` shop logic.
- Ability to see all 5 fish on screen.
</objective>

<context>
Load for context:
- src/main.cpp
- src/game_state.cpp
</context>

<tasks>

<task type="auto">
  <name>Implement Unlock Logic</name>
  <files>src/game_state.cpp</files>
  <action>
1. Update `gameStateInit` to unlock ALL fish for Phase 2 testing (Debug Mode).
   - Set `game.fishUnlocked = 0x1F;` (All 5 bits).
  </action>
  <verify>
Code change applied.
  </verify>
  <done>
All fish unlocked.
  </done>
</task>

<task type="auto">
  <name>Update Shop Logic</name>
  <files>src/main.cpp</files>
  <action>
1. Modify the Buy Button logic in `handlePlayingInput`:
   - Instead of hardcoding `FISH_RAINBOW_TROUT`:
   - Cycle through unlocked species on each buy?
   - OR: Randomize species?
   - **Chosen Approach:** Cycle. `static uint8_t nextFishToBuy = 0;`
   - Buy -> Spawn `nextFishToBuy` -> Increment (wrapped).
2. Update Button Text to show current selection? (Optional, nice to have).
  </action>
  <verify>
Build succeeds.
  </verify>
  <done>
Can spawn different fish.
  </done>
</task>

<task type="checkpoint:human-verify">
  <name>Verify All Fish Sprites</name>
  <files>N/A</files>
  <action>
**Verification Steps:**
1. Flash firmware.
2. Tap "Buy" button 5 times.
3. Verify 5 DIFFERENT fish sprites appear.
4. Verify Food and Coin sprites appear.
5. Verify Background Tiling still looks good.
  </action>
  <verify>
All sprites verified on hardware.
  </verify>
  <done>
Phase 2 complete.
  </done>
</task>

</tasks>

<verification>
After all tasks, verify:
- [ ] User can spawn all 5 fish
- [ ] All 5 fish render correctly
</verification>
