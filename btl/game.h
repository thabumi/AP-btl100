#ifndef GAME_HPP_
#define GAME_HPP_
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <vector>
#include "egg.h"
class Game {
public:
    Game();
    ~Game();


private:

    /* todo
    vector<vector<Egg>> board;
    SDL_Rect gameRect;
    Egg eggToShoot;
    */
    SDL_Window* window_ = nullptr;
    SDL_Renderer* renderer_ = nullptr;
};

#endif // GAME_HPP
