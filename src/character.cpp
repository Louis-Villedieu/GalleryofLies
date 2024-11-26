#include "../include/character.hpp"

Character::Character(std::string firstName, std::string lastName, std::string aiOrder, int positionX, int positionY, SDL_Texture* texture, SDL_Renderer* renderer) : firstName(firstName), lastName(lastName), aiOrder(aiOrder), positionX(positionX), positionY(positionY), texture(texture), renderer(renderer) {
    currentFrame = 0;
    currentRow = 0;
}