#pragma once
#include "grid.h"
#include "blocks.h"
#include <random>
#include <vector>

class Game
{
public:
    Game();
    void HandleInput(int key);
    void MoveBlockDown();
    const int (&GetGrid() const)[20][10];
    std::vector<Position> GetCurrentBlockCells() const;
    std::vector<Position> GetGhostBlockCells() const;
    int GetCurrentBlockId() const;
    int GetNextBlockId() const;
    int GetScore() const;
    int GetLinesCleared() const;
    int GetLevel() const;
    int GetDropIntervalMs() const;
    bool IsGameOver() const;
    bool IsPaused() const;
    static int CalculateLevel(int completedLines);
    static int CalculateDropIntervalMs(int level);

private:
    void MoveBlockLeft();
    void MoveBlockRight();
    void DropBlock();
    Block GetRandomBlock();
    std::vector<Block> GetAllBlocks();
    bool IsBlockOutside() const;
    bool IsBlockOutside(const Block &block) const;
    void RotateBlock();
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
    std::mt19937 randomGenerator;
    bool gameOver;
    bool paused;
    int score;
    int linesCleared;
};
