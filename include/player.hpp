#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <iostream>
#include "./character.hpp"


class Player : public Character
{
    private:
        int speed;
        int dx;
        int dy;
        bool isMoving;
        int lastUpdateTime;
        const int animationSpeed = 200;
    public:
        Player(std::string firstName, std::string lastName, std::string aiOrder, int positionX, int positionY, SDL_Texture* texture, SDL_Renderer* renderer);
        ~Player();
        void renderSprite() override;
        void move();
        void update();
        void handleEvent(SDL_Event& event);
        void setTexture(SDL_Texture* texture);
};

#endif 