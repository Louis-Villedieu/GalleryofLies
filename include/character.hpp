#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <string>
#include <vector>
#include <SDL2/SDL.h>

class Character
{
    protected:
        std::string firstName;
        std::string lastName;
        std::string aiOrder;
        int positionX;
        int positionY;
        SDL_Texture* texture;
        SDL_Renderer* renderer;
        int currentFrame = 1;
        int currentRow = 0;
        int frameWidth = 576 / 3;  
        int frameHeight = 768 / 4;
        SDL_Color color;
    public:
        Character(std::string firstName, std::string lastName, std::string aiOrder, int positionX, int positionY, SDL_Texture* texture, SDL_Renderer* renderer, SDL_Color color);
        virtual ~Character() = default;
        virtual void renderSprite() = 0;
        SDL_Texture* getTexture() const { return texture; }
        std::string getFirstName() const { return firstName; }
        std::string getLastName() const { return lastName; }
        std::string getAiOrder() const { return aiOrder; }
        int getPositionX() const { return positionX; }
        int getPositionY() const { return positionY; }
        SDL_Color getColor() const { return color; }
};

#endif