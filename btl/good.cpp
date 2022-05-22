#include <iostream>
#include <chrono>
#include <random>
#include <queue>
#include <SDL.h>
#include <SDL_ttf.h>
#include "drawcircle.h"
#include "someconst.h"
#include "initwindow.h"
#include "egg.h"
#include "game.h"
#include "test.h"
#include "good.h"

using namespace std;

Good::Good() {
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    gmrect.x = GAME_UPLEFT_X;
    gmrect.y = GAME_UPLEFT_Y;
    gmrect.w = GAME_WIDTH;
    gmrect.h = GAME_HEIGHT;

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
        for (int j = 0; j < board[i].size(); j++) {
            board[i][j].setVisible(false);
        }
    }
}

void Good::draw(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, BLACK_COLOR.r, BLACK_COLOR.g, BLACK_COLOR.b, BLACK_COLOR.a);
    SDL_RenderClear(renderer);
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[i].size(); j++) {
            if (board[i][j].getVisible()) {
                board[i][j].draw(renderer, 1);
            }
        }
    }
    shooter.draw(renderer, shooter.getVisible());
    SDL_SetRenderDrawColor(renderer, YELLOW_COLOR.r, YELLOW_COLOR.g, YELLOW_COLOR.b, YELLOW_COLOR.a);
    SDL_RenderDrawRect(renderer, &gmrect);
    SDL_RenderDrawLine(renderer, GAME_UPLEFT_X, board[GAME_ROW - 1][0].getY() + ROW_DISTANCE / 2 + 5, GAME_DOWNRIGHT_X, board[GAME_ROW - 1][0].getY() + ROW_DISTANCE / 2 + 5);
}

void Good::down() {
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[i].size(); j++) {
            board[i][j].moves(0, ROW_DISTANCE);
        }
    }
    for (int i = board.size() - 1; i >= 1; i--) {
        board[i] = board[i - 1];
    }
    if (board[1].size() == BIG_ROW) {
        board[0].assign(SMALL_ROW, Egg());
        for (int j = 0; j < board[0].size(); j++) {
            board[0][j] = Egg(static_cast<Egg::Type>(rng() % 3), GAME_UPLEFT_X + EGG_RADIUS * 2 + 2 * j * EGG_RADIUS, EGG_RADIUS);
        }
    }
    else {
        board[0].assign(BIG_ROW, Egg());
        for (int j = 0; j < board[0].size(); j++) {
            board[0][j] = Egg(static_cast<Egg::Type>(rng() % 3), GAME_UPLEFT_X + EGG_RADIUS + 2 * j * EGG_RADIUS, EGG_RADIUS);
        }
    }
    longRow ^= 1;
}

