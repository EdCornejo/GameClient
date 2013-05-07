//
//  TesterRPCReceiver.h
//  GameClient
//
//  Created by Kim Sinhyub on 5/1/13.
//  Copyright (c) 2013 RoughHands. All rights reserved.
//

#ifndef __GameClient__TesterRPCReceiver__
#define __GameClient__TesterRPCReceiver__

namespace flownet
{

class TesterRPCReceiver : public GameClientRPCInterface
{
private:

public:
    TesterRPCReceiver();
    virtual ~TesterRPCReceiver();
    
    #include "GameSCOverrideProtocolHandlerDeclaration.hpp"
    #include "GameFCOverrideProtocolHandlerDeclaration.hpp"
//    virtual void OnSCProtocolError() const {}
//    virtual void OnSCResponseConnect(flownet::ConnectionID connectionID) const {}
//    virtual void OnSCResponseSession(flownet::UserID userID, flownet::ActorID myPlayerID, flownet::SessionID sessionID) const {}
//    virtual void OnSCResponseHeartbeat(flownet::INT64 heartbeatCountAck) const {}
//    virtual void OnSCResponseCreateUserAccount(flownet::UserID userID) const {}
//    virtual void OnSCResponseLogInUserAccount(flownet::UserID userID, flownet::ActorID playerID, flownet::SessionID sessionID) const {}
//    virtual void OnSCResponseLogOutUserAccount(flownet::UserID userID) const {}
//    virtual void OnSCResponseCreateStage(flownet::StageID stageID, flownet::Stage stage) const {}
//    virtual void OnSCResponseRunningStages(flownet::StageInfoList stageInfoList) const {}
//    virtual void OnSCResponseJoinRunningStage(flownet::StageID stageID, flownet::Stage stage, flownet::ErrorType errorCode) const {}
//    virtual void OnSCResponseExitStage(flownet::StageID stageID, flownet::ActorID myPlayerID) const {}
//    virtual void OnSCNotifyExitStage(flownet::StageID stageID, flownet::ActorID playerID) const {}
//    virtual void OnSCResponseRejoinCurrentStage(flownet::StageID stageID, flownet::Stage stage) const {}
//    virtual void OnSCNotifyClearStage(flownet::StageID stageID) const {}
//    virtual void OnSCNotifySpawnMonster(flownet::StageID stageID, flownet::Monster newMonster) const {}
//    virtual void OnSCNotifySpawnPlayer(flownet::StageID stageID, flownet::Player newPlayer) const {}
//    virtual void OnSCNotifyMoveActor(flownet::StageID stageID, flownet::ActorID playerID, flownet::POINT currentPosition, flownet::POINT destinationPosition) const {}
//    virtual void OnSCNotifyActorAttributeChanged(flownet::StageID stageID, flownet::ActorID actorID, flownet::ActorAttribute actorAttribute, flownet::FLOAT amount) const {}
//    virtual void OnSCNotifyActorAttack(flownet::StageID stageID, flownet::ActorID actorID, flownet::ActorID targetID, flownet::AttackType attackType) const {}
//    virtual void OnSCNotifyActorAttacked(flownet::StageID stageID, flownet::ActorID actorID, flownet::ActorID attackerID, flownet::AttackType attackType) const {}
//    virtual void OnSCNotifyActorDead(flownet::StageID stageID, flownet::ActorID actorID) const {}
//    virtual void OnSCNotifyChangeTarget(flownet::StageID stageID, flownet::ActorID actorID, flownet::ActorID targetID) const {}
//    virtual void OnSCNotifyBeginCast(flownet::StageID stageID, flownet::ActorID playerID, flownet::SpellType spellType, flownet::POINT destination) const {}
//    virtual void OnSCNotifyEndCast(flownet::StageID stageID, flownet::ActorID playerID, flownet::SpellType spellType, flownet::POINT destination) const {}
//    virtual void OnSCResponseDropItemToField(flownet::StageID stageID, flownet::ActorID playerID, flownet::ItemID itemID) const {}
//    virtual void OnSCNotifySpawnItem(flownet::StageID stageID, flownet::Item item, flownet::POINT spawnPosition) const {}
//    virtual void OnSCNotifyPickUpItemFromField(flownet::StageID stageID, flownet::ActorID playerID, flownet::ItemID itemID) const {}
//    virtual void OnSCNotifyAddItemToStash(flownet::StageID stageID, flownet::ActorID playerID, flownet::Item item) const {}
//    virtual void OnSCNotifyRemoveItemFromStash(flownet::StageID stageID, flownet::ActorID playerID, flownet::ItemID itemID) const {}
//    virtual void OnSCResponseRegisterStashItemToInventory(flownet::StageID stageID, flownet::ActorID playerID, flownet::ItemID itemID, flownet::InventorySlot inventorySlot) const {}
//    virtual void OnSCResponseUnRegisterStashItemFromInventory(flownet::StageID stageID, flownet::ActorID playerID, flownet::ItemID itemID, flownet::InventorySlot inventorySlot) const {}
//    virtual void OnSCResponseSwapInventorySlot(flownet::StageID stageID, flownet::ActorID playerID, flownet::InventorySlot sourceSlot, flownet::InventorySlot destinationSlot) const {}
//    virtual void OnSCNotifyUseItem(flownet::StageID stageID, flownet::ActorID playerID, flownet::ItemID itemID) const {}
//    virtual void OnSCNotifyUnEquip(flownet::StageID stageID, flownet::ActorID playerID, flownet::ItemID itemID) const {}
};

} // namesapce flownet

#endif /* defined(__GameClient__TesterRPCReceiver__) */
