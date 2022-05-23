#ifndef INITWINDOW_H_
#define INITWINDOW_H_

#include <iostream>
#include <SDL.h>

using namespace std;

void logSDLError(ostream& os,
                 const string &msg, bool fatal = false);

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer);

void quitSDL(SDL_Window* window, SDL_Renderer* renderer);

void waitUntilKeyPressed();

void initTTF();

void quitTTF();
#endif // INITWINDOW_H_
