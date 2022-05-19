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
pair<double, double> standardize(pair<double, double> p) {
    double len = sqrt(p.first * p.first + p.second * p.second);
    return {p.first * 5 / len, p.second * 5 / len};
}
void testt(SDL_Window* window, SDL_Renderer* renderer) {
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

    vector<vector<Egg>> board(12);
    SDL_Rect gmrect;
    gmrect.x = GAME_UPLEFT_X;
    gmrect.y = GAME_UPLEFT_Y;
    gmrect.w = GAME_WIDTH;
    gmrect.h = GAME_HEIGHT;
    SDL_SetRenderDrawColor(renderer, YELLOW_COLOR.r, YELLOW_COLOR.g, YELLOW_COLOR.b, YELLOW_COLOR.a);
    SDL_RenderDrawRect(renderer, &gmrect);
    //Collision: x+=stepX y+=stepY

    for (int i = 0; i < 12; i++) {
        board[i].resize(9 - i % 2);
        for (int j = 0; j < 8 + 1 - i % 2; j++) {
            if (i % 2 == 0) {
                board[i][j] = Egg(static_cast<Egg::Type>(rng() % 3), GAME_UPLEFT_X + EGG_RADIUS + 2 * j * EGG_RADIUS, EGG_RADIUS + 2 * i * ROW_DISTANCE);
            }
            else {
                board[i][j] = Egg(static_cast<Egg::Type>(rng() % 3), GAME_UPLEFT_X + EGG_RADIUS * 2 + 2 * j * EGG_RADIUS, EGG_RADIUS + 2 * i * ROW_DISTANCE);
            }
        }
    }
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < board[i].size(); j++) {
            board[i][j].draw(renderer, board[i][j].getVisible());
        }
    }
    SDL_RenderPresent(renderer);

    SDL_Event e;
    while (true) {
        // Đợi 10 mili giây
        SDL_Delay(10);

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
            Egg test1(static_cast<Egg::Type>(0), EGG_TO_SHOOT_X, EGG_TO_SHOOT_Y);
            bool b = false;
            for (int i = 0; i < 12; i++) {
                for (int j = 0; j < board[i].size(); j++) {
                    if (test1.collision(board[i][j]) && board[i][j].getVisible()) {
                        board[i][j] = test1;
                        b = true;
                    }
                }
            }
            while (!b) {
                test1.moves(stepX, stepY);
                if (test1.getX() + EGG_RADIUS >= GAME_DOWNRIGHT_X || test1.getX() - EGG_RADIUS <= GAME_UPLEFT_X) {
                    test1.moves(-stepX, -stepY);
                    stepX = -stepX;
                    test1.moves(stepX, stepY);
                }
                if (test1.getY() + EGG_RADIUS >= GAME_DOWNRIGHT_Y || test1.getY() - EGG_RADIUS <= GAME_UPLEFT_Y) {
                    test1.moves(-stepX, -stepY);
                    stepY = -stepY;
                    test1.moves(stepX, stepY);
                }

                for (int i = 0; i < 12; i++) {
                    for (int j = 0; j < board[i].size(); j++) {
                        if (test1.collision(board[i][j])) {
                            board[i][j].setType(static_cast<Egg::Type>(test1.getType()));
                            b = true;
                            break;
                        }
                    }
                }
                if (b) {
                    SDL_SetRenderDrawColor(renderer, BLACK_COLOR.r, BLACK_COLOR.g, BLACK_COLOR.b, BLACK_COLOR.a);
                    SDL_RenderClear(renderer);
                    SDL_SetRenderDrawColor(renderer, YELLOW_COLOR.r, YELLOW_COLOR.g, YELLOW_COLOR.b, YELLOW_COLOR.a);
                    SDL_RenderDrawRect(renderer, &gmrect);
                    for (int i = 0; i < 12; i++) {
                        for (int j = 0; j < board[i].size(); j++) {
                            board[i][j].draw(renderer, 0);
                        }
                    }
                }
                else {
                    SDL_SetRenderDrawColor(renderer, BLACK_COLOR.r, BLACK_COLOR.g, BLACK_COLOR.b, BLACK_COLOR.a);
                    SDL_RenderClear(renderer);
                    SDL_SetRenderDrawColor(renderer, YELLOW_COLOR.r, YELLOW_COLOR.g, YELLOW_COLOR.b, YELLOW_COLOR.a);
                    SDL_RenderDrawRect(renderer, &gmrect);
                    for (int i = 0; i < 12; i++) {
                        for (int j = 0; j < board[i].size(); j++) {
                            board[i][j].draw(renderer, 0);
                        }
                    }
                    test1.draw(renderer, 0);
                }
                SDL_RenderPresent(renderer);
                SDL_Delay(10);
            }
        }


    }

//    SDL_RenderPresent(renderer);
//    return;
//    Egg test1(static_cast<Egg::Type>(0), EGG_TO_SHOOT_X, EGG_TO_SHOOT_Y);
////    return;
////    vector<vector<Egg>> board(12, vector<Egg>(9));
////    test1.draw(renderer, 0);
////    SDL_RenderPresent(renderer);
////    return;
//
////    Egg test1(static_cast<Egg::Type>(0), 20.5, 0);
//    auto beg = chrono::high_resolution_clock::now();
//    Egg test2(static_cast<Egg::Type>(1), GAME_UPLEFT_X + GAME_WIDTH / 2, EGG_RADIUS);
//    double stepX = 5;
//    double stepY = 5;
//    while (!test1.collision(test2)) {
//        auto en = chrono::high_resolution_clock::now();
////        if (std::chrono::duration_cast<std::chrono::duration<double>>(en - beg).count() > 10) {
////            break;
////        }
////        if (test1.getX() > game_)
//        test1.draw(renderer, 1);
//        test1.moves(stepX, stepY);
//        if (test1.getX() + EGG_RADIUS >= GAME_DOWNRIGHT_X || test1.getX() - EGG_RADIUS <= GAME_UPLEFT_X) {
//            test1.moves(-stepX, -stepY);
//            stepX = -stepX;
//            test1.moves(stepX, stepY);
//        }
//        if (test1.getY() + EGG_RADIUS >= GAME_DOWNRIGHT_Y || test1.getY() - EGG_RADIUS <= GAME_UPLEFT_Y) {
//            test1.moves(-stepX, -stepY);
//            stepY = -stepY;
//            test1.moves(stepX, stepY);
//        }
//        test1.draw(renderer, 0);
//        SDL_SetRenderDrawColor(renderer, YELLOW_COLOR.r, YELLOW_COLOR.g, YELLOW_COLOR.b, YELLOW_COLOR.a);
//        SDL_RenderDrawRect(renderer, &gmrect);
//        test2.draw(renderer, 0);
//        SDL_RenderPresent(renderer);
//        SDL_Delay(10);
//    }
}
