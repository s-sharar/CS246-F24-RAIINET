#ifndef GRAPHICS_H
#define GRAPHICS_H
#include "Observer.h"
#include "window.h"
#include "Player.h"
#include "Game.h"
#include <vector>
#include <memory>

class Graphics : public Observer {
    weak_ptr<Game> game;
    public:
    Graphics(shared_ptr<Game> &game);
    void notify() override;
};


#endif