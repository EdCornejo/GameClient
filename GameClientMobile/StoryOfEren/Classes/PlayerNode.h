//
//  PlayerNode.h
//  GameClientMobile
//
//  Created by SungJinYoo on 3/7/13.
//
//

#ifndef __GameClientMobile__PlayerNode__
#define __GameClientMobile__PlayerNode__

class PlayerNode : public ActorNode
{
public:
    PlayerNode();
    virtual ~PlayerNode();
    
    virtual bool init();
    virtual bool initWithGender(flownet::Gender gender);

    static PlayerNode* create(flownet::ActorID);
    static PlayerNode* create(flownet::Gender gender);

//    virtual void PlayIdleEffect(bool loop) override;
    virtual void PlayMovingEffect(bool loop) override;
//    virtual void PlayAttackingEffect(bool loop) override;
//    virtual void PlayAttackedEffect(bool loop) override;
//    virtual void PlayBeginCastingEffect(bool loop) override;
//    virtual void PlayRepeatCastingEffect(bool loop) override;
//    virtual void PlayEndCastingEffect(bool loop) override;
//    virtual void PlayDeadEffect(bool loop) override;

    virtual void ChangeWand(flownet::ItemType itemType);
    virtual void ChangeHat(flownet::ItemType itemType);
    virtual void ChangeRobe(flownet::ItemType itemType);
    virtual void ChangeCloak(flownet::ItemType itemType);
    virtual void ChangeShoes(flownet::ItemType itemType);
    virtual void ChangeRing(flownet::ItemType itemType);
    
    void ChangeGender(flownet::Gender gender);
};

#endif /* defined(__GameClientMobile__PlayerNode__) */
