//
//  GameClientObject.h
//  GameClient
//
//  Created by Sinhyub Kim on 2/13/13.
//  Copyright (c) 2013 RoughHands. All rights reserved.
//

#ifndef __GameClient__GameClientObject__
#define __GameClient__GameClientObject__

namespace flownet
{

class GameClientObject : public ClientObject
{
friend class GameClientPacketHandler;
friend class TaskCheckHeartbeatTimeOver;
friend class HeartbeatGenerator;

private:
    SessionID           m_SessionID;
    HeartbeatGenerator  m_HeartbeatGenerator;

private:
    DeviceID            m_DeviceID;
    ActorID             m_MyActorID;
    ClientStage*        m_ClientStage;

public:
    GameClientObject(BoostIOService& ioService, PacketParser* packetParser);
    virtual ~GameClientObject();
    virtual void        InitializeClient(const CHAR* connectAddress, const INT connectPort) override;
    
    void            SetSessionID(const SessionID sessionID)  {   m_SessionID = sessionID; }
    SessionID       GetSessionID()                          {return m_SessionID;}
    DeviceID        GetDeviceID(){      return this->m_DeviceID; }
    ActorID         GetMyActorID(){     return this->m_MyActorID;}
    ClientStage*    GetClientStage(){   return this->m_ClientStage;}
    PlayerMap&      GetPlayerMap(){    return this->m_ClientStage->GetPlayerMap();}
    MonsterMap&     GetMonsterMap(){    return this->m_ClientStage->GetMonsterMap();}
    void            SetDeviceID(DeviceID deviceID){    this->m_DeviceID = deviceID;    std::stringstream deviceIDStringStream;     deviceIDStringStream << deviceID; }
    void            SetMyActorID(ActorID actorID){    this->m_MyActorID = actorID;}
    void            SetClientStage(ClientStage *clientStage){    ASSERT_DEBUG(this->m_ClientStage==nullptr);    this->m_ClientStage = clientStage;}
    void            EndStage(){    delete this->m_ClientStage;}

private:
    void        ConfigureDefaultInfoToPacket(GamePacket* packet);
    
    inline INT64 DefaultInfoPacketSize()
    {
        static DeviceID dummyDeviceID;
        static ConnectionID dummyConnectionID;
        static SessionID dummySessionID;
        
        return SizeForSerialize(&dummyDeviceID) + SizeForSerialize(&dummyConnectionID) + SizeForSerialize(&dummySessionID);
    }
    
protected:
    //inline  HeartbeatChecker&   GetHeartbeatChecker(){ return m_HeartbeatChecker;}
    void        StartHeartbeat();
    void        CheckHeartbeatTimeOver(const INT64 heartbeatCount);
    void        KeepHeartbeat(const INT64 heartbeatCount);
    void        OnHeartbeatTimeOver();
    
  
protected:
    virtual void        OnConnect(const BoostErrorCode& error, BoostTCPSocket* connectedSocket) override;
    
public:
    #include "GameCSProtocolSenderDeclaration.hpp"
};
typedef Vector<GameClientObject*>::type GameClientObjectList;

} // namespace flownet

#endif /* defined(__GameClient__GameClientObject__) */
