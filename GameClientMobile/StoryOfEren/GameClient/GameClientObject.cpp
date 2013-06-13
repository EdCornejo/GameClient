//
//  GameClientObject.cpp
//  GameClient
//
//  Created by Sinhyub Kim on 2/13/13.
//  Copyright (c) 2013 RoughHands. All rights reserved.
//

#include "GameClientHeaders.pch"

namespace flownet
{

GameClientObject::GameClientObject(BoostIOService& ioService, PacketParser* packetParser):ClientObject(ioService, packetParser),m_HeartbeatGenerator(this)
{

}

GameClientObject::~GameClientObject()
{

}

void GameClientObject::InitializeClient(const CHAR* connectAddress, const INT connectPort)
{
    ClientObject::InitializeClient(connectAddress, connectPort);    // connect to server
    
    // and do some initialize
}

BOOL GameClientObject::InitializeClientWithBlocking(const CHAR *connectAddress, const INT connectPort)
{
    return ClientObject::InitializeClientWithBlocking(connectAddress, connectPort);
}
    
void GameClientObject::StartHeartbeat()
{
    SystemTimer& clientTimer = GameClient::Instance().GetClientTimer();

    m_HeartbeatGenerator.StartHeartbeat(clientTimer.Check());
}

void GameClientObject::CheckHeartbeatTimeOver(const INT64 heartbeatCount)
{
    SystemTimer& clientTimer = GameClient::Instance().GetClientTimer();
    
    const BOOL isTimeOver = m_HeartbeatGenerator.CheckHeartbeatTimeOver(heartbeatCount, clientTimer.Check());
    
    if( true == isTimeOver )
    {
        OnHeartbeatTimeOver();
    }
}

void GameClientObject::KeepHeartbeat(const INT64 heartbeatCount)
{
    SystemTimer& clientTimer = GameClient::Instance().GetClientTimer();
    
    m_HeartbeatGenerator.KeepHeartbeat(heartbeatCount, clientTimer.Check());
}

void GameClientObject::OnHeartbeatTimeOver()
{
    // TO Do : Self Disconnect or ReConnect
    std::cout << "OnHeartbeatTimeOver" << std::endl;
#ifndef HEARTBEAT_ALLOW_DEBUG
    Disconnect();
#endif //HEARTBEAT_ALLOW_DEBUG
    
}
    
void GameClientObject::OnConnect(const BoostErrorCode& error, BoostTCPSocket* connectedSocket)
{
    ClientObject::OnConnect(error, connectedSocket);
    
    std::cout << "connect completed clientObject" << std::endl;
    
    RecvStart();
    
//    this->SendCSRequestLogInWithOTP(GameClient::Instance().GetDeviceID(), GameClient::Instance().GetUserID(), GameClient::Instance().GetOTP());
    // To Do : Make deviceID Generating Code for Mobile
//    GameConnectionType connectionType = GCT_Mobile;
//    INT64 deviceID = 19881988;
//    
//    SendCSRequestConnect(connectionType, deviceID);
}

void GameClientObject::OnDisconnect()
{
    ClientObject::OnDisconnect();
    
    const GameClientRPCInterface* gameClientRPCInterface = GameClient::Instance().GetGameClientRPCInterface();
    if( gameClientRPCInterface )
    {
        GameClient::Instance().GetRenderingTaskWorkerRoutine().AddTask(CreateLambdaTask("OnDisconnectLambda", [ gameClientRPCInterface](){
            gameClientRPCInterface->PopUpMessageBox("GameServer Is Busy. Please try again LATER.");
        }));
    }
}
    
} // namespace flownet
