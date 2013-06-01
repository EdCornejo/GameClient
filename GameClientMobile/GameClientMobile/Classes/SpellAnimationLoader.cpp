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
        "spell/fireBall.plist",
        "spell/explosion.plist",
        "spell/meteor.plist",
        "spell/iceCrystalize.plist",
        "spell/iceField.plist",
        "spell/iceSpear.plist",
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
        case flownet::SpellType_FireBurst :
            spellAnimationName = "meteor";
            break;
        case flownet::SpellType_Crystalize :
            spellAnimationName = "iceCrystalize";
            break;
        case flownet::SpellType_IceArrow :
            spellAnimationName = "iceSpear";
            break;
        case flownet::SpellType_IceFog :
            spellAnimationName = "iceField";
            break;
        case flownet::SpellType_FireBall:
        default:
            spellAnimationName = "fireBall";
            break;
    }

    CCAnimation* animation = CCAnimationCache::sharedAnimationCache()->animationByName(spellAnimationName.c_str());
    
    ASSERT_DEBUG(animation != nullptr);
   
    return animation;
}