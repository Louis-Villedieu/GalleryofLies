#ifndef GAME_HPP
#define GAME_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "player.hpp"
#include <map>
#include <string>

class Game {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    Player* player;
    std::map<std::string, Character*> characters;
    bool isRunning;

public:
    Game();
    ~Game();

    bool init();
    void handleEvents();
    void update();
    void render();
    void gameLoop();
    void cleanup();
    bool running() const;

private:
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
};

#endif