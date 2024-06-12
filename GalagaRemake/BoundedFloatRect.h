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
    [[nodiscard]] BoundedFloatRect scale(float scalar) const;
    [[nodiscard]] BoundedFloatRect scaleWithMargins(float margin) const;

    float right;
    float bottom;
};

