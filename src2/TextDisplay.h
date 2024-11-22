#ifndef TEXTDISPLAY_H
#define TEXTDISPLAY_H
#include <vector>
#include <iostream>
#include <memory>
#include "Observer.h"
#include "Game.h"

using namespace std;

class TextObserver : public Observer {
    weak_ptr<Game> g;
    ostream &out;
    int playerNumber;
    public:
    TextObserver(weak_ptr<Game> &g, ostream &out, int playerNumber);
    void notify() override;
};

#endif