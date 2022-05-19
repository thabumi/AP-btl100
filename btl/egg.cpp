#include "egg.h"
#include "someconst.h"
#include "drawcircle.h"
#include <iostream>
#include <SDL.h>

using namespace std;

Egg::Egg() {

}

Egg::Egg(Type type, double x, double y, bool visible) {
    type_ = type;
    x_ = x;
    y_ = y;
    visible_ = visible;
}

void Egg::draw(SDL_Renderer* renderer, bool clear_) {
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
    }
    if (clear_) {
        SDL_SetRenderDrawColor(renderer, BLACK_COLOR.r, BLACK_COLOR.g, BLACK_COLOR.b, BLACK_COLOR.a);
    }
//    SDL_RenderDrawCircle(renderer, x_, y_, EGG_RADIUS);
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
