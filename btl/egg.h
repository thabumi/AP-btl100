#ifndef EGG_H_
#define EGG_H_

#include <iostream>
#include <SDL.h>

using namespace std;

class Egg {
public:
    enum Type {red, green, blue};
    Egg();
    Egg(Type type, double x, double y, bool visible_ = 1);
    void draw(SDL_Renderer* renderer, bool visible_);
    void moves(double dx, double dy);
    bool collision(Egg& other);
    bool toTheEnd();



    void setX(double x);
    void setY(double y);
    void setLocation(double x, double y);
    void setType(Type t);

    void setVisible(bool b);

    double getX();
    double getY();
    pair<double, double> getLocation();
    int getType();
    bool getVisible();

private:
    bool visible_;
    double x_;
    double y_;
    Type type_;
};

#endif // EGG_H_




