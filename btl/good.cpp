#include <iostream>
#include <chrono>
#include <random>
#include <queue>
#include <fstream>
#include <SDL.h>
#include <SDL_ttf.h>
#include "egg.h"
#include "good.h"
#include "someconst.h"
#include "Button.h"

using namespace std;

SDL_Texture* loadText(TTF_Font* &gFont, SDL_Renderer* &renderer, string text, SDL_Color textColor) {
    SDL_Texture* mTexture = NULL;
    SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, text.c_str(), textColor);
    if(textSurface == NULL) {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    }
    else {
        mTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if(mTexture == NULL) {
            printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        }
        SDL_FreeSurface(textSurface);
    }
    return mTexture;
}
Good::Good() {
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

    gmrect.x = GAME_UPLEFT_X;
    gmrect.y = GAME_UPLEFT_Y;
    gmrect.w = GAME_WIDTH;
    gmrect.h = GAME_HEIGHT;
    pauseButton.x = SCREEN_WIDTH - PAUSE_BUTTON_SIZE;
    pauseButton.y = SCREEN_HEIGHT - PAUSE_BUTTON_SIZE;
    pauseButton.w = PAUSE_BUTTON_SIZE;
    pauseButton.h = PAUSE_BUTTON_SIZE;
    shooter = Egg(static_cast<Egg::Type>(rng() % 3), EGG_TO_SHOOT_X, EGG_TO_SHOOT_Y);

    board.resize(13);
    for (int i = 0; i < 13; i++) {
        board[i].resize(9 - i % 2);
        for (int j = 0; j < 8 + 1 - i % 2; j++) {
            if (i % 2 == 0) {
                board[i][j] = Egg(static_cast<Egg::Type>(rng() % 3), GAME_UPLEFT_X + EGG_RADIUS + 2 * j * EGG_RADIUS, EGG_RADIUS + i * ROW_DISTANCE);
            }
            else {
                board[i][j] = Egg(static_cast<Egg::Type>(rng() % 3), GAME_UPLEFT_X + EGG_RADIUS * 2 + 2 * j * EGG_RADIUS, EGG_RADIUS + i * ROW_DISTANCE);
            }
        }
    }
    for (int i = 10; i < 13; i++) {
        for (int j = 0; j < (int)board[i].size(); j++) {
            board[i][j].setVisible(false);
        }
    }
}

void Good::draw(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, BLACK_COLOR.r, BLACK_COLOR.g, BLACK_COLOR.b, BLACK_COLOR.a);
    SDL_RenderClear(renderer);
    for (int i = 0; i < (int)board.size(); i++) {
        for (int j = 0; j < (int)board[i].size(); j++) {
            if (board[i][j].getVisible()) {
                board[i][j].draw(renderer, 1);
            }
        }
    }
    shooter.draw(renderer, shooter.getVisible());
    SDL_SetRenderDrawColor(renderer, YELLOW_COLOR.r, YELLOW_COLOR.g, YELLOW_COLOR.b, YELLOW_COLOR.a);
    SDL_RenderDrawRect(renderer, &gmrect);
    SDL_RenderDrawLine(renderer, GAME_UPLEFT_X, board[GAME_ROW - 1][0].getY() + ROW_DISTANCE / 2 + 5, GAME_DOWNRIGHT_X, board[GAME_ROW - 1][0].getY() + ROW_DISTANCE / 2 + 5);
    SDL_RenderDrawLine(renderer, GAME_DOWNRIGHT_X, SCORE_LINE_Y, SCREEN_WIDTH, SCORE_LINE_Y);
    renderScore(renderer);
    renderHighscore(renderer);
    renderOutsideEgg(renderer);
    renderPauseButton(renderer);
}

void Good::down() {
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

    for (int i = 0; i < (int)board.size(); i++) {
        for (int j = 0; j < (int)board[i].size(); j++) {
            board[i][j].moves(0, ROW_DISTANCE);
        }
    }
    for (int i = board.size() - 1; i >= 1; i--) {
        board[i] = board[i - 1];
    }
    if (board[1].size() == BIG_ROW) {
        board[0].assign(SMALL_ROW, Egg());
        for (int j = 0; j < (int)board[0].size(); j++) {
            board[0][j] = Egg(static_cast<Egg::Type>(rng() % 3), GAME_UPLEFT_X + EGG_RADIUS * 2 + 2 * j * EGG_RADIUS, EGG_RADIUS);
        }
    }
    else {
        board[0].assign(BIG_ROW, Egg());
        for (int j = 0; j < (int)board[0].size(); j++) {
            board[0][j] = Egg(static_cast<Egg::Type>(rng() % 3), GAME_UPLEFT_X + EGG_RADIUS + 2 * j * EGG_RADIUS, EGG_RADIUS);
        }
    }
    longRow ^= 1;
}

