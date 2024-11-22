#include "Cell.h"

using namespace std;

const int Player1 = 1;
const int Player2 = 2;
const int Player3 = 3;
const int Player4 = 4;
const char ServerPortChar = 'S';

Cell::Cell(int row, int col) 
    : row{row}, col{col}, c{'\0'} {
    if ((row == 0)  && ((col == 3) || (col == 4))) {
        whichPlayersServerPort = Player1;
        c = '.';
    } else if ((row == 7)  && ((col == 3) || (col == 4))) {
        whichPlayersServerPort = Player2;
        c = '.';
    } else if (((row == 0) && (col < 3)) || ((row == 1) && (col > 2) && (col < 5)) || ((row == 0) && (col > 4))) {
    	c = ('a' + col);
    } else if (((row == 7) && (col < 3)) || ((row == 6) && (col > 2) && (col < 5)) || ((row == 7) && (col > 4))) {
    	c = ('A' + col);
    } else {
        c = '.';
    }
}

Cell::Cell(const Cell &other) : row{other.row}, col{other.col}, c{other.c}, whichPlayersFirewall{0}, whichPlayersServerPort{other.whichPlayersServerPort}, locked{other.locked} {}

int Cell::getRow() const {
    return row;
}

int Cell::getCol() const {
    return col;
}

char Cell::getContent() const {
    return c;
}

bool Cell::hasFirewall() const {
    return whichPlayersFirewall != 0;
}

bool Cell::hasOwnFirewall(int playerNumber) const {
    return whichPlayersFirewall == playerNumber;
}

bool Cell::hasOpponnentFirewall(int playerNumber) const {
    return hasFirewall() && whichPlayersFirewall != playerNumber;
}

void Cell::setFirewall(int playerNumber, bool status) {
    if (status == false) {
        whichPlayersFirewall = 0;
    } else {
        whichPlayersFirewall = playerNumber;
    }
}

bool Cell::isServerPort() const {
    return whichPlayersServerPort != 0;
}

bool Cell::isOwnServerPort(int playerNumber) const {
    return whichPlayersServerPort == playerNumber;
}

bool Cell::isOpponentServerPort(int playerNumber) const {
    return isServerPort() && !(whichPlayersServerPort == playerNumber);
}

int Cell::getPlayersServerPort() const {
    return whichPlayersServerPort;
}

bool Cell::isEmpty() const {
    return c == '.';
}

bool Cell::isLocked() const {
    return locked;
}

void Cell::setContent(char content) {
    c = content;
}
