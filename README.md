# C++ Console Tetris Game

A small Tetris game written in C++ for the Windows console.

This version does **not** use Raylib. It only needs a C++ compiler, so there is
no graphics-library setup.

## Features

- Classic Tetris movement
- Score tracking
- Next-block preview
- Soft drop and instant drop
- Simple console rendering
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
this console version.

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
g++ -std=c++17 -Wall -Wextra src\*.cpp -o main.exe
```

Or use the included batch file:

```powershell
.\build.bat
```

Run:

```powershell
.\main.exe
```

## Build with VS Code

1. Open `main.code-workspace` in VS Code.
2. Press `Ctrl + Shift + B`.
3. Choose `build debug` or `build release`.
4. Run `.\main.exe`, or use the `Run Console Tetris` launch configuration.

## Project Structure

```text
src/
  main.cpp       Console input, drawing, and game loop
  game.cpp       Game state, scoring, movement, rotation, beep sounds
  grid.cpp       Tetris board and row clearing
  block.cpp      Shared block behavior
  blocks.cpp     Individual Tetris block shapes
```

## Troubleshooting

### Space key does not drop the block

Rebuild the project first. Source code changes do not update `main.exe`
automatically.

```powershell
g++ -std=c++17 -Wall -Wextra src\*.cpp -o main.exe
.\main.exe
```

### `g++` is not recognized

Your compiler is not installed or not on `PATH`. Install MinGW-w64 or another
C++ compiler, then reopen PowerShell and try:

```powershell
g++ --version
```

### The game flickers

This is a simple console renderer, so some flicker is normal in older terminals.
Windows Terminal usually looks better than the classic console window.
