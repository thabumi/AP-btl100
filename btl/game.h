#ifndef GAME_HPP_
#define GAME_HPP_
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
