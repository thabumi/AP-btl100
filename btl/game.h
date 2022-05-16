#ifndef GAME_HPP_
#define GAME_HPP_

class Game {
public:
    Game();
    ~Game();

private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

};

#endif // GAME_HPP
