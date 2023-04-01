#pragma once
#include <SFML/Graphics.hpp>
class GameObject :
    public sf::Drawable
{
public:
    GameObject();
    GameObject(const sf::RectangleShape& collisonBox);
    void setPosition(float x, float y);
    void setPosition(const sf::Vector2f& position);


protected:
    sf::RectangleShape m_collisonBox;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