bool Good::explosion(int startingRow, int startingCol) {

    vector<vector<bool>> visited(board.size());
    for (int i = 0; i < (int)board.size(); i++) {
        visited[i].assign(board[i].size(), false);;
    }
    queue<pair<int, int>> q;

    int _type = board[startingRow][startingCol].getType();

    int cnt = 0;
    q.push({startingRow, startingCol});
    visited[startingRow][startingCol] = true;
    cnt++;
    while (!q.empty()) {
        pair<int, int> t = q.front();
        q.pop();
        int x = t.first, y = t.second;
        if (board[x].size() == BIG_ROW) {
            for (int i = 0; i < 6; i++) {
                int x0 = x + aLongRow[i];
                int y0 = y + aLongCol[i];
                if (x0 >= 0 && x0 < (int)board.size() && y0 >= 0 && y0 < (int)board[x0].size()) {
                    if (!visited[x0][y0] && board[x0][y0].getVisible() && board[x0][y0].getType() == _type) {
                        visited[x0][y0] = true;
                        q.push({x0, y0});
                        cnt++;
                    }
                }
            }
        }
        else {
            for (int i = 0; i < 6; i++) {
                int x0 = x + aShortRow[i];
                int y0 = y + aShortCol[i];
                if (x0 >= 0 && x0 < (int)board.size() && y0 >= 0 && y0 < (int)board[x0].size()) {
                    if (!visited[x0][y0] && board[x0][y0].getVisible() && board[x0][y0].getType() == _type) {
                        visited[x0][y0] = true;
                        q.push({x0, y0});
                        cnt++;
                    }
                }
            }
        }
    }
    if (cnt >= 3) {
        for (int i = 0; i < (int)board.size(); i++) {
            for (int j = 0; j < (int)board[i].size(); j++) {
                if (visited[i][j]) {
                    board[i][j].setVisible(0);
                }
            }
        }
        for (int j = 0; j < (int)board[0].size(); j++) {
            if (board[0][j].getVisible() && !visited[0][j]) {
                queue<pair<int, int>> q;
                q.push({0, j});
                visited[0][j] = true;
                while (!q.empty()) {
                    pair<int, int> t = q.front();
                    q.pop();
                    int x = t.first, y = t.second;
                    if (board[x].size() == BIG_ROW) {
                        for (int i = 0; i < 6; i++) {
                            int x0 = x + aLongRow[i];
                            int y0 = y + aLongCol[i];
                            if (x0 >= 0 && x0 < (int)board.size() && y0 >= 0 && y0 < (int)board[x0].size()) {
                                if (!visited[x0][y0] && board[x0][y0].getVisible()) {
                                    visited[x0][y0] = true;
                                    q.push({x0, y0});
                                }
                            }
                        }
                    }
                    else {
                        for (int i = 0; i < 6; i++) {
                            int x0 = x + aShortRow[i];
                            int y0 = y + aShortCol[i];
                            if (x0 >= 0 && x0 < (int)board.size() && y0 >= 0 && y0 < (int)board[x0].size()) {
                                if (!visited[x0][y0] && board[x0][y0].getVisible()) {
                                    visited[x0][y0] = true;
                                    q.push({x0, y0});
                                }
                            }
                        }
                    }
                }
            }
        }
        for (int i = 0; i < (int)board.size(); i++) {
            for (int j = 0; j < (int)board[i].size(); j++) {
                if (!visited[i][j]) {
                    board[i][j].setVisible(0);
                }
            }
        }
        score += cnt * 10;
        return true;
    }
    return false;
}

void Good::reloadAmmo() {
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    shooter = Egg(static_cast<Egg::Type>(rng() % 3), EGG_TO_SHOOT_X, EGG_TO_SHOOT_Y);
}

