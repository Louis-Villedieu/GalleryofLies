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
    std::cout << "Rendering player sprite" << std::endl;
    std::cout << "Current frame: " << currentFrame << std::endl;
    std::cout << "Current row: " << currentRow << std::endl;
    std::cout << "Frame width: " << frameWidth << std::endl;
    std::cout << "Frame height: " << frameHeight << std::endl; 
    SDL_Rect srcRect = {currentFrame * frameWidth, currentRow * frameHeight, frameWidth, frameHeight};
    SDL_Rect destRect = {positionX, positionY, frameWidth, frameHeight};
    SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
}

void Player::move() {
    positionX += dx;
    positionY += dy;
    std::cout << "Player moved to (" << positionX << ", " << positionY << ")" << std::endl;
}

void Player::handleEvent(SDL_Event& event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_UP:
                dx = 0;
                dy = -speed;
                isMoving = true;
                break;
            case SDLK_DOWN:
                dx = 0;
                dy = speed;
                isMoving = true;
                break;
            case SDLK_LEFT:
                dx = -speed;
                dy = 0;
                isMoving = true;
                break;
            case SDLK_RIGHT:
                dx = speed;
                dy = 0;
                isMoving = true;
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