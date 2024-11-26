#ifndef GAME_HPP
#define GAME_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "player.hpp"
#include "npc.hpp"
#include <map>
#include <string>

class Game {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* backgroundTexture;
    Player* player;
    std::map<std::string, NPC*> npcs;
    bool isRunning;
    Mix_Music* backgroundMusic;

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
    bool isPlayerNearNPC(float detectionRadius = 100.0f);
    std::string getNearestNPCName(float detectionRadius = 100.0f);
    void startInteraction(std::string npcName);

private:
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
};

#endif