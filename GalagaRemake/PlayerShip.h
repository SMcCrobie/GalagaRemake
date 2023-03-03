#pragma once
#include "Ship.h"

class PlayerShip :
    public Ship
{
public:
    PlayerShip(const sf::Texture& texture, BoundedFloatRect worldDimensions);
    void setTextureRectBasedOnShipState() override;
    void rotateIfTriggered();
    void rotate180() override;
private:
    void updateShipVelocity(BoundedFloatRect worldBounds) override;
};

