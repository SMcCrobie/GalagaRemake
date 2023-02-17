#pragma once
#include <SFML/Graphics.hpp>
class GameObject :
    public sf::Drawable
{
public:
    GameObject();
    GameObject(sf::RectangleShape collisonBox);
   // GameObject(sf::Shape& collisonBox);
    GameObject(sf::Sprite sprite);
    GameObject(sf::Sprite sprite, sf::RectangleShape collisonBox);
    GameObject(const GameObject& gameObject);
    //GameObject(sf::Sprite& sprite, sf::Shape& collisonBox);
    void setPosition(float x, float y);
    void setPosition(const sf::Vector2f& position);


protected:
    std::shared_ptr<sf::Shape> m_collisonBox;
    std::unique_ptr<sf::Sprite> m_sprite;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

