//
//  FCPacketHandler.h
//  GameClient
//
//  Created by Kim Sinhyub on 4/30/13.
//  Copyright (c) 2013 RoughHands. All rights reserved.
//

#ifndef __GameClient__FCPacketHandler__
#define __GameClient__FCPacketHandler__

namespace flownet
{

class CFConnection;
class FCPacketHandler : public PacketHandler
{
    typedef void (FCPacketHandler::*PacketHandlerFunction)(GamePacket* packet);
private:
	static PacketHandlerFunction*          m_HandlerMap;
    CFConnection*                          m_CFConnection;
    
    RenderingTaskWorkerRoutine*     m_RenderingTaskWorkerRoutine;
    const GameClientRPCInterface*   m_GameClientRPCReceiver;
//    const FCRPCInterface*   m_FCRPCReceiver;

public:
    FCPacketHandler(RenderingTaskWorkerRoutine* renderingTaskWorkerRoutine);
    virtual ~FCPacketHandler();

    void            LinkCFConnection(CFConnection* cfConnection);
    
    void            BindHandlerFunction(INT protocolNumber, const PacketHandlerFunction& packetHandlerFunction);
    
    void            SetGameClientRPCReceiver(const GameClientRPCInterface* gameClientRPCReceiver);
    
    virtual void    HandlePacket(BasePacket* packet) override;
    

    #undef AutoGenerateHandlerName
    #define AutoGenerateHandlerName FCPacketHandler

    #include "GameFCProtocolHandlerDeclaration.hpp"
};

} // namespace flownet


#endif /* defined(__GameClient__FCPacketHandler__) */
