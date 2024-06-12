#include "TempText.h"


TempText::TempText()
	: Text(), m_duration(100)
{
}

TempText::TempText(const sf::String& string, const sf::Font& font, unsigned duration)
	: Text(string, font), m_duration(duration)
{
}

TempText::TempText(const TempText& tempText) : sf::Text(tempText),
                                               m_duration(tempText.m_duration),
                                               m_fadeStart(tempText.m_fadeStart), m_fadeIncrement(tempText.m_fadeIncrement),
                                               m_currentAlpha(tempText.m_currentAlpha)
{
	setDelay(tempText.m_delay);
}

bool TempText::updateDelay()
{
	if(m_delay > 0)
	{
		m_delay--;
		if (m_delay == 0) {
			m_currentAlpha = 255.f;//might want to add a base alpha at some point
			setFillColorAlpha(255);
		}
		return true;
	}
	return false;
}

bool TempText::updateDuration()
{
	if (m_duration > 0)
		m_duration--;
	if(m_duration > m_fadeStart)
		return true;
	return false;
}

void TempText::updateFade()
{
	m_currentAlpha -= m_fadeIncrement;
	if (m_currentAlpha <= 0.f)
		setFillColorAlpha(0);
	else
		setFillColorAlpha(static_cast<sf::Uint8>(m_currentAlpha));
}

void TempText::setFillColorAlpha(const sf::Uint8 alpha)
{
	sf::Color color = getFillColor();
	color.a = alpha;
	setFillColor(color);
}

void TempText::updateText()
{
	if (updateDelay()) return;
	if (updateDuration()) return;

	updateFade();
}

void TempText::addToDuration(const unsigned int increment)
{
	m_duration += increment;
}

void TempText::setDuration(const unsigned duration)
{
	m_duration = duration;
}

void TempText::setDelay(const unsigned delay)
{
	m_delay = delay;
	if(delay <= 0)
		return;
	m_currentAlpha = 0.0f;
	setFillColorAlpha(0);
}

bool TempText::isDone() const
{
	return m_duration == 0;
}


void TempText::setFadeOut(const unsigned fadeStart)
{
	if (fadeStart > m_duration)
		m_fadeStart = m_duration;
	else
		m_fadeStart = fadeStart;

	m_fadeIncrement = 255.f / static_cast<float>(m_fadeStart);
}
