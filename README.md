# C++ Tetris Game

A small Tetris game written in C++ with a native Windows GUI.

This version does **not** use Raylib. It only needs a C++ compiler, so there is
no graphics-library setup.

## Features

- Classic Tetris movement
- Score tracking
- Next-block preview
- Soft drop and instant drop
- Native Windows window rendering
- Small Windows beep sounds for rotate and row clear

## Controls

| Key | Action |
| --- | --- |
| Left Arrow or A | Move block left |
| Right Arrow or D | Move block right |
| Down Arrow or S | Soft drop |
| Up Arrow or W | Rotate block |
| Space | Drop block instantly |
| Any key after game over | Restart |
| Q | Quit |

## Requirements

- Windows
- A C++ compiler such as MinGW-w64 `g++`
- VS Code is optional

You do not need Raylib, `C:\raylib`, fonts, image assets, or audio files for
this Windows GUI version.

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

Run:

```powershell
.\main.exe
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
  block.cpp      Shared block behavior
  blocks.cpp     Individual Tetris block shapes
```

## Development Log

Every repository change should include a new entry in `DEVLOG.md`. Record what
changed, why it changed, the risk, verification steps, and any follow-up work.

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
