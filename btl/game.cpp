#include "game.h"
#include "initwindow.h"
#include <iostream>
#include <SDL_ttf.h>
using namespace std;

Game::Game() {
    initSDL(window_, renderer_);
}
Game::~Game() {
    quitSDL(window_, renderer_);
}
