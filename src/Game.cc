#include "Game.h"
#include <iostream>
#include <sstream>
#include <stdexcept>

using namespace std;

Game::Game(int playerCount, const vector<string> &linkOrders, const vector<string> &abilities, bool graphicsEnabled) : playerCount{playerCount}, graphicsEnabled{graphicsEnabled} {
    board = make_shared<Board>();
    
}

string Game::getAbilityName(int i) {
    return players[currentTurn - 1]->getAbilityName(i);
}



