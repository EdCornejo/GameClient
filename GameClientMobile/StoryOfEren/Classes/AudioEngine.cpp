//
//  AudioEngine.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 5/29/13.
//
//

#include "Headers.pch"

AudioEngine* AudioEngine::instance = nullptr;

AudioEngine::AudioEngine(): m_Engine(nullptr), m_CurrentBackgroundMusic()
{
    this->m_Engine = CocosDenshion::SimpleAudioEngine::sharedEngine();
}

AudioEngine::~AudioEngine() {}

AudioEngine* AudioEngine::Instance()
{
    if(!instance) {
        instance = new AudioEngine();
    }
    return instance;
}

void AudioEngine::PlayBackgroundMusic(std::string fileName, bool loop)
{
    if(this->m_CurrentBackgroundMusic.compare(fileName) == 0)
    {
        return;
    }
    this->m_CurrentBackgroundMusic = fileName;
    
    this->m_Engine->playBackgroundMusic(fileName.c_str(), loop);
}

void AudioEngine::PlayBackgroundMusic(std::string fileName)
{
    if(this->m_CurrentBackgroundMusic.compare(fileName) == 0)
    {
        return;
    }
    this->m_CurrentBackgroundMusic = fileName;
    
    this->m_Engine->playBackgroundMusic(fileName.c_str(), true);
}

void AudioEngine::StopBackgroundMusic(bool release)
{
    this->m_CurrentBackgroundMusic.clear();
    this->m_Engine->stopBackgroundMusic(release);
}

void AudioEngine::StopBackgroundMusic()
{
    this->m_CurrentBackgroundMusic.clear();
    this->m_Engine->stopBackgroundMusic();
}
void AudioEngine::PauseBackgroundMusic()
{
    this->m_Engine->pauseBackgroundMusic();
}

void AudioEngine::ResumeBackgroundMusic()
{
    this->m_Engine->resumeBackgroundMusic();
}

unsigned int AudioEngine::PlayEffect(std::string fileName, bool loop)
{
    return this->m_Engine->playEffect(fileName.c_str(), loop);
}

unsigned int AudioEngine::PlayEffect(std::string fileName)
{
    return this->m_Engine->playEffect(fileName.c_str());
}

void AudioEngine::StopAllEffects()
{
    this->m_Engine->stopAllEffects();
}

void AudioEngine::StopEffect(unsigned int effectID)
{
    this->m_Engine->stopEffect(effectID);
}

void AudioEngine::PauseAllEffects()
{
    this->m_Engine->pauseAllEffects();
}

void AudioEngine::ResumeAllEffects()
{
    this->m_Engine->resumeAllEffects();
}
