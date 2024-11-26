#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../include/game.hpp"

int main(int argc, char* argv[]) {
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        SDL_Log("Erreur d'initialisation de SDL_image : %s", IMG_GetError());
        return 1;
    }

    Game game;
    if (!game.init()) {
        SDL_Log("Erreur d'initialisation du jeu");
        IMG_Quit();
        return 1;
    }

    game.gameLoop();

    IMG_Quit();
    
    return 0;
}