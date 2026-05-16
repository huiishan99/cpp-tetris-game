#include "game.h"
#include "sound.h"

namespace
{
const int LinesPerLevel = 10;
const int BaseDropIntervalMs = 350;
const int DropIntervalStepMs = 25;
const int MinimumDropIntervalMs = 100;

const Position WallKickOffsets[] = {
    Position(0, 0),
    Position(0, -1),
    Position(0, 1),
    Position(0, -2),
    Position(0, 2),
    Position(1, 0),
    Position(1, -1),
    Position(1, 1),
    Position(-1, 0),
};
}

Game::Game()
{
    grid = Grid();
    randomGenerator = std::mt19937(std::random_device{}());
    blocks = GetAllBlocks();
    currentBlock = GetRandomBlock();
    nextBlock = GetRandomBlock();
    gameOver = false;
    started = false;
    paused = false;
    hasHeldBlock = false;
    holdUsed = false;
    score = 0;
    highScore = 0;
    linesCleared = 0;
    lastClearLines = 0;
    lastClearScore = 0;
    combo = 0;
}

Game::Game(const Block &startingBlock, const Block &upcomingBlock)
{
    grid = Grid();
    randomGenerator = std::mt19937(0);
    blocks = GetAllBlocks();
    currentBlock = startingBlock;
    nextBlock = upcomingBlock;
    gameOver = false;
    started = false;
    paused = false;
    hasHeldBlock = false;
    holdUsed = false;
    score = 0;
    highScore = 0;
    linesCleared = 0;
    lastClearLines = 0;
    lastClearScore = 0;
    combo = 0;
}

