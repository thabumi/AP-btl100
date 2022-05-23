#include <iostream>
#include <chrono>
#include <random>
#include <SDL.h>
#include <SDL_ttf.h>
#include "drawcircle.h"
#include "someconst.h"
#include "initwindow.h"
#include "egg.h"
#include "game.h"
#include "test.h"
using namespace std;
//
Game::Game() {
    initSDL(window_, renderer_);
}
Game::~Game() {
    waitUntilKeyPressed();
    quitSDL(window_, renderer_);
}
//
void Game::run() {
    good_.draw(renderer_);
    SDL_RenderPresent(renderer_);
    good_.run(window_, renderer_);
}
