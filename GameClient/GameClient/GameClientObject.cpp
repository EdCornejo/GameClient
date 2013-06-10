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

GameClientObject::GameClientObject(BoostIOService& ioService, PacketParser* packetParser):
    ClientObject(ioService, packetParser),
    m_SessionID(SessionID_NONE),
    m_HeartbeatGenerator(this),
    m_DeviceID(DeviceID_None),
    m_GameServerID(GameServerID_None),
    m_GameServerIP(""),
    m_UserID(UserID_None),
    m_MyActorID(ActorID_None),
    m_ClientStage(nullptr),
    m_OTP(OTP_None)
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
    SystemTimer& clientTimer = GameClientTester::Instance().GetClientTimer();

    m_HeartbeatGenerator.StartHeartbeat(clientTimer.Check());
}

void GameClientObject::CheckHeartbeatTimeOver(const INT64 heartbeatCount)
{
    SystemTimer& clientTimer = GameClientTester::Instance().GetClientTimer();
    
    const BOOL isTimeOver = m_HeartbeatGenerator.CheckHeartbeatTimeOver(heartbeatCount, clientTimer.Check());
    
    if( true == isTimeOver )
    {
        OnHeartbeatTimeOver();
    }
}

void GameClientObject::KeepHeartbeat(const INT64 heartbeatCount)
{
    SystemTimer& clientTimer = GameClientTester::Instance().GetClientTimer();
    
    m_HeartbeatGenerator.KeepHeartbeat(heartbeatCount, clientTimer.Check());
}

void GameClientObject::OnHeartbeatTimeOver()
{
    // TO Do : Self Disconnect or ReConnect
    #ifndef GAMECLIENTTESTER
    LogSystem::Instance() << "OnHeartbeatTimeOver";
    LogSystem::Instance().Commit();
    #endif
#ifndef HEARTBEAT_ALLOW_DEBUG
    Disconnect();
#endif //HEARTBEAT_ALLOW_DEBUG
    
}
    
void GameClientObject::OnConnect(const BoostErrorCode& error, BoostTCPSocket* connectedSocket)
{
    NetworkObject::OnConnect(error, connectedSocket);
    
    if( !error )
    {
        #ifndef GAMECLIENTTESTER
        LogSystem::Instance() << "connect completed clientObject";
        LogSystem::Instance().Commit();
        #endif
        RecvStart();
        
        this->SendCSRequestLogInWithOTP(this->GetDeviceID(), this->GetUserID(), this->GetOTP());
    }
    else
    {
        ASSERT_RELEASE(false);
    }
    // To Do : Make deviceID Generating Code for Mobile
//    GameConnectionType connectionType = GCT_Mobile;
//    INT64 deviceID = 19881988;
//    
//    SendCSRequestConnect(connectionType, deviceID);
}
    
} // namespace flownet
