#include <iostream>
#include "game.h"

/*
always return 0 with quit
                            quit
                             ^
                             |
    load(1)                  |            delay
home-------->(run(1)<--->(1)pause)->end(1)----->home
  | reset(2)   |
  |            |save
  |            |
  v            v
quit          quit
*/
int main(int argc, char* argv[]) {
    Game game;
    while (true) {
        int type = game.home();
        if (type == 1) {
            game.load();
            if (game.run() == 1) {
                game.reset();
            }
            else {
                game.save();
                game.~Game();
                break;
            }
        }
        else if (type == 2) {
            game.reset();
            game.load();
            if (game.run() == 1) {
                game.reset();
            }
            else {
                game.save();
                game.~Game();
                break;
            }
        }
        else {
            game.~Game();
            break;
        }
    }
    return 0;
}
