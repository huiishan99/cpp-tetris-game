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

### 2026-05-16 - Add cross-platform CI

- Changed: Added a GitHub Actions workflow that configures CMake, builds the
  project, and runs `ctest` on Ubuntu, macOS, and Windows. Added build
  directory ignores and documented the CI workflow in the README.
- Why: Catch core logic regressions across common development environments and
  make future changes easier to trust.
- Risk: The workflow has not run on GitHub from this environment; local CMake
  and core test verification were used instead.
- Verified: Local `cmake` was not available in this environment. Built with
  `c++ -std=c++17 -Wall -Wextra -Isrc tests/core_tests.cpp src/block.cpp
  src/blocks.cpp src/game.cpp src/grid.cpp src/high_score.cpp src/position.cpp
  src/sound.cpp -o /private/tmp/tetris_core_tests` and ran
  `/private/tmp/tetris_core_tests`; all core tests passed.
- Follow-ups: Check the first GitHub Actions run after pushing and adjust the
  workflow if a hosted runner exposes any generator-specific issue.

### 2026-05-16 - Add three-block next queue

- Changed: Added a three-block upcoming queue behind the existing next-block
  API, advanced the queue through hold/spawn flow, rendered the Next panel as
  three compact previews, and added core tests for queue size and promotion.
- Why: Give players more planning information and make Hold decisions more
  strategic without changing the basic rules.
- Risk: Queue advancement touches block spawning and Hold behavior; Win32 GUI
  build was not available in this environment.
- Verified: Built with `c++ -std=c++17 -Wall -Wextra -Isrc tests/core_tests.cpp
  src/block.cpp src/blocks.cpp src/game.cpp src/grid.cpp src/high_score.cpp
  src/position.cpp src/sound.cpp -o /private/tmp/tetris_core_tests` and ran
  `/private/tmp/tetris_core_tests`; all core tests passed.
- Follow-ups: Recheck Next panel spacing on Windows and consider a five-block
  queue if the sidebar is redesigned later.

### 2026-05-16 - Add richer action sounds

- Changed: Added separate lightweight Win32 beep cues for movement, soft drop,
  hard drop, hold, pause, and game over, while keeping rotate and line-clear
  cues. Updated the feature list to describe the broader sound feedback.
- Why: Make controls feel more responsive and give players clearer feedback for
  successful actions and terminal states.
- Risk: Win32 `Beep` is intentionally simple and blocking for very short
  durations; exact feel should be checked on Windows speakers.
- Verified: Built with `c++ -std=c++17 -Wall -Wextra -Isrc tests/core_tests.cpp
  src/block.cpp src/blocks.cpp src/game.cpp src/grid.cpp src/high_score.cpp
  src/position.cpp src/sound.cpp -o /private/tmp/tetris_core_tests` and ran
  `/private/tmp/tetris_core_tests`; all core tests passed.
- Follow-ups: Replace beep cues with non-blocking audio files if the project
  later gains an asset pipeline.

### 2026-05-16 - Delay row collapse during line clear

- Changed: Added a `lineClearPending` state, delayed row collapse through
  `Game::FinishLineClear`, paused the Win32 drop timer while rows flash, and
  expanded core tests to verify completed rows stay visible until the animation
  finishes.
- Why: Make line clears feel more intentional and readable, with the board
  showing the completed rows before they collapse.
- Risk: Input is ignored during the short clear delay, and the Win32 GUI build
  was not available in this environment.
- Verified: Built with `c++ -std=c++17 -Wall -Wextra -Isrc tests/core_tests.cpp
  src/block.cpp src/blocks.cpp src/game.cpp src/grid.cpp src/high_score.cpp
  src/position.cpp src/sound.cpp -o /private/tmp/tetris_core_tests` and ran
  `/private/tmp/tetris_core_tests`; all core tests passed.
- Follow-ups: Tune the clear delay duration on Windows if it feels too snappy
  or too slow during real play.

### 2026-05-16 - Add line clear flash

- Changed: Added `Grid::GetFullRows`, tracked the last cleared row indices and
  clear event id in `Game`, added a short Win32 clear-flash timer/render pass,
  and expanded core tests for cleared-row/event tracking.
- Why: Make cleared lines visibly pop on the board instead of only updating the
  side-panel text.
- Risk: The flash is a lightweight post-clear overlay, not a delayed clear
  animation; Win32 GUI build was not available in this environment.
- Verified: Built with `c++ -std=c++17 -Wall -Wextra -Isrc tests/core_tests.cpp
  src/block.cpp src/blocks.cpp src/game.cpp src/grid.cpp src/high_score.cpp
  src/position.cpp src/sound.cpp -o /private/tmp/tetris_core_tests` and ran
  `/private/tmp/tetris_core_tests`; all core tests passed.
- Follow-ups: If desired, convert this to a true delayed clear animation that
  holds the completed rows for a few frames before collapsing the board.

### 2026-05-16 - Add game state overlays

- Changed: Added a start-gated game state, `Game::Start`, `Game::IsStarted`,
  start/pause/game-over overlay panels, READY status display, and core tests
  that verify automatic drop is blocked before start.
- Why: Make the game feel like a complete playable experience instead of
  dropping immediately when the window opens.
- Risk: The first gameplay key now starts the game without also moving the
  block; Win32 GUI build was not available in this environment.
- Verified: Built with `c++ -std=c++17 -Wall -Wextra -Isrc tests/core_tests.cpp
  src/block.cpp src/blocks.cpp src/game.cpp src/grid.cpp src/high_score.cpp
  src/position.cpp src/sound.cpp -o /private/tmp/tetris_core_tests` and ran
  `/private/tmp/tetris_core_tests`; all core tests passed.
