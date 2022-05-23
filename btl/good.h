#ifndef GOOD_H_
#define GOOD_H_

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
double pointDistance(pair<double, double> a, pair<double, double> b);
pair<double, double> standardize(pair<double, double> p);
SDL_Texture* loadText(TTF_Font* &gFont, SDL_Renderer* &renderer, string text, SDL_Color textColor);
class Good {
public:
    Good();
    void draw(SDL_Renderer* renderer);
    void down();
    bool explosion(int startingRow, int startingCol);
    void reloadAmmo();
    bool notGoodAnymore();
    pair<int, int> closestNeighbour(int targetRow, int targetCol);
    void run(SDL_Window* window, SDL_Renderer* renderer);
    bool blank();
    void renderScore(SDL_Renderer* renderer);
    void renderOutsideEgg(SDL_Renderer* renderer);
    void renderEndGame(SDL_Renderer* renderer);
private:
    vector<vector<Egg>> board;
    Egg shooter;
    SDL_Rect gmrect;
    int score = 0;
    int countDown = 0;
    bool longRow = true;
    int aLongRow[6] = {1, 1, 0, 0, -1, -1};
    int aLongCol[6] = {-1, 0, -1, 1, -1, 0};
    int aShortRow[6] = {1, 1, 0, 0, -1, -1};
    int aShortCol[6] = {0, 1, -1, 1, 0, 1};
};


#endif // GOOD_H_
