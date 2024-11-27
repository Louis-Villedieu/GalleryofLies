#include "../include/player.hpp"

Player::Player(std::string firstName, std::string lastName, std::string aiOrder, int positionX, int positionY, SDL_Texture* texture, SDL_Renderer* renderer, SDL_Color color) : Character(firstName, lastName, aiOrder, positionX, positionY, texture, renderer, color) {
    speed = 1;
    isMoving = false;
    dx = 0;
    dy = 0;
    currentFrame = 0;
    currentRow = 0;
    frameWidth = 576 / 3;
    frameHeight = 768 / 4;
    walkSound = nullptr;
    loadWalkSound();
}

Player::~Player() {
    if (walkSound) {
        Mix_FreeChunk(walkSound);
        walkSound = nullptr;
    }
}

void Player::renderSprite() {
    SDL_Rect srcRect = {currentFrame * frameWidth, currentRow * frameHeight, frameWidth, frameHeight};
    SDL_Rect destRect = {positionX, positionY, frameWidth / 3, frameHeight / 3};
    SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
}

void Player::move() {
    if (!isMoving) return;
    
    static const float MOVEMENT_SPEED = 3.50f;
    
    positionX += dx * MOVEMENT_SPEED;
    positionY += dy * MOVEMENT_SPEED;
    
    int currentTime = SDL_GetTicks();
    if (currentTime - lastUpdateTime > animationSpeed) {
        currentFrame = (currentFrame + 1) % 3;
        lastUpdateTime = currentTime;
    }
    
    if (walkSound && (dx != 0 || dy != 0) && Mix_Playing(-1) == 0) {
        Mix_PlayChannel(-1, walkSound, 0);
    }
}

void Player::handleEvent(SDL_Event& event) {
    static const struct {
        SDL_Keycode key;
        int dx;
        int dy;
        int row;
    } moveMap[] = {
        {SDLK_UP,    0, -1, 3},
        {SDLK_DOWN,  0,  1, 0},
        {SDLK_LEFT, -1,  0, 1},
        {SDLK_RIGHT, 1,  0, 2}
    };

    if (event.type == SDL_KEYDOWN) {
        for (const auto& move : moveMap) {
            if (event.key.keysym.sym == move.key) {
                dx = move.dx * speed;
                dy = move.dy * speed;
                currentRow = move.row;
                isMoving = true;
                return;
            }
        }
    }
    else if (event.type == SDL_KEYUP) {
        for (const auto& move : moveMap) {
            if (event.key.keysym.sym == move.key) {
                dx = 0;
                dy = 0;
                isMoving = false;
                return;
            }
        }
    }
}

void Player::setTexture(SDL_Texture* texture) {
    this->texture = texture;
}

void Player::setIsMoving(bool isMoving) {
    this->isMoving = isMoving;
}

void Player::loadWalkSound() {
    const char* soundPath = "./assets/walk.wav";
    walkSound = Mix_LoadWAV(soundPath);
    if (!walkSound) {
        std::cout << "Failed to load walk sound! SDL_mixer Error: " << Mix_GetError() << std::endl;
    } else {
        Mix_VolumeChunk(walkSound, 20);
        std::cout << "Walk sound loaded successfully!" << std::endl;
    }
}