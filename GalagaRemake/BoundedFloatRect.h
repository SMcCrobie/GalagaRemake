#pragma once
#include <SFML/Graphics.hpp>


class BoundedFloatRect :
    public sf::FloatRect
{
public:
    BoundedFloatRect();
    BoundedFloatRect(float rectLeft, float rectTop, float rectWidth, float rectHeight);
    BoundedFloatRect(const sf::FloatRect& floatRect);
    BoundedFloatRect(const BoundedFloatRect& boundedFloatRect);
    void operator+=(float inwardMargin);

    float right;
    float bottom;
};

