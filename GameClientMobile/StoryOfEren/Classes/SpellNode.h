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
    const flownet::SpellInfo    m_SpellInfo;
    flownet::ActorID            m_CasterID;
    flownet::POINT              m_StartPoint;
    flownet::POINT              m_Destination;
    float                       m_LastTickTime;
    unsigned int                m_StartingEffectID;
    bool                        m_HasReachedToDestination;
    
    ShadowNode* m_Shadow;

public:
    SpellNode(const flownet::SpellInfo& spellInfo, flownet::ActorID casterID, flownet::POINT destination);
    virtual ~SpellNode();
    
    virtual bool initWithFile(const char* fileName);
    static SpellNode* create(const flownet::SpellInfo& spellInfo, flownet::ActorID casterID, flownet::POINT destination);
    
    virtual void update(float deltaTime);
    
    flownet::SpellInfo GetSpellInfo()   {   return this->m_SpellInfo; }
    CCPoint GetDestination()            {   return PointConverter::Convert(this->m_Destination); }
    CCPoint GetStartPoint()             {   return PointConverter::Convert(this->m_StartPoint); }
    bool    HasReachedToDestination()   {   return m_HasReachedToDestination; }
    
private:
    void StartSpellAnimation();
    void Destroy();
    
    std::string GetFileName(flownet::SpellType spellType);
    void PlayStartingEffect();
    void PlayEndingEffect();
    
    void BeginAfterEffect();
    void SetAnchorPointToCCPointLowerMid();
    void SetRotateToOrigin();
};

#endif /* defined(__GameClientMobile__SpellNode__) */