bool Good::notGoodAnymore() {
    for (int j = 0; j < (int)board[board.size() - 1].size(); j++) {
        if (board[board.size() - 1][j].getVisible()) {
            return true;
        }
    }
    return false;
}
double pointDistance(pair<double, double> a, pair<double, double> b) {
    return sqrt((a.first - b.first) * (a.first - b.first) + (a.second - b.second) * (a.second - b.second));
}

pair<int, int> Good::closestNeighbour(int targetRow, int targetCol) {
    pair<double, double> shooterLocation = shooter.getLocation();
    double minDistance = 1e9;
    int idx = -1;
    if (board[targetRow].size() == BIG_ROW) {
        int aRow[] = {1, 1, 0, 0, -1, -1};
        int aCol[] = {-1, 0, -1, 1, -1, 0};

        for (int i = 0; i < 6; i++) {
            int i0 = targetRow + aRow[i];
            int j0 = targetCol + aCol[i];
            if (i0 >= 0 && i0 < (int)board.size() && j0 >= 0 && j0 < (int)board[i0].size()) {
                pair<double, double> neighbourLocation = board[i0][j0].getLocation();
                if (pointDistance(neighbourLocation, shooterLocation) < minDistance) {
                    idx = i;
                    minDistance = pointDistance(neighbourLocation, shooterLocation);
                }
            }
        }
        if (idx < 0) {
            return {-1, -1};
        }

        int i0 = targetRow + aRow[idx];
        int j0 = targetCol + aCol[idx];
        board[i0][j0].setType(static_cast<Egg::Type>(shooter.getType()));
        board[i0][j0].setVisible(1);
        return {i0, j0};
    }
    else {
        int aRow[] = {1, 1, 0, 0, -1, -1};
        int aCol[] = {0, 1, -1, 1, 0, 1};
        for (int i = 0; i < 6; i++) {
            int i0 = targetRow + aRow[i];
            int j0 = targetCol + aCol[i];
            if (i0 >= 0 && i0 < (int)board.size() && j0 >= 0 && j0 < (int)board[i0].size() && !board[i0][j0].getVisible()) {
                pair<double, double> neighbourLocation = board[i0][j0].getLocation();
                if (pointDistance(neighbourLocation, shooterLocation) < minDistance) {
                    idx = i;
                    minDistance = pointDistance(neighbourLocation, shooterLocation);
                }
            }
        }
        if (idx < 0) {
            return {-1, -1};
        }

        int i0 = targetRow + aRow[idx];
        int j0 = targetCol + aCol[idx];
        board[i0][j0].setType(static_cast<Egg::Type>(shooter.getType()));
        board[i0][j0].setVisible(1);
        return {i0, j0};
    }
}
pair<double, double> standardize(pair<double, double> p) {
    double len = sqrt(p.first * p.first + p.second * p.second);
    return {p.first * 10 / len, p.second * 10 / len};
}

