#include "Game.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include "Err.h"

using namespace std;

const int maxAbilities = 5;
const int numCol = 8;

Game::Game(int playerCount, const vector<string> &linkOrders, const vector<string> &abilities, bool graphicsEnabled) : playerCount{playerCount}, graphicsEnabled{graphicsEnabled} {
    const int numRow = (playerCount == 2) ? 8 : 10;
    board = make_shared<Board>(numRow);
    
}

string Game::getAbilityName(int i) {
    if (i < 1 || i > maxAbilities) throw runtime_error(Err::invalidAbilityIndex);
    return players[currentTurn - 1]->getAbilityName(i);
}

void Game::useAbility(int row, int col) {
    const int numRow = (playerCount == 2) ? 8 : 10;
    if (!(row >= 0 && row < numRow && col >= 0 && col < numCol)) throw out_of_range(Err::invalidCoordinates);
    useFireWall(row, col);
}

void Game::useAbility(const string &abilityName, char link) {
    if (!validLink(link)) throw runtime_error(Err::invalidLink);
    if (abilityName == "LinkBoost") {
        useLinkBoost(link);
    } else if (abilityName == "Download") {
        useDownload(link);
    } else if (abilityName == "Polarise") {
        usePolarise(link);
    } else if (abilityName == "Scan") {
        useScan(link);
    }
}

void Game::move(char link, const string &direction) {
    if (!validLink(link)) throw runtime_error(Err::invalidLink);
    if (!validDirection(direction)) throw runtime_error(Err::invalidDirection);
}

bool validLink(char link) {
    return (link >= 'a' && link <= 'z') || (link >= 'A' && link <= 'Z');
}

bool validDirection(const string &direction) {
    return direction == "up" || direction == "down" || direction == "left" || direction == "right";
}
