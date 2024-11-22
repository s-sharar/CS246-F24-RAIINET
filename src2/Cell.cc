#include "Cell.h"

using namespace std;

const int Player1 = 1;
const int Player2 = 2;
const int Player3 = 3;
const int Player4 = 4;
const char ServerPortChar = 'S';

Cell::Cell(int row, int col, int size) : row{row}, col{col}, size{size}, c{'\0'} {
    int shift = (size == 10) ? - 1 : 0;
    locked = false;
    // locked cells for 4 players
    if ((row == 0) && (col == 0) && (size == 10)) locked = true;
    if ((row == 0) && (col == 9) && (size == 10)) locked = true;
    if ((row == 9) && (col == 0) && (size == 10)) locked = true;
    if ((row == 9) && (col == 9) && (size == 10)) locked = true;

    // serverPort coord
    int serverPortCoord = (size - 1) / 2; // for 8 -> 7 / 2 == 3, for 10 -> 9 / 2 = 4

    if ((col == serverPortCoord) || (col == serverPortCoord + 1)) {
        if (row == 0) whichPlayersServerPort = Player1;
        if (row == size - 1) whichPlayersServerPort = Player1;
        c = '.';
    } else if ((row == serverPortCoord) || (row == serverPortCoord + 1)) {
        if (col == 0) whichPlayersServerPort = Player3;
        if (col == size - 1) whichPlayersServerPort = Player4;
        c = '.';
    } 
    
    // initial
    if (((row == 0) && (col < serverPortCoord)) || ((row == 1) && (col >= serverPortCoord) && (col <= serverPortCoord + 1)) || ((row == 0) && (col > serverPortCoord + 1))) {
    	c = ('a' + col + shift);
    } else if (((row == size - 1) && (col < serverPortCoord)) || ((row == size - 2) && (col >= serverPortCoord) && (col <= serverPortCoord + 1)) || ((row == size - 1) && (col > serverPortCoord + 1))) {
    	c = ('A' + col + shift);
    } else if ((((col == 0) && (row < serverPortCoord)) || ((col == 1) && (row >= serverPortCoord) && (row <= serverPortCoord + 1)) || ((col == 0) && (row > serverPortCoord + 1))) && (size == 10)) {
    	c = ('s' + row + shift);
    } else if ((((col == size - 1) && (row < serverPortCoord)) || ((col == size - 2) && (row >= serverPortCoord) && (row <= serverPortCoord + 1)) || ((col == size - 1) && (row > serverPortCoord + 1))) && (size == 10)) {
    	c = ('S' + row + shift);
    } else {
        c = '.';
    }
    
    if (locked) c = '.';
}

Cell::Cell(const Cell &other) : row{other.row}, col{other.col}, size{other.size}, c{other.c}, whichPlayersFirewall{0}, whichPlayersServerPort{other.whichPlayersServerPort}, locked{other.locked} {}

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

int Cell::getPlayersFirewall() const {
    return whichPlayersFirewall;
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
