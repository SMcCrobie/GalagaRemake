#pragma once
#include <SFML/Graphics.hpp>

static float PROJECTILE_START_SPEED = 12;
//static float PROJECTILE_RESISTENCE_MULTIPLIER = .995;

class Projectile
	: public sf::Shape{
public:
    // ~Projectile() override = 0; // can't make virtual class if I want to pass it around as a shared pointer
								  // can do it if I reset the pointer to its type 
	virtual void move();
    std::size_t getPointCount() const override;
    sf::Vector2f getPoint(std::size_t index) const override;
    void setVelocity(const sf::Vector2f& velocity);
    const sf::Vector2f& getVelocity() const;
    virtual std::shared_ptr<Projectile> clone();//returns nullptr
protected:
    sf::Vector2f m_velocity;
};



class RectangleProjectile :
    public Projectile
{
public:
	RectangleProjectile(const sf::Vector2f& size = sf::Vector2f(3.f, 8.f), const sf::Vector2f& velocity = sf::Vector2f(0.f, -PROJECTILE_START_SPEED));
	//RectangleProjectile(const RectangleProjectile& rectangleProjectile);
	void setSize(const sf::Vector2f& size);
    const sf::Vector2f& getSize() const;
    std::size_t getPointCount() const override;
    sf::Vector2f getPoint(std::size_t index) const override;
    std::shared_ptr<Projectile> clone() override;

private:
    sf::Vector2f m_size;

};

class CircleProjectile :
    public Projectile
{
public:
	void setRadius(float radius);
	float getRadius() const;
	void setPointCount(std::size_t count);
	std::size_t getPointCount() const override;
	sf::Vector2f getPoint(std::size_t index) const override;
	CircleProjectile(float radius = 5.f, std::size_t pointCount = 30,
        const sf::Vector2f& velocity = sf::Vector2f(0, -PROJECTILE_START_SPEED));
    std::shared_ptr<Projectile> clone() override;
private:
    sf::Vector2f m_velocity;
    float       m_radius;   
    std::size_t m_pointCount;
};

