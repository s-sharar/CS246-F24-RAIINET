#include "Graphics.h"
#include <sstream>
#include <vector>

using namespace std;

const int windowWidth = 640;
const int windowHeight = 1000;
const int namePadding = 50;
const int cellSize = 80;

Graphics::Graphics(weak_ptr<Game> &g, int playerNumber) : g{g}, playerNumber{playerNumber}, w{windowWidth, windowHeight} {
    auto game = g.lock();
    game->
}
