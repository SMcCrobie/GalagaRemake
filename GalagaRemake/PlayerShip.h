#pragma once

#include "Ship.h"
#include "ProjectileManager.h"
#include "ItemType.h"

class PlayerShip :
    public Ship, public IManager
{
public:
	void initStartState();
	void calculateStartPosition();
	PlayerShip(const sf::Texture& texture);
    PlayerShip();
    void respawnShip() override;
    void init(const sf::Texture& texture);
	void handleIntro();
	void updateShip(KeyboardController& controller);
    void resetManager() override;
    void useItem(ItemType itemType);
   
private:
    void initStartHealth();
    void testAndApplyCushion(BoundedFloatRect& shipBounds, BoundedFloatRect worldBounds, float cushion);
    bool isWithinLeftCushionAndMovingThatDirection(BoundedFloatRect& shipBounds, BoundedFloatRect& worldBounds);
    bool isWithinRightCusionAndMovingThatDirection(BoundedFloatRect& shipBounds, BoundedFloatRect& worldBounds);
    bool isShipWithinTopCusionAndMovingThatDirection(BoundedFloatRect& shipBounds, BoundedFloatRect& worldBounds);
    bool isShipwithBottomCushionAndMovingThatDirection(BoundedFloatRect& shipBounds, BoundedFloatRect& worldBounds);
    sf::Vector2f m_startPosition;
};

