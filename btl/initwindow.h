#ifndef INITWINDOW_H_
#define INITWINDOW_H_

#include <iostream>
#include <SDL.h>
#include "someconst.h"

using namespace std;

void logSDLError(std::ostream& os,
                 const std::string &msg, bool fatal = false);

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer);

void quitSDL(SDL_Window* window, SDL_Renderer* renderer);

void waitUntilKeyPressed();
#endif // INITWINDOW_H_
