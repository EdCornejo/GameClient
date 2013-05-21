//
//  GameClientPacketHandler.cpp
//  GameClient
//
//  Created by Sinhyub Kim on 2/13/13.
//  Copyright (c) 2013 RoughHands. All rights reserved.
//

#include "GameClientHeaders.pch"

namespace flownet
{

GameClientPacketHandler::PacketHandlerFunction* GameClientPacketHandler::m_HandlerMap = nullptr;

GameClientPacketHandler::GameClientPacketHandler(RenderingTaskWorkerRoutine* renderingTaskWorkerRoutine):PacketHandler(),m_GameClientObject(nullptr),m_RenderingTaskWorkerRoutine(renderingTaskWorkerRoutine),m_GameClientRPCReceiver(nullptr)
{
    InitializeHandlerMap();
}

GameClientPacketHandler::~GameClientPacketHandler()
{
    if( m_HandlerMap == nullptr )
    {
        delete []m_HandlerMap;
        m_HandlerMap = nullptr;
    }
}

void GameClientPacketHandler::LinkGameClientObject(GameClientObject* gameClientObject)
{
    ASSERT_DEBUG(m_GameClientObject==nullptr);
    m_GameClientObject = gameClientObject;
}

void GameClientPacketHandler::BindHandlerFunction(INT protocolNumber, const PacketHandlerFunction& packetHandlerFunction)
{
	m_HandlerMap[protocolNumber] = packetHandlerFunction;
}

void GameClientPacketHandler::SetGameClientRPCReceiver(const GameClientRPCInterface* gameClientRPCReceiver)
{
    ASSERT_RELEASE(m_GameClientRPCReceiver==nullptr);
    
    m_GameClientRPCReceiver = gameClientRPCReceiver;
}


void GameClientPacketHandler::HandlePacket(BasePacket* packet)
{
#ifdef PACKET_SIZE_LOGGING
    // Add Packet Size Logger Here
    PacketSizeLogger* packetSizeLogger = PacketSizeLogger::Instance();
    packetSizeLogger->AddPacketSize(packet->GetPacketSize());
    // end of Add Packet Size Logger Here
#endif

    GameSCProtocol protocol = static_cast<GameSCProtocol>(packet->GetProtocol());
    LOG_STDOUT( std::cout << "handled Protocol" << protocol << std::endl; );
    
    (this->*m_HandlerMap[protocol])(static_cast<GamePacket*>(packet));

    // 'packet' will be removed (deleted) after this return on packet parser
}

//void GameClientPacketHandler::SCProtocolErrorHandler(GamePacket* packet)
//{
//    m_RenderingTaskWorkerRoutine->AddTask(CreateLambdaTask("SCProtocolErrorHandler",[this](){
//        this->OnSCProtocolError();
//    }));
//}

void GameClientPacketHandler::OnSCProtocolError()
{
    ASSERT_DEBUG(this->m_GameClientRPCReceiver != nullptr );
    
    LOG_STDOUT( std::cout << "Wrong Protocol. Error occured " << std::endl; );
    
    this->m_GameClientRPCReceiver->OnSCProtocolError();
}
   
void GameClientPacketHandler::OnSCResponseConnect(ConnectionID connectionID)
{
    ASSERT_DEBUG(this->m_GameClientRPCReceiver != nullptr );
    
    // Do Handle Game Jobs
    m_GameClientObject->SetConnectionID(connectionID);
    m_GameClientObject->StartHeartbeat();

    this->m_GameClientRPCReceiver->OnSCResponseConnect(connectionID);
}

void GameClientPacketHandler::OnSCNotifySCErrorMessage(SCErrorMessage scErrorMessage, STRING errorMessage)
{
    ASSERT_DEBUG(this->m_GameClientRPCReceiver != nullptr);
    this->m_GameClientRPCReceiver->OnSCNotifySCErrorMessage(scErrorMessage, errorMessage);
}

void GameClientPacketHandler::OnSCResponseSession(UserID userID, ActorID myPlayerID, SessionID sessionID)
{
    GameClient::Instance().SetSessionID(sessionID);
    
    ASSERT_DEBUG(this->m_GameClientRPCReceiver != nullptr );
    this->m_GameClientRPCReceiver->OnSCResponseSession(userID, myPlayerID, sessionID);
}

void GameClientPacketHandler::OnSCResponseCreatePlayer(UserID userID, ActorID playerID, SessionID sessionID)
{
    ASSERT_DEBUG(this->m_GameClientRPCReceiver != nullptr);
    this->m_GameClientRPCReceiver->OnSCResponseCreatePlayer(userID, playerID, sessionID);
}

void GameClientPacketHandler::OnSCResponseHeartbeat(INT64 heartbeatCountAck)
{
    // Delay checking
    DelayChecker* checker = DelayChecker::Instance();
    checker->CheckEndTime();
    // end of Delay checking

    if( heartbeatCountAck%100 == 0)
    {
        std::cout << "fSCResponseHeartbeat : heartbeatCountAck" << heartbeatCountAck << std::endl;
    }

    
//    GameClientObject->Send
//    GameClient::Instance().GetClientObject().CheckHeartbeatTimeOver();
    GameClient::Instance().GetClientObject().KeepHeartbeat(heartbeatCountAck);
    
    ASSERT_DEBUG(this->m_GameClientRPCReceiver != nullptr );
    this->m_GameClientRPCReceiver->OnSCResponseHeartbeat(heartbeatCountAck);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
//void GameClientPacketHandler::OnSCResponseCreateUserAccount(UserID userID)
//{
//    ASSERT_DEBUG(this->m_GameClientRPCReceiver != nullptr);
//    this->m_GameClientRPCReceiver->OnSCResponseCreateUserAccount(userID);
//}
//    
//void GameClientPacketHandler::OnSCResponseLogInUserAccount(UserID userID, ActorID playerID, SessionID sessionID)
//{
//    ASSERT_DEBUG(this->m_GameClientRPCReceiver != nullptr);
//    this->m_GameClientRPCReceiver->OnSCResponseLogInUserAccount(userID, playerID, sessionID);
//}
//
//void GameClientPacketHandler::OnSCResponseLogOutUserAccount(UserID userID)
//{
//    ASSERT_DEBUG(this->m_GameClientRPCReceiver != nullptr);
//    this->m_GameClientRPCReceiver->OnSCResponseLogOutUserAccount(userID);
//}

void GameClientPacketHandler::OnSCResponseLogInWithOTP(UserID userID, ActorID playerID, SessionID sessionID)
{   
    ASSERT_DEBUG(m_GameClientRPCReceiver != nullptr);
    m_GameClientRPCReceiver->OnSCResponseLogInWithOTP(userID, playerID, sessionID);
}

void GameClientPacketHandler::OnSCResponseLogOutUserAccount(UserID userID)
{
    ASSERT_DEBUG(m_GameClientRPCReceiver!=nullptr);
    m_GameClientRPCReceiver->OnSCResponseLogOutUserAccount(userID);
}

// stage
void GameClientPacketHandler::OnSCResponseCreateStage(StageID stageID, Stage stage)
{
    ASSERT_DEBUG(m_GameClientRPCReceiver != nullptr);
    m_GameClientRPCReceiver->OnSCResponseCreateStage(stageID, stage);
}

void GameClientPacketHandler::OnSCResponseRunningStages(StageInfoList stageInfoList)
{
    ASSERT_DEBUG(this->m_GameClientRPCReceiver != nullptr);
    this->m_GameClientRPCReceiver->OnSCResponseRunningStages(stageInfoList);
}

void GameClientPacketHandler::OnSCResponseJoinRunningStage(StageID stageID, Stage stage, ErrorType errorType)
{
    ASSERT_DEBUG(this->m_GameClientRPCReceiver != nullptr);
    this->m_GameClientRPCReceiver->OnSCResponseJoinRunningStage(stageID, stage, errorType);
}

void GameClientPacketHandler::OnSCResponseExitStage(StageID stageID, ActorID playerID)
{
    ASSERT_DEBUG(this->m_GameClientRPCReceiver != nullptr);
    this->m_GameClientRPCReceiver->OnSCResponseExitStage(stageID, playerID);
}

void GameClientPacketHandler::OnSCNotifyExitStage(StageID stageID, ActorID playerID)
{
    ASSERT_DEBUG(this->m_GameClientRPCReceiver != nullptr);
    this->m_GameClientRPCReceiver->OnSCNotifyExitStage(stageID, playerID);
}

void GameClientPacketHandler::OnSCResponseRejoinCurrentStage(StageID stageID, Stage stage)
{
    ASSERT_DEBUG(this->m_GameClientRPCReceiver != nullptr);
    this->m_GameClientRPCReceiver->OnSCResponseRejoinCurrentStage(stageID, stage);
}

void GameClientPacketHandler::OnSCNotifyClearStage(StageID stageID)
{
    ASSERT_DEBUG(this->m_GameClientRPCReceiver != nullptr);
    this->m_GameClientRPCReceiver->OnSCNotifyClearStage(stageID);
}

void GameClientPacketHandler::OnSCNotifySpawnPlayer(StageID stageID, Player player)
{
    ASSERT_DEBUG(this->m_GameClientRPCReceiver != nullptr);
    this->m_GameClientRPCReceiver->OnSCNotifySpawnPlayer(stageID, player);
}

void GameClientPacketHandler::OnSCNotifySpawnMonster(StageID stageID, Monster monster)
{
    ASSERT_DEBUG(this->m_GameClientRPCReceiver != nullptr);
    this->m_GameClientRPCReceiver->OnSCNotifySpawnMonster(stageID, monster);
}    
    
void GameClientPacketHandler::OnSCNotifyMoveActor(StageID stageID, ActorID playerID, flownet::POINT currentPosition, flownet::POINT destinationPosition)
{
    ASSERT_DEBUG(m_GameClientRPCReceiver != nullptr);
    m_GameClientRPCReceiver->OnSCNotifyMoveActor(stageID, playerID, currentPosition, destinationPosition);
}

void GameClientPacketHandler::OnSCNotifyActorAttributeChanged(StageID stageID, ActorID actorID, ActorAttribute actorAttribute, FLOAT amount)
{
    ASSERT_DEBUG(m_GameClientRPCReceiver != nullptr);
    m_GameClientRPCReceiver->OnSCNotifyActorAttributeChanged(stageID, actorID, actorAttribute, amount);
}

void GameClientPacketHandler::OnSCNotifyResetActorAttributeAmplifier(StageID stageID, ActorID actorID)
{
    ASSERT_DEBUG(this->m_GameClientRPCReceiver != nullptr);
    this->m_GameClientRPCReceiver->OnSCNotifyResetActorAttributeAmplifier(stageID, actorID);
}

void GameClientPacketHandler::OnSCNotifyActorAttack(StageID stageID, ActorID actorID, ActorID targetPlayerID, AttackType attackType)
{
    ASSERT_DEBUG(m_GameClientRPCReceiver != nullptr);
    m_GameClientRPCReceiver->OnSCNotifyActorAttack(stageID, actorID, targetPlayerID, attackType);
}
    
void GameClientPacketHandler::OnSCNotifyActorAttacked(StageID stageID, ActorID actorID, ActorID attackerActorID, AttackType attackType)
{
    ASSERT_DEBUG(m_GameClientRPCReceiver != nullptr);
    m_GameClientRPCReceiver->OnSCNotifyActorAttacked(stageID, actorID, attackerActorID, attackType);
}

void GameClientPacketHandler::OnSCNotifyActorDead(StageID stageID, ActorID actorID)
{
    ASSERT_DEBUG(m_GameClientRPCReceiver != nullptr);
    m_GameClientRPCReceiver->OnSCNotifyActorDead(stageID, actorID);
}

void GameClientPacketHandler::OnSCNotifyChangeTarget(StageID stageID, ActorID actorID, ActorID targetID)
{
    ASSERT_DEBUG(this->m_GameClientRPCReceiver != nullptr);
    this->m_GameClientRPCReceiver->OnSCNotifyChangeTarget(stageID, actorID, targetID);
}

void GameClientPacketHandler::OnSCNotifyBeginCast(StageID stageID, ActorID myPlayerID, SpellType spellType, POINT destination)
{
    ASSERT_DEBUG(this->m_GameClientRPCReceiver != nullptr);
    this->m_GameClientRPCReceiver->OnSCNotifyBeginCast(stageID, myPlayerID, spellType, destination);
}

void GameClientPacketHandler::OnSCNotifyEndCast(StageID stageID, ActorID playerID, SpellType spellType, POINT destination)
{
    ASSERT_DEBUG(m_GameClientRPCReceiver != nullptr);
    m_GameClientRPCReceiver->OnSCNotifyEndCast(stageID, playerID, spellType, destination);
}

// TO DO : do automation on these functions
void GameClientPacketHandler::OnSCResponseDropItemToField(flownet::StageID stageID, flownet::ActorID playerID, flownet::ItemID itemID)
{
    ASSERT_DEBUG(m_GameClientRPCReceiver != nullptr);
    m_GameClientRPCReceiver->OnSCResponseDropItemToField(stageID, playerID, itemID);
}

void GameClientPacketHandler::OnSCNotifySpawnItem(flownet::StageID stageID, flownet::Item item, flownet::POINT spawnPosition)
{
    ASSERT_DEBUG(m_GameClientRPCReceiver != nullptr);
    m_GameClientRPCReceiver->OnSCNotifySpawnItem(stageID, item, spawnPosition);
}

void GameClientPacketHandler::OnSCNotifyPickUpItemFromField(flownet::StageID stageID, flownet::ActorID playerID, flownet::ItemID itemID)
{
    ASSERT_DEBUG(m_GameClientRPCReceiver != nullptr);
    m_GameClientRPCReceiver->OnSCNotifyPickUpItemFromField(stageID, playerID, itemID);
}

void GameClientPacketHandler::OnSCNotifyAddItemToStash(flownet::StageID stageID, flownet::ActorID playerID, flownet::Item item)
{
    ASSERT_DEBUG(m_GameClientRPCReceiver != nullptr);
    m_GameClientRPCReceiver->OnSCNotifyAddItemToStash(stageID, playerID, item);
}

void GameClientPacketHandler::OnSCNotifyRemoveItemFromStash(flownet::StageID stageID, flownet::ActorID playerID, flownet::ItemID itemID)
{
    ASSERT_DEBUG(m_GameClientRPCReceiver != nullptr);
    m_GameClientRPCReceiver->OnSCNotifyRemoveItemFromStash(stageID, playerID, itemID);
}

void GameClientPacketHandler::OnSCResponseRegisterStashItemToInventory(flownet::StageID stageID, flownet::ActorID playerID, flownet::ItemID itemID, flownet::ItemGroup itemGroup, flownet::InventorySlot slotNumber)
{
    ASSERT_DEBUG(m_GameClientRPCReceiver != nullptr);
    m_GameClientRPCReceiver->OnSCResponseRegisterStashItemToInventory(stageID, playerID, itemID, itemGroup, slotNumber);
}

void GameClientPacketHandler::OnSCResponseUnRegisterStashItemFromInventory(flownet::StageID stageID, flownet::ActorID playerID, flownet::ItemID itemID, flownet::ItemGroup itemGroup, flownet::InventorySlot slotNumber)
{
    ASSERT_DEBUG(m_GameClientRPCReceiver != nullptr);
    m_GameClientRPCReceiver->OnSCResponseUnRegisterStashItemFromInventory(stageID, playerID, itemID, itemGroup, slotNumber);
}

void GameClientPacketHandler::OnSCResponseSwapInventorySlot(flownet::StageID stageID, flownet::ActorID playerID, flownet::ItemGroup itemGroup, flownet::InventorySlot sourceSlotNumber, flownet::InventorySlot destinationSlotNumber)
{
    ASSERT_DEBUG(m_GameClientRPCReceiver != nullptr);
    m_GameClientRPCReceiver->OnSCResponseSwapInventorySlot(stageID, playerID, itemGroup, sourceSlotNumber, destinationSlotNumber);
}

void GameClientPacketHandler::OnSCNotifyUseItem(flownet::StageID stageID, flownet::ActorID playerID, flownet::ItemID itemID)
{
    ASSERT_DEBUG(this->m_GameClientRPCReceiver != nullptr);
    this->m_GameClientRPCReceiver->OnSCNotifyUseItem(stageID, playerID, itemID);
}

void GameClientPacketHandler::OnSCNotifyEquipItem(flownet::StageID stageID, flownet::ActorID playerID, flownet::ItemID itemID, flownet::EquipmentSlot equipedSlot)
{
    ASSERT_DEBUG(this->m_GameClientRPCReceiver != nullptr);
    this->m_GameClientRPCReceiver->OnSCNotifyEquipItem(stageID, playerID, itemID, equipedSlot);
}

void GameClientPacketHandler::OnSCNotifyUnEquipItem(StageID stageID, ActorID playerID, EquipmentSlot unequipedSlot)
{
    ASSERT_DEBUG(this->m_GameClientRPCReceiver != nullptr);
    this->m_GameClientRPCReceiver->OnSCNotifyUnEquipItem(stageID, playerID, unequipedSlot);
}

void GameClientPacketHandler::OnSCNotifySendMessageToStagePlayers(StageID stageID, ActorID playerID, STRING message)
{
    ASSERT_DEBUG(this->m_GameClientRPCReceiver != nullptr);
    this->m_GameClientRPCReceiver->OnSCNotifySendMessageToStagePlayers(stageID, playerID, message);
}

void GameClientPacketHandler::OnSCNotifyApplySpellAbility(flownet::StageID stageID, flownet::ActorID targetID, flownet::ActorID invokerID, flownet::SpellAbility spellAbility, flownet::FLOAT amount)
{
    ASSERT_DEBUG(this->m_GameClientRPCReceiver != nullptr);
    this->m_GameClientRPCReceiver->OnSCNotifyApplySpellAbility(stageID, targetID, invokerID, spellAbility, amount);
}

void GameClientPacketHandler::OnSCNotifyClearSpellAbility(flownet::StageID stageID, flownet::ActorID targetID, flownet::SpellAbility spellAbility, flownet::FLOAT amount)
{
    ASSERT_DEBUG(this->m_GameClientRPCReceiver != nullptr);
    this->m_GameClientRPCReceiver->OnSCNotifyClearSpellAbility(stageID, targetID, spellAbility, amount);
}

void GameClientPacketHandler::OnSCNotifySetFreeze(flownet::StageID stageID, flownet::ActorID targetID, flownet::BOOL isFreezed)
{
    ASSERT_DEBUG(this->m_GameClientRPCReceiver != nullptr);
    this->m_GameClientRPCReceiver->OnSCNotifySetFreeze(stageID, targetID, isFreezed);
}




    
} // namespace flownet
