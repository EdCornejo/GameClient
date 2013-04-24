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

typedef int WeaponType;
typedef int HatType;
typedef int ClothType;

class ActorNode : public CCNode
{
private:
    enum{
        HP_BAR_REMAIN_TAG,
        HP_BAR_CONSUMED_TAG,
        MP_BAR_REMAIN_TAG,
        MP_BAR_CONSUMED_TAG,
    };

private:
    flownet::ActorID m_ActorID;
    CCSprite* m_HUD;
    CCSprite* m_SpellGuideLine;
    CCSprite* m_SpellGuideIcon;

protected:
    CCSkeleton* m_Skeleton;

public:
    ActorNode(flownet::ActorID actorID);
    ActorNode(flownet::Actor* player);
    virtual ~ActorNode();
    
public:
    void HighLight();
    
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

    void ChangeWeapon(WeaponType weaponType);
    void ChangeHat(HatType hatType);
    void ChangeCloth(ClothType clothType);

    flownet::ActorID GetActorID();
    flownet::Actor* GetActorInfo();

    CCRect GetRect();
    
public:
    void ChangeHealthPointBar(float scaleFactor);
    void ChangeManaPointBar(float scaleFactor);
    void ShowSpellGuide(flownet::SpellType spellType, CCPoint destination);
    void HideSpellGuide();
    
private:
    virtual CCSprite* LoadHighLightImage() = 0;
    void InitializeHUD();
    void ShowHUD();
    void HideHUD();
    void StopCharacterAnimate();
};

#endif /* defined(__GameClientMobile__ActorNode__) */
