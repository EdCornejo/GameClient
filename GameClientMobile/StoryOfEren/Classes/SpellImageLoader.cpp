//
//  SpellImageLoader.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 4/11/13.
//
//

#include "Headers.pch"

std::string SpellImageLoader::GetImageFileName(SpellType spellType)
{
    std::string imageName;
    switch (spellType) {
        case SpellType_FireBall:
            imageName = "fire_ball.png";
            break;
        case SpellType_FireBurst:
            imageName = "fire_burst.png";
            break;
        case SpellType_IceArrow:
            imageName = "ice_arrow.png";
            break;
        case SpellType_IceFog:
            imageName = "ice_fog.png";
            break;
        case SpellType_Crystalize:
            imageName = "fire_ball.png";
            break;
        case SpellType_NONE:
        default:
            imageName = "empty.png";
            break;
    }
    return imageName;
}
    
CCSprite* SpellImageLoader::GetSpellQuickSlotImage(SpellType spellType)
{
    std::string iconName = "ui/spell_icon/";
    iconName += GetImageFileName(spellType);
    
    CCSprite* icon = CCSprite::create(iconName.c_str());
    ASSERT_DEBUG(icon != nullptr);
    
    return icon;
}

CCSprite* SpellImageLoader::GetSpellGuideImage(SpellType spellType)
{
    std::string iconName = "ui/guide_line/";
    iconName += GetImageFileName(spellType);
    
    CCSprite* icon = CCSprite::create(iconName.c_str());
    ASSERT_DEBUG(icon != nullptr);
    
    return icon;
}

CCSprite* SpellImageLoader::GetSpellEffectImage(flownet::SpellAbility spellAbility)
{
    std::string effectFileName = "spell/effect/";
    
    switch (spellAbility) {
        case flownet::SpellAbility_Freeze :
            effectFileName += "freeze.png";
            break;
        case flownet::SpellAbility_Slow :
            effectFileName += "slow.png";
            break;
        default:
            return nullptr;
            break;
    }

    return CCSprite::create(effectFileName.c_str());
}