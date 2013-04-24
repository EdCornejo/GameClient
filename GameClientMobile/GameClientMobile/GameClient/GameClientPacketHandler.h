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
    RenderingTaskWorkerRoutine*     m_RenderingTaskWorkerRoutine;
    
    const GameClientRPCInterface*   m_GameClientRPCReceiver;

public:
    GameClientPacketHandler(RenderingTaskWorkerRoutine* renderingTaskWorkerRoutine);
    virtual ~GameClientPacketHandler();
    
    void            BindHandlerFunction(INT protocolNumber, const PacketHandlerFunction& packetHandlerFunction);
    
    void            SetGameClientRPCReceiver(const GameClientRPCInterface* gameClientRPCReceiver);
    
    virtual void    HandlePacket(BasePacket* packet) override;
    

    #undef AutoGenerateHandlerName
    #define AutoGenerateHandlerName GameClientPacketHandler

    #include "GameSCProtocolHandlerDeclaration.hpp"
};

} // namespace flownet

#endif /* defined(__GameClient__GameClientPacketHandler__) */
