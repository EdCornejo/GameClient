//
//  GameClientPacketHandler.h
//  GameClient
//
//  Created by Sinhyub Kim on 2/13/13.
//  Copyright (c) 2013 RoughHands. All rights reserved.
//

#ifndef __GameClient__GameClientPacketHandler__
#define __GameClient__GameClientPacketHandler__

namespace flownet
{

class GameClientPacketHandler : public PacketHandler
{
    typedef void (GameClientPacketHandler::*PacketHandlerFunction)(GamePacket* packet);
private:
	// To do : Implement Callback Map
    // use array to directly call the packet handler
	PacketHandlerFunction*          m_HandlerMap;
    TaskWorkerRoutine*              m_RPCTaskWorkerRoutine;
    
    const GameClientRPCInterface*   m_GameClientRPCReceiver;

public:
    GameClientPacketHandler(TaskWorkerRoutine* rpcTaskWorkerRoutine);
    virtual ~GameClientPacketHandler();
    
    void            InitializeHandlerMap();
    void            BindHandlerFunction(INT protocolNumber, const PacketHandlerFunction& packetHandlerFunction);
    
    void            SetGameClientRPCReceiver(const GameClientRPCInterface* gameClientRPCReceiver);
    
    virtual void    HandlePacket(BasePacket* packet) override;
    
    
    // RPC Handlers and Callback Functions
    void SCProtocolErrorHandler(GamePacket* packet);
    void OnSCProtocolError();
    void SCResponseConnectHandler(GamePacket* packet);
    void OnSCResponseConnect(ConnectionID connectionID);
    void SCResponseSessionHandler(GamePacket* packet);
    void OnSCResponseSession(SessionID sessionID);
    void SCResponseHeartbeatHandler(GamePacket* packet);
    void OnSCResponseHeartbeat(INT64 heartbeatCountAck);
    
    void SCResponseMyPlayerInfoHandler(GamePacket* packet);
    void OnSCResponseMyPlayerInfo(Player player);
    
};

} // namespace flownet

#endif /* defined(__GameClient__GameClientPacketHandler__) */
