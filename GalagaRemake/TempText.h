#pragma once
#include <SFML/Graphics/Text.hpp>

class TempText :
	public sf::Text
{
public:
	TempText();
	TempText(const sf::String& string, const sf::Font& font, unsigned int duration = 100);
	void updateText();
	void addToDuration(unsigned int increment);
	void setDuration(unsigned int duration);
	bool isDone() const;
	void addFadeOut(unsigned int fadeStart);

private:
	unsigned int m_duration;
	unsigned int m_fadeStart{0};
	float m_fadeIncrement{0.f};
	float m_currentAlpha{255.f};

};


