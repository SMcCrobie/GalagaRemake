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
    float m_angularVelocity;
    sf::Vector2f m_localCenterOfMass;
    int m_pointValue{};


private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void move();
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
    ItemType m_itemType = ItemType::Repair_Kit;

};

class Collidable :
	public GameObject
{
public:
    void setHealth(int health);
    int getHealth() const;
    sf::Vector2f getMomentum() const;
    void setColor(const sf::Color& color);
    void decrementHealth();
    void applyMomentum(const sf::Vector2f momentum);
    void applyMomentum(sf::Vector2f momentum, sf::Vector2f impactLocation);
    void applyAngularVelocity(float angularVelocity, sf::Vector2f impactLocation, sf::Vector2f momentum);
	void update() override;
    std::optional<CollisionResult> detectProjectileCollision();
    void setMass(float mass);
    float getAngularVelocity() const;
    void applyPhysicsToEachOther(Collidable& collidable, const sf::Vector2f pointOfImpact);
    std::optional<sf::Vector2f> detectCollision(const Collidable& collidable) const;
    std::optional<sf::Vector2f> detectCollision(const sf::RectangleShape& rect) const;
    std::optional<sf::Vector2f> detectCollision(const sf::Sprite& sprite) const;
    void applyPhysicsFromProjectile(CollisionResult collisionResult);
    int last_collision;

private:
    void updateObjectHitTimer();
    void explode();
    sf::Transform getTransform() const;


	int m_health = 1;
    float m_mass = 1.0;//baseline mass
    int m_objectHitTimer = -1;

};

