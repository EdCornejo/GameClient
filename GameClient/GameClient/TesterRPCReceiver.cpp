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

TesterRPCReceiver::TesterRPCReceiver()
{
}

TesterRPCReceiver::~TesterRPCReceiver()
{
}


void TesterRPCReceiver::OnSCProtocolError() const
{
}

void TesterRPCReceiver::OnSCResponseConnect(flownet::ConnectionID connectionID) const 
{
}

void TesterRPCReceiver::OnSCResponseSession(flownet::UserID userID, flownet::ActorID myPlayerID, flownet::SessionID sessionID) const 
{
}

void TesterRPCReceiver::OnSCResponseLogInWithOTP(flownet::UserID userID, flownet::ActorID playerID, flownet::SessionID sessionID) const
{

}

void TesterRPCReceiver::OnSCResponseHeartbeat(flownet::INT64 heartbeatCountAck) const 
{
}
void TesterRPCReceiver::OnSCResponseLogOutUserAccount(flownet::UserID userID) const 
{
}

void TesterRPCReceiver::OnSCResponseCreateStage(flownet::StageID stageID, flownet::Stage stage) const 
{
}

void TesterRPCReceiver::OnSCResponseRunningStages(flownet::StageInfoList stageInfoList) const 
{
}

void TesterRPCReceiver::OnSCResponseJoinRunningStage(flownet::StageID stageID, flownet::Stage stage, flownet::ErrorType errorCode) const 
{
}

void TesterRPCReceiver::OnSCResponseExitStage(flownet::StageID stageID, flownet::ActorID myPlayerID) const 
{
}

void TesterRPCReceiver::OnSCNotifyExitStage(flownet::StageID stageID, flownet::ActorID playerID) const 
{
}

void TesterRPCReceiver::OnSCResponseRejoinCurrentStage(flownet::StageID stageID, flownet::Stage stage) const 
{
}

void TesterRPCReceiver::OnSCNotifyClearStage(flownet::StageID stageID) const 
{
}

void TesterRPCReceiver::OnSCNotifySpawnMonster(flownet::StageID stageID, flownet::Monster newMonster) const 
{
}

void TesterRPCReceiver::OnSCNotifySpawnPlayer(flownet::StageID stageID, flownet::Player newPlayer) const 
{
}

void TesterRPCReceiver::OnSCNotifyMoveActor(flownet::StageID stageID, flownet::ActorID playerID, flownet::POINT currentPosition, flownet::POINT destinationPosition) const 
{
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

void TesterRPCReceiver::OnSCNotifyUnEquip(flownet::StageID stageID, flownet::ActorID playerID, flownet::ItemID itemID) const 
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
