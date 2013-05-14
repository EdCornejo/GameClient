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
    HeartbeatGenerator  m_HeartbeatGenerator;

public:
    GameClientObject(BoostIOService& ioService, PacketParser* packetParser);
    virtual ~GameClientObject();
    virtual void        InitializeClient(const CHAR* connectAddress, const INT connectPort) override;
    virtual BOOL        InitializeClientWithBlocking(const CHAR* connectAddress, const INT connectPort) override;
        
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
