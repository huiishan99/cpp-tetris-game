#pragma once
#include <vector>

class Grid
{
public:
    Grid();
    void Initialize();
    void Print() const;
    bool IsCellOutside(int row, int column) const;
    bool IsCellEmpty(int row, int column) const;
    std::vector<int> GetFullRows() const;
    int ClearFullRows();
    int grid[20][10];

private:
    bool IsRowFull(int row) const;
    void ClearRow(int row);
    void MoveRowDown(int row, int numRows);
    int numRows;
    int numCols;
};