bool Good::blank() {
    for (int j = 0; j < (int)board[0].size(); j++) {
        if (!board[0][j].getVisible()) {
            return true;
        }
    }
    return false;
}
int Good::run(SDL_Window* window, SDL_Renderer* renderer) {
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

    draw(renderer);
    SDL_RenderPresent(renderer);
    SDL_Event e;
    while (true) {
        SDL_Delay(5);

        if ( SDL_PollEvent(&e) == 0) continue;

        if (e.type == SDL_QUIT) {
//            saveData();
            return 0;
        }

        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
//            saveData();
            return 0;
        }

        int x, y;

        if (e.type == SDL_MOUSEBUTTONDOWN) {
            x = e.button.x; // Lấy hoành độ x của chuột
            y = e.button.y; // Lấy tung độ y của chuột
            if (inButton(pauseButton, x, y)) {
                if (pause(window, renderer) == 0) {
                    return 0;
                }
            }
            else {
                pair<double, double> vt = standardize({x - EGG_TO_SHOOT_X, y - EGG_TO_SHOOT_Y});
                double stepX = vt.first;
                double stepY = vt.second;

                bool b = false;
                while (!b) {

                    shooter.moves(stepX, stepY);
                    if (shooter.getX() + EGG_RADIUS >= GAME_DOWNRIGHT_X || shooter.getX() - EGG_RADIUS <= GAME_UPLEFT_X) {
                        shooter.moves(-stepX, -stepY);
                        stepX = -stepX;
                        shooter.moves(stepX, stepY);
                    }
                    if (shooter.getY() + EGG_RADIUS >= GAME_DOWNRIGHT_Y || shooter.getY() - EGG_RADIUS <= GAME_UPLEFT_Y) {
                        shooter.moves(-stepX, -stepY);
                        stepY = -stepY;
                        shooter.moves(stepX, stepY);
                    }

                    for (int i = 0; i < 12; i++) {
                        bool brk = false;
                        for (int j = 0; j < (int)board[i].size(); j++) {
                            if (shooter.collision(board[i][j]) && board[i][j].getVisible()) {

                                pair<int, int> p = closestNeighbour(i, j);
                                if (!explosion(p.first, p.second)) {
                                    countDown++;
                                    if (countDown == 5) {
                                        countDown = 0;
                                        down();
                                    }
                                    if(notGoodAnymore()) {
                                        shooter = Egg(static_cast<Egg::Type>(rng() % 3), EGG_TO_SHOOT_X, EGG_TO_SHOOT_Y, 0);
                                        draw(renderer);
                                        renderEndGame(renderer);
                                        SDL_RenderPresent(renderer);
                                        resetGame();
                                        SDL_Delay(2000);
                                        return 1;
                                    }
                                }
                                else {
                                    highscore = max(highscore, score);
                                    if (notGoodAnymore()) {
                                        shooter = Egg(static_cast<Egg::Type>(rng() % 3), EGG_TO_SHOOT_X, EGG_TO_SHOOT_Y, 0);
                                        draw(renderer);
                                        renderEndGame(renderer);
                                        SDL_RenderPresent(renderer);
                                        resetGame();
                                        SDL_Delay(2000);
                                        return 1;
                                    }
                                    if (blank()) {
                                        down();
                                    }
                                }
                                shooter = Egg(static_cast<Egg::Type>(rng() % 3), EGG_TO_SHOOT_X, EGG_TO_SHOOT_Y);
                                b = true;
                                brk = true;
                                break;
                            }
                        }
                        if (brk) {
                            break;
                        }
                    }
                    draw(renderer);
                    SDL_RenderPresent(renderer);
                    SDL_Delay(5);
                }
            }

        }
    }
}

void Good::renderScore(SDL_Renderer* renderer) {
    TTF_Font* myFont = TTF_OpenFont("futureforces.ttf", 40);

    SDL_Texture* scoreWord = loadText(myFont, renderer, "SCORE", YELLOW_COLOR);
    SDL_Rect scoreWordRect;

    SDL_QueryTexture(scoreWord, NULL, NULL, &scoreWordRect.w, &scoreWordRect.h);
    scoreWordRect.x = (SCREEN_WIDTH + GAME_DOWNRIGHT_X - scoreWordRect.w) / 2;
    scoreWordRect.y = 0;
    SDL_RenderCopy(renderer, scoreWord, NULL, &scoreWordRect);

    SDL_Texture* realScore = loadText(myFont, renderer, to_string(score), YELLOW_COLOR);
    SDL_Rect realScoreRect;

    SDL_QueryTexture(realScore, NULL, NULL, &realScoreRect.w, &realScoreRect.h);
    realScoreRect.x = (SCREEN_WIDTH + GAME_DOWNRIGHT_X - realScoreRect.w) / 2;
    realScoreRect.y = SCORE_Y;
    SDL_RenderCopy(renderer, realScore, NULL, &realScoreRect);
    TTF_CloseFont(myFont);
    SDL_DestroyTexture(scoreWord);
    SDL_DestroyTexture(realScore);
}

void Good::renderHighscore(SDL_Renderer* renderer) {
    TTF_Font* myFont = TTF_OpenFont("futureforces.ttf", 40);

    SDL_Texture* highscoreWord = loadText(myFont, renderer, "HIGHSCORE", YELLOW_COLOR);
    SDL_Rect highscoreWordRect;

    SDL_QueryTexture(highscoreWord, NULL, NULL, &highscoreWordRect.w, &highscoreWordRect.h);
    highscoreWordRect.x = (SCREEN_WIDTH + GAME_DOWNRIGHT_X - highscoreWordRect.w) / 2;
    highscoreWordRect.y = 70;
    SDL_RenderCopy(renderer, highscoreWord, NULL, &highscoreWordRect);

    SDL_Texture* realHighscore = loadText(myFont, renderer, to_string(highscore), YELLOW_COLOR);
    SDL_Rect realHighscoreRect;

    SDL_QueryTexture(realHighscore, NULL, NULL, &realHighscoreRect.w, &realHighscoreRect.h);
    realHighscoreRect.x = (SCREEN_WIDTH + GAME_DOWNRIGHT_X - realHighscoreRect.w) / 2;
    realHighscoreRect.y = HIGHSCORE_Y;
    SDL_RenderCopy(renderer, realHighscore, NULL, &realHighscoreRect);
    TTF_CloseFont(myFont);
    SDL_DestroyTexture(highscoreWord);
    SDL_DestroyTexture(realHighscore);
}

