#include "game.h"
#include <windows.h>
#include <string>

const int WINDOW_WIDTH = 520;
const int WINDOW_HEIGHT = 680;
const int BOARD_LEFT = 28;
const int BOARD_TOP = 28;
const int CELL_SIZE = 28;
const int TIMER_ID = 1;
const int DROP_INTERVAL_MS = 350;

Game game;

COLORREF GetBlockColor(int blockId)
{
    switch (blockId)
    {
    case 1:
        return RGB(226, 116, 17);
    case 2:
        return RGB(13, 64, 216);
    case 3:
        return RGB(21, 204, 209);
    case 4:
        return RGB(237, 234, 4);
    case 5:
        return RGB(47, 230, 23);
    case 6:
        return RGB(166, 0, 247);
    case 7:
        return RGB(232, 18, 18);
    default:
        return RGB(26, 31, 40);
    }
}

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

void DrawTextLine(HDC hdc, int x, int y, const std::string &text, int size = 24)
{
    HFONT font = CreateFontA(size, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                             ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                             DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Consolas");
    HFONT oldFont = static_cast<HFONT>(SelectObject(hdc, font));
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(245, 247, 255));
    TextOutA(hdc, x, y, text.c_str(), static_cast<int>(text.size()));
    SelectObject(hdc, oldFont);
    DeleteObject(font);
}

void FillRectColor(HDC hdc, int left, int top, int right, int bottom, COLORREF color)
{
    HBRUSH brush = CreateSolidBrush(color);
    RECT rect = {left, top, right, bottom};
    FillRect(hdc, &rect, brush);
    DeleteObject(brush);
}

void DrawCell(HDC hdc, int row, int column, int blockId)
{
    int left = BOARD_LEFT + column * CELL_SIZE;
    int top = BOARD_TOP + row * CELL_SIZE;
    FillRectColor(hdc, left + 1, top + 1, left + CELL_SIZE - 1, top + CELL_SIZE - 1, GetBlockColor(blockId));
}

void DrawGame(HDC hdc)
{
    FillRectColor(hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, RGB(44, 44, 127));
    FillRectColor(hdc, BOARD_LEFT - 4, BOARD_TOP - 4,
                  BOARD_LEFT + CELL_SIZE * 10 + 4, BOARD_TOP + CELL_SIZE * 20 + 4,
                  RGB(18, 22, 32));

    int display[20][10];
    const int (&grid)[20][10] = game.GetGrid();
    for (int row = 0; row < 20; row++)
    {
        for (int column = 0; column < 10; column++)
        {
            display[row][column] = grid[row][column];
        }
    }

    for (Position item : game.GetCurrentBlockCells())
    {
        if (item.row >= 0 && item.row < 20 && item.column >= 0 && item.column < 10)
        {
            display[item.row][item.column] = game.GetCurrentBlockId();
        }
    }

    for (int row = 0; row < 20; row++)
    {
        for (int column = 0; column < 10; column++)
        {
            DrawCell(hdc, row, column, display[row][column]);
        }
    }

    int panelX = BOARD_LEFT + CELL_SIZE * 10 + 32;
    DrawTextLine(hdc, panelX, 34, "TETRIS", 36);
    DrawTextLine(hdc, panelX, 96, "Score", 24);
    DrawTextLine(hdc, panelX, 128, std::to_string(game.score), 32);
    DrawTextLine(hdc, panelX, 204, "Next", 24);
    DrawTextLine(hdc, panelX, 236, GetBlockName(game.GetNextBlockId()), 42);

    DrawTextLine(hdc, panelX, 340, "Keys", 22);
    DrawTextLine(hdc, panelX, 374, "Arrows/WASD", 18);
    DrawTextLine(hdc, panelX, 402, "Space: drop", 18);
    DrawTextLine(hdc, panelX, 430, "Q: quit", 18);

    if (game.gameOver)
    {
        DrawTextLine(hdc, panelX, 520, "GAME OVER", 28);
        DrawTextLine(hdc, panelX, 558, "Press any key", 18);
    }
}

void HandleGameKey(HWND hwnd, WPARAM key)
{
    if (key == 'Q')
    {
        PostQuitMessage(0);
        return;
    }

    int input = 0;
    switch (key)
    {
    case VK_LEFT:
        input = 'a';
        break;
    case VK_RIGHT:
        input = 'd';
        break;
    case VK_DOWN:
        input = 's';
        break;
    case VK_UP:
        input = 'w';
        break;
    case VK_SPACE:
        input = ' ';
        break;
    case 'A':
    case 'D':
    case 'S':
    case 'W':
        input = static_cast<int>(key);
        break;
    default:
        input = game.gameOver ? 'r' : 0;
        break;
    }

    if (input != 0)
    {
        game.HandleInput(input);
        InvalidateRect(hwnd, nullptr, FALSE);
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        SetTimer(hwnd, TIMER_ID, DROP_INTERVAL_MS, nullptr);
        return 0;
    case WM_TIMER:
        game.MoveBlockDown();
        InvalidateRect(hwnd, nullptr, FALSE);
        return 0;
    case WM_KEYDOWN:
        HandleGameKey(hwnd, wParam);
        return 0;
    case WM_PAINT:
    {
        PAINTSTRUCT paint;
        HDC hdc = BeginPaint(hwnd, &paint);
        HDC memoryDc = CreateCompatibleDC(hdc);
        HBITMAP bitmap = CreateCompatibleBitmap(hdc, WINDOW_WIDTH, WINDOW_HEIGHT);
        HBITMAP oldBitmap = static_cast<HBITMAP>(SelectObject(memoryDc, bitmap));

        DrawGame(memoryDc);
        BitBlt(hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, memoryDc, 0, 0, SRCCOPY);

        SelectObject(memoryDc, oldBitmap);
        DeleteObject(bitmap);
        DeleteDC(memoryDc);
        EndPaint(hwnd, &paint);
        return 0;
    }
    case WM_DESTROY:
        KillTimer(hwnd, TIMER_ID);
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int showCommand)
{
    const char className[] = "HuiShanTetrisWindow";

    WNDCLASSA windowClass = {};
    windowClass.lpfnWndProc = WindowProc;
    windowClass.hInstance = instance;
    windowClass.lpszClassName = className;
    windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    windowClass.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);

    RegisterClassA(&windowClass);

    RECT windowRect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

    HWND hwnd = CreateWindowExA(0, className, "Tetris - By HuiShan",
                                WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
                                CW_USEDEFAULT, CW_USEDEFAULT,
                                windowRect.right - windowRect.left,
                                windowRect.bottom - windowRect.top,
                                nullptr, nullptr, instance, nullptr);

    if (hwnd == nullptr)
    {
        return 0;
    }

    ShowWindow(hwnd, showCommand);
    UpdateWindow(hwnd);

    MSG message = {};
    while (GetMessage(&message, nullptr, 0, 0))
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    return static_cast<int>(message.wParam);
}
