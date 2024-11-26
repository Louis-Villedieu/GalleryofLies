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
        int currentFrame = 0;
        int currentRow = 0;
        int frameWidth = 576 / 3;  
        int frameHeight = 768 / 4;
    public:
        Character(std::string firstName, std::string lastName, std::string aiOrder, int positionX, int positionY, SDL_Texture* texture, SDL_Renderer* renderer);
        virtual ~Character() = default;
        virtual void renderSprite() = 0;
};

#endif