- Follow-ups: Tune overlay spacing and text metrics after another Windows
  visual pass.

### 2026-05-16 - Persist best score

- Changed: Added `src/high_score.h`, `src/high_score.cpp`, high-score loading
  on startup, saving on shutdown, `.gitignore` entries for generated score
  files, CMake source wiring, and core tests for missing/saved/invalid score
  files.
- Why: Keep the Best score across program runs instead of limiting it to one
  session.
- Risk: The score file is a simple text file in the working directory, so
  changing launch directories changes which high-score file is used. Win32 GUI
  build was not available in this environment.
- Verified: Built with `c++ -std=c++17 -Wall -Wextra -Isrc tests/core_tests.cpp
  src/block.cpp src/blocks.cpp src/game.cpp src/grid.cpp src/high_score.cpp
  src/position.cpp src/sound.cpp -o /private/tmp/tetris_core_tests` and ran
  `/private/tmp/tetris_core_tests`; all core tests passed.
- Follow-ups: Save beside the executable or in a platform user-data folder if
  launch-directory independence becomes important.

### 2026-05-16 - Add session best score

- Changed: Added session high-score tracking, a public `Restart()` path that
  preserves the best score, Best score display in the side panel, and core tests
  for restart/high-score behavior and blocked soft-drop scoring.
- Why: Give players a simple score target across restarts without adding file
  persistence yet.
- Risk: High score is in-memory only and resets when the program closes; soft
  drop scoring now only counts successful downward movement. Win32 GUI build was
  not available in this environment.
- Verified: Built with `c++ -std=c++17 -Wall -Wextra -Isrc tests/core_tests.cpp
  src/block.cpp src/blocks.cpp src/game.cpp src/grid.cpp src/position.cpp
  src/sound.cpp -o /private/tmp/tetris_core_tests` and ran
  `/private/tmp/tetris_core_tests`; all core tests passed.
- Follow-ups: Add optional file-backed high-score persistence later.

### 2026-05-16 - Add clear feedback and combo

- Changed: Added last-clear line/score tracking, combo state, a test constructor
  that accepts a starting grid, status-panel clear feedback, and core tests for
  line-clear feedback reset behavior.
- Why: Make line clears feel more responsive and easier to read than score
  changes alone.
- Risk: Locking now resets combo/feedback on non-clearing pieces; Win32 GUI
  build was not available in this environment.
- Verified: Built with `c++ -std=c++17 -Wall -Wextra -Isrc tests/core_tests.cpp
  src/block.cpp src/blocks.cpp src/game.cpp src/grid.cpp src/position.cpp
  src/sound.cpp -o /private/tmp/tetris_core_tests` and ran
  `/private/tmp/tetris_core_tests`; all core tests passed.
- Follow-ups: Add a short visual flash on the board itself once the Win32 build
  can be checked interactively.

### 2026-05-16 - Add rotation kick assist

- Changed: Added simple wall/spawn kick offsets for rotation, a deterministic
  game constructor for core tests, and a test that verifies an I block can
  rotate near the spawn top without leaving the board.
- Why: Make rotation feel more forgiving near walls and the top of the playfield
  instead of immediately rejecting close rotations.
- Risk: This is a simple kick table, not full SRS; some advanced rotation cases
  may still differ from modern guideline Tetris. Win32 GUI build was not
  available in this environment.
- Verified: Built with `c++ -std=c++17 -Wall -Wextra -Isrc tests/core_tests.cpp
  src/block.cpp src/blocks.cpp src/game.cpp src/grid.cpp src/position.cpp
  src/sound.cpp -o /private/tmp/tetris_core_tests` and ran
  `/private/tmp/tetris_core_tests`; all core tests passed.
- Follow-ups: Replace the simple kick table with piece-specific SRS data if the
  project aims for guideline-accurate Tetris behavior.

### 2026-05-16 - Add hold block

- Changed: Added hold-block state, `C`/`Shift` input handling, Hold/Next preview
  panels, a safe default block id, and a core test that verifies hold can only
  be used once before a block lands.
- Why: Bring the controls closer to modern Tetris and give players more
  strategic choice during placement.
- Risk: Current/next/held block swapping touches spawn flow; Win32 GUI build was
  not available in this environment.
- Verified: Built with `c++ -std=c++17 -Wall -Wextra -Isrc tests/core_tests.cpp
  src/block.cpp src/blocks.cpp src/game.cpp src/grid.cpp src/position.cpp
  src/sound.cpp -o /private/tmp/tetris_core_tests` and ran
  `/private/tmp/tetris_core_tests`; all core tests passed.
- Follow-ups: Tune Hold panel spacing on Windows and consider a subtle visual
  lock indicator after Hold is used.

### 2026-05-16 - Polish Win32 game UI

- Changed: Reworked the Win32 drawing style with a wider window, warmer dark
  background, rounded board/sidebar panels, highlighted block cells, dotted
  ghost cells, status panel, and a graphical next-block preview.
- Why: Make the game feel less like a debug sample and more like a finished
  playable window.
- Risk: Main GUI rendering changed substantially and could need spacing/color
  tuning on Windows; Win32 GUI build was not available in this environment.
- Verified: Built with `c++ -std=c++17 -Wall -Wextra -Isrc tests/core_tests.cpp
  src/block.cpp src/blocks.cpp src/game.cpp src/grid.cpp src/position.cpp
  src/sound.cpp -o /private/tmp/tetris_core_tests` and ran
  `/private/tmp/tetris_core_tests`; all core tests passed.
- Follow-ups: Open on Windows to tune exact text metrics and color contrast.

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
