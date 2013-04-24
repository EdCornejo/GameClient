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

GameClientObject::GameClientObject(BoostIOService& ioService, PacketParser* packetParser):ClientObject(ioService, packetParser),m_SessionID(SessionID_NONE),m_HeartbeatGenerator(this)
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
    LOG_STDOUT( std::cout << "OnHeartbeatTimeOver" << std::endl; );
#ifndef HEARTBEAT_ALLOW_DEBUG
    Disconnect();
#endif //HEARTBEAT_ALLOW_DEBUG
    
}
    
void GameClientObject::OnConnect(const BoostErrorCode& error, BoostTCPSocket* connectedSocket)
{
    LOG_STDOUT( std::cout << "connect completed clientObject" << std::endl;);
    
    RecvStart();
    // To Do : Make deviceID Generating Code for Mobile
//    GameConnectionType connectionType = GCT_Mobile;
//    INT64 deviceID = 19881988;
//    
//    SendCSRequestConnect(connectionType, deviceID);
}

void GameClientObject::SendCSRequestConnect(INT connectionType, DeviceID deviceID)
{
    BasePacket* requestConnectPacket = new BasePacket(GCS_Request_Connect, SizeForSerialize(&connectionType)+SizeForSerialize(&deviceID));
    *requestConnectPacket << connectionType << deviceID;
    
    Send(requestConnectPacket);
    delete requestConnectPacket;
    requestConnectPacket = nullptr;
    
//    Vector<BYTE>::type requestConnectPacketBuffer(requestConnectPacket->GetPacketSize());
//    ASSERT_DEBUG(requestConnectPacketBuffer.size() == requestConnectPacket->GetPacketSize());
//
//    requestConnectPacket->DeserializeToBuffer(&requestConnectPacketBuffer[0], requestConnectPacketBuffer.size());
//    
//    delete requestConnectPacket;
//    requestConnectPacket = nullptr;
//    
//    Send(requestConnectPacketBuffer);
}

void GameClientObject::SendCSRequestSession(DeviceID deviceID)
{
    ConnectionID connectionID = GetConnectionID();
    ASSERT_DEBUG(connectionID!=ConnectionID_NONE);  // had to recieve SCResponseConnect to Initialize connectionID
    
    BasePacket* requestSessionPacket = new BasePacket(GCS_Request_Session, SizeForSerialize(&deviceID)+SizeForSerialize(&connectionID));
    *requestSessionPacket << deviceID << connectionID;
    
    Send(requestSessionPacket);
    delete requestSessionPacket;
    requestSessionPacket = nullptr;
    
}

void GameClientObject::SendCSRequestHeartbeat(INT64 heartbeatCount)
{
    LOG_STDOUT( std::cout << "SendHeartbeat : " << heartbeatCount << std::endl; );
    
    ConnectionID connectionID = GetConnectionID();
    ASSERT_DEBUG(connectionID!=ConnectionID_NONE);  // had to recieve SCResponseConnect to Initialize connectionID
    
    BasePacket* requestHeartbeatPacket = new BasePacket(GCS_Request_Heartbeat, SizeForSerialize(&connectionID)+SizeForSerialize(&heartbeatCount));
    *requestHeartbeatPacket << connectionID << heartbeatCount;
    
    Send(requestHeartbeatPacket);
    delete requestHeartbeatPacket;
    requestHeartbeatPacket = nullptr;
}
    
// TEST
void GameClientObject::SendCSRequestMyPlayerInfo(PlayerID playerID)
{
    ConnectionID connectionID = GetConnectionID();
    SessionID sessionID = GetSessionID();
    
    ASSERT_DEBUG(connectionID!=ConnectionID_NONE);  // had to recieve SCResponseConnect to Initialize connectionID
    
    BasePacket* requestMyPlayerInfoPacket = new BasePacket(GCS_Request_MyPlayerInfo, SizeForSerialize(&connectionID)+SizeForSerialize(&sessionID)+SizeForSerialize(&playerID));
    *requestMyPlayerInfoPacket << connectionID << sessionID << playerID;
    
    Send(requestMyPlayerInfoPacket);
    delete requestMyPlayerInfoPacket;
    
    requestMyPlayerInfoPacket = nullptr;
}

} // namespace flownet