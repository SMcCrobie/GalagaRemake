#pragma once
#include <SFML/Graphics.hpp>
#include "PlayerShip.h"


class GameObject :
    public sf::Drawable
{
public:
    GameObject();
    void setSprite(const sf::Sprite& sprite, bool withCollisionBox = true);
    void setRect(const sf::RectangleShape& rect);
    void setCircle(const sf::CircleShape& circle);
    void setRotation(float rotation);
    void setVelocity(const sf::Vector2f& velocity);
    void setVelocity(float x, float y);
    void setScale(float x, float y);
    sf::Vector2f getVelocity() const;
    void setOscillation(const sf::Vector2f& scalar, int framesTillSwitch);
    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;
    void setPointValue(int pointValue);

    virtual void update();
    bool detectCollision(const PlayerShip& playerShip) const;
    int getPointValue() const;

protected:
    sf::Sprite m_sprite;
    sf::RectangleShape m_rectangle;
    sf::CircleShape m_circle;

    bool m_isThereSprite;
    bool m_isThereRect;
    bool m_isThereCircle;

    sf::Vector2f m_scalar;
    int m_oscillationTimer;
    int m_oscillationThreshold;
    sf::Vector2f m_velocity;
    float m_angularVelocity;
    sf::Vector2f m_localCenterOfMass;
    int m_pointValue{};


private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void move();
    void rotateObject();
    void oscillateObject();
};
