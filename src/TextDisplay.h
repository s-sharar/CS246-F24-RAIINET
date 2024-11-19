#ifndef TEXT_DISPLAY_H
#define TEXT_DISPLAY_H
#include <vector>
#include <iostream>
#include <memory>
#include "Observer.h"
#include "Subject.h"
#include "Player.h"

using namespace std;

class TextObserver : public Observer {
    weak_ptr<Game> game;
    public:
    TextObserver(shared_ptr<Game> &g);
    void notify() override;
};

#endif