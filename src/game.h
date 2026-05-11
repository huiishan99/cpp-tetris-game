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
    std::vector<Position> GetCurrentBlockCells();
    int GetCurrentBlockId() const;
    int GetNextBlockId() const;
    bool gameOver;
    int score;

private:
    void MoveBlockLeft();
    void MoveBlockRight();
    void DropBlock();
    Block GetRandomBlock();
    std::vector<Block> GetAllBlocks();
    bool IsBlockOutside();
    void RotateBlock();
    void LockBlock();
    bool BlockFits();
    void Reset();
    void UpdateScore(int LinesCleared, int moveDownPoints);
    Grid grid;
    std::vector<Block> blocks;
    Block currentBlock;
    Block nextBlock;
    std::mt19937 randomGenerator;
};
