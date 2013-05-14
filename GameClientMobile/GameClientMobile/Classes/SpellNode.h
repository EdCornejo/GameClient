//
//  SpellNode.h
//  GameClientMobile
//
//  Created by SungJinYoo on 3/22/13.
//
//

#ifndef __GameClientMobile__SpellNode__
#define __GameClientMobile__SpellNode__

class SpellNode : public CCSprite
{
private:
    const flownet::SpellInfo m_SpellInfo;
    flownet::ActorID m_CasterID;
    flownet::POINT m_Destination;
    float m_LastTickTime;

public:
    SpellNode(const flownet::SpellInfo& spellInfo, flownet::ActorID casterID, flownet::POINT destination);
    virtual ~SpellNode();
    
    virtual bool initWithFile(const char* fileName);
    static SpellNode* create(const flownet::SpellInfo& spellInfo, flownet::ActorID casterID, flownet::POINT destination);
    
    virtual void update(float deltaTime);
    
    
private:
    void StartSpellAnimation();
    void Destroy();
};

#endif /* defined(__GameClientMobile__SpellNode__) */
