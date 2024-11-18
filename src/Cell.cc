#include "Cell.h"

using namespace std;

const int Player1 = 1;
const int Player2 = 2;
const char ServerPortChar = 'S';

Cell::Cell(int row, int col) 
    : row{row}, col{col}, c{'\0'} {
    if ((row == 0)  && ((col == 3) || (col == 4))) {
        c = ServerPortChar;
        serverPort = true;
        whichPlayersServerPort = Player1;
    } else if ((row == 7)  && ((col == 3) || (col == 4))) {
        c = ServerPortChar;
        serverPort = true;
        whichPlayersServerPort = Player2;
    } else if (((row == 0) && (col < 3)) || ((row == 1) && (col > 2) && (col < 5)) || ((row == 0) && (col > 4))) {
    	c = ('a' + col);
    } else if (((row == 7) && (col < 3)) || ((row == 6) && (col > 2) && (col < 5)) || ((row == 7) && (col > 4))) {
    	c = ('A' + col);
    } else {
        c = '.';
    }
}

int Cell::getRow() const {
    return row;
}

int Cell::getCol() const {
    return col;
}

char Cell::getChar() const {
    return c;
}

bool Cell::hasOwnFirewall(int playerNumber) const {
    return fireWallPresent && whichPlayersFirewall == playerNumber;
}

bool Cell::hasOpponnentFirewall(int playerNumber) const {
    return fireWallPresent && !(whichPlayersFirewall == playerNumber);
}

void Cell::setFirewall(int playerNumber, bool status) {
    fireWallPresent = status;
    if (status == false) {
        whichPlayersFirewall = 0;
    } else {
        whichPlayersFirewall = playerNumber;
    }
}

bool Cell::isOwnServerPort(int playerNumber) const {
    return serverPort && whichPlayersServerPort == playerNumber;
}

bool Cell::isOpponentServerPort(int playerNumber) const {
    return serverPort && !(whichPlayersServerPort == playerNumber);
}

bool Cell::isEmpty() const {
    return c == '.';
}
