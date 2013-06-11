//
//  SpellEffectNode.h
//  GameClientMobile
//
//  Created by SungJinYoo on 5/13/13.
//
//

#ifndef __GameClientMobile__SpellEffectNode__
#define __GameClientMobile__SpellEffectNode__

class SpellEffectNode : public CCNode
{
private:
    bool m_IsOverTheCharacter;
    flownet::ActorID m_ActorID;
    flownet::SpellAbility m_SpellAbility;
    flownet::SpellEffectType m_SpellEffectType;
    
public:
    SpellEffectNode();
    virtual ~SpellEffectNode();
    
    virtual bool init();
    // NOTE : this self destroy function can be active on some cases only
    static SpellEffectNode* create(flownet::ActorID actorID, flownet::SpellAbility spellAbility);
    static SpellEffectNode* create(flownet::ActorID actorID, flownet::SpellEffectType spellEffectType);
    
    virtual void update(float deltaTime);
    
    bool IsOverTheCharacter();
    
private:
    void Destroy();
};

#endif /* defined(__GameClientMobile__SpellEffectNode__) */
