//
//  GameClientPacketHandler.cpp
//  GameClient
//
//  Created by Sinhyub Kim on 2/13/13.
//  Copyright (c) 2013 RoughHands. All rights reserved.
//

#include "GameClientHeaders.pch"

namespace flownet
{

GameClientPacketHandler::GameClientPacketHandler(TaskWorkerRoutine* rpcTaskWorkerRoutine):PacketHandler(),m_RPCTaskWorkerRoutine(rpcTaskWorkerRoutine),m_GameClientRPCReceiver(nullptr)
{
    InitializeHandlerMap();
}

GameClientPacketHandler::~GameClientPacketHandler()
{
    if( m_HandlerMap == nullptr )
    {
        delete []m_HandlerMap;
        m_HandlerMap = nullptr;
    }
}

void GameClientPacketHandler::InitializeHandlerMap()
{
    m_HandlerMap = new PacketHandlerFunction[GSC_Protocol_Max];
    
    // Bind Entire Protocols
    BindHandlerFunction(GSC_Protocol_Error,     &GameClientPacketHandler::SCProtocolErrorHandler);
    BindHandlerFunction(GSC_Response_Connect,   &GameClientPacketHandler::SCResponseConnectHandler);
    BindHandlerFunction(GSC_Response_Session,   &GameClientPacketHandler::SCResponseSessionHandler);
    BindHandlerFunction(GSC_Response_Heartbeat, &GameClientPacketHandler::SCResponseHeartbeatHandler);
    BindHandlerFunction(GSC_Response_MyPlayerInfo, &GameClientPacketHandler::SCProtocolErrorHandler);
    BindHandlerFunction(GSC_Response_CreateStage, &GameClientPacketHandler::SCProtocolErrorHandler);
    BindHandlerFunction(GSC_Notify_NewStagePlayer, &GameClientPacketHandler::SCProtocolErrorHandler);
    BindHandlerFunction(GSC_Response_RunningStages, &GameClientPacketHandler::SCProtocolErrorHandler);
    BindHandlerFunction(GSC_Response_JoinRunningStage, &GameClientPacketHandler::SCProtocolErrorHandler);
    
    
    // TEST
    BindHandlerFunction(GSC_Response_MyPlayerInfo, &GameClientPacketHandler::SCResponseMyPlayerInfoHandler);
    
    // Validity Check
	for(INT i=0; i< GSC_Protocol_Max; ++i )
	{
		if( m_HandlerMap[i] == nullptr )
		{
			ASSERT_RELEASE(false);
			// TO Do : Error Log
		}
	}
}

void GameClientPacketHandler::BindHandlerFunction(INT protocolNumber, const PacketHandlerFunction& packetHandlerFunction)
{
	m_HandlerMap[protocolNumber] = packetHandlerFunction;
}

void GameClientPacketHandler::SetGameClientRPCReceiver(const GameClientRPCInterface* gameClientRPCReceiver)
{
    ASSERT_RELEASE(m_GameClientRPCReceiver==nullptr);
    
    m_GameClientRPCReceiver = gameClientRPCReceiver;
}


void GameClientPacketHandler::HandlePacket(BasePacket* packet)
{
    GameSCProtocol protocol = static_cast<GameSCProtocol>(packet->GetProtocol());
    LOG_STDOUT( std::cout << "handled Protocol" << protocol << std::endl; );
    
    (this->*m_HandlerMap[protocol])(static_cast<GamePacket*>(packet));

    // 'packet' will be removed (deleted) after this return on packet parser
}

void GameClientPacketHandler::SCProtocolErrorHandler(GamePacket* packet)
{
    LOG_STDOUT( std::cout << "Wrong Protocol. Error occured " << std::endl; );
    m_RPCTaskWorkerRoutine->AddTask(CreateLambdaTask("SCProtocolErrorHandler",[this](){
        this->OnSCProtocolError();
    }));
}

void GameClientPacketHandler::OnSCProtocolError()
{
    LOG_STDOUT(std::cout << "Wrong Protocol. Error occured " << std::endl;);
    ASSERT_DEBUG(m_GameClientRPCReceiver != nullptr );
    m_GameClientRPCReceiver->OnSCProtocolError();
}

void GameClientPacketHandler::SCResponseConnectHandler(GamePacket* packet)
{
    ConnectionID connectionID;
    *packet >> connectionID;
    
    m_RPCTaskWorkerRoutine->AddTask(CreateLambdaTask("SCResponseConnectHandler",[this, connectionID](){
        this->OnSCResponseConnect(connectionID);
    }));
}

void GameClientPacketHandler::OnSCResponseConnect(ConnectionID connectionID)
{
    // Do Handle Game Jobs
    LOG_STDOUT( std::cout << "SCResponseConnect : connectionID" << connectionID << std::endl; );
    GameClientObject& gameClientObject = GameClient::Instance().GetClientObject();
    gameClientObject.SetConnectionID(connectionID);
    
    gameClientObject.StartHeartbeat();
    
    // To Do : DeviceID Generation
    const DeviceID myDeviceID = 88;
    gameClientObject.SendCSRequestSession(myDeviceID);
    
    ASSERT_DEBUG(m_GameClientRPCReceiver != nullptr );
    m_GameClientRPCReceiver->OnSCResponseConnect(connectionID);
}

void GameClientPacketHandler::SCResponseSessionHandler(GamePacket* packet)
{
    SessionID sessionID;
    *packet >> sessionID;
    
    m_RPCTaskWorkerRoutine->AddTask(CreateLambdaTask("SCResponseSessionHandler",[this, sessionID](){
        this->OnSCResponseSession(sessionID);
    }));
}

void GameClientPacketHandler::OnSCResponseSession(SessionID sessionID)
{
    LOG_STDOUT(std::cout << "SCResponseSession : sessionID" << sessionID << std::endl;);
    
    GameClientObject& gameClientObject = GameClient::Instance().GetClientObject();
    gameClientObject.SetSessionID(sessionID);
    
    ASSERT_DEBUG(m_GameClientRPCReceiver != nullptr );
    m_GameClientRPCReceiver->OnSCResponseSession(sessionID);
}

void GameClientPacketHandler::SCResponseHeartbeatHandler(GamePacket* packet)
{
    INT64 heartbeatCountAck;
    *packet >> heartbeatCountAck;
    
    m_RPCTaskWorkerRoutine->AddTask(CreateLambdaTask("SCResponseHeartbeat",[this, heartbeatCountAck](){
        this->OnSCResponseHeartbeat(heartbeatCountAck);
    }));
    
}

void GameClientPacketHandler::OnSCResponseHeartbeat(INT64 heartbeatCountAck)
{
    if( heartbeatCountAck%100 == 0)
    {
        std::cout << "fSCResponseHeartbeat : heartbeatCountAck" << heartbeatCountAck << std::endl;
    }

    
//    GameClientObject->Send
//    GameClient::Instance().GetClientObject().CheckHeartbeatTimeOver();
    GameClient::Instance().GetClientObject().KeepHeartbeat(heartbeatCountAck);
    
    ASSERT_DEBUG(m_GameClientRPCReceiver != nullptr );
    m_GameClientRPCReceiver->OnSCResponseHeartbeat(heartbeatCountAck);
}

    
void GameClientPacketHandler::SCResponseMyPlayerInfoHandler(GamePacket* packet)
{
    Player player;
    *packet >> &player;
    
    m_RPCTaskWorkerRoutine->AddTask(CreateLambdaTask("SCResponseMyPlayerInfo",[this, player](){
        this->OnSCResponseMyPlayerInfo(player);
    }));
}

void GameClientPacketHandler::OnSCResponseMyPlayerInfo(Player player)
{
    ASSERT_DEBUG(m_GameClientRPCReceiver != nullptr );
    
    // To Do :
    // Save MyPlayer
    
    m_GameClientRPCReceiver->OnSCResponseMyPlayerInfo(player);
}



} // namespace flownet
