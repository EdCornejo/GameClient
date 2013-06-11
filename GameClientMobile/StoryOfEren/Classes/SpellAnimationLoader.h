//
//  SpellAnimationLoader.h
//  GameClientMobile
//
//  Created by SungJinYoo on 3/22/13.
//
//

#ifndef __GameClientMobile__SpellAnimationLoader__
#define __GameClientMobile__SpellAnimationLoader__

class SpellAnimationLoader
{
private:
    SpellAnimationLoader();
    ~SpellAnimationLoader();
    
    static SpellAnimationLoader* instance_;
    
public:
    static SpellAnimationLoader* Instance();
    
    CCAnimation* GetSpellAnimation(flownet::SpellType spellType);
    CCAnimation* GetSpellAnimation(flownet::SpellEffectType spellEffectType);
    CCAnimation* GetAfterEffectAnimation(flownet::SpellType spellType);
};

#endif /* defined(__GameClientMobile__SpellAnimationLoader__) */
