//
//  TesterRPCReceiver.cpp
//  GameClient
//
//  Created by Kim Sinhyub on 5/1/13.
//  Copyright (c) 2013 RoughHands. All rights reserved.
//

#include "GameClientHeaders.pch"

namespace flownet
{

TesterRPCReceiver::TesterRPCReceiver():
    m_IsPlayerInStage(false),
    m_LastStageAITickTime(0),
    m_GameClientObject(nullptr),
    m_UserID(UserID_None),
    m_SessionID(SessionID_NONE),
    m_StageStartTime(0),
    m_StageID(StageID_None),
    m_Stage(nullptr),
    m_PlayerID(ActorID_None),
    m_Player(nullptr),
    m_TargetMonster(nullptr),
    m_CurrentPosition(0.f,0.f),
    m_DestinationPosition(0.f,0.f),
    m_LastMoveRequestTime(0),
    m_LastCastRequestTime(0)
{
}

TesterRPCReceiver::~TesterRPCReceiver()
{
}

void TesterRPCReceiver::Tick()
{
    if( m_IsPlayerInStage == true )
    {
        this->RunStageAI();
    }

}

void TesterRPCReceiver::RunStageAI()
{
    if( m_GameClientObject == nullptr )
    {
        LogSystem::Instance() << " Linked GameClientObject == nullptr " << __FILE__ << __FUNCTION__;
        LogSystem::Instance().Commit();
        return;
    }
    
    if( m_Player == nullptr )
    {
        return;
    }
    
    const ServerTime currentTime = GameClientTester::Instance().GetClientTimer().Check();
    ServerTime timeAfterTick = currentTime - m_LastStageAITickTime;
    if( timeAfterTick < milliseconds(1000) )
    {
        return;
    }
    m_LastStageAITickTime = currentTime;
    
    
    // Cast Spell
    if( m_TargetMonster != nullptr && (m_TargetMonster->GetCurrentPosition().DistanceTo(m_CurrentPosition) < 8.f) )
    {
        ServerTime timeAfterCastRequest = currentTime - m_LastCastRequestTime;
        if( timeAfterCastRequest >= milliseconds(3500) )
        {
            SpellType spell = static_cast<SpellType>(RandomNumberGenerator::Int(0, SpellType_Max-1));
            FLOAT x = RandomNumberGeneratorFloat(-2.f, 2.f)(RandomNumberGenerator::GetGeneratorEngine());
            FLOAT y = RandomNumberGeneratorFloat(-2.f, 2.f)(RandomNumberGenerator::GetGeneratorEngine());
            POINT spellFireVector(x,y);
            
            m_GameClientObject->SendCSRequestEndCast(m_StageID, m_PlayerID, spell, m_CurrentPosition+spellFireVector);
            m_LastCastRequestTime = currentTime;
        }
    }

    
    // Move
    ServerTime timeAfterMoveRequest = currentTime - m_LastMoveRequestTime;
    if( timeAfterMoveRequest >= milliseconds(2000) )
    {
        POINT moveDestination = m_CurrentPosition;
        if( ((RandomNumberGenerator::Int(0, 5) % 5) == 0) )
        {
            const FLOAT randomDistanceUnit = 8.f;
            const FLOAT randomX = RandomNumberGeneratorFloat(-randomDistanceUnit, randomDistanceUnit)(RandomNumberGenerator::GetGeneratorEngine());
            const FLOAT randomY = RandomNumberGeneratorFloat(-randomDistanceUnit, randomDistanceUnit)(RandomNumberGenerator::GetGeneratorEngine());
            moveDestination.x = m_CurrentPosition.x+randomX;
            moveDestination.y = m_CurrentPosition.y+randomY;
            
            if( moveDestination.x != moveDestination.x )
            {
                int i=0;
            }
            if( moveDestination.y != moveDestination.y )
            {
                int i=0;
            }
        }
        else
        {
            FLOAT distanceToMonster = 9999.f;
            if( m_Stage )
            {
                const MonsterMap& monsterMap = m_Stage->GetMonsterMap();
                
                std::for_each(monsterMap.begin(), monsterMap.end(), [this,&distanceToMonster](const MonsterMap::value_type& valuePair){
                    Monster* monster = valuePair.second;
                    if( monster->IsAlive() )
                    {
                        FLOAT distanceTo = this->m_CurrentPosition.DistanceTo(monster->GetCurrentPosition());
                        if( distanceToMonster > distanceTo )
                        {
                            distanceToMonster = distanceTo;
                            this->m_TargetMonster = monster;
                        }
                    }
                });
            }

            if( m_TargetMonster != nullptr )
            {
                POINT targetPosition = m_TargetMonster->GetCurrentPosition();
                POINT directionVector = (targetPosition - m_CurrentPosition).GetNormalizedVector();
                
                const FLOAT desiredDistanceToMonster = 14.f;
                const INT destinationSign = RandomNumberGenerator::Int(0, 2)-1;
                
                
                moveDestination.x = targetPosition.x-destinationSign*directionVector.x*desiredDistanceToMonster;
                moveDestination.y = targetPosition.y-destinationSign*directionVector.y*desiredDistanceToMonster;
                if( moveDestination.x != moveDestination.x )
                {
                    int i=0;
                }
                if( moveDestination.y != moveDestination.y )
                {
                    int i=0;
                }

            }
        }
        
        m_DestinationPosition = moveDestination;
        if( m_DestinationPosition.x > m_Stage->GetWidth() )
        {
            m_DestinationPosition.x = m_Stage->GetWidth();
        }
        if( m_DestinationPosition.x < 0.f )
        {
            m_DestinationPosition.x = 0.f;
        }
        if( m_DestinationPosition.y > m_Stage->GetHeight() )
        {
            m_DestinationPosition.y = m_Stage->GetHeight();
        }
        if( m_DestinationPosition.y < 0.f )
        {
            m_DestinationPosition.y = 0.f;
        }
        
        if( m_DestinationPosition.x != m_DestinationPosition.x )
        {
            int i=0;
        }
        if( m_DestinationPosition.y != m_DestinationPosition.y )
        {
            int i=0;
        }
        m_GameClientObject->SendCSRequestMoveActor(m_StageID, m_PlayerID, m_CurrentPosition, m_DestinationPosition);
        m_LastMoveRequestTime = currentTime;
    }
    
    
    if( currentTime - m_StageStartTime > milliseconds(240000) )
    {
        m_StageID = StageID_None;
        m_Stage = nullptr;
        m_Player = nullptr;
        m_TargetMonster = nullptr;
        
        m_GameClientObject->SendCSRequestExitStage(m_StageID);
        m_StageStartTime = currentTime;
    }
}


void TesterRPCReceiver::OnSCProtocolError() const
{
}

void TesterRPCReceiver::OnSCResponseConnect(flownet::ConnectionID connectionID) const 
{
}

void TesterRPCReceiver::OnSCResponseSession(flownet::UserID userID, flownet::ActorID myPlayerID, flownet::SessionID sessionID) const 
{
    m_UserID = userID;
    m_PlayerID = myPlayerID;
}

void TesterRPCReceiver::OnSCResponseLogInWithOTP(flownet::UserID userID, flownet::ActorID playerID, flownet::SessionID sessionID) const
{
    m_UserID = userID;
    m_PlayerID = playerID;
    m_SessionID = sessionID;
}

void TesterRPCReceiver::OnSCResponseHeartbeat(flownet::INT64 heartbeatCountAck) const 
{
}
void TesterRPCReceiver::OnSCResponseLogOutUserAccount(flownet::UserID userID) const 
{
}

void TesterRPCReceiver::OnSCResponseCreateStage(flownet::StageID stageID, flownet::Stage stage) const 
{
    m_IsPlayerInStage = true;
    m_StageID = stageID;
    m_StageStartTime = GameClientTester::Instance().GetClientTimer().Check();
    
    if( m_Stage != nullptr )
    {
        delete m_Stage;
        m_Stage = nullptr;
        m_TargetMonster = nullptr;
    }
    m_Stage = new ClientStage(stage);
}

void TesterRPCReceiver::OnSCResponseGetStagePlayInfoList(flownet::StagePlayInfoList stageInfoList) const
{
}

void TesterRPCReceiver::OnSCResponseJoinRunningStage(flownet::StageID stageID, flownet::Stage stage, flownet::ErrorType errorCode) const 
{
    m_IsPlayerInStage = true;
    m_StageID = stageID;
    m_StageStartTime = GameClientTester::Instance().GetClientTimer().Check();

    if( m_Stage != nullptr )
    {
        delete m_Stage;
        m_Stage = nullptr;
        m_TargetMonster = nullptr;
        delete m_Player;
        m_Player = nullptr;
    }
    m_Stage = new ClientStage(stage);
}

void TesterRPCReceiver::OnSCResponseExitStage(flownet::StageID stageID, flownet::ActorID myPlayerID) const 
{
    m_IsPlayerInStage = false;
    m_StageID = StageID_None;
    
    if( m_Stage != nullptr )
    {
        delete m_Stage;
        m_Stage = nullptr;
        m_TargetMonster = nullptr;
        delete m_Player;
        m_Player = nullptr;
    }
    
    
    m_GameClientObject->SendCSRequestCreateStage(StageType_MushroomField, GPSPoint(0.f,0.f,0.f));
}

void TesterRPCReceiver::OnSCNotifyExitStage(flownet::StageID stageID, flownet::ActorID playerID) const 
{
    if( playerID == m_PlayerID )
    {
        m_IsPlayerInStage = false;
        m_StageID = StageID_None;
        
        if( m_Stage != nullptr )
        {
            delete m_Stage;
            m_Stage = nullptr;
            m_TargetMonster = nullptr;
            delete m_Player;
            m_Player = nullptr;
        }
    }
    else
    {
        if( m_Stage != nullptr )
        {
            if( IsMonsterID(playerID) )
            {
                if( (m_TargetMonster!=nullptr) && playerID == m_TargetMonster->GetActorID() )
                {
                    m_TargetMonster = nullptr;
                }
                m_Stage->RemoveMonster(playerID);

            }
            else if ( IsPlayerID(playerID) )
            {
                m_Stage->RemovePlayer(playerID);
            }
        }
    }
        
}

void TesterRPCReceiver::OnSCResponseRejoinCurrentStage(flownet::StageID stageID, flownet::Stage stage) const 
{
    m_IsPlayerInStage = true;
    m_StageID = stageID;
    m_StageStartTime = GameClientTester::Instance().GetClientTimer().Check();
    
    if( m_Stage != nullptr )
    {
        delete m_Stage;
        m_Stage = nullptr;
        m_TargetMonster = nullptr;
        delete m_Player;
        m_Player = nullptr;
    }
    m_Stage = new ClientStage(stage);

}

void TesterRPCReceiver::OnSCNotifyClearStage(flownet::StageID stageID) const 
{
}

void TesterRPCReceiver::OnSCNotifySpawnMonster(flownet::StageID stageID, flownet::Monster newMonster) const 
{
    if( m_Stage != nullptr )
    {
        Monster* monster = new Monster(newMonster);
        m_Stage->AddMonster(monster->GetActorID(), monster);
    }
}

void TesterRPCReceiver::OnSCNotifySpawnPlayer(flownet::StageID stageID, flownet::Player newPlayer) const 
{
    if( newPlayer.GetActorID() == m_PlayerID )
    {
        m_Player = new Player(newPlayer);
        m_CurrentPosition = newPlayer.GetCurrentPosition();
        m_DestinationPosition = newPlayer.GetDestinationPosition();
    }
    
    if( m_Stage != nullptr )
    {
        Player* player = new Player(newPlayer);
        m_Stage->AddPlayer(player->GetActorID(), player);
    }
}

void TesterRPCReceiver::OnSCNotifyMoveActor(flownet::StageID stageID, flownet::ActorID playerID, flownet::POINT currentPosition, flownet::POINT destinationPosition) const 
{
    if( m_PlayerID == playerID )
    {
        m_CurrentPosition = currentPosition;
        m_DestinationPosition = destinationPosition;
    }
}

void TesterRPCReceiver::OnSCNotifyTeleportActor(flownet::StageID stageID, flownet::ActorID playerID, flownet::POINT currentPosition, flownet::POINT destinationPosition) const
{
    if( m_PlayerID == playerID)
    {
        m_CurrentPosition = currentPosition;
        m_DestinationPosition = destinationPosition;
    }
}

void TesterRPCReceiver::OnSCNotifyActorAttributeChanged(flownet::StageID stageID, flownet::ActorID actorID, flownet::ActorAttribute actorAttribute, flownet::FLOAT amount) const 
{
}

void TesterRPCReceiver::OnSCNotifyActorAttack(flownet::StageID stageID, flownet::ActorID actorID, flownet::ActorID targetID, flownet::AttackType attackType) const 
{
}

void TesterRPCReceiver::OnSCNotifyActorAttacked(flownet::StageID stageID, flownet::ActorID actorID, flownet::ActorID attackerID, flownet::AttackType attackType) const 
{
}

void TesterRPCReceiver::OnSCNotifyActorDead(flownet::StageID stageID, flownet::ActorID actorID) const 
{
}

void TesterRPCReceiver::OnSCNotifyChangeTarget(flownet::StageID stageID, flownet::ActorID actorID, flownet::ActorID targetID) const 
{
}

void TesterRPCReceiver::OnSCNotifyBeginCast(flownet::StageID stageID, flownet::ActorID playerID, flownet::SpellType spellType, flownet::POINT destination) const 
{
}

void TesterRPCReceiver::OnSCNotifyEndCast(flownet::StageID stageID, flownet::ActorID playerID, flownet::SpellType spellType, flownet::POINT destination) const 
{
}

void TesterRPCReceiver::OnSCResponseDropItemToField(flownet::StageID stageID, flownet::ActorID playerID, flownet::ItemID itemID) const 
{
}

void TesterRPCReceiver::OnSCNotifySpawnItem(flownet::StageID stageID, flownet::Item item, flownet::POINT spawnPosition) const 
{
}

void TesterRPCReceiver::OnSCNotifyPickUpItemFromField(flownet::StageID stageID, flownet::ActorID playerID, flownet::ItemID itemID) const 
{
}

void TesterRPCReceiver::OnSCNotifyAddItemToStash(flownet::StageID stageID, flownet::ActorID playerID, flownet::Item item) const 
{
}

void TesterRPCReceiver::OnSCNotifyRemoveItemFromStash(flownet::StageID stageID, flownet::ActorID playerID, flownet::ItemID itemID) const 
{
}

void TesterRPCReceiver::OnSCResponseRegisterStashItemToInventory(flownet::StageID stageID, flownet::ActorID playerID, flownet::ItemID itemID, ItemGroup itemGroup, flownet::InventorySlot inventorySlot) const
{
}

void TesterRPCReceiver::OnSCResponseUnRegisterStashItemFromInventory(flownet::StageID stageID, flownet::ActorID playerID, flownet::ItemID itemID, ItemGroup itemGroup, flownet::InventorySlot inventorySlot) const
{
}

void TesterRPCReceiver::OnSCResponseSwapInventorySlot(flownet::StageID stageID, flownet::ActorID playerID, ItemGroup itemGroup, flownet::InventorySlot sourceSlot, flownet::InventorySlot destinationSlot) const
{
}

void TesterRPCReceiver::OnSCNotifyUseItem(flownet::StageID stageID, flownet::ActorID playerID, flownet::ItemID itemID) const 
{
}

void TesterRPCReceiver::OnSCNotifyUnEquipItem(flownet::StageID stageID, flownet::ActorID playerID, flownet::EquipmentSlot equipmentSlot) const
{
}

void TesterRPCReceiver::OnSCNotifyEquipItem(flownet::StageID stageID, flownet::ActorID playerID, flownet::ItemID itemID, flownet::EquipmentSlot equipedSlot) const
{
}

void TesterRPCReceiver::OnSCNotifySendMessageToStagePlayers(flownet::StageID stageID, flownet::ActorID playerID, flownet::STRING message) const
{
}

void TesterRPCReceiver::OnSCNotifyResetActorAttributeAmplifier(flownet::StageID stageID, flownet::ActorID actorID) const
{
}

void TesterRPCReceiver::OnSCNotifySpawnNPC(long long, flownet::NPC) const
{
}

void TesterRPCReceiver::OnSCNotifySetFreeze(long long, long long, bool) const
{
}

void TesterRPCReceiver::OnFCNotifyFCErrorMessage(flownet::FCErrorMessage, std::__1::basic_string<char, std::__1::char_traits<char>, flownet::STLAllocator<char> >) const
{
}

void TesterRPCReceiver::OnSCNotifyKnockBackActor(long long, long long, flownet::POINT, flownet::POINT) const
{
}

void TesterRPCReceiver::OnSCNotifySCErrorMessage(flownet::SCErrorMessage, std::__1::basic_string<char, std::__1::char_traits<char>, flownet::STLAllocator<char> >) const
{
}

void TesterRPCReceiver::OnSCResponseCreatePlayer(long long, long long, long long) const
{
}

void TesterRPCReceiver::OnSCNotifyRunOutSpellMana(long long, long long) const
{
}

void TesterRPCReceiver::OnSCNotifyRunOutMovingMana(flownet::StageID stageID, flownet::ActorID actorID) const
{
    if( actorID == m_PlayerID )
    {
    
    }
}

void TesterRPCReceiver::OnSCNotifySpawnStageObject(long long, flownet::StageObject) const
{
}

void TesterRPCReceiver::OnSCNotifyApplySpellAbility(long long, long long, long long, flownet::SpellAbility, float) const
{
}

void TesterRPCReceiver::OnSCNotifyClearSpellAbility(long long, long long, flownet::SpellAbility, float) const
{
}

////////////////////////////////////////

void TesterRPCReceiver::OnFCProtocolError() const
{
}

void TesterRPCReceiver::OnFCResponseConnect(flownet::ConnectionID feConnectionID) const 
{
}

void TesterRPCReceiver::OnFCResponseCreateUserAccount(flownet::UserID userID) const 
{
}

void TesterRPCReceiver::OnFCResponseLogInUserAccount(flownet::UserID userID, flownet::GameServerID gameServerID, flownet::STRING gameServerIP, flownet::OTP otp) const
{
    // Implemented on FCPacketHandler
}

} // namespace flownet
