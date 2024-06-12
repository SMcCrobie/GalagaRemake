#include "BoundedFloatRect.h"


BoundedFloatRect::BoundedFloatRect()
{
    top = 0;
    bottom = 0;

    left = 0;
    right = 0;
}

BoundedFloatRect::BoundedFloatRect(float rectLeft, float rectTop, float rectWidth, float rectHeight)
    : sf::FloatRect(rectLeft, rectTop, rectWidth, rectHeight) {
    right = rectLeft + rectWidth;
    bottom = rectTop + rectHeight;

}
BoundedFloatRect::BoundedFloatRect(const sf::FloatRect& floatRect) : sf::FloatRect(floatRect) {
    right = floatRect.left + floatRect.width;
    bottom = floatRect.top + floatRect.height;
}
BoundedFloatRect::BoundedFloatRect(const BoundedFloatRect& boundedFloatRect) : sf::FloatRect(boundedFloatRect) {
    right = boundedFloatRect.right;
    bottom = boundedFloatRect.bottom;
}

void BoundedFloatRect::operator+=(float inwardMargin)
{
    top += inwardMargin;
    bottom -= inwardMargin;

    left += inwardMargin;
    right -= inwardMargin;
}

BoundedFloatRect BoundedFloatRect::scale(const float scalar) const
{
    BoundedFloatRect scaledRect = *this; 

    scaledRect.width *= scalar;
    scaledRect.height *= scalar;
    scaledRect.left -= (scaledRect.width - this->width) / 2.0f;  
    scaledRect.top -= (scaledRect.height - this->height) / 2.0f; 
    scaledRect.right = scaledRect.left + scaledRect.width;
    scaledRect.bottom = scaledRect.top + scaledRect.height;
  

    return scaledRect;

}

BoundedFloatRect BoundedFloatRect::scaleWithMargins(const float margin) const
{
    BoundedFloatRect scaledRect = *this;

    scaledRect.width += margin*2;
    scaledRect.height += margin*2;

    scaledRect.left -= margin;
    scaledRect.top -= margin;
    scaledRect.right += margin;
    scaledRect.bottom += margin;


    return scaledRect;

}
