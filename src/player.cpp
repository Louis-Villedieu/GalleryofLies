#include "../include/player.hpp"

Player::Player(std::string firstName, std::string lastName, std::string aiOrder, int positionX, int positionY, SDL_Texture* texture, SDL_Renderer* renderer) : Character(firstName, lastName, aiOrder, positionX, positionY, texture, renderer) {
    speed = 1;
    isMoving = false;
    dx = 0;
    dy = 0;
    currentFrame = 0;
    currentRow = 0;
    frameWidth = 576 / 3;
    frameHeight = 768 / 4;
}

Player::~Player() {
    std::cout << "Player destroyed" << std::endl;
}

void Player::renderSprite() {
    SDL_Rect srcRect = {currentFrame * frameWidth, currentRow * frameHeight, frameWidth, frameHeight};
    SDL_Rect destRect = {positionX, positionY, frameWidth / 3, frameHeight / 3};
    SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
}

void Player::move() {
    if (!isMoving)
        return;
    positionX += dx;
    positionY += dy;
    int currentTime = SDL_GetTicks();
    if (currentTime - lastUpdateTime > animationSpeed) {
        currentFrame++;
        lastUpdateTime = currentTime;
    }
    if (currentFrame >= 3)
        currentFrame = 0;
    std::cout << "Player moved to (" << positionX << ", " << positionY << ")" << std::endl;
}

void Player::handleEvent(SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_UP:
                dx = 0;
                dy = -speed;
                isMoving = true;
                currentRow = 3;
                break;
            case SDLK_DOWN:
                dx = 0;
                dy = speed;
                isMoving = true;
                currentRow = 0;
                break;
            case SDLK_LEFT:
                dx = -speed;
                dy = 0;
                isMoving = true;
                currentRow = 1;
                break;
            case SDLK_RIGHT:
                dx = speed;
                dy = 0;
                isMoving = true;
                currentRow = 2;
                break;
        }
    }
    else if (event.type == SDL_KEYUP) {
        switch (event.key.keysym.sym) {
            case SDLK_UP:
            case SDLK_DOWN:
            case SDLK_LEFT:
            case SDLK_RIGHT:
                dx = 0;
                dy = 0;
                isMoving = false;
                break;
        }
    }
}

void Player::setTexture(SDL_Texture* texture) {
    this->texture = texture;
}