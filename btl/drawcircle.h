#ifndef DRAWCIRCLE_H_
#define DRAWCIRCLE_H_

#include <iostream>
#include <SDL.h>

int
SDL_RenderDrawCircle(SDL_Renderer * renderer, int x, int y, int radius);
int
SDL_RenderFillCircle(SDL_Renderer * renderer, int x, int y, int radius);

#endif // DRAWCIRCLE_H_
