#ifndef GAME_H
#define GAME_H
#include <vector>
#include <memory>
#include <string>
#include "Cell.h"
#include "Player.h"
#include "Link.h"
#include "Subject.h"
#include "Board.h"

using namespace std;

class Game : public Subject {
    shared_ptr<Board> board;
    vector<shared_ptr<Player>> players;
    const int playerCount;
    int currentTurn = 1;
    bool gameOver = false;
    int playerWon = 0;
    bool graphicsEnabled;
    public:
    Game(int playerCount, const vector<string> &linkOrders, const vector<string> &abilities, bool graphicsEnabled);
    void move();
    void displayAbilities() const;
    string getAbilityName(int index);
};


#endif