void Good::renderOutsideEgg(SDL_Renderer* renderer) {
    int amount;
    if (countDown == 0 || countDown == 5) {
        amount = 5;
    }
    else {
        amount = 5 - countDown;
    }
    vector<Egg> outside(amount);

    for (int i = 0; i < amount; i++) {
        outside[i] = Egg(static_cast<Egg::Type>(0), FIRST_OUTSIDE_EGG_X + OUTSIDE_EGGS_DISTANCE * i, OUTSIDE_EGG_Y);
        outside[i].draw(renderer, 1);
    }
}


void Good::renderPauseButton(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, YELLOW_COLOR.r, YELLOW_COLOR.g, YELLOW_COLOR.b, YELLOW_COLOR.a);
    SDL_Rect pauseIcon1;
    pauseIcon1.x = pauseButton.x + 10;
    pauseIcon1.y = pauseButton.y + 10;
    pauseIcon1.w = 30;
    pauseIcon1.h = 30;

    SDL_Rect pauseIcon2;
    pauseIcon2.x = pauseButton.x + 20;
    pauseIcon2.y = pauseButton.y + 10;
    pauseIcon2.w = 10;
    pauseIcon2.h = 30;

    SDL_RenderDrawRect(renderer, &pauseButton);

    SDL_RenderFillRect(renderer, &pauseIcon1);

    SDL_SetRenderDrawColor(renderer, BLACK_COLOR.r, BLACK_COLOR.g, BLACK_COLOR.b, BLACK_COLOR.a);

    SDL_RenderFillRect(renderer, &pauseIcon2);
}

void Good::loadData() {
    ifstream fin("board.txt");
    int state;
    fin >> state;
    if (state != 0) {
        for (int i = 0; i < 13; i++) {
            int len;
            fin >> len;
            board[i].clear();
            board[i].resize(len);
            for (int j = 0; j < len; j++) {
                int typee, visiblee;
                fin >> typee >> visiblee;
                if (len == BIG_ROW) {
                    board[i][j] = Egg(static_cast<Egg::Type>(typee), GAME_UPLEFT_X + EGG_RADIUS + 2 * j * EGG_RADIUS, EGG_RADIUS + i * ROW_DISTANCE, visiblee);
                }
                else {
                    board[i][j] = Egg(static_cast<Egg::Type>(typee), GAME_UPLEFT_X + EGG_RADIUS * 2 + 2 * j * EGG_RADIUS, EGG_RADIUS + i * ROW_DISTANCE, visiblee);
                }
            }
        }
        int typee;
        fin >> typee;
        shooter.setType(static_cast<Egg::Type>(typee));
        fin >> score;
    }
    fin.close();
    ifstream finn("highscore.txt");
    finn >> highscore;

    finn.close();
}
void Good::saveData() {
    remove("board.txt");
    ofstream fout("board.txt");
    fout << 1 << '\n';
    for (int i = 0; i < 13; i++) {
        fout << board[i].size() << " ";
        for (int j = 0; j < (int)board[i].size(); j++) {
            fout << board[i][j].getType() << " " << board[i][j].getVisible() << " ";
        }
        fout << '\n';
    }
    fout << shooter.getType() << '\n';
    fout << score;
    fout.close();
    remove("highscore.txt");
    ofstream foutt("highscore.txt");
    foutt << highscore;
    foutt.close();
}