Game::Game(const Block &startingBlock, const Block &upcomingBlock, const Grid &initialGrid)
{
    grid = initialGrid;
    randomGenerator = std::mt19937(0);
    blocks = GetAllBlocks();
    currentBlock = startingBlock;
    nextBlock = upcomingBlock;
    gameOver = false;
    started = false;
    paused = false;
    hasHeldBlock = false;
    holdUsed = false;
    score = 0;
    highScore = 0;
    linesCleared = 0;
    lastClearLines = 0;
    lastClearScore = 0;
    combo = 0;
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

std::vector<Position> Game::GetCurrentBlockCells() const
{
    return currentBlock.GetCellPositions();
}

std::vector<Position> Game::GetGhostBlockCells() const
{
    Block ghostBlock = currentBlock;
    while (true)
    {
        ghostBlock.Move(1, 0);
        if (IsBlockOutside(ghostBlock) || BlockFits(ghostBlock) == false)
        {
            ghostBlock.Move(-1, 0);
            return ghostBlock.GetCellPositions();
        }
    }
}

std::vector<Position> Game::GetNextBlockCells() const
{
    return nextBlock.GetCellPositions();
}

std::vector<Position> Game::GetHeldBlockCells() const
{
    if (!hasHeldBlock)
    {
        return {};
    }
    return heldBlock.GetCellPositions();
}

int Game::GetCurrentBlockId() const
{
    return currentBlock.id;
}

int Game::GetNextBlockId() const
{
    return nextBlock.id;
}

int Game::GetHeldBlockId() const
{
    if (!hasHeldBlock)
    {
        return 0;
    }
    return heldBlock.id;
}

int Game::GetScore() const
{
    return score;
}

int Game::GetHighScore() const
{
    return highScore;
}

void Game::SetHighScore(int value)
{
    if (value < score)
    {
        value = score;
    }
    highScore = value;
}

int Game::GetLinesCleared() const
{
    return linesCleared;
}

int Game::GetLastClearLines() const
{
    return lastClearLines;
}

int Game::GetLastClearScore() const
{
    return lastClearScore;
}

int Game::GetCombo() const
{
    return combo;
}

int Game::GetLevel() const
{
    return CalculateLevel(linesCleared);
}

int Game::GetDropIntervalMs() const
{
    return CalculateDropIntervalMs(GetLevel());
}

bool Game::IsStarted() const
{
    return started;
}

bool Game::IsGameOver() const
{
    return gameOver;
}

bool Game::IsPaused() const
{
    return paused;
}

bool Game::HasHeldBlock() const
{
    return hasHeldBlock;
}

bool Game::CanHold() const
{
    return started && !gameOver && !paused && !holdUsed;
}

int Game::CalculateLineClearScore(int completedLines)
{
    switch (completedLines)
    {
    case 1:
        return 100;
    case 2:
        return 300;
    case 3:
        return 500;
    case 4:
        return 800;
    default:
        return 0;
    }
}

int Game::CalculateLevel(int completedLines)
{
    if (completedLines <= 0)
    {
        return 1;
    }
    return completedLines / LinesPerLevel + 1;
}

int Game::CalculateDropIntervalMs(int level)
{
    if (level < 1)
    {
        level = 1;
    }

    int interval = BaseDropIntervalMs - (level - 1) * DropIntervalStepMs;
    if (interval < MinimumDropIntervalMs)
    {
        return MinimumDropIntervalMs;
    }
    return interval;
}

void Game::HandleInput(int key)
{
    if (gameOver && key != 0)
    {
        Restart();
        return;
    }

    if (!started && key != 0)
    {
        Start();
        return;
    }

    if (key == 'p' || key == 'P')
    {
        TogglePause();
        return;
    }

    if (paused)
    {
        return;
    }

    switch (key)
    {
    case 'c':
    case 'C':
        HoldBlock();
        break;
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
        if (MoveBlockDown())
        {
            UpdateScore(0, 1);
        }
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

void Game::Start()
{
    if (!gameOver)
    {
        started = true;
        paused = false;
    }
}

void Game::Restart()
{
    Reset();
    Start();
}

void Game::MoveBlockLeft()
{
    if (started && !gameOver && !paused)
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
    if (started && !gameOver && !paused)
    {
        currentBlock.Move(0, 1);
        if (IsBlockOutside() || BlockFits() == false)
        {
            currentBlock.Move(0, -1);
        }
    }
}

bool Game::MoveBlockDown()
{
    if (started && !gameOver && !paused)
    {
        currentBlock.Move(1, 0);
        if (IsBlockOutside() || BlockFits() == false)
        {
            currentBlock.Move(-1, 0);
            LockBlock();
            return false;
        }
        return true;
    }
    return false;
}

void Game::DropBlock()
{
    if (started && !gameOver && !paused)
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

void Game::HoldBlock()
{
    if (!CanHold())
    {
        return;
    }

    Block previousCurrent = CreateBlockById(currentBlock.id);
    if (!hasHeldBlock)
    {
        heldBlock = previousCurrent;
        hasHeldBlock = true;
        currentBlock = nextBlock;
        nextBlock = GetRandomBlock();
    }
    else
    {
        Block previousHeld = heldBlock;
        heldBlock = previousCurrent;
        currentBlock = previousHeld;
    }

    holdUsed = true;
    if (BlockFits() == false)
    {
        gameOver = true;
        paused = false;
    }
}

bool Game::IsBlockOutside() const
{
    return IsBlockOutside(currentBlock);
}

bool Game::IsBlockOutside(const Block &block) const
{
    std::vector<Position> tiles = block.GetCellPositions();
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
    if (started && !gameOver && !paused)
    {
        currentBlock.Rotate();
        if (!TryWallKick())
        {
            currentBlock.UndoRotation();
        }
        else
        {
            PlayRotateSound();
        }
    }
}

bool Game::TryWallKick()
{
    for (Position offset : WallKickOffsets)
    {
        currentBlock.Move(offset.row, offset.column);
        if (!IsBlockOutside() && BlockFits())
        {
            return true;
        }
        currentBlock.Move(-offset.row, -offset.column);
    }
    return false;
}

void Game::LockBlock()
{
    std::vector<Position> tiles = currentBlock.GetCellPositions();
    for (Position item : tiles)
    {
        grid.grid[item.row][item.column] = currentBlock.id;
    }
    currentBlock = nextBlock;
    holdUsed = false;
    if (BlockFits() == false)
    {
        gameOver = true;
        started = true;
        paused = false;
    }
    nextBlock = GetRandomBlock();
    int rowsCleared = grid.ClearFullRows();
    if (rowsCleared > 0)
    {
        combo++;
        lastClearLines = rowsCleared;
        lastClearScore = CalculateLineClearScore(rowsCleared);
        PlayLineClearSound();
        UpdateScore(rowsCleared, 0);
    }
    else
    {
        combo = 0;
        lastClearLines = 0;
        lastClearScore = 0;
    }
}

bool Game::BlockFits() const
{
    return BlockFits(currentBlock);
}

bool Game::BlockFits(const Block &block) const
{
    std::vector<Position> tiles = block.GetCellPositions();
    for (Position item : tiles)
    {
        if (grid.IsCellEmpty(item.row, item.column) == false)
        {
            return false;
        }
    }
    return true;
}

Block Game::CreateBlockById(int blockId) const
{
    switch (blockId)
    {
    case 1:
        return LBlock();
    case 2:
        return JBlock();
    case 3:
        return IBlock();
    case 4:
        return OBlock();
    case 5:
        return SBlock();
    case 6:
        return TBlock();
    case 7:
        return ZBlock();
    default:
        return Block();
    }
}

void Game::Reset()
{
    grid.Initialize();
    blocks = GetAllBlocks();
    currentBlock = GetRandomBlock();
    nextBlock = GetRandomBlock();
    gameOver = false;
    started = false;
    paused = false;
    hasHeldBlock = false;
    holdUsed = false;
    score = 0;
    linesCleared = 0;
    lastClearLines = 0;
    lastClearScore = 0;
    combo = 0;
}

void Game::UpdateScore(int linesCompleted, int moveDownPoints)
{
    score += CalculateLineClearScore(linesCompleted);
    linesCleared += linesCompleted;
    score += moveDownPoints;
    UpdateHighScore();
}

void Game::UpdateHighScore()
{
    if (score > highScore)
    {
        highScore = score;
    }
}

void Game::TogglePause()
{
    if (!gameOver)
    {
        paused = !paused;
    }
}
