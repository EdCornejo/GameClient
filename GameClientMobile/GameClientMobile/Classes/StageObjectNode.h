//
//  StageObjectNode.h
//  GameClientMobile
//
//  Created by SungJinYoo on 5/28/13.
//
//

#ifndef __GameClientMobile__StageObjectNode__
#define __GameClientMobile__StageObjectNode__

class StageObjectNode : public ActorNode
{
private:
    CCSprite* m_Body;
    
public:
    StageObjectNode();
    virtual ~StageObjectNode();
    
    virtual bool init() override;
    
    static StageObjectNode* create(flownet::ActorID actorID);
    
    virtual void AnimateIdle() override;
    virtual void AnimateMoving() override;
    virtual void AnimateAttacking() override;
    virtual void AnimateAttacked() override;
    virtual void AnimateBeginCasting() override;
    virtual void AnimateRepeatCasting() override;
    virtual void AnimateEndCasting() override;
    virtual void AnimateFire() override;
    virtual void AnimateDead() override;
};

#endif /* defined(__GameClientMobile__StageObjectNode__) */
