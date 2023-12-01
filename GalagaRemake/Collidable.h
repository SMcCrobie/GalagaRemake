#pragma once

#include "GameObject.h"


class Collidable :
    public GameObject
{
public:
    void setHealth(const float health);
    float getHealth() const;
    sf::Vector2f getMomentum() const;
    void setColor(const sf::Color& color);
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
    void decrementHealth(const sf::Vector2f& changeInMomentum);

private:
    void updateObjectHitTimer();
    void explode();
    sf::Transform getTransform() const;
    float vectorMagnitude(const sf::Vector2f& vector) const;


    float m_health = 1.f;
    float m_mass = 1.0f;//baseline mass
    int m_objectHitTimer = -1;

};
