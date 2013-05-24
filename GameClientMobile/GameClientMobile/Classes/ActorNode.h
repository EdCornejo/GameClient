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


class ShadowNode : public CCNode
{
private:
    flownet::ActorID m_ActorID;
    
public:
    ShadowNode();
    virtual ~ShadowNode();
    
    virtual bool init();
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
    
    CCNode* m_HidingPart;
    CCSprite* m_RemainHealthPointBar;
    CCSprite* m_GreenBar;
    CCSprite* m_YellowBar;
    CCSprite* m_RedBar;
    CCSprite* m_DamagedHealthPointBar;
    CCSprite* m_RemainManaPointBar;
    CCSprite* m_DrainedManaPointBar;
    
public:
    HUDNode();
    virtual ~HUDNode();
    
    virtual bool init() override;
    static HUDNode* create(flownet::ActorID actorID);
    
    virtual void update(float deltaTime);
    
    void ChangeHealthPointBar(float scaleFactor);
    void ChangeManaPointBar(float scaleFactor);
    
private:
    void ShowHUD();
    void HideHUD();
};



class ActorNode : public CCNode
{
protected:
    flownet::ActorID m_ActorID;
    CCSkeletonAnimation* m_Skeleton;

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
