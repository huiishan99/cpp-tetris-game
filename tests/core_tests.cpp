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
    int startingScore = game.score;

    game.HandleInput('s');

    Expect(game.score == startingScore + 1, "soft drop scores one point");
}
}

int main()
{
    TestGridClearsSingleRow();
    TestGridClearsMultipleRows();
    TestBlockRotationCycles();
    TestSoftDropScoresOnePoint();

    std::cout << "All core tests passed." << std::endl;
    return 0;
}
