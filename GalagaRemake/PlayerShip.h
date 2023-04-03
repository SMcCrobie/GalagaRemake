#pragma once
#include "Ship.h"
#include "ProjectileManager.h"

class PlayerShip :
    public Ship, public Manager
{
public:
	void initStartState();
	void calculateStartPosition(const BoundedFloatRect& worldDimensions);
	PlayerShip(const sf::Texture& texture, const BoundedFloatRect& worldDimensions);
    void updateShip(BoundedFloatRect worldBounds) override;
    void setTextureRectBasedOnShipState() override;


    void rotateIfTriggered() override;
    void rotate180() override;
    void resetManager() override;
   
private:
    void updateShipVelocity(BoundedFloatRect worldBounds) override;
    void testAndApplyCushion(BoundedFloatRect& shipBounds, BoundedFloatRect worldBounds, float cushion);
    bool isWithinLeftCushionAndMovingThatDirection(BoundedFloatRect& shipBounds, BoundedFloatRect& worldBounds);
    bool isWithinRightCusionAndMovingThatDirection(BoundedFloatRect& shipBounds, BoundedFloatRect& worldBounds);
    bool isShipWithinTopCusionAndMovingThatDirection(BoundedFloatRect& shipBounds, BoundedFloatRect& worldBounds);
    bool isShipwithBottomCushionAndMovingThatDirection(BoundedFloatRect& shipBounds, BoundedFloatRect& worldBounds);
    void testAndApplyVerticalWorldBounds(BoundedFloatRect& shipBounds, BoundedFloatRect& worldBounds);
    void testAndApplyHorizontalWorldBounds(BoundedFloatRect& shipBounds, BoundedFloatRect& worldBounds);

    sf::Vector2f m_startPosition;
};

