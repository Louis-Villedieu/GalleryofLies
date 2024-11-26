#include "../include/game.hpp"
#include <cmath>
#include <sstream>

Game::Game() : window(nullptr), renderer(nullptr), backgroundTexture(nullptr), player(nullptr), isRunning(true), backgroundMusic(nullptr) {}

Game::~Game() {
    cleanup();
    Mix_CloseAudio();
    Mix_Quit();
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

    backgroundTexture = IMG_LoadTexture(renderer, "assets/background.png");
    if (!backgroundTexture) {
        SDL_Log("Failed to load background texture! SDL_Error: %s\n", IMG_GetError());
        return false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }
    std::cout << "SDL_mixer initialized successfully!" << std::endl;

    backgroundMusic = Mix_LoadMUS("assets/musicloop.mp3");
    if (!backgroundMusic) {
        std::cout << "Failed to load background music! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }
    
    Mix_PlayMusic(backgroundMusic, -1);
    Mix_VolumeMusic(10);

    player = new Player("Louis", "V", "Inspect the crime scene", 400, 300, NULL, renderer, {255, 255, 255, 255});
    NPC* John = new NPC("John", "Plas", "assets/NPC/John/john.gol", 800, 300, NULL, renderer, {100, 100, 255, 255});
    NPC* Laura = new NPC("Laura", "Vardi", "assets/NPC/Laura/laura.gol",100, 500, NULL, renderer, {240, 100, 90, 255});
    npcs["John"] = John;
    npcs["Laura"] = Laura;
     
    SDL_Texture* playerTexture = IMG_LoadTexture(renderer, "assets/player.png");
    SDL_Texture* npcTexture = IMG_LoadTexture(renderer, "assets/NPC/John/npc1.png");
    SDL_Texture* LauraTexture = IMG_LoadTexture(renderer, "assets/NPC/Laura/laura.png");
    if (!playerTexture) {
        SDL_Log("SDL could not load player texture! SDL_Error: %s\n", IMG_GetError());
        return false;
    }
    player->setTexture(playerTexture);
    John->setTexture(npcTexture);
    Laura->setTexture(LauraTexture);

    return true;
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
        }
        else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
            if (isPlayerNearNPC(100.0f)) {
                std::string nearestNPC = getNearestNPCName(100.0f);
                if (!nearestNPC.empty()) {
                    startInteraction(nearestNPC);
                }
            }
        }
        player->handleEvent(event);
    }
}

void Game::update() {
    player->move();
}

