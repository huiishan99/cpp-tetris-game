#include "game.h"
#include <windows.h>

Game::Game()
{
    grid = Grid();
    randomGenerator = std::mt19937(std::random_device{}());
    blocks = GetAllBlocks();
    currentBlock = GetRandomBlock();
    nextBlock = GetRandomBlock();
    gameOver = false;
    score = 0;
}

Block Game::GetRandomBlock()
{
    if (blocks.empty())
    {
        blocks = GetAllBlocks();
    }
    std::uniform_int_distribution<int> distribution(0, static_cast<int>(blocks.size()) - 1);
    int randomIndex = distribution(randomGenerator);
    Block block = blocks[randomIndex];
    blocks.erase(blocks.begin() + randomIndex);
    return block;
}

std::vector<Block> Game ::GetAllBlocks()
{
    return {IBlock(), JBlock(), LBlock(), OBlock(), SBlock(), TBlock(), ZBlock()};
}

const int (&Game::GetGrid() const)[20][10]
{
    return grid.grid;
}

std::vector<Position> Game::GetCurrentBlockCells()
{
    return currentBlock.GetCellPositions();
}

int Game::GetNextBlockId() const
{
    return nextBlock.id;
}

void Game::HandleInput(int key)
{
    if (gameOver && key != 0)
    {
        gameOver = false;
        Reset();
        return;
    }
    switch (key)
    {
    case 'a':
    case 'A':
        MoveBlockLeft();
        break;
    case 'd':
    case 'D':
        MoveBlockRight();
        break;
    case 's':
    case 'S':
        MoveBlockDown();
        UpdateScore(0, 1);
        break;
    case 'w':
    case 'W':
        RotateBlock();
        break;
    case ' ':
        DropBlock();
        break;
    }
}

void Game::MoveBlockLeft()
{
    if (!gameOver)
    {
        currentBlock.Move(0, -1);
        if (IsBlockOutside() || BlockFits() == false)
        {
            currentBlock.Move(0, 1);
        }
    }
}

void Game::MoveBlockRight()
{
    if (!gameOver)
    {
        currentBlock.Move(0, 1);
        if (IsBlockOutside() || BlockFits() == false)
        {
            currentBlock.Move(0, -1);
        }
    }
}

void Game::MoveBlockDown()
{
    if (!gameOver)
    {
        currentBlock.Move(1, 0);
        if (IsBlockOutside() || BlockFits() == false)
        {
            currentBlock.Move(-1, 0);
            LockBlock();
        }
    }
}

void Game::DropBlock()
{
    if (!gameOver)
    {
        int rowsDropped = 0;
        while (true)
        {
            currentBlock.Move(1, 0);
            if (IsBlockOutside() || BlockFits() == false)
            {
                currentBlock.Move(-1, 0);
                LockBlock();
                break;
            }
            rowsDropped++;
        }
        UpdateScore(0, rowsDropped * 2);
    }
}

bool Game::IsBlockOutside()
{
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for (Position item : tiles)
    {
        if (grid.IsCellOutside(item.row, item.column))
        {
            return true;
        }
    }
    return false;
}

void Game::RotateBlock()
{
    if (!gameOver)
    {
        currentBlock.Rotate();
        if (IsBlockOutside() || BlockFits() == false)
        {
            currentBlock.UndoRotation();
        }
        else
        {
            Beep(880, 25);
        }
    }
}

void Game::LockBlock()
{
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for (Position item : tiles)
    {
        grid.grid[item.row][item.column] = currentBlock.id;
    }
    currentBlock = nextBlock;
    if (BlockFits() == false)
    {
        gameOver = true;
    }
    nextBlock = GetRandomBlock();
    int rowsCleared = grid.ClearFullRows();
    if (rowsCleared > 0)
    {
        Beep(660, 60);
        UpdateScore(rowsCleared, 0);
    }
}

bool Game::BlockFits()
{
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for (Position item : tiles)
    {
        if (grid.IsCellEmpty(item.row, item.column) == false)
        {
            return false;
        }
    }
    return true;
}

void Game::Reset()
{
    grid.Initialize();
    blocks = GetAllBlocks();
    currentBlock = GetRandomBlock();
    nextBlock = GetRandomBlock();
    score = 0;
}

void Game::UpdateScore(int LinesCleared, int moveDownPoints)
{
    switch (LinesCleared)
    {
    case 1:
        score += 100;
        break;
    case 2:
        score += 300;
        break;
    case 3:
        score += 500;
        break;
    case 4:
        score += 800;
        break;
    default:
        break;
    }

    score += moveDownPoints;
}
