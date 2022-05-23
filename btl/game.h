#ifndef GAME_HPP_
#define GAME_HPP_

#include <iostream>
#include <SDL.h>
#include "good.h"

class Game {
public:
    Game();
    ~Game();
    void run();
private:
    SDL_Window* window_ = nullptr;
    SDL_Renderer* renderer_ = nullptr;
    Good good_;
};

#endif // GAME_HPP
