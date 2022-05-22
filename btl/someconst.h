#ifndef SOMECONST_H_
#define SOMECONST_H_

#include <iostream>
#include <algorithm>
#include <cmath>
#include <SDL.h>

using namespace std;

const double eps = 1e-9;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const string WINDOW_TITLE = "Egg Shooter";

const int GAME_WIDTH = 360;
const int GAME_HEIGHT = 600;
const int GAME_UPLEFT_X = (SCREEN_WIDTH - GAME_WIDTH) / 2;
const int GAME_UPLEFT_Y = (SCREEN_HEIGHT - GAME_HEIGHT) / 2;
const int GAME_DOWNRIGHT_X = (SCREEN_WIDTH + GAME_WIDTH) / 2;
const int GAME_DOWNRIGHT_Y = (SCREEN_HEIGHT + GAME_HEIGHT) / 2;

const int EGG_RADIUS = 20;
const int EGG_TO_SHOOT_X = GAME_UPLEFT_X + GAME_WIDTH / 2;
const int EGG_TO_SHOOT_Y = GAME_HEIGHT - EGG_RADIUS;

const int GAME_ROW = 12;

const long double ROW_DISTANCE = 20 * sqrt(3) + 0.5;

const int BIG_ROW = 9;
const int SMALL_ROW = 8;

const SDL_Color CYAN_COLOR = {0, 255, 255};
const SDL_Color BLUE_COLOR = {0, 0, 255};
const SDL_Color ORANGE_COLOR = {255, 165, 0};
const SDL_Color YELLOW_COLOR = {255, 255, 0};
const SDL_Color LIME_COLOR = {0, 255, 0};
const SDL_Color PURPLE_COLOR = {128, 0, 128};
const SDL_Color RED_COLOR = {255, 0, 0};
const SDL_Color WHITE_COLOR = {255, 255, 255};
const SDL_Color BLACK_COLOR = {0, 0, 0};
const SDL_Color GREEN_COLOR = {0, 128, 0};

const SDL_Color DEFAULT_COLOR = BLACK_COLOR;


#endif // SOMECONST_H_
