#include "game.h"
#include <windows.h>
#include <string>
#include <vector>

const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 700;
const int BOARD_LEFT = 32;
const int BOARD_TOP = 64;
const int CELL_SIZE = 28;
const int TIMER_ID = 1;

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

COLORREF AdjustColor(COLORREF color, int amount)
{
    int red = GetRValue(color) + amount;
    int green = GetGValue(color) + amount;
    int blue = GetBValue(color) + amount;

    if (red < 0)
    {
        red = 0;
    }
    if (green < 0)
    {
        green = 0;
    }
    if (blue < 0)
    {
        blue = 0;
    }
    if (red > 255)
    {
        red = 255;
    }
    if (green > 255)
    {
        green = 255;
    }
    if (blue > 255)
    {
        blue = 255;
    }
    return RGB(red, green, blue);
}

void DrawTextLine(HDC hdc, int x, int y, const std::string &text, int size = 24,
                  COLORREF color = RGB(238, 238, 228), int weight = FW_NORMAL)
{
    HFONT font = CreateFontA(size, 0, 0, 0, weight, FALSE, FALSE, FALSE,
                             ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                             DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Segoe UI");
    HFONT oldFont = static_cast<HFONT>(SelectObject(hdc, font));
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, color);
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

void FillVerticalGradient(HDC hdc, int left, int top, int right, int bottom,
                          COLORREF topColor, COLORREF bottomColor)
{
    int height = bottom - top;
    for (int index = 0; index < height; index++)
    {
        int red = GetRValue(topColor) + (GetRValue(bottomColor) - GetRValue(topColor)) * index / height;
        int green = GetGValue(topColor) + (GetGValue(bottomColor) - GetGValue(topColor)) * index / height;
        int blue = GetBValue(topColor) + (GetBValue(bottomColor) - GetBValue(topColor)) * index / height;
        FillRectColor(hdc, left, top + index, right, top + index + 1, RGB(red, green, blue));
    }
}

void FillRoundRectColor(HDC hdc, int left, int top, int right, int bottom, int radius,
                        COLORREF fillColor, COLORREF borderColor)
{
    HBRUSH brush = CreateSolidBrush(fillColor);
    HPEN pen = CreatePen(PS_SOLID, 1, borderColor);
    HGDIOBJ oldBrush = SelectObject(hdc, brush);
    HGDIOBJ oldPen = SelectObject(hdc, pen);

    RoundRect(hdc, left, top, right, bottom, radius, radius);

    SelectObject(hdc, oldPen);
    SelectObject(hdc, oldBrush);
    DeleteObject(pen);
    DeleteObject(brush);
}

void DrawCellAt(HDC hdc, int left, int top, int size, int blockId)
{
    if (blockId == 0)
    {
        FillRoundRectColor(hdc, left + 1, top + 1, left + size - 1, top + size - 1, 5,
                           RGB(17, 20, 20), RGB(27, 33, 31));
        return;
    }

    COLORREF color = GetBlockColor(blockId);
    FillRoundRectColor(hdc, left + 2, top + 2, left + size - 2, top + size - 2, 7,
                       color, AdjustColor(color, -55));
    FillRectColor(hdc, left + 5, top + 5, left + size - 5, top + 9, AdjustColor(color, 38));
    FillRectColor(hdc, left + 5, top + size - 9, left + size - 5, top + size - 5, AdjustColor(color, -45));
}

void DrawCell(HDC hdc, int row, int column, int blockId)
{
    int left = BOARD_LEFT + column * CELL_SIZE;
    int top = BOARD_TOP + row * CELL_SIZE;
    DrawCellAt(hdc, left, top, CELL_SIZE, blockId);
}

void DrawGhostCell(HDC hdc, int row, int column, int blockId)
{
    int left = BOARD_LEFT + column * CELL_SIZE;
    int top = BOARD_TOP + row * CELL_SIZE;
    COLORREF color = GetBlockColor(blockId);
    HPEN pen = CreatePen(PS_DOT, 1, AdjustColor(color, 30));
    HGDIOBJ oldPen = SelectObject(hdc, pen);
    HGDIOBJ oldBrush = SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));

    RoundRect(hdc, left + 6, top + 6, left + CELL_SIZE - 6, top + CELL_SIZE - 6, 6, 6);

    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(pen);
}

void DrawMetricPanel(HDC hdc, int x, int y, int width, int height,
                     const std::string &label, const std::string &value)
{
    FillRoundRectColor(hdc, x, y, x + width, y + height, 8,
                       RGB(31, 35, 36), RGB(61, 70, 62));
    DrawTextLine(hdc, x + 14, y + 10, label, 16, RGB(170, 178, 158), FW_NORMAL);
    DrawTextLine(hdc, x + 14, y + 32, value, 28, RGB(248, 244, 225), FW_BOLD);
}

