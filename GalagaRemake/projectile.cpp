#include "projectile.h"


void Projectile::updateProjectile()
{
	move(m_velocity);
}

std::size_t Projectile::getPointCount() const
{
    return -1;
}

sf::Vector2f Projectile::getPoint(std::size_t index) const
{
    return {0, 0};
}

RectangleProjectile::RectangleProjectile(const sf::Vector2f& size, const sf::Vector2f& velocity)
	
{
	setSize(size);
	m_velocity = velocity;
}

void RectangleProjectile::setSize(const sf::Vector2f& size)
{
    m_size = size;
    update();
}


const sf::Vector2f& RectangleProjectile::getSize() const
{
    return m_size;
}

std::size_t RectangleProjectile::getPointCount() const
{
    return 4;
}


sf::Vector2f RectangleProjectile::getPoint(std::size_t index) const
{
    switch (index)
    {
    default:
	    case 0: return {0, 0};
	    case 1: return {m_size.x, 0};
	    case 2: return {m_size.x, m_size.y};
	    case 3: return {0, m_size.y};
    }
}

std::shared_ptr<Projectile> RectangleProjectile::clone()
{
    return std::make_shared<RectangleProjectile>(RectangleProjectile(*this));
}

const sf::Vector2f& Projectile::getVelocity() const
{
	return m_velocity;
}

std::shared_ptr<Projectile> Projectile::clone()
{
    return nullptr;
}

void Projectile::setVelocity(const sf::Vector2f& velocity)
{
	m_velocity = velocity;
}

void Projectile::setInitOffSets(float x, float y)
{
    m_initializationOffsets = sf::Vector2f(x, y);
}

void Projectile::initStartPosition(const float x, const float y)
{
    setPosition(x + m_initializationOffsets.x, y + m_initializationOffsets.y);
}

void CircleProjectile::rotateProjectile()
{
    m_rotationCounter++;
    if(m_rotationCounter < 4)
        return;

    m_rotationCounter = 0;
    rotate(-22.5f);
    update();
}

void CircleProjectile::updateProjectile()
{
    //rotateProjectile();
    Projectile::updateProjectile();
}

void CircleProjectile::setRadius(const float radius)
{
    m_radius = radius;
    update();
}


float CircleProjectile::getRadius() const
{
    return m_radius;
}



void CircleProjectile::setPointCount(const std::size_t count)
{
    m_pointCount = count;
    update();
}

void CircleProjectile::setShieldColor(const sf::Color& color)
{
    setFillColor(color);
    m_color = color;
}

void CircleProjectile::refreshColor()
{
    if (m_color != getFillColor())
        setFillColor(m_color);
}


std::size_t CircleProjectile::getPointCount() const
{
    return m_pointCount;
}


sf::Vector2f CircleProjectile::getPoint(const std::size_t index) const
{
    static constexpr float pi = 3.141592654f;

    const float angle = index * 2 * pi / m_pointCount - pi / 2;
    const float x = std::cos(angle) * m_radius;
    const float y = std::sin(angle) * m_radius;

    return {m_radius + x, m_radius + y};
}


CircleProjectile::CircleProjectile(const float radius, std::size_t pointCount, const sf::Vector2f& velocity)
	: m_velocity(velocity), m_radius(radius), m_pointCount(pointCount)
{
}

std::shared_ptr<Projectile> CircleProjectile::clone()
{
    return std::make_shared<CircleProjectile>(CircleProjectile(*this));
}

