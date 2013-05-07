//
//  FCPacketHandler.cpp
//  GameClient
//
//  Created by Kim Sinhyub on 4/30/13.
//  Copyright (c) 2013 RoughHands. All rights reserved.
//

#include "GameClientHeaders.pch"

namespace flownet
{
FCPacketHandler::PacketHandlerFunction* FCPacketHandler::m_HandlerMap = nullptr;

FCPacketHandler::FCPacketHandler(RenderingTaskWorkerRoutine* renderingTaskWorkerRoutine):PacketHandler(),m_RenderingTaskWorkerRoutine(renderingTaskWorkerRoutine),m_CFConnection(nullptr),m_GameClientRPCReceiver(nullptr)/*,m_FCRPCReceiver(nullptr)*/
{
    InitializeHandlerMap();
}

FCPacketHandler::~FCPacketHandler()
{
}

void FCPacketHandler::LinkCFConnection(flownet::CFConnection *cfConnection)
{
    ASSERT_DEBUG(m_CFConnection == nullptr);
    m_CFConnection = cfConnection;
}

void FCPacketHandler::BindHandlerFunction(INT protocolNumber, const PacketHandlerFunction& packetHandlerFunction)
{
	m_HandlerMap[protocolNumber] = packetHandlerFunction;
}

//void FCPacketHandler::SetFCRPCReceiver(const FCRPCInterface* fcRPCReceiver)
//{
//    ASSERT_RELEASE(m_FCRPCReceiver==nullptr);
//    
//    m_FCRPCReceiver = fcRPCReceiver;
//}


void FCPacketHandler::HandlePacket(BasePacket* packet)
{
#ifdef PACKET_SIZE_LOGGING
    // Add Packet Size Logger Here
    PacketSizeLogger* packetSizeLogger = PacketSizeLogger::Instance();
    packetSizeLogger->AddPacketSize(packet->GetPacketSize());
    // end of Add Packet Size Logger Here
#endif

    GameFCProtocol protocol = static_cast<GameFCProtocol>(packet->GetProtocol());
    LOG_STDOUT( std::cout << "handled Protocol" << protocol << std::endl; );
    
    (this->*m_HandlerMap[protocol])(static_cast<GamePacket*>(packet));

    // 'packet' will be removed (deleted) after this return on packet parser
}

void FCPacketHandler::SetGameClientRPCReceiver(const GameClientRPCInterface* gameClientRPCReceiver)
{
    ASSERT_RELEASE(m_GameClientRPCReceiver==nullptr);
    
    m_GameClientRPCReceiver = gameClientRPCReceiver;
}


void FCPacketHandler::OnFCProtocolError()
{
    m_GameClientRPCReceiver->OnFCProtocolError();
}

void FCPacketHandler::OnFCResponseConnect(ConnectionID feConnectionID)
{
    m_CFConnection->SetConnectionID(feConnectionID);
    if( feConnectionID % 100 == 0)
    {
        std::cout << "ConnectionID:" << feConnectionID << std::endl;
    }
    
    m_GameClientRPCReceiver->OnFCResponseConnect(feConnectionID);
}

void FCPacketHandler::OnFCResponseCreateUserAccount(UserID userID)
{
    m_GameClientRPCReceiver->OnFCResponseCreateUserAccount(userID);
}

void FCPacketHandler::OnFCResponseLogInUserAccount(flownet::UserID userID, flownet::GameServerID gameServerID, flownet::STRING gameServerIP, flownet::OTP otp)
{
    if( userID == UserID_None )
    {
        #ifndef GAMECLIENTTESTER
        std::cout << "LogIn Failed" << std::endl;
        #endif
        return;
//        ASSERT_DEBUG(userID!=UserID_None);
    }

    m_CFConnection->Disconnect();
    
    GameClientObject* gameClientObject = GameClientTester::Instance().GetGameClientObjectManager().CreateGameClientObject(gameServerIP, SERVER_CONNECT_PORT);
//    GameClientObject* gameClientObject = GameClientTester::Instance().GetGameClientObjectManager().GetNextGameClientObject();

    gameClientObject->SetDeviceID(m_CFConnection->GetDeviceID());
    gameClientObject->SetUserID(userID);
    gameClientObject->SetGameServerID(gameServerID);
    gameClientObject->SetOTP(otp);

    m_GameClientRPCReceiver->OnFCResponseLogInUserAccount(userID, gameServerID, gameServerIP, otp);
}


    
    
} // namespace flownet
