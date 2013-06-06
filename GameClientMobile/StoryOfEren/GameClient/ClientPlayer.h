//
//  ClientPlayer.h
//  GameClientMobile
//
//  Created by SungJinYoo on 3/19/13.
//
//

#ifndef __GameClientMobile__ClientPlayer__
#define __GameClientMobile__ClientPlayer__

#include "cocos2d.h"

namespace flownet
{
    
class ClientPlayer : public Player, public cocos2d::CCObject
{
public:
    ClientPlayer(ActorID playerID = ActorID_None, const Gender gender=Gender_Male, const ClanID clanID = ClanID_None, const POINT spawnPosition=POINT(5.f,5.f), const STRING& gameObjectName="ClientPlayer");
    ClientPlayer(const Player& player, const STRING& gameObjectName="ClientPlayer");
    virtual ~ClientPlayer();
    
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

public:
    virtual void OnAttacked(FLOAT damageAmount);
    
    virtual BOOL UseItem(Stage* stage, const ItemID itemID) override;
    virtual BOOL EquipItem(Stage* stage, const EquipmentSlot equipmentSlot, const ItemID itemID) override;
    virtual ItemID UnEquipItem(Stage* stage, const EquipmentSlot equipmentSlot) override;
public:
//    void    RequestFireSpell(const ActorID targetID);
};

} // namespace flownet
#endif /* defined(__GameClientMobile__ClientPlayer__) */
