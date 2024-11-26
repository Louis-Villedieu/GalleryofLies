#ifndef NPC_HPP
#define NPC_HPP

#include "./character.hpp"

class NPC : public Character
{
    private:
        std::string pastConversation;
    public:
        NPC(std::string firstName, std::string lastName, std::string aiOrder, int positionX, int positionY, SDL_Texture* texture, SDL_Renderer* renderer);
        ~NPC();
        void renderSprite() override;
        std::string thinkAndAnswer(std::string question);
        void setTexture(SDL_Texture* texture);
        void setPastConversation(std::string newResponse);
};

#endif