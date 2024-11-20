#ifndef CELL_H
#define CELL_H
#include <string>
#include <memory>

struct Cell {
    int row;
    int col;
    char c;
    bool fireWallPresent = false;
    int whichPlayersFirewall = 0; // 0 refers to no player
    bool serverPort = false;
    int whichPlayersServerPort = 0;
    Cell(int row, int col);
    bool hasOwnFirewall(int playerNumber) const;
    bool hasOpponnentFirewall(int playerNumber) const;
    bool isOwnServerPort(int playerNumber) const;
    bool isOpponentServerPort(int playerNumber) const;
    bool isEmpty() const;
};

#endif
