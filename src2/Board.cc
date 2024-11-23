#include "Board.h"
#include "Err.h"
#include <stdexcept>
#include <iostream>

const int numCol = 8;

Board::Board(int numRow) : size{numRow} {
    grid.resize(size);
    for (int i = 0; i < size; ++i) {
        grid[i].resize(size);
        for (int j = 0; j < numCol; ++j) {
            Cell c{i, j, size};
            grid[i][j] = c;
        }
    }
}

Cell& Board::getCell(int row, int col){
    if (row < size && col < numCol) {
        return grid[row][col];
    }
    throw std::out_of_range(Err::invalidCoordinates);
}

int Board::getSize() const {
    return size;
}

