#include "BoundedFloatRect.h"


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