void DrawNextPreview(HDC hdc, int x, int y, int width, int height)
{
    FillRoundRectColor(hdc, x, y, x + width, y + height, 8,
                       RGB(31, 35, 36), RGB(61, 70, 62));
    DrawTextLine(hdc, x + 14, y + 10, "Next", 16, RGB(170, 178, 158), FW_NORMAL);

    std::vector<Position> cells = game.GetNextBlockCells();
    int minRow = cells[0].row;
    int maxRow = cells[0].row;
    int minColumn = cells[0].column;
    int maxColumn = cells[0].column;
    for (Position cell : cells)
    {
        if (cell.row < minRow)
        {
            minRow = cell.row;
        }
        if (cell.row > maxRow)
        {
            maxRow = cell.row;
        }
        if (cell.column < minColumn)
        {
            minColumn = cell.column;
        }
        if (cell.column > maxColumn)
        {
            maxColumn = cell.column;
        }
    }

    const int previewCellSize = 22;
    int shapeWidth = (maxColumn - minColumn + 1) * previewCellSize;
    int shapeHeight = (maxRow - minRow + 1) * previewCellSize;
    int originX = x + (width - shapeWidth) / 2;
    int originY = y + 44 + (height - 56 - shapeHeight) / 2;

    for (Position cell : cells)
    {
        int cellX = originX + (cell.column - minColumn) * previewCellSize;
        int cellY = originY + (cell.row - minRow) * previewCellSize;
        DrawCellAt(hdc, cellX, cellY, previewCellSize, game.GetNextBlockId());
    }
}

void DrawStatusPanel(HDC hdc, int x, int y, int width)
{
    std::string status = "PLAYING";
    COLORREF statusColor = RGB(122, 214, 176);
    if (game.IsGameOver())
    {
        status = "GAME OVER";
        statusColor = RGB(240, 101, 95);
    }
    else if (game.IsPaused())
    {
        status = "PAUSED";
        statusColor = RGB(241, 194, 100);
    }

    FillRoundRectColor(hdc, x, y, x + width, y + 78, 8,
                       RGB(31, 35, 36), RGB(61, 70, 62));
    DrawTextLine(hdc, x + 14, y + 10, "Status", 16, RGB(170, 178, 158), FW_NORMAL);
    DrawTextLine(hdc, x + 14, y + 34, status, 24, statusColor, FW_BOLD);
}

void DrawGame(HDC hdc)
{
    FillVerticalGradient(hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT,
                         RGB(18, 22, 21), RGB(35, 38, 32));
    DrawTextLine(hdc, BOARD_LEFT, 20, "TETRIS", 30, RGB(248, 244, 225), FW_BOLD);
    DrawTextLine(hdc, BOARD_LEFT + 118, 28, "native C++", 14, RGB(154, 164, 145), FW_NORMAL);

    FillRoundRectColor(hdc, BOARD_LEFT - 8, BOARD_TOP - 8,
                       BOARD_LEFT + CELL_SIZE * 10 + 8, BOARD_TOP + CELL_SIZE * 20 + 8,
                       10, RGB(12, 15, 15), RGB(67, 76, 66));

    int display[20][10];
    const int (&grid)[20][10] = game.GetGrid();
    for (int row = 0; row < 20; row++)
    {
        for (int column = 0; column < 10; column++)
        {
            display[row][column] = grid[row][column];
        }
    }

    for (int row = 0; row < 20; row++)
    {
        for (int column = 0; column < 10; column++)
        {
            DrawCell(hdc, row, column, display[row][column]);
        }
    }

    for (Position item : game.GetGhostBlockCells())
    {
        if (item.row >= 0 && item.row < 20 && item.column >= 0 && item.column < 10 && display[item.row][item.column] == 0)
        {
            DrawGhostCell(hdc, item.row, item.column, game.GetCurrentBlockId());
        }
    }

    for (Position item : game.GetCurrentBlockCells())
    {
        if (item.row >= 0 && item.row < 20 && item.column >= 0 && item.column < 10)
        {
            DrawCell(hdc, item.row, item.column, game.GetCurrentBlockId());
        }
    }

    int panelX = BOARD_LEFT + CELL_SIZE * 10 + 34;
    int panelWidth = WINDOW_WIDTH - panelX - 32;
    FillRoundRectColor(hdc, panelX - 12, BOARD_TOP - 8,
                       WINDOW_WIDTH - 24, BOARD_TOP + CELL_SIZE * 20 + 8,
                       10, RGB(24, 28, 28), RGB(58, 68, 58));

    DrawMetricPanel(hdc, panelX, BOARD_TOP + 10, panelWidth, 86,
                    "Score", std::to_string(game.GetScore()));
    DrawMetricPanel(hdc, panelX, BOARD_TOP + 112, (panelWidth - 12) / 2, 78,
                    "Lines", std::to_string(game.GetLinesCleared()));
    DrawMetricPanel(hdc, panelX + (panelWidth + 12) / 2, BOARD_TOP + 112,
                    (panelWidth - 12) / 2, 78, "Level", std::to_string(game.GetLevel()));
    DrawNextPreview(hdc, panelX, BOARD_TOP + 208, panelWidth, 156);
    DrawStatusPanel(hdc, panelX, BOARD_TOP + 388, panelWidth);
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
    case 'P':
        input = static_cast<int>(key);
        break;
    default:
        input = game.IsGameOver() ? 'r' : 0;
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
        SetTimer(hwnd, TIMER_ID, game.GetDropIntervalMs(), nullptr);
        return 0;
    case WM_TIMER:
        game.MoveBlockDown();
        SetTimer(hwnd, TIMER_ID, game.GetDropIntervalMs(), nullptr);
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
