#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

#include "TempText.h"


template<typename TextType>
class MultiColorText : public sf::Drawable {
public:
    static_assert(std::is_same_v<TextType, sf::Text> || std::is_same_v<TextType, TempText>,
        "MultiColorGenericText can only be instantiated with sf::Text or TempText");

    explicit MultiColorText(const TextType& templateText);
    MultiColorText<TextType>& addTextSegment(const std::string& text, const sf::Color& color, float leftMargin = 0.f, bool isBold = false);
    void setCharacterSize(int size);
    void setPosition(sf::Vector2f pos);
    void horizontallyCenter();
    void verticallyCenter();
    void dumpSegments(std::vector<TextType>& targetVector);
    sf::FloatRect getGlobalBounds();

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    std::vector<TextType> m_segments;
    TextType m_templateText;

    [[nodiscard]] float getTotalWidth() const;
};
