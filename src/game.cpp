#include "../include/game.hpp"

Game::Game() : window(nullptr), renderer(nullptr), player(nullptr), isRunning(true) {}

Game::~Game() {
    cleanup();
}

bool Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    
    window = SDL_CreateWindow("Gallery of Lies", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1600, 900, SDL_WINDOW_SHOWN);
    
    if (!window) {
        SDL_Log("SDL could not create window! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_Log("SDL could not create renderer! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    
    player = new Player("Louis", "V", "Inspect the crime scene", 400, 300, NULL, renderer);
    NPC* npc = new NPC("John", "Plas", 
        "you are the killer you can lie to the player to make him think you are innocent. "
        "you killed the victim with a knife. You did it at 2AM it is now 4AM and you didn't leaved the museum (scene of crime)", 
        800, 300, NULL, renderer);
    npcs["John"] = npc;
     
    SDL_Texture* playerTexture = IMG_LoadTexture(renderer, "assets/player.png");
    SDL_Texture* npcTexture = IMG_LoadTexture(renderer, "assets/npc1.png");
    if (!playerTexture) {
        SDL_Log("SDL could not load player texture! SDL_Error: %s\n", IMG_GetError());
        return false;
    }
    player->setTexture(playerTexture);
    npc->setTexture(npcTexture);
    return true;
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
        }
        player->handleEvent(event);
    }
}

void Game::update() {
    player->move();
}

void Game::render() {
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);
    player->renderSprite();
    for (const auto& [name, npc] : npcs) {
        npc->renderSprite();
    }
    SDL_RenderPresent(renderer);
}

void Game::gameLoop() {
    while (isRunning) {
        handleEvents();
        update();
        render();
        if (isPlayerNearNPC(100.0f)) {
            std::string npcName = getNearestNPCName(100.0f);
            std::cout << "Player is near " << npcName << std::endl;
        }
    }
}

void Game::cleanup() {
    if (player) {
        SDL_Texture* playerTexture = player->getTexture();
        if (playerTexture) {
            SDL_DestroyTexture(playerTexture);
        }
        delete player;
        player = nullptr;
    }
    
    for (auto& [name, npc] : npcs) {
        SDL_Texture* npcTexture = npc->getTexture();
        if (npcTexture) {
            SDL_DestroyTexture(npcTexture);
        }
        delete npc;
    }
    npcs.clear();
    
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    
    SDL_Quit();
}

bool Game::running() const {
    return isRunning;
}

bool Game::isPlayerNearNPC(float detectionRadius) {
    for (const auto& [npcName, npc] : npcs) {
    float dx = player->getPositionX() - npc->getPositionX();
        float dy = player->getPositionY() - npc->getPositionY();
        float distance = std::sqrt(dx * dx + dy * dy);
        std::cout << "Distance to " << npcName << ": " << distance << std::endl;
        if (distance <= detectionRadius) {
            return true;
        }
    }
    return false;
}

std::string Game::getNearestNPCName(float detectionRadius) {
    for (const auto& [npcName, npc] : npcs) {
        float dx = player->getPositionX() - npc->getPositionX();
        float dy = player->getPositionY() - npc->getPositionY();
        float distance = std::sqrt(dx * dx + dy * dy);
        if (distance <= detectionRadius) {
            return npcName;
        }
    }
    return "";
}