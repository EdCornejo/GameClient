//
//  AudioEngine.h
//  GameClientMobile
//
//  Created by SungJinYoo on 5/29/13.
//
//

#ifndef __GameClientMobile__AudioEngine__
#define __GameClientMobile__AudioEngine__

// NOTE : this is a wrapper class of CocosDenshion::SimpleAudioEngine
// it can now track of what kind of BGM is playing

class AudioEngine
{
public:
    static AudioEngine* Instance();
    static void Destory();

private:
    static AudioEngine* instance;
    
    AudioEngine();
    ~AudioEngine();

    CocosDenshion::SimpleAudioEngine* m_Engine;
    std::string m_CurrentBackgroundMusic;
  
public:
    void PlayBackgroundMusic(std::string fileName, bool loop);
    void PlayBackgroundMusic(std::string fileName);
    void StopBackgroundMusic(bool release);
    void StopBackgroundMusic();
    void PauseBackgroundMusic();
    void ResumeBackgroundMusic();
    
    unsigned int PlayEffect(std::string fileName, bool loop);
    unsigned int PlayEffect(std::string fileName);
    void StopAllEffects();
    void StopEffect(unsigned int effectID);
    void PauseAllEffects();
    void ResumeAllEffects();
};

#endif /* defined(__GameClientMobile__AudioEngine__) */
