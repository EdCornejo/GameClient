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

class GameClientObject;
class GameClientPacketHandler : public PacketHandler
{
    typedef void (GameClientPacketHandler::*PacketHandlerFunction)(GamePacket* packet);
private:
	GameClientObject*               m_GameClientObject;
    
	static PacketHandlerFunction*   m_HandlerMap;
    RenderingTaskWorkerRoutine*     m_RenderingTaskWorkerRoutine;
    const GameClientRPCInterface*   m_GameClientRPCReceiver;
    

public:
    GameClientPacketHandler(RenderingTaskWorkerRoutine* renderingTaskWorkerRoutine);
    virtual ~GameClientPacketHandler();
    
    void            LinkGameClientObject(GameClientObject* gameClientObject);
    
    void            BindHandlerFunction(INT protocolNumber, const PacketHandlerFunction& packetHandlerFunction);
    
    void            SetGameClientRPCReceiver(const GameClientRPCInterface* gameClientRPCReceiver);
    
    virtual void    HandlePacket(BasePacket* packet) override;
    

    #undef AutoGenerateHandlerName
    #define AutoGenerateHandlerName GameClientPacketHandler

    #include "GameSCProtocolHandlerDeclaration.hpp"
};

} // namespace flownet

#endif /* defined(__GameClient__GameClientPacketHandler__) */