void Game::render() {
    SDL_RenderClear(renderer);
    
    SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    
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
    
    if (backgroundTexture) {
        SDL_DestroyTexture(backgroundTexture);
        backgroundTexture = nullptr;
    }
    
    if (backgroundMusic) {
        Mix_FreeMusic(backgroundMusic);
        backgroundMusic = nullptr;
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

void Game::startInteraction(std::string npcName) {
    player->setIsMoving(false);
    auto npcIt = npcs.find(npcName);

    if (npcIt == npcs.end()) {
        return;
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
    
    SDL_Rect dialogBox = {
        200,
        600,
        1200,
        200
    };
    
    SDL_RenderFillRect(renderer, &dialogBox);
    SDL_RenderPresent(renderer);

    SDL_Event event;
    bool waitingForInput = true;
    SDL_StartTextInput();
    
    std::string textInput;
    
    if (TTF_Init() < 0) {
        SDL_Log("TTF_Init: %s\n", TTF_GetError());
        return;
    }

    const int MAX_LINE_WIDTH = dialogBox.w;
    const int LINE_HEIGHT = 30;
    const int FONT_SIZE = 24;

    TTF_Font* font = TTF_OpenFont("./assets/Minecraft.ttf", FONT_SIZE);
    if (!font) {
        SDL_Log("Failed to load font: %s\n", TTF_GetError());
        return;
    }

    std::vector<SDL_Texture*> lineTextures;
    std::vector<std::string> lines;
    SDL_Color color = player->getColor();

    while (waitingForInput) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_TEXTINPUT) {
                textInput += event.text.text;
            }
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_BACKSPACE && !textInput.empty()) {
                    textInput.pop_back();
                }
                else if (event.key.keysym.sym == SDLK_RETURN) {
                    waitingForInput = false;
                    SDL_StopTextInput();
                }
            }
            if (event.type == SDL_QUIT) {
                isRunning = false;
                waitingForInput = false;
                SDL_StopTextInput();
            }

            if (event.type == SDL_TEXTINPUT || 
               (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_BACKSPACE)) {
                
                for (auto texture : lineTextures) {
                    SDL_DestroyTexture(texture);
                }
                lineTextures.clear();
                lines.clear();

                std::string currentLine;
                std::string word;
                std::istringstream iss(textInput);

                while (iss >> word) {
                    int wordWidth;
                    if (currentLine.empty()) {
                        TTF_SizeText(font, word.c_str(), &wordWidth, nullptr);
                    } else {
                        TTF_SizeText(font, (currentLine + " " + word).c_str(), &wordWidth, nullptr);
                    }
                    
                    if (wordWidth > MAX_LINE_WIDTH) {
                        if (!currentLine.empty()) {
                            lines.push_back(currentLine);
                        }
                        currentLine = word;
                    } else {
                        if (!currentLine.empty()) {
                            currentLine += " ";
                        }
                        currentLine += word;
                    }
                }
                if (!currentLine.empty()) {
                    lines.push_back(currentLine);
                }

                for (const auto& line : lines) {
                    SDL_Surface* tmp = TTF_RenderText_Solid(font, line.c_str(), color);
                    if (tmp) {
                        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, tmp);
                        lineTextures.push_back(tex);
                        SDL_FreeSurface(tmp);
                    }
                }
            }

            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
            
            player->renderSprite();
            for (const auto& [name, npc] : npcs) {
                npc->renderSprite();
            }

            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
            SDL_RenderFillRect(renderer, &dialogBox);

            for (size_t i = 0; i < lineTextures.size(); i++) {
                SDL_Rect lineRect = {
                    dialogBox.x + 20,
                    dialogBox.y + 20 + (int)(i * LINE_HEIGHT), 
                    0,
                    LINE_HEIGHT
                };
                
                SDL_QueryTexture(lineTextures[i], nullptr, nullptr, &lineRect.w, &lineRect.h);
                
                SDL_RenderCopy(renderer, lineTextures[i], nullptr, &lineRect);
            }

            SDL_RenderPresent(renderer);
        }
    }
    NPC* npc = npcIt->second;
    std::string answer = npc->thinkAndAnswer(textInput);
    npc->setPastConversation("The player asked you: " + textInput + " and you answered: " + answer);
    std::cout << answer << std::endl;

    std::vector<SDL_Texture*> answerTextures;
    std::vector<std::string> answerLines;
    
    std::string currentLine;
    std::string word;
    std::istringstream iss(answer);

    while (iss >> word) {
        int wordWidth;
        if (currentLine.empty()) {
            TTF_SizeText(font, word.c_str(), &wordWidth, nullptr);
        } else {
            TTF_SizeText(font, (currentLine + " " + word).c_str(), &wordWidth, nullptr);
        }
        
        if (wordWidth > MAX_LINE_WIDTH) {
            if (!currentLine.empty()) {
                answerLines.push_back(currentLine);
            }
            currentLine = word;
        } else {
            if (!currentLine.empty()) {
                currentLine += " ";
            }
            currentLine += word;
        }
    }
    if (!currentLine.empty()) {
        answerLines.push_back(currentLine);
    }

    SDL_Color npcTextColor = npc->getColor();

    for (const auto& line : answerLines) {
        SDL_Surface* tmp = TTF_RenderText_Solid(font, line.c_str(), npcTextColor);
        if (tmp) {
            SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, tmp);
            answerTextures.push_back(tex);
            SDL_FreeSurface(tmp);
        }
    }

    Uint32 startTime = SDL_GetTicks();
    bool showingAnswer = true;
    while (showingAnswer && SDL_GetTicks() - startTime < 20000) {
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
        
        player->renderSprite();
        for (const auto& [name, npc] : npcs) {
            npc->renderSprite();
        }

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
        SDL_RenderFillRect(renderer, &dialogBox);

        for (size_t i = 0; i < answerTextures.size(); i++) {
            SDL_Rect lineRect = {
                dialogBox.x + 20,
                dialogBox.y + 20 + (int)(i * LINE_HEIGHT), 
                0,
                LINE_HEIGHT
            };
            
            SDL_QueryTexture(answerTextures[i], nullptr, nullptr, &lineRect.w, &lineRect.h);
            SDL_RenderCopy(renderer, answerTextures[i], nullptr, &lineRect);
        }

        SDL_RenderPresent(renderer);

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                isRunning = false;
                showingAnswer = false;
            }
            else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN) {
                showingAnswer = false;
            }
        }
    }

    for (auto texture : answerTextures) {
        SDL_DestroyTexture(texture);
    }

    for (auto texture : lineTextures) {
        SDL_DestroyTexture(texture);
    }
    TTF_CloseFont(font);
    TTF_Quit();
}