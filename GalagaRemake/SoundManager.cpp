#include "SoundManager.h"

#include "GameState.h"
#include "Loader.h"

constexpr auto MAX_SOUNDS = 30;

//make a sound volume map, so that volume is always set on creation, and you have the ability to change all at any time, with one lever


void SoundManager::init()
{
    // Iterate over all SoundType values and insert a new sf::SoundBuffer for each
    for (int i = 0; i < static_cast<int>(SoundType::Count); ++i) {
		auto type = static_cast<SoundType>(i);
        m_soundMap.insert({ type, sf::SoundBuffer{} });
    }

    // Now you can load the actual sound files
    Loader::LOAD_SAFELY(m_soundMap[SoundType::Laser], R"(Assets\Sounds\Blast Laser 2.wav)");
    Loader::LOAD_SAFELY(m_soundMap[SoundType::Laser2], R"(Assets\Sounds\shotFired.wav)");
    Loader::LOAD_SAFELY(m_soundMap[SoundType::Laser3], R"(Assets\Sounds\Blast Laser 4.wav)");
    Loader::LOAD_SAFELY(m_soundMap[SoundType::ShipDeath], R"(Assets\Sounds\Big Explosion.wav)");
    Loader::LOAD_SAFELY(m_soundMap[SoundType::Repair], R"(Assets\Sounds\passive gain life-sound.wav)"); 
    Loader::LOAD_SAFELY(m_soundMap[SoundType::Hit], R"(Assets\Sounds\8bit_hit_13.wav)");

    m_levelCompleteMusic.openFromFile(R"(Assets\Sounds\Final Fantasy VII - Victory Fanfare.wav)");
    m_levelCompleteMusic.setVolume(60.f);



    m_levelMusic.openFromFile(R"(Assets\Sounds\DSGNDron_Sci-Fi Pad Digital Fallen Angel.wav)");
    m_levelMusic.setVolume(40.f);
    m_levelMusic.setLoop(true);
    m_levelMusic.play();

    m_bossMusic.openFromFile(R"(Assets\Sounds\DSGNDron_Sci-Fi Bg Roomtone Spaceship Alert.wav)");
    m_bossMusic.setVolume(60.f);
}





void SoundManager::resetManager()
{
	m_sounds.clear();
    m_levelCompleteMusic.stop();
}

void SoundManager::playSound(const SoundType soundType)
{
    m_sounds.emplace_back(m_soundMap.at(soundType));
    auto& newSound = m_sounds.back();
    newSound.setVolume(35.f);
    //can do this as a temp, and then you have to go back and adjust the file volume itself
	if (soundType == SoundType::Laser2)
        newSound.setVolume(newSound.getVolume() * .5f);
    if (soundType == SoundType::Laser3)
        newSound.setVolume(newSound.getVolume() * .5f);
    if (soundType == SoundType::ShipDeath)
        newSound.setVolume(newSound.getVolume() * 1.3f);
    if (soundType == SoundType::Hit)
        newSound.setVolume(newSound.getVolume() * .3f);
    m_sounds.back().play();
}



void SoundManager::update()
{
    while (m_sounds.size() > MAX_SOUNDS)
    {
        m_sounds.pop_front(); // Directly removes the oldest sound from the front
    }
   
}

void SoundManager::pause()
{
    for (auto& sound : m_sounds) {
        sound.pause();
    }
    m_levelCompleteMusic.pause();
    m_levelMusic.pause();
    m_levelMusic.pause();
}

void SoundManager::resume()
{
    for (auto& sound : m_sounds) {
        if(sound.getStatus() == sf::Sound::Status::Paused)
            sound.play();
    }
    
    if (m_levelCompleteMusic.getStatus() == sf::Sound::Status::Paused)
        m_levelCompleteMusic.play();

    if (m_levelMusic.getStatus() == sf::Sound::Status::Paused)
        m_levelMusic.play();
}

void SoundManager::playLevelCompleteMusic()
{
    m_levelMusic.stop();
    m_bossMusic.stop();
    m_levelCompleteMusic.play();
}

void SoundManager::playBossMusic()
{
    m_levelMusic.setVolume(20.f);
    m_bossMusic.play();
}
