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

#pragma once
using namespace std;

//uet
int main(int argc, char* argv[])
{
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    SDL_Window* window;
    SDL_Renderer* renderer;
    initSDL(window, renderer);
    // main code


    //testing code

    // Your drawing code here

//    SDL_RenderClear(renderer);
//    SDL_Rect square1 {360, 0, 250, 720};
//    SDL_SetRenderDrawColor(renderer, 252, 169, 68, 0 );
//    SDL_RenderFillRect(renderer, &square1);
//
//    SDL_Rect square2 {360 + 5, 0, 250 - 20, 720 };
//    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0 );
//    SDL_RenderFillRect(renderer, &square2);
//
//    SDL_Rect square3 {360, 150, 360, 5};
//    SDL_SetRenderDrawColor(renderer, 252, 169, 68, 0);
//    SDL_RenderFillRect(renderer, &square3);

//    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);   // white
//    SDL_RenderDrawPoint(renderer, SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
//    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);   // red
//    SDL_RenderDrawLine(renderer, 0, 0, 200, 200);
//    SDL_Rect filled_rect;
//    filled_rect.x = SCREEN_WIDTH - 400;
//    filled_rect.y = SCREEN_HEIGHT - 150;
//    filled_rect.w = 320;
//    filled_rect.h = 100;
//    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // green
//    SDL_RenderFillRect(renderer, &filled_rect);
//    SDL_Color c = ORANGE_COLOR;
//    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
//    SDL_RenderDrawLine(renderer, 150, 150, 200, 200);
//
//
//
//    Egg test1(static_cast<Egg::Type>(0), 20.5, 0);
//    Egg test2(static_cast<Egg::Type>(2), 60.6, 0);
//    test1.draw(renderer, false);
//    test2.draw(renderer, false);
//    if (test1.collision(test2)) {
//        SDL_SetRenderDrawColor(renderer, PURPLE_COLOR.r, PURPLE_COLOR.g, PURPLE_COLOR.b, PURPLE_COLOR.a);
//    }
//    if (test1.toTheEnd()) {
//        SDL_SetRenderDrawColor(renderer, CYAN_COLOR.r, CYAN_COLOR.g, CYAN_COLOR.b, CYAN_COLOR.a);
//    }
//    SDL_RenderFillCircle(renderer, EGG_TO_SHOOT_X, EGG_TO_SHOOT_Y, EGG_RADIUS);
//    SDL_Delay(3000);

    testt(window, renderer);
//    SDL_RenderPresent(renderer);

    waitUntilKeyPressed();
    quitSDL(window, renderer);
    return 0;
}

/*
// xóa màn hình
int SDL_RenderClear(SDL_Renderer* renderer)
 // đặt màu vẽ r: red, g: green, b: blue, a: alpha opaque (255: mầu đặc nhất)
int SDL_SetRenderDrawColor(SDL_Renderer* renderer, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
// vẽ điểm
int SDL_RenderDrawPoint(SDL_Renderer* renderer, int x, int y)
	// vẽ đoạn thẳng
int SDL_RenderDrawLine(SDL_Renderer* renderer, int x1, int y1, int x2, int y2)
	// vẽ hình chữ nhật rỗng
int SDL_RenderDrawRect(SDL_Renderer* renderer, const SDL_Rect* rect)
	// vẽ hình chữ nhật đặc
int SDL_RenderFillRect(SDL_Renderer* renderer, const SDL_Rect* rect)
*/


