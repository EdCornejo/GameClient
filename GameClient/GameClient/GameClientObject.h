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

public:
    GameClientObject(BoostIOService& ioService, PacketParser* packetParser);
    virtual ~GameClientObject();
    virtual void        InitializeClient(const CHAR* connectAddress, const INT connectPort) override;
    
    void        SetSessionID(const SessionID sessionID)  {   m_SessionID = sessionID; }
    SessionID   GetSessionID()                          {return m_SessionID;}
    
protected:
    //inline  HeartbeatChecker&   GetHeartbeatChecker(){ return m_HeartbeatChecker;}
    void        StartHeartbeat();
    void        CheckHeartbeatTimeOver(const INT64 heartbeatCount);
    void        KeepHeartbeat(const INT64 heartbeatCount);
    void        OnHeartbeatTimeOver();
    

  
protected:
    virtual void        OnConnect(const BoostErrorCode& error, BoostTCPSocket* connectedSocket) override;
    
public:
    void SendCSRequestConnect(INT connectionType, DeviceID deviceID);
    void SendCSRequestSession(DeviceID deviceID);
    void SendCSRequestHeartbeat(INT64 heartbeatCount);
    
    // TEST
    void SendCSRequestMyPlayerInfo(PlayerID playerID);

};

} // namespace flownet

#endif /* defined(__GameClient__GameClientObject__) */
