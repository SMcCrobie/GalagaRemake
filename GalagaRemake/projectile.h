#pragma once
#include <SFML/Graphics.hpp>

static float PROJECTILE_START_SPEED = 12;
#define PROJECTILE_MASS 5.F


class Projectile
	: public sf::Shape{
public:
	virtual void updateProjectile();
    std::size_t getPointCount() const override;
    sf::Vector2f getPoint(std::size_t index) const override;
    void setVelocity(const sf::Vector2f& velocity);
    void setInitOffSets(float x, float y);
    void initStartPosition(float x, float y, bool isBackwards);
    const sf::Vector2f& getVelocity() const;
    virtual std::shared_ptr<Projectile> clone() = 0;
protected:
    sf::Vector2f m_velocity;
    sf::Vector2f m_initializationOffsets;

};



class RectangleProjectile :
    public Projectile
{
public:
	RectangleProjectile(const sf::Vector2f& size = sf::Vector2f(3.f, 8.f), const sf::Vector2f& velocity = sf::Vector2f(0.f, -PROJECTILE_START_SPEED));
	void setSize(const sf::Vector2f& size);
    const sf::Vector2f& getSize() const;
    std::size_t getPointCount() const override;
    sf::Vector2f getPoint(std::size_t index) const override;
    std::shared_ptr<Projectile> clone() override;
    void setGrowthSpeed(sf::Vector2f& growthAmount);

private:
    sf::Vector2f m_size;

};

class CircleProjectile :
    public Projectile
{
public:
    CircleProjectile(float radius = 5.f, std::size_t pointCount = 30,
        const sf::Vector2f& velocity = sf::Vector2f(0, -PROJECTILE_START_SPEED));

	void rotateProjectile();
	void updateProjectile() override;
	void setRadius(float radius);
	float getRadius() const;
	void setPointCount(std::size_t count);
    void setShieldColor(const sf::Color& color);//can prob hide base color functions
    void refreshColor();
    void makeRotating();

	std::size_t getPointCount() const override;
	sf::Vector2f getPoint(std::size_t index) const override;
    std::shared_ptr<Projectile> clone() override;
private:
    sf::Vector2f m_velocity;
    float       m_radius;   
    std::size_t m_pointCount;
    int m_rotationCounter;
    sf::Color m_color;

};

