//
//  ClientPlayer.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 3/19/13.
//
//

#include "GameClientHeaders.pch"

namespace flownet
{
    
ClientPlayer::ClientPlayer(ActorID playerID, const Gender gender, const POINT spawnPosition, const STRING& gameObjectName):Player(playerID,gender,spawnPosition,gameObjectName)
{
    
}

ClientPlayer::ClientPlayer(const Player& player,const STRING& gameObjectName):Player(player,gameObjectName)
{
    
}

ClientPlayer::~ClientPlayer()
{
    
}
    
bool ClientPlayer::IsStateMoving()
{
    return this->GetActorState() == ActorState_Moving;
}

bool ClientPlayer::IsStateAttacking()
{
    return this->GetActorState() == ActorState_Attacking;
}

bool ClientPlayer::IsStateCasting()
{
    return this->GetActorState() == ActorState_Casting;
}
    
bool ClientPlayer::IsStateIdle()
{
    return this->GetActorState() == ActorState_Idle;
}
    
void ClientPlayer::ChangeToMovingState()
{
    this->SetActorState(ActorState_Moving);
}
    
void ClientPlayer::ChangeToIdleState()
{
    this->SetActorState(ActorState_Idle);
}

void ClientPlayer::ChangeToCastingState()
{
    this->SetActorState(ActorState_Casting);
}

void ClientPlayer::ChangeToAttackingState()
{
    this->SetActorState(ActorState_Attacking);
}

void ClientPlayer::ChangeToDeadState()
{
    this->SetActorState(ActorState_Dead);
}

void ClientPlayer::OnAttacked(FLOAT damageAmount)
{
    this->m_HealthPoint -= damageAmount;
    if(this->m_HealthPoint < 0)
    {
        this->m_HealthPoint = 0;
    }
}


//void ClientPlayer::RequestFireSpell(const ActorID targetID)
//{
//    if( this->SpellStackEmpty() )
//    {
//        return;
//    }
//    
//    GameClient::Instance().GetClientObject().SendCSRequestFireSpell(GameClient::Instance().GetClientStage()->GetStageID(), this->GetActorID(), this->PopSpell(), targetID);
//}

} // namespace flownet

