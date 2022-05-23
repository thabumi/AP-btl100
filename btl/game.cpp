#include <iostream>
#include "initwindow.h"
#include "game.h"

Game::Game() {
    initSDL(window_, renderer_);
    initTTF();
}
Game::~Game() {
    waitUntilKeyPressed();
    quitSDL(window_, renderer_);
    quitTTF();
}
//
void Game::run() {
    good_.draw(renderer_);
    SDL_RenderPresent(renderer_);
    good_.run(window_, renderer_);
}
