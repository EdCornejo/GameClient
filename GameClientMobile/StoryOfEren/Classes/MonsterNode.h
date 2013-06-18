//
//  MonsterNode.h
//  GameClientMobile
//
//  Created by SungJinYoo on 3/7/13.
//
//

#ifndef __GameClientMobile__MonsterNode__
#define __GameClientMobile__MonsterNode__

class MonsterNode : public ActorNode
{
public:
    MonsterNode();
    virtual ~MonsterNode();

    virtual bool init();
    
    static MonsterNode* create(flownet::ActorID actorID);
    
    virtual void PlayIdleEffect(bool loop) override;
    virtual void PlayMovingEffect(bool loop) override;
    virtual void PlayAttackingEffect(bool loop) override;
    virtual void PlayAttackedEffect(bool loop) override;
    virtual void PlayBeginCastingEffect(bool loop) override;
    virtual void PlayRepeatCastingEffect(bool loop) override;
    virtual void PlayEndCastingEffect(bool loop) override;
    virtual void PlayDeadEffect(bool loop) override;

};

#endif /* defined(__GameClientMobile__MonsterNode__) */
