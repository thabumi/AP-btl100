#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <random>
#include <chrono>
#include "egg.h"
#include "good.h"
#include "someconst.h"
#include "drawcircle.h"


using namespace std;

Egg::Egg() {

}

Egg::Egg(Type type, double x, double y, bool visible) {
    type_ = type;
    x_ = x;
    y_ = y;
    visible_ = visible;
}

void Egg::draw(SDL_Renderer* renderer, bool visible_) {
    if (!visible_) {
        return;
    }
    if (type_ == bomb) {
        SDL_SetRenderDrawColor(renderer, GRAY_COLOR.r, GRAY_COLOR.g, GRAY_COLOR.b, GRAY_COLOR.a);
        SDL_RenderFillCircle(renderer, x_, y_, EGG_RADIUS);
        TTF_Font* myFont = TTF_OpenFont("futureforces.ttf", 15);

        SDL_Texture* bombTexture = loadText(myFont, renderer, "TNT", RED_COLOR);
        SDL_Rect bombTextureRect;

        SDL_QueryTexture(bombTexture, NULL, NULL, &bombTextureRect.w, &bombTextureRect.h);
        bombTextureRect.x = x_ - bombTextureRect.w / 2;
        bombTextureRect.y = y_ - bombTextureRect.h / 2;
        SDL_RenderCopy(renderer, bombTexture, NULL, &bombTextureRect);
        TTF_CloseFont(myFont);
        SDL_DestroyTexture(bombTexture);
        return;
    }
    switch (type_) {
        case red:
            SDL_SetRenderDrawColor(renderer, RED_COLOR.r, RED_COLOR.g, RED_COLOR.b, RED_COLOR.a);
            break;
        case green:
            SDL_SetRenderDrawColor(renderer, GREEN_COLOR.r, GREEN_COLOR.g, GREEN_COLOR.b, GREEN_COLOR.a);
            break;
        case blue:
            SDL_SetRenderDrawColor(renderer, BLUE_COLOR.r, BLUE_COLOR.g, BLUE_COLOR.b, BLUE_COLOR.a);
            break;
        case purple:
            SDL_SetRenderDrawColor(renderer, PURPLE_COLOR.r, PURPLE_COLOR.g, PURPLE_COLOR.b, PURPLE_COLOR.a);
            break;
        case orange:
            SDL_SetRenderDrawColor(renderer, ORANGE_COLOR.r, ORANGE_COLOR.g, ORANGE_COLOR.b, ORANGE_COLOR.a);
            break;
        case bomb:
            break;
    }
    SDL_RenderFillCircle(renderer, x_, y_, EGG_RADIUS);
}

void Egg::moves(double dx, double dy) {
    x_ += dx;
    y_ += dy;
};

bool Egg::collision(Egg& other) {
    if ((x_ - other.x_) * (x_ - other.x_) + (y_ - other.y_) * (y_ - other.y_) - 4.0 * EGG_RADIUS * EGG_RADIUS < eps) {
        return true;
    }
    return false;
};

bool Egg::toTheEnd() {
    return (y_ < eps);
}

void Egg::setX(double x) {
    x_ = x;
}
void Egg::setY(double y) {
    y_ = y;
}
void Egg::setLocation(double x, double y) {
    x_ = x;
    y_ = y;
}
void Egg::setType(Type t) {
    type_ = t;
}
double Egg::getX() {
    return x_;
}
double Egg::getY() {
    return y_;
}
pair<double, double> Egg::getLocation() {
    return {x_, y_};
}
int Egg::getType() {
    return (int)type_;
}
void Egg::setVisible(bool b) {
    visible_ = b;
}

bool Egg::getVisible() {
    return visible_;
}
