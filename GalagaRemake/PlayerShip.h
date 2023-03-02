#pragma once
#include "Ship.h"

class PlayerShip :
    public Ship
{
public:
    PlayerShip();
    void setTextureRectBasedOnShipState() override;
    void updateShipVelocity(BoundedFloatRect worldBounds) override;
    void rotateIfTriggered();
    void rotate180() override;
};