void Good::resetData() {
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    shooter = Egg(static_cast<Egg::Type>(rng() % 3), EGG_TO_SHOOT_X, EGG_TO_SHOOT_Y);

    board.resize(13);
    for (int i = 0; i < 13; i++) {
        board[i].resize(9 - i % 2);
        for (int j = 0; j < 8 + 1 - i % 2; j++) {
            if (i % 2 == 0) {
                board[i][j] = Egg(static_cast<Egg::Type>(rng() % 3), GAME_UPLEFT_X + EGG_RADIUS + 2 * j * EGG_RADIUS, EGG_RADIUS + i * ROW_DISTANCE);
            }
            else {
                board[i][j] = Egg(static_cast<Egg::Type>(rng() % 3), GAME_UPLEFT_X + EGG_RADIUS * 2 + 2 * j * EGG_RADIUS, EGG_RADIUS + i * ROW_DISTANCE);
            }
        }
    }
    for (int i = 10; i < 13; i++) {
        for (int j = 0; j < (int)board[i].size(); j++) {
            board[i][j].setVisible(false);
        }
    }
    score = 0;
    saveData();
}
int Good::pause(SDL_Window* window, SDL_Renderer* renderer) {
    TTF_Font* myFont = TTF_OpenFont("futureforces.ttf", 40);

    SDL_Texture* resumeButton = loadText(myFont, renderer, "Resume", YELLOW_COLOR);

    SDL_Rect resumeButtonRect;

    SDL_QueryTexture(resumeButton, NULL, NULL, &resumeButtonRect.w, &resumeButtonRect.h);
    resumeButtonRect.x = (SCREEN_WIDTH - resumeButtonRect.w) / 2;
    resumeButtonRect.y = (SCREEN_HEIGHT - resumeButtonRect.h) / 2;


    SDL_Rect resumeButtonOutline;
    resumeButtonOutline.w = RESUME_BUTTON_WIDTH;
    resumeButtonOutline.h = RESUME_BUTTON_HEIGHT;
    resumeButtonOutline.x = resumeButtonRect.x - (resumeButtonOutline.w - resumeButtonRect.w) / 2;
    resumeButtonOutline.y = resumeButtonRect.y - (resumeButtonOutline.h - resumeButtonRect.h) / 2;

    SDL_SetRenderDrawColor(renderer, BLACK_COLOR.r, BLACK_COLOR.g, BLACK_COLOR.b, BLACK_COLOR.a);
    SDL_RenderClear(renderer);


    SDL_RenderCopy(renderer, resumeButton, NULL, &resumeButtonRect);
    SDL_SetRenderDrawColor(renderer, YELLOW_COLOR.r, YELLOW_COLOR.g, YELLOW_COLOR.b, YELLOW_COLOR.a);
    SDL_RenderDrawRect(renderer, &resumeButtonOutline);
    SDL_RenderPresent(renderer);
    TTF_CloseFont(myFont);
    SDL_DestroyTexture(resumeButton);


    SDL_Event e;

    while (true) {
        SDL_Delay(5);
        if ( SDL_PollEvent(&e) == 0) continue;

        if (e.type == SDL_QUIT) {
            saveData();
            return 0;
        }

        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
            saveData();
            return 0;
        }

        int x, y;

        if (e.type == SDL_MOUSEBUTTONDOWN) {
            x = e.button.x; // Lấy hoành độ x của chuột
            y = e.button.y; // Lấy tung độ y của chuột
            if (inButton(resumeButtonOutline, x, y)) {
                draw(renderer);
                SDL_RenderPresent(renderer);
                return 1;
            }
        }
    }
}
// put in game file
void Good::renderEndGame(SDL_Renderer* renderer) {
    TTF_Font* myFont = TTF_OpenFont("futureforces.ttf", 40);

    SDL_Texture* endGame = loadText(myFont, renderer, "Game over!", YELLOW_COLOR);

    SDL_Rect endGameRect;

    SDL_QueryTexture(endGame, NULL, NULL, &endGameRect.w, &endGameRect.h);
    endGameRect.x = (SCREEN_WIDTH - endGameRect.w) / 2;
    endGameRect.y = (SCREEN_HEIGHT - endGameRect.h) / 2;
    SDL_RenderCopy(renderer, endGame, NULL, &endGameRect);
    SDL_RenderPresent(renderer);
    TTF_CloseFont(myFont);
    SDL_DestroyTexture(endGame);
}
void Good::resetGame() {
    score = 0;
    countDown = 0;
}
void Good::getHighscore() {
    ifstream fin("highscore.txt");
    fin >> highscore;
    fin.close();
}