bool Good::explosion(int startingRow, int startingCol) {

    vector<vector<bool>> visited(board.size());
    for (int i = 0; i < board.size(); i++) {
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
                if (x0 >= 0 && x0 < board.size() && y0 >= 0 && y0 < board[x0].size()) {
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
                if (x0 >= 0 && x0 < board.size() && y0 >= 0 && y0 < board[x0].size()) {
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
        for (int i = 0; i < board.size(); i++) {
            for (int j = 0; j < board[i].size(); j++) {
                if (visited[i][j]) {
                    board[i][j].setVisible(0);
                }
            }
        }
        for (int j = 0; j < board[0].size(); j++) {
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
                            if (x0 >= 0 && x0 < board.size() && y0 >= 0 && y0 < board[x0].size()) {
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
                            if (x0 >= 0 && x0 < board.size() && y0 >= 0 && y0 < board[x0].size()) {
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
        for (int i = 0; i < board.size(); i++) {
            for (int j = 0; j < board[i].size(); j++) {
                if (!visited[i][j]) {
                    board[i][j].setVisible(0);
                }
            }
        }
        return true;
    }
    return false;
}

void Good::reloadAmmo() {
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    shooter = Egg(static_cast<Egg::Type>(rng() % 3), EGG_TO_SHOOT_X, EGG_TO_SHOOT_Y);
}

bool Good::notGoodAnymore() {
    for (int j = 0; j < board[board.size() - 1].size(); j++) {
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
            if (i0 >= 0 && i0 < board.size() && j0 >= 0 && j0 < board[i0].size()) {
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
            if (i0 >= 0 && i0 < board.size() && j0 >= 0 && j0 < board[i0].size() && !board[i0][j0].getVisible()) {
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
    for (int j = 0; j < board[0].size(); j++) {
        if (board[0][j].getVisible()) {
            return false;
        }
    }
    return true;
}
void Good::run(SDL_Window* window, SDL_Renderer* renderer) {
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    SDL_Event e;

    int cnt = 0;
    while (true) {
        // Đợi 10 mili giây
        SDL_Delay(5);

        // Nếu không có sự kiện gì thì tiếp tục trở về đầu vòng lặp
        if ( SDL_WaitEvent(&e) == 0) continue;

        // Nếu sự kiện là kết thúc (như đóng cửa sổ) thì thoát khỏi vòng lặp
        if (e.type == SDL_QUIT) break;

        // Nếu nhấn phìm ESC thì thoát khỏi vòng lặp
        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) break;

        // Nếu chuột (mouse) được nhấn (xuống)
        int x, y;

        if (e.type == SDL_MOUSEBUTTONDOWN) {
            x = e.button.x; // Lấy hoành độ x của chuột
            y = e.button.y; // Lấy tung độ y của chuột
            // Xoá toàn bộ màn hình và vẽ lại với màu sắc tuỳ thuộc chuột trái hay phải được nhấn
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
                    for (int j = 0; j < board[i].size(); j++) {
                        if (shooter.collision(board[i][j]) && board[i][j].getVisible()) {
//                            board[i][j].setType(static_cast<Egg::Type>(shooter.getType()));
//                            board[i][j].setVisible(0);

                            pair<int, int> p = closestNeighbour(i, j);

                            if (!explosion(p.first, p.second)) {

                                cnt++;
                                if (cnt == 5) {
                                    cnt = 0;
                                    down();
                                    if(notGoodAnymore()) {
                                        return;
                                    }
                                }
                            }
                            else {
                                if (notGoodAnymore()) {
                                    return;
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
//                SDL_SetRenderDrawColor(renderer, BLACK_COLOR.r, BLACK_COLOR.g, BLACK_COLOR.b, BLACK_COLOR.a);
//                SDL_RenderClear(renderer);
//                if (b) {
//
//
//                    for (int i = 0; i < 12; i++) {
//                        for (int j = 0; j < board[i].size(); j++) {
//                            if (board[i][j].getVisible()) {
//                                board[i][j].draw(renderer, 1);
//                            }
////                            board[i][j].draw(renderer, board[i][j].getVisible());
//                        }
//                    }
//                                   }
//                else {
//
//                    for (int i = 0; i < 12; i++) {
//                        for (int j = 0; j < board[i].size(); j++) {
//                            if (board[i][j].getVisible()) {
//                                board[i][j].draw(renderer, 1);
//                            }
////                            board[i][j].draw(renderer, board[i][j].getVisible());
//                        }
//                    }
//                }
//                shooter.draw(renderer, shooter.getVisible());
//                SDL_SetRenderDrawColor(renderer, YELLOW_COLOR.r, YELLOW_COLOR.g, YELLOW_COLOR.b, YELLOW_COLOR.a);
//                SDL_RenderDrawRect(renderer, &gmrect);
//                SDL_RenderDrawLine(renderer, GAME_UPLEFT_X, board[GAME_ROW - 1][0].getY() + ROW_DISTANCE / 2 + 5, GAME_DOWNRIGHT_X, board[GAME_ROW - 1][0].getY() + ROW_DISTANCE / 2 + 5);
                SDL_RenderPresent(renderer);
                SDL_Delay(5);
            }
//            shooter = Egg(static_cast<Egg::Type>(rng() % 3), EGG_TO_SHOOT_X, EGG_TO_SHOOT_Y);
//            shooter.draw(renderer, shooter.getVisible());
        }
    }
}

