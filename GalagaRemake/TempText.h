#pragma once
#include <SFML/Graphics/Text.hpp>

class TempText :
	public sf::Text
{
public:
	TempText();
	TempText(const sf::String& string, const sf::Font& font, unsigned int duration = 100);
	TempText(const TempText& tempText);
	void updateText();
	void addToDuration(unsigned int increment);
	void setDuration(unsigned int duration);
	void setDelay(unsigned int delay);
	bool isDone() const;
	void setFadeOut(unsigned int fadeStart);

private:
	bool updateDelay();
	bool updateDuration();
	void updateFade();
	void setFillColorAlpha(sf::Uint8 alpha);

	unsigned int m_delay{ 0 };
	unsigned int m_duration;
	unsigned int m_fadeStart{0};
	float m_fadeIncrement{0.f};
	float m_currentAlpha{255.f};

};


