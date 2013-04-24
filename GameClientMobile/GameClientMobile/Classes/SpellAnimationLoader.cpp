//
//  SpellAnimationLoader.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 3/22/13.
//
//

#include "Headers.pch"

SpellAnimationLoader* SpellAnimationLoader::instance_ = nullptr;

SpellAnimationLoader::SpellAnimationLoader()
{
    // NOTE : every spell animation plist file here
    std::vector<std::string> spellAnimationPListFileList = {
        "spell/spell_fireball.plist",
    };
    
    for_each(spellAnimationPListFileList.begin(), spellAnimationPListFileList.end(), [](std::string& plistFileName){
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(plistFileName.c_str());
    });

}

SpellAnimationLoader::~SpellAnimationLoader()
{

}

SpellAnimationLoader* SpellAnimationLoader::Instance()
{
    if(!SpellAnimationLoader::instance_)
    {
        SpellAnimationLoader::instance_ = new SpellAnimationLoader();
    }
    
    return SpellAnimationLoader::instance_;
}

CCAnimation* SpellAnimationLoader::GetSpellAnimation(flownet::SpellType spellType)
{
    std::string spellAnimationName;
    switch (spellType) {
        case flownet::SpellType_FireBall:
        default:
            spellAnimationName = "spell_fireball";
            break;
    }

    CCAnimation* animation = CCAnimationCache::sharedAnimationCache()->animationByName(spellAnimationName.c_str());
    
    ASSERT_DEBUG(animation != nullptr);
   
    return animation;
}