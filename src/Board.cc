#include "Board.h"

const int numCol = 8;

Board::Board(int playerCount) : size{playerCount == 2 ? 8 : 10} {
    grid.resize(size);
    for (int i = 0; i < size; ++i) {
        grid[i].resize(numCol);
        for (int j = 0; j < numCol; ++j) {
            grid[i].emplace_back(Cell(i, j));
        }
    }
}

Cell& Board::getCell(int row, int col) {
    if (row < size && col < numCol) {
        return grid[row][col];
    }
    throw std::out_of_range("Cell coordinates out of bounds");
}
