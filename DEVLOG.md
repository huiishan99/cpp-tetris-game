# Devlog

This file records project changes, the reason for each change, verification
notes, and follow-up work. Keep it current so future bugs can be traced back to
specific edits.

## Change Policy

- Add one entry for every code, asset, build, or documentation change.
- Keep newest entries at the top.
- Include the files touched, why the change was made, what risk it carries, and
  how it was verified.
- If a change cannot be built or tested locally, say that explicitly.

## Entry Template

```md
### YYYY-MM-DD - Short summary

- Changed: ...
- Why: ...
- Risk: ...
- Verified: ...
- Follow-ups: ...
```

## Entries

### 2026-05-16 - Add line stats and level speed

- Changed: Added total cleared lines, level calculation, dynamic drop interval,
  side-panel stats for lines/level, and core tests for level/speed formulas.
- Why: Make the game pace evolve over time instead of staying at one fixed
  speed.
- Risk: Timer interval now changes after each tick; Win32 GUI build was not
  available in this environment.
- Verified: Built with `c++ -std=c++17 -Wall -Wextra -Isrc tests/core_tests.cpp
  src/block.cpp src/blocks.cpp src/game.cpp src/grid.cpp src/position.cpp
  src/sound.cpp -o /private/tmp/tetris_core_tests` and ran
  `/private/tmp/tetris_core_tests`; all core tests passed.
- Follow-ups: Tune the speed curve after testing the Windows build by hand.

### 2026-05-16 - Add ghost landing preview

- Changed: Added `Game::GetGhostBlockCells`, const-safe block/grid helpers,
  ghost-cell outline rendering, and a core test that verifies ghost preview does
  not move the current block.
- Why: Make placement easier to read and improve the moment-to-moment feel of
  the game.
- Risk: Rendering order changed so the board is drawn before ghost/current
  pieces; Win32 GUI build was not available in this environment.
- Verified: Built with `c++ -std=c++17 -Wall -Wextra -Isrc tests/core_tests.cpp
  src/block.cpp src/blocks.cpp src/game.cpp src/grid.cpp src/position.cpp
  src/sound.cpp -o /private/tmp/tetris_core_tests` and ran
  `/private/tmp/tetris_core_tests`; all core tests passed.
- Follow-ups: Tune ghost styling on Windows and consider adding a setting to
  hide/show ghost preview later.

### 2026-05-16 - Add pause and resume

- Changed: Added paused game state, `P` input handling, paused UI text, accessors
  for game state, and a core test for paused automatic drop behavior.
- Why: Improve basic playability and start moving direct state access behind
  small query methods.
- Risk: Input flow changed around game-over and paused states; timer still fires
  but gameplay movement is ignored while paused.
- Verified: Built with `c++ -std=c++17 -Wall -Wextra -Isrc tests/core_tests.cpp
  src/block.cpp src/blocks.cpp src/game.cpp src/grid.cpp src/position.cpp
  src/sound.cpp -o /private/tmp/tetris_core_tests` and ran
  `/private/tmp/tetris_core_tests`; all core tests passed.
- Follow-ups: Add a visual overlay for pause/game-over states and consider
  stopping the timer while paused if future frontends need it.

### 2026-05-16 - Make core logic portable

- Changed: Added `src/sound.h`, `src/sound.cpp`, moved Windows `Beep` calls out
  of `Game`, changed CMake to build a `tetris_core` library, and added
  `tests/core_tests.cpp`.
- Why: Let the game rules compile outside the Win32 GUI so future gameplay work
  can be tested on more environments.
- Risk: Windows sound behavior now goes through a wrapper; CMake target layout
  changed.
- Verified: Built with `c++ -std=c++17 -Wall -Wextra -Isrc tests/core_tests.cpp
  src/block.cpp src/blocks.cpp src/game.cpp src/grid.cpp src/position.cpp
  src/sound.cpp -o /private/tmp/tetris_core_tests` and ran
  `/private/tmp/tetris_core_tests`; all core tests passed.
- Follow-ups: Add more tests around locking, game-over, hard drop scoring, and
  future gameplay features.

### 2026-05-16 - Add CMake Windows build

- Changed: Added `CMakeLists.txt` and documented MinGW/Visual Studio CMake
  build commands in `README.md`.
- Why: Support more Windows compiler and IDE setups with one build definition.
- Risk: CMake configuration is new and currently limited to Win32 builds.
- Verified: Tried `cmake -S . -B /private/tmp/cpp-tetris-cmake-check`;
  `cmake` is not installed in this environment. Win32 GUI build not run.
- Follow-ups: Split game logic from Win32 rendering so the core can build and
  be tested on non-Windows platforms.

### 2026-05-16 - Add maintenance devlog

- Changed: Created `DEVLOG.md` and linked it from `README.md`.
- Why: Establish a traceable change history before future fixes and refactors.
- Risk: Documentation-only change; no runtime impact.
- Verified: Not run; documentation-only change.
- Follow-ups: Add a new devlog entry for every future repository change.
