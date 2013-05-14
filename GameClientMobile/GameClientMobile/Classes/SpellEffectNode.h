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
    flownet::ActorID m_ActorID;
    flownet::SpellAbility m_SpellAbility;
    
public:
    SpellEffectNode();
    virtual ~SpellEffectNode();
    
    virtual bool init();
    static SpellEffectNode* create(flownet::ActorID actorID, flownet::SpellAbility spellAbility);
    
    virtual void update(float deltaTime);
};

#endif /* defined(__GameClientMobile__SpellEffectNode__) */
