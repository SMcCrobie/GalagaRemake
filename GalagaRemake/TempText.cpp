#include "TempText.h"


TempText::TempText()
	: Text(), m_duration(100)
{
}

TempText::TempText(const sf::String& string, const sf::Font& font, unsigned duration)
	: Text(string, font), m_duration(duration)
{
}

void TempText::updateText()
{
	if (m_duration > 0)
		m_duration--;
	if(m_duration > m_fadeStart)
		return;
	auto tempColor(getFillColor());//more costly than it needs to be

	m_currentAlpha -= m_fadeIncrement;
	if (m_currentAlpha <= 0.f)
		tempColor.a = 0;
	else
		tempColor.a = static_cast<sf::Uint8>(m_currentAlpha);

	setFillColor(tempColor);
}

void TempText::addToDuration(const unsigned int increment)
{
	m_duration += increment;
}

void TempText::setDuration(const unsigned duration)
{
	m_duration = duration;
}

bool TempText::isDone() const
{
	return m_duration == 0;
}


void TempText::addFadeOut(const unsigned fadeStart)
{
	if (fadeStart > m_duration)
		m_fadeStart = m_duration;
	else
		m_fadeStart = fadeStart;

	m_fadeIncrement = 255.f / static_cast<float>(m_fadeStart);
}
