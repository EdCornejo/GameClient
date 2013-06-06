//
//  SpellImageLoader.h
//  GameClientMobile
//
//  Created by SungJinYoo on 4/11/13.
//
//

#ifndef __GameClientMobile__SpellImageLoader__
#define __GameClientMobile__SpellImageLoader__

class SpellImageLoader
{
private:
    static std::string GetImageFileName(flownet::SpellType spellType);
    
public:
    static CCSprite* GetSpellQuickSlotImage(flownet::SpellType spellType);
    static CCSprite* GetSpellGuideImage(flownet::SpellType spellType);
    static CCSprite* GetSpellEffectImage(flownet::SpellAbility spellAbility);
};

#endif /* defined(__GameClientMobile__SpellImageLoader__) */
