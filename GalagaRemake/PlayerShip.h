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
    void testAndApplyCushion(BoundedFloatRect& shipBounds, BoundedFloatRect worldBounds, float cushion);
    bool isWithinLeftCushionAndMovingThatDirection(BoundedFloatRect& shipBounds, BoundedFloatRect& worldBounds);
    bool isWithinRightCusionAndMovingThatDirection(BoundedFloatRect& shipBounds, BoundedFloatRect& worldBounds);
    bool isShipWithinTopCusionAndMovingThatDirection(BoundedFloatRect& shipBounds, BoundedFloatRect& worldBounds);
    bool isShipwithBottomCushionAndMovingThatDirection(BoundedFloatRect& shipBounds, BoundedFloatRect& worldBounds);
    void testAndApplyVerticalWorldBounds(BoundedFloatRect& shipBounds, BoundedFloatRect& worldBounds);
    void testAndApplyHorizontalWorldBounds(BoundedFloatRect& shipBounds, BoundedFloatRect& worldBounds);
};

