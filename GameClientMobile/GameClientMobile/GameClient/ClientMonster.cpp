//
//  ClientMonster.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 3/19/13.
//
//

#include "GameClientHeaders.pch"

namespace flownet
{
    
ClientMonster::ClientMonster(const MonsterType monsterType, const ActorID monsterID,const POINT spawnPosition, const STRING& gameObjectName): Monster(monsterType, monsterID, spawnPosition, gameObjectName)
{
    
}

ClientMonster::ClientMonster(const Monster& monster,const STRING& gameObjectName) : Monster(monster,gameObjectName)
{
    
}

ClientMonster::~ClientMonster()
{
    
}

bool ClientMonster::IsStateMoving()
{
    return this->GetActorState() == ActorState_Moving;
}

bool ClientMonster::IsStateAttacking()
{
    return this->GetActorState() == ActorState_Attacking;
}

bool ClientMonster::IsStateCasting()
{
    return this->GetActorState() == ActorState_Casting;
}

bool ClientMonster::IsStateIdle()
{
    return this->GetActorState() == ActorState_Idle;
}
    
void ClientMonster::ChangeToMovingState()
{
    this->SetActorState(ActorState_Moving);
}

void ClientMonster::ChangeToIdleState()
{
    this->SetActorState(ActorState_Idle);
}

void ClientMonster::ChangeToCastingState()
{
    this->SetActorState(ActorState_Casting);
}

void ClientMonster::ChangeToAttackingState()
{
    this->SetActorState(ActorState_Attacking);
}

void ClientMonster::ChangeToDeadState()
{
    this->m_HealthPoint = 0;
    this->SetActorState(ActorState_Dead);
}

void ClientMonster::OnAttacked(FLOAT damageAmount)
{
    this->m_HealthPoint -= damageAmount;
    if(this->m_HealthPoint < 0)
    {
        this->m_HealthPoint = 0;
    }
}

} // namespace flownet