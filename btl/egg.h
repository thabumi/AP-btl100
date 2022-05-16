#ifndef EGG_H_
#define EGG_H_

#include <iostream>
#include <SDL.h>

using namespace std;

class Egg {
public:
    enum Type {red, green, blue};
    Egg(Type type, double x, double y);
    void draw(SDL_Renderer* renderer);
    void moves(double dx, double dy);
    bool collision(Egg& other);
    bool toTheEnd();
//    vector<pair<double, double>> neighbour;
private:
    double x_;
    double y_;
    Type type_;
};

#endif // EGG_H_




