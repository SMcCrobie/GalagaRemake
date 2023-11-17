#pragma once
#include <SFML/Graphics.hpp>
#include "PlayerShip.h"
#include "ItemType.h"


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
    float m_rotation;
    int m_pointValue{};


    

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void moveObject();
    void rotateObject();
    void oscillateObject();
};

class Item :
	public GameObject
{
public:
    ItemType getItemType() const;
    void setItemType(ItemType type);

private:
    ItemType m_itemType;

};

class Collidable :
	public GameObject
{
public:
    void setHealth(int health);
    int getHealth() const;
    void setColor(const sf::Color& color);
    void decrementHealth();
    void applyForce(sf::Vector2f vel);
    void update() override;
    bool detectCollision();
    void setMass(float mass);

private:
    void updateObjectHitTimer();
    void explode();
    int m_health = 1;
    float m_mass = 1.0;//baseline mass
    int m_objectHitTimer = -1;
};
