//
//  CFConnectionManager.cpp
//  GameClient
//
//  Created by Kim Sinhyub on 4/30/13.
//  Copyright (c) 2013 RoughHands. All rights reserved.
//

#include "GameClientHeaders.pch"

namespace flownet
{

CFConnectionManager::CFConnectionManager(BoostIOService& boostIOService, const STRING& dbServerAddress, const INT dbServerPort):m_IOService(boostIOService),m_FEServerAddress(dbServerAddress),m_FEServerPort(dbServerPort),m_NumberOfCFConnections(0),m_NextCFConnection(0)
{
}

CFConnectionManager::~CFConnectionManager()
{
    ScopedLock lockConnectionList(m_LockConnectionList);
    std::for_each(m_CFConnectionList.begin(), m_CFConnectionList.end(), [](CFConnection* cfConnection)
    {
        cfConnection->Disconnect();
        delete cfConnection;
    });
}

void CFConnectionManager::Initialize(const INT numberOfCFConnection)
{
    for( int i=0; i<numberOfCFConnection; ++i )
    {
        CFConnection* newConnection = CreateCFConnection();
    }
}

CFConnection* CFConnectionManager::GetNextCFConnection()
{
    ScopedLock lockConnectionList(m_LockConnectionList);
    
    const INT nextCFConnection = (m_NextCFConnection++)%m_NumberOfCFConnections;
    return m_CFConnectionList[nextCFConnection];
    
}

CFConnection* CFConnectionManager::CreateCFConnection()
{
    FCPacketHandler* packetHandler = new FCPacketHandler(&GameClientTester::Instance().GetRenderingTaskWorkerRoutine());
    FCPacketParser* packetParser = new FCPacketParser(packetHandler);
    TesterRPCReceiver* testerRPCReceiver = new TesterRPCReceiver;
    
    CFConnection* cfConnection = new CFConnection(m_IOService, packetParser);
    packetHandler->LinkCFConnection(cfConnection);
    packetHandler->SetGameClientRPCReceiver(testerRPCReceiver);
    
    cfConnection->InitializeClient(m_FEServerAddress.c_str(), m_FEServerPort);
    
    {
        ScopedLock lockList(m_LockConnectionList);
        m_CFConnectionList.push_back(cfConnection);
        ++m_NumberOfCFConnections;
    }
    return cfConnection;
}

} // namespace flownet

