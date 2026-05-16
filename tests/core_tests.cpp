#include "blocks.h"
#include "game.h"
#include "grid.h"
#include "high_score.h"

#include <cstdio>
#include <cstdlib>
#include <fstream>
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

    std::vector<int> fullRows = grid.GetFullRows();
    int rowsCleared = grid.ClearFullRows();

    Expect(fullRows.size() == 1 && fullRows[0] == 19, "reports one full row before clearing");
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

void TestRotationKickAllowsIBlockToRotateAtTop()
{
    Game game{IBlock(), OBlock()};
    std::vector<Position> initialCells = game.GetCurrentBlockCells();

    game.Start();
    game.HandleInput('w');
    std::vector<Position> rotatedCells = game.GetCurrentBlockCells();

    Expect(!HasSameCells(initialCells, rotatedCells), "rotation kick allows I block to rotate near the spawn top");
    for (const Position &cell : rotatedCells)
    {
        Expect(cell.row >= 0, "rotation kick keeps rotated cells inside the top of the board");
        Expect(cell.column >= 0 && cell.column < 10, "rotation kick keeps rotated cells inside board columns");
    }
}

void TestStartGateBlocksMovementUntilInputStarts()
{
    Game game;
    std::vector<Position> initialCells = game.GetCurrentBlockCells();

    Expect(!game.IsStarted(), "new game waits on the start screen");
    Expect(!game.MoveBlockDown(), "automatic drop is blocked before start");
    Expect(HasSameCells(initialCells, game.GetCurrentBlockCells()), "start screen keeps current block still");

    game.HandleInput('a');

    Expect(game.IsStarted(), "first gameplay input starts the game");
    Expect(HasSameCells(initialCells, game.GetCurrentBlockCells()), "first start input does not also move the block");
}

void TestSoftDropScoresOnePoint()
{
    Game game;
    int startingScore = game.GetScore();

    game.Start();
    game.HandleInput('s');

    Expect(game.GetScore() == startingScore + 1, "soft drop scores one point");
}

void TestBlockedSoftDropDoesNotScore()
{
    OBlock block;
    block.Move(18, 0);
    Game game{block, IBlock()};

    game.Start();
    game.HandleInput('s');

    Expect(game.GetScore() == 0, "blocked soft drop does not score");
    Expect(game.GetHighScore() == 0, "blocked soft drop does not update high score");
}

void TestHighScoreSurvivesRestart()
{
    Game game;

    game.Start();
    game.HandleInput('s');
    Expect(game.GetScore() == 1, "soft drop creates a score for high score tracking");
    Expect(game.GetHighScore() == 1, "high score updates when score increases");

    game.Restart();

    Expect(game.IsStarted(), "restart immediately starts a new game");
    Expect(game.GetScore() == 0, "restart clears current score");
    Expect(game.GetHighScore() == 1, "restart keeps session high score");
}

void TestHighScoreFilePersistence()
{
    const char *path = "tetris_highscore_test.tmp";
    std::remove(path);

    Expect(LoadHighScore(path) == 0, "missing high score file loads as zero");
    Expect(SaveHighScore(path, 1234), "high score file saves successfully");
    Expect(LoadHighScore(path) == 1234, "saved high score can be loaded");

    {
        std::ofstream file(path, std::ios::trunc);
        file << "not-a-score\n";
    }
    Expect(LoadHighScore(path) == 0, "invalid high score file loads as zero");

    std::remove(path);
}

void TestPauseStopsAutomaticDrop()
{
    Game game;
    std::vector<Position> initialCells = game.GetCurrentBlockCells();

    game.Start();
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

void TestNextBlockPreviewHasFourCells()
{
    Game game;
    Expect(game.GetNextBlockCells().size() == 4, "next block preview exposes four cells");
}

void TestHoldStoresCurrentBlockOncePerDrop()
{
    Game game;
    int startingBlockId = game.GetCurrentBlockId();
    int startingNextBlockId = game.GetNextBlockId();

    game.Start();
    Expect(game.CanHold(), "hold is available before use");
    game.HandleInput('c');

    Expect(game.HasHeldBlock(), "hold stores a block");
    Expect(game.GetHeldBlockId() == startingBlockId, "hold stores the original current block");
    Expect(game.GetCurrentBlockId() == startingNextBlockId, "first hold advances the next block");
    Expect(!game.CanHold(), "hold is locked until the block lands");

    int currentAfterHold = game.GetCurrentBlockId();
    game.HandleInput('c');
    Expect(game.GetCurrentBlockId() == currentAfterHold, "second hold before landing is ignored");

    game.HandleInput(' ');
    Expect(game.CanHold(), "hold unlocks after the active block lands");
}

void TestLineClearFeedbackAndComboReset()
{
    Grid grid;
    for (int column = 0; column < 10; column++)
    {
        if (column < 3 || column > 6)
        {
            grid.grid[19][column] = 7;
        }
    }

    Game game{IBlock(), OBlock(), grid};
    game.Start();
    int startingClearEvent = game.GetClearEventId();
    game.HandleInput(' ');

    Expect(game.GetLastClearLines() == 1, "line clear feedback stores cleared line count");
    Expect(game.GetLastClearScore() == 100, "line clear feedback stores clear score");
    Expect(game.GetLastClearedRows().size() == 1 && game.GetLastClearedRows()[0] == 19, "line clear feedback stores cleared row index");
    Expect(game.GetClearEventId() == startingClearEvent + 1, "line clear increments clear event id");
    Expect(game.GetCombo() == 1, "first clear starts combo at one");
    Expect(game.GetLinesCleared() == 1, "game tracks total cleared lines after lock");

    game.HandleInput(' ');

    Expect(game.GetLastClearLines() == 0, "non-clear lock resets clear line feedback");
    Expect(game.GetLastClearScore() == 0, "non-clear lock resets clear score feedback");
    Expect(game.GetCombo() == 0, "non-clear lock resets combo");
}

void TestLevelAndDropSpeedProgression()
{
    Expect(Game::CalculateLineClearScore(0) == 0, "zero lines score zero line-clear points");
    Expect(Game::CalculateLineClearScore(4) == 800, "four-line clear uses Tetris score");
    Expect(Game::CalculateLevel(0) == 1, "level starts at one");
    Expect(Game::CalculateLevel(9) == 1, "level stays one before ten lines");
    Expect(Game::CalculateLevel(10) == 2, "level increases every ten lines");
    Expect(Game::CalculateDropIntervalMs(1) == 350, "level one uses the base drop interval");
    Expect(Game::CalculateDropIntervalMs(3) == 300, "drop interval gets faster as level rises");
    Expect(Game::CalculateDropIntervalMs(99) == 100, "drop interval has a minimum speed cap");
}
}

int main()
{
    TestGridClearsSingleRow();
    TestGridClearsMultipleRows();
    TestBlockRotationCycles();
    TestRotationKickAllowsIBlockToRotateAtTop();
    TestStartGateBlocksMovementUntilInputStarts();
    TestSoftDropScoresOnePoint();
    TestBlockedSoftDropDoesNotScore();
    TestHighScoreSurvivesRestart();
    TestHighScoreFilePersistence();
    TestPauseStopsAutomaticDrop();
    TestGhostBlockPreviewIsBelowCurrentBlock();
    TestNextBlockPreviewHasFourCells();
    TestHoldStoresCurrentBlockOncePerDrop();
    TestLineClearFeedbackAndComboReset();
    TestLevelAndDropSpeedProgression();

    std::cout << "All core tests passed." << std::endl;
    return 0;
}
