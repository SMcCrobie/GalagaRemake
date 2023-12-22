#include "MultiColorText.h"
#include "GameState.h"

template class MultiColorText<sf::Text>;
template class MultiColorText<TempText>;

constexpr float element_buffer = 5.f;

template <typename TextType>
MultiColorText<TextType>::MultiColorText(const TextType& templateText)
{
    m_templateText = templateText;
}

template <typename TextType>
MultiColorText<TextType>& MultiColorText<TextType>::addTextSegment(const std::string& text, const sf::Color& color, float leftMargin, const bool isBold)
{
    auto temp = m_templateText;
    temp.setString(text);
    temp.setFillColor(color);
    if (isBold) {
        temp.setOutlineColor(color);
        temp.setOutlineThickness(0.5f);
    }


    if (!m_segments.empty()) {
        const auto& lastSegment = m_segments.back();
        auto xPos = lastSegment.getPosition().x + lastSegment.getGlobalBounds().width + leftMargin;

        temp.setPosition(xPos, lastSegment.getPosition().y);
    }

    m_segments.push_back(temp);
    return *this;
}

//REVIEW---------------------------------------------------------------------
template <typename TextType>
void MultiColorText<TextType>::setCharacterSize(int size)
{
    if (m_segments.empty())
        return;

    // Store the original distances (margins) between the end of one segment and the start of the next
    std::vector<float> margins;
    for (std::size_t i = 0; i < m_segments.size() - 1; ++i) {
	    const float currentSegmentRight = m_segments[i].getPosition().x + m_segments[i].getGlobalBounds().width;
	    const float nextSegmentLeft = m_segments[i + 1].getPosition().x;
        margins.push_back(nextSegmentLeft - currentSegmentRight);
    }

    // Update character size for the template text
    m_templateText.setCharacterSize(size);

    // Update the character size for each segment and adjust their positions
    float currentX = m_segments.front().getPosition().x; // Starting X position of the first segment
    for (std::size_t i = 0; i < m_segments.size(); ++i) {
        m_segments[i].setCharacterSize(size);
        m_segments[i].setPosition(currentX, m_segments[i].getPosition().y);

        // Update currentX for the next segment, including the original margin
        if (i < m_segments.size() - 1) {
            currentX += m_segments[i].getGlobalBounds().width + margins[i];
        }
    }
}

template <typename TextType>
void MultiColorText<TextType>::setPosition(const sf::Vector2f pos)
{
    if (m_segments.empty())
        return;

    // Calculate the initial offset of the first segment
    auto initialOffset = m_segments.front().getPosition().x - m_segments.front().getGlobalBounds().left;

    // Start with the new position plus the initial offset
    auto currentX = pos.x + initialOffset;

    for (size_t i = 0; i < m_segments.size(); ++i) {
        auto& segment = m_segments[i];
        auto currentSegmentRight = segment.getGlobalBounds().left + segment.getGlobalBounds().width;

        segment.setPosition(currentX, pos.y); // Set the new position

        // Only add the gap if this is not the last segment
        if (i < m_segments.size() - 1) {
            // Calculate the gap (distance) to the next segment
            auto nextSegmentLeft = m_segments[i + 1].getGlobalBounds().left;
        	auto gap = nextSegmentLeft - currentSegmentRight;

			// Update currentX for the next segment, including the gap
			currentX += segment.getGlobalBounds().width + gap;
        }
    }
}
//REVIEW---------------------------------------------------------------------


template <typename TextType>
void MultiColorText<TextType>::horizontallyCenter()
{
    const float totalWidth = getTotalWidth();
    float currentX = (GameState::world_bounds.width - totalWidth) / 2;

    for (auto& segment : m_segments) {
        const sf::FloatRect bounds = segment.getGlobalBounds();
        segment.setPosition(currentX, segment.getPosition().y);
        currentX += bounds.width;
    }
}

template <typename TextType>
void MultiColorText<TextType>::verticallyCenter()
{
    const float yPos = (GameState::world_bounds.height - m_segments.front().getGlobalBounds().height) / 2;

    for (auto& segment : m_segments) {
        segment.setPosition(segment.getPosition().x, yPos);
    }
}

template <typename TextType>
void MultiColorText<TextType>::dumpSegments(std::vector<TextType>& targetVector)
{
    targetVector.insert(targetVector.end(), m_segments.begin(), m_segments.end());
}

template <typename TextType>
sf::FloatRect MultiColorText<TextType>::getGlobalBounds()
{
    if (m_segments.empty())
        return sf::FloatRect();

    auto bounds = m_segments.front().getGlobalBounds();
    auto fullWidth = 0.f;
    for (const auto& segment : m_segments) {

    	fullWidth += segment.getGlobalBounds().width;
    }
    bounds.width = fullWidth;
    return bounds;
}

template <typename TextType>
void MultiColorText<TextType>::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (const auto& segment : m_segments) {
        target.draw(segment, states);
    }
}

template <typename TextType>
float MultiColorText<TextType>::getTotalWidth() const
{
    float totalWidth = 0.f;
    for (const auto& segment : m_segments) {
        totalWidth += segment.getGlobalBounds().width;
    }
    return totalWidth;
}