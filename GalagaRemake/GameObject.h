#pragma once
#include <SFML/Graphics.hpp>
class GameObject :
    public sf::Drawable
{
public:
    GameObject();
    void setSprite(const sf::Sprite& sprite);
    void setRect(const sf::RectangleShape& rect);
    void setCircle(const sf::CircleShape& circle);
    void setRotation(float rotation);
    void setVelocity(const sf::Vector2f& velocity);
    void setOscillation(const sf::Vector2f& scalar, int framesTillSwitch);

    void updateGameObject();


protected:
    sf::Sprite m_sprite;
	//can use one of these as the collision box for the object, unsure if I should rename
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


    

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void moveObject();
    void rotateObject();
    void oscillateObject();
};

