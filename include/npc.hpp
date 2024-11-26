#ifndef NPC_HPP
#define NPC_HPP

#include "./character.hpp"

class NPC : public Character
{
    public:
        NPC(std::string firstName, std::string lastName, std::string aiOrder, int positionX, int positionY, SDL_Texture* texture, SDL_Renderer* renderer);
        ~NPC();
        void renderSprite() override;
        std::string thinkAndAnswer(std::string question);
        void setTexture(SDL_Texture* texture);
};

#endif