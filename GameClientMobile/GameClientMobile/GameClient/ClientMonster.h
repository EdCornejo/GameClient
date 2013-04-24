//
//  ClientMonster.h
//  GameClientMobile
//
//  Created by SungJinYoo on 3/19/13.
//
//

#ifndef __GameClientMobile__ClientMonster__
#define __GameClientMobile__ClientMonster__

#include "cocos2d.h"

namespace flownet
{
    
class ClientMonster : public Monster, public cocos2d::CCObject
{
public:
    ClientMonster(const MonsterType monsterType=MonsterType_None, const ActorID monsterID=ActorID_None, const POINT spawnPosition = POINT(5.f,5.f), const STRING& gameObjectName="ClientMonster");
    ClientMonster(const Monster& monster,const STRING& gameObjectName="ClientMonster");
    virtual ~ClientMonster();
    
public:
    bool IsStateMoving();
    bool IsStateAttacking();
    bool IsStateCasting();
    bool IsStateIdle();
    bool IsStateFire();
    
    void ChangeToMovingState();
    void ChangeToIdleState();
    void ChangeToCastingState();
    void ChangeToAttackingState();
    void ChangeToDeadState();
    
    void OnAttacked(FLOAT damageAmount);
    
};

} // namespace flownet
#endif /* defined(__GameClientMobile__ClientMonster__) */
