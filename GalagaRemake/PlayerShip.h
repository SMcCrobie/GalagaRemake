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
	void handleIntro();
	void updateShip(KeyboardController& controller, BoundedFloatRect worldBounds);
    void resetManager() override;
   
private:
    void testAndApplyCushion(BoundedFloatRect& shipBounds, BoundedFloatRect worldBounds, float cushion);
    bool isWithinLeftCushionAndMovingThatDirection(BoundedFloatRect& shipBounds, BoundedFloatRect& worldBounds);
    bool isWithinRightCusionAndMovingThatDirection(BoundedFloatRect& shipBounds, BoundedFloatRect& worldBounds);
    bool isShipWithinTopCusionAndMovingThatDirection(BoundedFloatRect& shipBounds, BoundedFloatRect& worldBounds);
    bool isShipwithBottomCushionAndMovingThatDirection(BoundedFloatRect& shipBounds, BoundedFloatRect& worldBounds);
    sf::Vector2f m_startPosition;
};

