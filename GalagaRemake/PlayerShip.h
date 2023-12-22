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
    bool isWithinLeftCushionAndMovingThatDirection(BoundedFloatRect& shipBounds, BoundedFloatRect& worldBounds) const;
    bool isWithinRightCusionAndMovingThatDirection(BoundedFloatRect& shipBounds, BoundedFloatRect& worldBounds) const;
    bool isShipWithinTopCusionAndMovingThatDirection(BoundedFloatRect& shipBounds, BoundedFloatRect& worldBounds) const;
    bool isShipwithBottomCushionAndMovingThatDirection(BoundedFloatRect& shipBounds, BoundedFloatRect& worldBounds) const;
    sf::Vector2f m_startPosition;
};

