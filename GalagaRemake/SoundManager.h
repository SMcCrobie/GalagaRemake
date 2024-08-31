#pragma once
#include <deque>
#include <map>
#include <SFML/Audio.hpp>

#include "IManager.h"

enum class SoundType
{
	Laser,
    Laser2,
    Laser3,
	ShipDeath,
    Repair,
    Hit,
    Count
};



class SoundManager :
	public IManager
{
public:
    void init();
    void resetManager() override;
    void playSound(SoundType soundType);
    void update();
    void pause();
    void resume();
    void playLevelCompleteMusic();
    void playBossMusic();
    //need a pause function

private:
    std::deque<sf::Sound> m_sounds;
    std::map<SoundType, sf::SoundBuffer> m_soundMap;
    sf::Music m_levelCompleteMusic;
    sf::Music m_levelMusic;
    sf::Music m_bossMusic;
};

