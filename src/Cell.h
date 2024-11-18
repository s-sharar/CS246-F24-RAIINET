#ifndef CELL_H
#define CELL_H
#include <string>
#include <memory>

class Cell {
private:
    int row;
    int col;
    char c;
    bool fireWallPresent = false;
    int whichPlayersFirewall = 0; // 0 refers to no player
    bool serverPort = false;
    int whichPlayersServerPort = 0;
public:
    Cell(int row, int col);
    int getRow() const;
    int getCol() const;
    char getChar() const;
    bool hasOwnFirewall(int playerNumber) const;
    bool hasOpponnentFirewall(int playerNumber) const;
    void setFirewall(int playerNumber, bool status);
    bool isOwnServerPort(int playerNumber) const;
    bool isOpponentServerPort(int playerNumber) const;
    bool isEmpty() const;
};

#endif
