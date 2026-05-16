#pragma once
#include "grid.h"
#include "blocks.h"
#include <random>
#include <vector>

class Game
{
public:
    Game();
    Game(const Block &startingBlock, const Block &upcomingBlock);
    Game(const Block &startingBlock, const Block &upcomingBlock, const Grid &initialGrid);
    void HandleInput(int key);
    void Start();
    void Restart();
    bool MoveBlockDown();
    void FinishLineClear();
    const int (&GetGrid() const)[20][10];
    std::vector<Position> GetCurrentBlockCells() const;
    std::vector<Position> GetGhostBlockCells() const;
    std::vector<Position> GetNextBlockCells() const;
    std::vector<std::vector<Position>> GetUpcomingBlockCells() const;
    std::vector<Position> GetHeldBlockCells() const;
    int GetCurrentBlockId() const;
    int GetNextBlockId() const;
    std::vector<int> GetUpcomingBlockIds() const;
    int GetHeldBlockId() const;
    int GetScore() const;
    int GetHighScore() const;
    void SetHighScore(int value);
    int GetLinesCleared() const;
    int GetLastClearLines() const;
    int GetLastClearScore() const;
    const std::vector<int> &GetLastClearedRows() const;
    int GetClearEventId() const;
    int GetCombo() const;
    int GetLevel() const;
    int GetDropIntervalMs() const;
    bool IsStarted() const;
    bool IsGameOver() const;
    bool IsPaused() const;
    bool IsLineClearPending() const;
    bool HasHeldBlock() const;
    bool CanHold() const;
    static int CalculateLineClearScore(int completedLines);
    static int CalculateLevel(int completedLines);
    static int CalculateDropIntervalMs(int level);

private:
    void MoveBlockLeft();
    void MoveBlockRight();
    void DropBlock();
    void HoldBlock();
    Block GetRandomBlock();
    std::vector<Block> GetAllBlocks();
    Block CreateBlockById(int blockId) const;
    bool IsBlockOutside() const;
    bool IsBlockOutside(const Block &block) const;
    void RotateBlock();
    bool TryWallKick();
    void LockBlock();
    void StartLineClear(const std::vector<int> &fullRows);
    void SpawnNextBlock();
    Block AdvanceNextBlock();
    void FillUpcomingBlocks();
    bool BlockFits() const;
    bool BlockFits(const Block &block) const;
    void Reset();
    void UpdateScore(int LinesCleared, int moveDownPoints);
    void UpdateHighScore();
    void TogglePause();
    Grid grid;
    std::vector<Block> blocks;
    Block currentBlock;
    Block nextBlock;
    std::vector<Block> upcomingBlocks;
    Block heldBlock;
    std::mt19937 randomGenerator;
    bool gameOver;
    bool started;
    bool paused;
    bool lineClearPending;
    bool hasHeldBlock;
    bool holdUsed;
    int score;
    int highScore;
    int linesCleared;
    int lastClearLines;
    int lastClearScore;
    std::vector<int> lastClearedRows;
    int clearEventId;
    int combo;
};
