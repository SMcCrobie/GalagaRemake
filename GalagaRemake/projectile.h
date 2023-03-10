#pragma once
#include <SFML/Graphics.hpp>

static float PROJECTILE_START_SPEED = 12;
//static float PROJECTILE_RESISTENCE_MULTIPLIER = .995;

class RectangleProjectile :
    public sf::RectangleShape
{
public:
    RectangleProjectile(const sf::Vector2f& size = sf::Vector2f(0, 0), const sf::Vector2f& velocity = sf::Vector2f(0, -PROJECTILE_START_SPEED));
    const sf::Vector2f& getVelocity() const;
    bool setVelocity(const sf::Vector2f& velocity);
    void move();

private:
    sf::Vector2f m_velocity;

};

