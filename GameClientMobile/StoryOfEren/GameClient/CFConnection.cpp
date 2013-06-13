//
//  CFConnection.cpp
//  GameClient
//
//  Created by Kim Sinhyub on 4/30/13.
//  Copyright (c) 2013 RoughHands. All rights reserved.
//

#include "GameClientHeaders.pch"

namespace flownet
{

//NetworkObject* DBServerConnectionFactory::CreateNetworkObject()
//{
//    GameDSPacketHandler* packetHandler = new GameDSPacketHandler();
//    GameDSPacketParser* packetParser = new GameDSPacketParser(packetHandler);
//    DBServerConnection* dbServerConnection = new DBServerConnection(m_IOService, packetParser);
//    packetHandler->LinkDBServerConnection(dbServerConnection);
//    
//    return dbServerConnection;
//}

CFConnection::CFConnection(BoostIOService& ioService, PacketParser* packetParser):ClientObject(ioService, packetParser)
{
}

CFConnection::~CFConnection()
{

}

void CFConnection::InitializeClient(const CHAR* connectAddress, const INT connectPort)
{
    ClientObject::InitializeClient(connectAddress, connectPort);    // connect to server
    
    // and do some initialize
}

    
void CFConnection::OnConnect(const BoostErrorCode& error, BoostTCPSocket* connectedSocket)
{
    ClientObject::OnConnect(error, connectedSocket);
    
    if( !error )
    {
        RecvStart();
        
    }
    else{
        HandleBoostErrorCode(error, "CFConnect Error");
    }
}

void CFConnection::OnDisconnect()
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