//
//  GameClientRPCInterface.h
//  GameClientMobile
//
//  Created by Sinhyub Kim on 2/28/13.
//
//

#ifndef GameClientMobile_GameClientRPCInterface_h
#define GameClientMobile_GameClientRPCInterface_h

namespace flownet
{

// Pure virtual class, just define interface
class GameClientRPCInterface
{
protected:
    GameClientRPCInterface(){}
public:
    virtual ~GameClientRPCInterface(){}

public:
    #include "GameSCVirtualProtocolHandlerDeclaration.hpp"
//    virtual void OnSCProtocolError() const = 0;
//    virtual void OnSCResponseConnect(INT64 connectionID) const = 0;
//    virtual void OnSCResponseSession(INT64 sessionID) const = 0;
//    virtual void OnSCResponseHeartbeat(INT64 heartbeatCountAck) const = 0;
//    virtual void OnSCResponseCreateUserAccount(UserID userID, ActorID playerID, SessionID sessionID) const = 0;
//    virtual void OnSCResponseLogInUserAccount(UserID userID, ActorID playerID, SessionID sessionID) const = 0;
//    virtual void OnSCResponseLogOutUserAccount(UserID userID) const = 0;
//    virtual void OnSCResponseCreateStage(StageID stageID, ClientStage stage) const = 0;
//    virtual void OnSCResponseRunningStages(StageInfoList stageInfoList) const = 0;
//    virtual void OnSCResponseJoinRunningStage(StageID stageID, ClientStage stage, ErrorType errorType) const = 0;
//    virtual void OnSCResponseExitStage(StageID stageID, ActorID playerID) const = 0;
//    virtual void OnSCNotifyExitStage(StageID stageID, ActorID playerID) const = 0;
//    virtual void OnSCNotifyClearStage(StageID stageID) const = 0;
//    virtual void OnSCNotifySpawnPlayer(StageID stageID, ClientPlayer player) const = 0;
//    virtual void OnSCNotifySpawnMonster(StageID stageID, ClientMonster monster) const = 0;
//    virtual void OnSCNotifyMoveActor(StageID stageID, ActorID actorID, POINT currentPosition, POINT destinationPosition) const = 0;
//    virtual void OnSCNotifyActorAttack(StageID stageID, ActorID actorID, ActorID targetID, AttackType attackType) const = 0;
//    virtual void OnSCNotifyActorAttacked(StageID stageID, ActorID actorID, ActorID attackerActorID, AttackType attackType) const = 0;
//    virtual void OnSCNotifyActorTakeDamage(StageID stageID, ActorID actorID, FLOAT damageAmount) const = 0;
//    virtual void OnSCNotifyActorDead(StageID stageID, ActorID actorID) const = 0;
//    virtual void OnSCNotifyChangeTarget(StageID stageID, ActorID actorID, ActorID targetID) const = 0;
//    virtual void OnSCNotifyBeginCast(StageID stageID, ActorID actorID, SpellType spellType, POINT destination) const = 0;
//    virtual void OnSCNotifyEndCast(StageID stageID, ActorID actorID, SpellType spellType, POINT destination) const = 0;
};

} // namespace


#endif
