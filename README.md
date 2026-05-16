# C++ Tetris Game

A small Tetris game written in C++ with a native Windows GUI.

This version does **not** use Raylib. It only needs a C++ compiler, so there is
no graphics-library setup.

## Features

- Classic Tetris movement
- Score tracking
- Persistent best score tracking
- Start, pause, and game-over overlay panels
- Line count and level progression
- Line-clear feedback and combo counter
- Line-clear flash animation
- Hold-block swap
- Next-block preview
- Ghost landing preview
- Rotation kick assist near walls and the spawn top
- Pause and resume
- Soft drop and instant drop
- Polished dark UI with block preview and status panels
- Native Windows window rendering
- Small Windows beep sounds for rotate and row clear

## Controls

| Key | Action |
| --- | --- |
| Any non-Q key on start screen | Start |
| Left Arrow or A | Move block left |
| Right Arrow or D | Move block right |
| Down Arrow or S | Soft drop |
| Up Arrow or W | Rotate block |
| Space | Drop block instantly |
| C or Shift | Hold or swap block |
| P | Pause or resume |
| Any non-Q key after game over | Restart |
| Q | Quit |

## Scoring and Pace

- Soft drop: 1 point per successful row
- Hard drop: 2 points per row
- Line clear scoring: 100 / 300 / 500 / 800 points
- Consecutive line-clearing locks increase the combo counter
- Level increases every 10 cleared lines
- Automatic drop speed increases with level, capped at a minimum interval
- Best score is saved to `tetris_highscore.txt` in the working directory

## Requirements

### Full Windows GUI

- Windows
- A C++ compiler such as MinGW-w64 `g++`
- CMake is optional, but recommended for Visual Studio, Ninja, and other IDEs
- VS Code is optional

You do not need Raylib, `C:\raylib`, fonts, image assets, or audio files for
this Windows GUI version.

### Portable Core Tests

- macOS, Linux, or Windows
- Any C++17 compiler
- CMake is optional

## Check Whether `g++` Is Installed

Open PowerShell and run:

```powershell
g++ --version
```

If you see version information, you can build the game.

If PowerShell says `g++` is not recognized, install a C++ compiler first. A
common option is MinGW-w64. After installation, make sure the compiler's `bin`
folder is added to your Windows `PATH`.

## Build and Run

Open PowerShell in the project folder:

```powershell
cd C:\dev\cpp-tetris-game
```

Build:

```powershell
g++ -std=c++17 -Wall -Wextra src\*.cpp -mwindows -lgdi32 -luser32 -o main.exe
```

Or use the included batch file:

```powershell
.\build.bat
```

`build.bat` uses `g++` when it is available. If you run it from a Visual Studio
Developer Command Prompt, it can also build with `cl`.

Or use CMake with MinGW:

```powershell
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
.\build\main.exe
```

Or use CMake with Visual Studio:

```powershell
cmake -S . -B build -G "Visual Studio 17 2022"
cmake --build build --config Release
.\build\Release\main.exe
```

Run:

```powershell
.\main.exe
```

## Run Core Tests

The game rules can be compiled without the Win32 GUI. This is useful on macOS
or Linux, and it helps catch logic regressions before touching rendering code.

With a local C++ compiler:

```bash
c++ -std=c++17 -Wall -Wextra -Isrc tests/core_tests.cpp src/block.cpp src/blocks.cpp src/game.cpp src/grid.cpp src/high_score.cpp src/position.cpp src/sound.cpp -o /tmp/tetris_core_tests
/tmp/tetris_core_tests
```

With CMake:

```bash
cmake -S . -B build
cmake --build build --target tetris_core_tests
ctest --test-dir build
```

## Build with VS Code

1. Open `main.code-workspace` in VS Code.
2. Press `Ctrl + Shift + B`.
3. Choose `build debug` or `build release`.
4. Run `.\main.exe`, or use the `Run Windows Tetris` launch configuration.

## Project Structure

```text
src/
  main.cpp       Windows GUI, drawing, input, and game loop
  game.cpp       Game state, scoring, movement, rotation, beep sounds
  grid.cpp       Tetris board and row clearing
  high_score.cpp Best-score file loading and saving
  block.cpp      Shared block behavior
  blocks.cpp     Individual Tetris block shapes
  sound.cpp      Small sound adapter; no-op outside Windows
tests/
  core_tests.cpp Portable game-logic checks
```

## Development Log

Every repository change should include a new entry in `DEVLOG.md`. Record what
changed, why it changed, the risk, verification steps, and any follow-up work.

## Portability Note

The current game window is built directly on the Win32 API, so the full game
currently targets Windows. The rules layer now builds without the GUI, so future
frontends can reuse the same game logic.

## Troubleshooting

### Space key does not drop the block

Rebuild the project first. Source code changes do not update `main.exe`
automatically.

```powershell
g++ -std=c++17 -Wall -Wextra src\*.cpp -mwindows -lgdi32 -luser32 -o main.exe
.\main.exe
```

### `g++` is not recognized

Your compiler is not installed or not on `PATH`. Install MinGW-w64 or another
C++ compiler, then reopen PowerShell and try:

```powershell
g++ --version
```

### The window does not open

Make sure the build succeeded and `main.exe` was updated. If the compiler is not
installed, `build.bat` will fail before creating the GUI executable.
