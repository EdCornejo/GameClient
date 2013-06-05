//
//  ActorNode.h
//  GameClientMobile
//
//  Created by SungJinYoo on 3/7/13.
//
//

#ifndef __GameClientMobile__ActorNode__
#define __GameClientMobile__ActorNode__

USING_NS_CC;

using namespace spine;

class SpellNode;

class ShadowNode : public CCNode
{
private:
    flownet::ActorID m_ActorID;
    SpellNode* m_SpellNode;
    
public:
    ShadowNode();
    virtual ~ShadowNode();
    
    virtual bool init();
    static ShadowNode* create(SpellNode* spellNode);
    static ShadowNode* create(flownet::ActorID actorID);
    virtual void update(float deltaTime);
};

class HighlightNode : public CCNode
{
private:
    flownet::ActorID m_ActorID;
    
public:
    HighlightNode();
    virtual ~HighlightNode();
    
    virtual bool init();
    static HighlightNode* create(flownet::ActorID actorID);
    
    virtual void update(float deltaTime);
};

class GuideLineNode : public CCNode
{
private:
    flownet::SpellType m_SpellType;
    CCPoint m_Source;
    CCPoint m_Destination;
    
    CCSprite* m_SpellCastingIcon;
    CCSprite* m_SpellGuideLine;
    CCSprite* m_SpellGuideIcon;
    
public:
    GuideLineNode();
    virtual ~GuideLineNode();
    
    virtual bool init();
    static GuideLineNode* create(flownet::SpellType spellType, CCPoint source, CCPoint destination);
};

class HUDNode : public CCNode
{
private:
    enum {
        PositionX = 0,
        PositionY = -20,
    };
    
private:
    flownet::ActorID m_ActorID;
    
    CCLabelTTF* m_NameLabel;
    
    CCSprite* m_RemainHealthPointBar;
    CCSprite* m_RemainManaPointBar;
    
public:
    HUDNode();
    virtual ~HUDNode();
    
    virtual bool init() override;
    static HUDNode* create(flownet::ActorID actorID);
    
    virtual void update(float deltaTime);
    
    void ChangeHealthPointBar(float scaleFactor);
    void ChangeManaPointBar(float scaleFactor);
};

class ChatBalloonNode : public CCNode
{
private:
    flownet::ActorID m_ActorID;
    flownet::ServerTime m_LastInputTime;
    CCLabelTTF* m_MessageLabel;
    
public:
    ChatBalloonNode();
    virtual ~ChatBalloonNode();
    
    virtual bool init() override;
    
    static ChatBalloonNode* create(flownet::ActorID actorID);
    
    virtual void update(float deltaTime);
    
    void ChangeMessage(flownet::STRING message);
};


class ActorNode : public CCNode
{
protected:
    flownet::ActorID m_ActorID;
    CCSkeletonAnimation* m_Skeleton;
    unsigned int m_StateEffectID;
    
public:
    ActorNode();
    virtual ~ActorNode();
    
    virtual bool init();
    
public:
    void StopAnimationActions();
    
    virtual void AnimateIdle();
    virtual void AnimateMoving();
    virtual void AnimateAttacking();
    virtual void AnimateAttacked();
    virtual void AnimateBeginCasting();
    virtual void AnimateRepeatCasting();
    virtual void AnimateEndCasting();
    virtual void AnimateFire();
    virtual void AnimateDead();
    
    virtual void PlayIdleEffect(bool loop);
    virtual void PlayMovingEffect(bool loop);
    virtual void PlayAttackingEffect(bool loop);
    virtual void PlayAttackedEffect(bool loop);
    virtual void PlayBeginCastingEffect(bool loop);
    virtual void PlayRepeatCastingEffect(bool loop);
    virtual void PlayEndCastingEffect(bool loop);
    virtual void PlayDeadEffect(bool loop);

    virtual void Reload();

    flownet::ActorID GetActorID();
    flownet::Actor* GetActorInfo();

    CCRect GetRect();
    
    virtual float getScale();
    virtual void setScale(float scaleFactor);
    
    CCPoint GetSpellPosition();
    CCPoint GetTopPosition();
    CCPoint GetMidPosition();
    
private:
    void StopCharacterAnimate();
    
    virtual void ChangeEquipment(flownet::EquipmentSlot equipmentSlot, flownet::ItemType itemType);
    virtual void ChangeWand(flownet::ItemType itemType);
    virtual void ChangeHat(flownet::ItemType itemType);
    virtual void ChangeRobe(flownet::ItemType itemType);
    virtual void ChangeCloak(flownet::ItemType itemType);
    virtual void ChangeShoes(flownet::ItemType itemType);
    virtual void ChangeRing(flownet::ItemType itemType);
//    virtual void ChangeHair(flownet::ItemType itemType);
};

#endif /* defined(__GameClientMobile__ActorNode__) */
