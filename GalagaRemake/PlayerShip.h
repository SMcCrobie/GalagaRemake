#pragma once
#include "Ship.h"
#include "ProjectileManager.h"

class PlayerShip :
    public Ship
{
public:
    PlayerShip(const sf::Texture& texture, BoundedFloatRect worldDimensions);
    void setTextureRectBasedOnShipState() override;
    void rotateIfTriggered();
    void rotate180() override;
    bool detectCollision(ProjectileManager& projectileManager);

private:
    void updateShipVelocity(BoundedFloatRect worldBounds) override;
};

