#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include "initwindow.h"
#include "someconst.h"
#include "game.h"
#include "good.h"
#include "Button.h"
Game::Game() {
    initSDL(window_, renderer_);
    initTTF();
    getHighscore();
}
Game::~Game() {
//    waitUntilKeyPressed();
    quitSDL(window_, renderer_);
    quitTTF();
}
//
int Game::run() {
//    good_.draw(renderer_);
//    SDL_RenderPresent(renderer_);
    return good_.run(window_, renderer_);
}

int Game::home() {
    TTF_Font* myFont = TTF_OpenFont("futureforces.ttf", 40);

    SDL_Texture* playButton = loadText(myFont, renderer_, "PLAY", YELLOW_COLOR);

    SDL_Rect playButtonRect;

    SDL_QueryTexture(playButton, NULL, NULL, &playButtonRect.w, &playButtonRect.h);
    playButtonRect.x = (SCREEN_WIDTH - playButtonRect.w) / 2;
    playButtonRect.y = (SCREEN_HEIGHT - playButtonRect.h) / 2;


    SDL_Rect playButtonOutline;
    playButtonOutline.w = MENU_PLAY_BUTTON_WIDTH;
    playButtonOutline.h = MENU_PLAY_BUTTON_HEIGHT;
    playButtonOutline.x = playButtonRect.x - (playButtonOutline.w - playButtonRect.w) / 2;
    playButtonOutline.y = playButtonRect.y - (playButtonOutline.h - playButtonRect.h) / 2;

    SDL_Texture* continueButton = loadText(myFont, renderer_, "CONTINUE", YELLOW_COLOR);

    SDL_Rect continueButtonRect;

    SDL_QueryTexture(continueButton, NULL, NULL, &continueButtonRect.w, &continueButtonRect.h);
    continueButtonRect.x = (SCREEN_WIDTH - continueButtonRect.w) / 2;
    continueButtonRect.y = (SCREEN_HEIGHT - continueButtonRect.h + 120) / 2;


    SDL_Rect continueButtonOutline;
    continueButtonOutline.w = MENU_CONTINUE_BUTTON_WIDTH;
    continueButtonOutline.h = MENU_CONTINUE_BUTTON_HEIGHT;
    continueButtonOutline.x = continueButtonRect.x - (continueButtonOutline.w - continueButtonRect.w) / 2;
    continueButtonOutline.y = continueButtonRect.y - (continueButtonOutline.h - continueButtonRect.h) / 2;
    SDL_SetRenderDrawColor(renderer_, BLACK_COLOR.r, BLACK_COLOR.g, BLACK_COLOR.b, BLACK_COLOR.a);
    SDL_RenderClear(renderer_);


    SDL_RenderCopy(renderer_, playButton, NULL, &playButtonRect);
    SDL_RenderCopy(renderer_, continueButton, NULL, &continueButtonRect);
    SDL_SetRenderDrawColor(renderer_, YELLOW_COLOR.r, YELLOW_COLOR.g, YELLOW_COLOR.b, YELLOW_COLOR.a);
    SDL_RenderDrawRect(renderer_, &playButtonOutline);
    SDL_RenderDrawRect(renderer_, &continueButtonOutline);
    SDL_RenderPresent(renderer_);
    TTF_CloseFont(myFont);
    SDL_DestroyTexture(playButton);
    SDL_DestroyTexture(continueButton);

    SDL_Event e;
    while (true) {
        SDL_Delay(5);

        if ( SDL_PollEvent(&e) == 0) continue;

        if (e.type == SDL_QUIT) return 0;

        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) return 0;

        int x, y;

        if (e.type == SDL_MOUSEBUTTONDOWN) {
            x = e.button.x; // Lấy hoành độ x của chuột
            y = e.button.y; // Lấy tung độ y của chuột

            if (inButton(playButtonOutline, x, y)) {
                return 2;
            }
            if (inButton(continueButtonOutline, x, y)) {
                return 1;
            }
        }
    }
}

void Game::load() {
    good_.loadData();
}

void Game::reset() {
    good_.resetData();
}

void Game::save() {
    good_.saveData();
}
void Game::getHighscore() {
    good_.getHighscore();
}
