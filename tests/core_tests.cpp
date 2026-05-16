#include "blocks.h"
#include "game.h"
#include "grid.h"

#include <cstdlib>
#include <iostream>
#include <vector>

namespace
{
bool HasCell(const std::vector<Position> &cells, int row, int column)
{
    for (const Position &cell : cells)
    {
        if (cell.row == row && cell.column == column)
        {
            return true;
        }
    }
    return false;
}

bool HasSameCells(const std::vector<Position> &left, const std::vector<Position> &right)
{
    if (left.size() != right.size())
    {
        return false;
    }

    for (const Position &cell : left)
    {
        if (!HasCell(right, cell.row, cell.column))
        {
            return false;
        }
    }
    return true;
}

int MinimumRow(const std::vector<Position> &cells)
{
    int minimum = cells.front().row;
    for (const Position &cell : cells)
    {
        if (cell.row < minimum)
        {
            minimum = cell.row;
        }
    }
    return minimum;
}

void Expect(bool condition, const char *message)
{
    if (!condition)
    {
        std::cerr << "FAILED: " << message << std::endl;
        std::exit(1);
    }
}

void TestGridClearsSingleRow()
{
    Grid grid;
    for (int column = 0; column < 10; column++)
    {
        grid.grid[19][column] = 1;
    }
    grid.grid[18][4] = 7;

    int rowsCleared = grid.ClearFullRows();

    Expect(rowsCleared == 1, "clears one completed row");
    Expect(grid.grid[19][4] == 7, "moves rows above a cleared row down");
    Expect(grid.grid[18][4] == 0, "clears the original moved row");
}

void TestGridClearsMultipleRows()
{
    Grid grid;
    for (int column = 0; column < 10; column++)
    {
        grid.grid[18][column] = 2;
        grid.grid[19][column] = 3;
    }
    grid.grid[17][0] = 6;

    int rowsCleared = grid.ClearFullRows();

    Expect(rowsCleared == 2, "clears two completed rows");
    Expect(grid.grid[19][0] == 6, "moves rows down by the number of cleared rows");
    Expect(grid.grid[17][0] == 0, "empties moved source rows after multi-clear");
}

void TestBlockRotationCycles()
{
    TBlock block;
    std::vector<Position> initialCells = block.GetCellPositions();

    for (int index = 0; index < 4; index++)
    {
        block.Rotate();
    }

    std::vector<Position> finalCells = block.GetCellPositions();
    Expect(finalCells.size() == initialCells.size(), "rotation preserves cell count");
    for (const Position &cell : initialCells)
    {
        Expect(HasCell(finalCells, cell.row, cell.column), "four rotations return to the original cells");
    }
}

void TestSoftDropScoresOnePoint()
{
    Game game;
    int startingScore = game.GetScore();

    game.HandleInput('s');

    Expect(game.GetScore() == startingScore + 1, "soft drop scores one point");
}

void TestPauseStopsAutomaticDrop()
{
    Game game;
    std::vector<Position> initialCells = game.GetCurrentBlockCells();

    game.HandleInput('p');
    game.MoveBlockDown();

    Expect(game.IsPaused(), "pause input toggles paused state on");
    Expect(HasSameCells(initialCells, game.GetCurrentBlockCells()), "paused game does not auto-drop");

    game.HandleInput('p');
    game.MoveBlockDown();

    Expect(!game.IsPaused(), "pause input toggles paused state off");
    Expect(!HasSameCells(initialCells, game.GetCurrentBlockCells()), "unpaused game resumes auto-drop");
}

void TestGhostBlockPreviewIsBelowCurrentBlock()
{
    Game game;
    std::vector<Position> currentCells = game.GetCurrentBlockCells();
    std::vector<Position> ghostCells = game.GetGhostBlockCells();

    Expect(ghostCells.size() == currentCells.size(), "ghost block keeps the same number of cells");
    Expect(MinimumRow(ghostCells) > MinimumRow(currentCells), "ghost block lands below the current block");
    Expect(HasSameCells(currentCells, game.GetCurrentBlockCells()), "ghost preview does not move the current block");
}
}

int main()
{
    TestGridClearsSingleRow();
    TestGridClearsMultipleRows();
    TestBlockRotationCycles();
    TestSoftDropScoresOnePoint();
    TestPauseStopsAutomaticDrop();
    TestGhostBlockPreviewIsBelowCurrentBlock();

    std::cout << "All core tests passed." << std::endl;
    return 0;
}
