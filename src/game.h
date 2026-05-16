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
    void HandleInput(int key);
    void MoveBlockDown();
    const int (&GetGrid() const)[20][10];
    std::vector<Position> GetCurrentBlockCells() const;
    std::vector<Position> GetGhostBlockCells() const;
    std::vector<Position> GetNextBlockCells() const;
    std::vector<Position> GetHeldBlockCells() const;
    int GetCurrentBlockId() const;
    int GetNextBlockId() const;
    int GetHeldBlockId() const;
    int GetScore() const;
    int GetLinesCleared() const;
    int GetLevel() const;
    int GetDropIntervalMs() const;
    bool IsGameOver() const;
    bool IsPaused() const;
    bool HasHeldBlock() const;
    bool CanHold() const;
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
    bool BlockFits() const;
    bool BlockFits(const Block &block) const;
    void Reset();
    void UpdateScore(int LinesCleared, int moveDownPoints);
    void TogglePause();
    Grid grid;
    std::vector<Block> blocks;
    Block currentBlock;
    Block nextBlock;
    Block heldBlock;
    std::mt19937 randomGenerator;
    bool gameOver;
    bool paused;
    bool hasHeldBlock;
    bool holdUsed;
    int score;
    int linesCleared;
};
