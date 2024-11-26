#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "../include/player.hpp"

SDL_Window* window;
SDL_Renderer* renderer;

void init() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Gallery of Lies", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1600, 900, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void gameLoop() {
    Player player("Louis", "V", "Inspect the crime scene", 400, 300, NULL, renderer);
    SDL_Texture* playerTexture = IMG_LoadTexture(renderer, "assets/player.png");
    player.setTexture(playerTexture);
    while (true) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return;
            }
            player.handleEvent(event);
        }
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);
        player.renderSprite();
        player.move();
        
        SDL_RenderPresent(renderer);
    }
}

void free() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char** argv) {
    init();
    gameLoop();
    free();
    return 0;
}