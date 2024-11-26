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
    int currentTime = SDL_GetTicks();
    if (currentTime - lastMoveTime > 5) {
    positionX += dx;
        positionY += dy;
        lastMoveTime = currentTime;
    }
    if (currentTime - lastUpdateTime > animationSpeed) {
        currentFrame++;
        lastUpdateTime = currentTime;
    }
    if (currentFrame >= 3)
        currentFrame = 0;
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