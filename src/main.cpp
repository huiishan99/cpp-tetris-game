#include "game.h"
#include <conio.h>
#include <windows.h>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

const int KEY_QUIT = 'q';

const char *GetBlockName(int blockId)
{
    switch (blockId)
    {
    case 1:
        return "L";
    case 2:
        return "J";
    case 3:
        return "I";
    case 4:
        return "O";
    case 5:
        return "S";
    case 6:
        return "T";
    case 7:
        return "Z";
    default:
        return "?";
    }
}

int ReadInput()
{
    if (!_kbhit())
    {
        return 0;
    }

    int key = _getch();
    if (key == 0 || key == 224)
    {
        int arrow = _getch();
        switch (arrow)
        {
        case 72:
            return 'w';
        case 80:
            return 's';
        case 75:
            return 'a';
        case 77:
            return 'd';
        default:
            return 0;
        }
    }
    return key;
}

void MoveCursorToTop()
{
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(output, {0, 0});
}

void HideCursor()
{
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(output, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(output, &cursorInfo);
}

void DrawGame(Game &game)
{
    char display[20][10];
    const int (&grid)[20][10] = game.GetGrid();
    for (int row = 0; row < 20; row++)
    {
        for (int column = 0; column < 10; column++)
        {
            display[row][column] = grid[row][column] == 0 ? ' ' : '#';
        }
    }

    for (Position item : game.GetCurrentBlockCells())
    {
        if (item.row >= 0 && item.row < 20 && item.column >= 0 && item.column < 10)
        {
            display[item.row][item.column] = '#';
        }
    }

    MoveCursorToTop();
    std::cout << "C++ Console Tetris\n";
    std::cout << "Score: " << game.score << "\n";
    std::cout << "Next: " << GetBlockName(game.GetNextBlockId()) << "\n";
    std::cout << "Controls: Arrow keys/WASD move, Up/W rotate, Space drop, Q quit\n\n";

    std::cout << "+--------------------+\n";
    for (int row = 0; row < 20; row++)
    {
        std::cout << "|";
        for (int column = 0; column < 10; column++)
        {
            std::cout << display[row][column] << display[row][column];
        }
        std::cout << "|\n";
    }
    std::cout << "+--------------------+\n";

    if (game.gameOver)
    {
        std::cout << "\nGAME OVER - press any key to restart, or Q to quit.     \n";
    }
    else
    {
        std::cout << "\n                                                \n";
    }
}

int main()
{
    SetConsoleTitleA("Tetris - By HuiShan");
    HideCursor();

    Game game = Game();
    auto lastDropTime = std::chrono::steady_clock::now();

    while (true)
    {
        int key = ReadInput();
        if (key == KEY_QUIT || key == 'Q')
        {
            break;
        }
        if (key != 0)
        {
            game.HandleInput(key);
        }

        auto now = std::chrono::steady_clock::now();
        if (now - lastDropTime >= std::chrono::milliseconds(350))
        {
            game.MoveBlockDown();
            lastDropTime = now;
        }

        DrawGame(game);
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    return 0;
}
