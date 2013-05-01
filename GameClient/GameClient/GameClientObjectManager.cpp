//
//  GameClientObjectManager.cpp
//  GameClient
//
//  Created by Kim Sinhyub on 4/30/13.
//  Copyright (c) 2013 RoughHands. All rights reserved.
//

#include "GameClientHeaders.pch"

namespace flownet
{

GameClientObjectManager::GameClientObjectManager(BoostIOService& boostIOService, const STRING& serverAddress, const INT serverPort):m_IOService(boostIOService),m_ServerAddress(serverAddress),m_ServerPort(serverPort),m_NumberOfGameClientObjects(0),m_NextGameClientObject(0)
{
}

GameClientObjectManager::~GameClientObjectManager()
{
    ScopedLock lockList(m_LockList);
    std::for_each(m_GameClientObjectList.begin(), m_GameClientObjectList.end(), [](GameClientObject* gameClientObject)
    {
        gameClientObject->Disconnect();
        delete gameClientObject;
    });
}

void GameClientObjectManager::Initialize(const INT numberOfGameClientObject)
{
    for( int i=0; i<numberOfGameClientObject; ++i )
    {
        GameClientObject* newConnection = CreateGameClientObject();
    }
}

GameClientObject* GameClientObjectManager::GetNextGameClientObject()
{
    ScopedLock lockList(m_LockList);
    const INT nextGameClientObject = (m_NextGameClientObject++)%m_NumberOfGameClientObjects;
    return m_GameClientObjectList[nextGameClientObject];
}

GameClientObject* GameClientObjectManager::CreateGameClientObject()
{
    GameClientPacketHandler* packetHandler = new GameClientPacketHandler(&GameClientTester::Instance().GetRenderingTaskWorkerRoutine());
    GameClientPacketParser* packetParser = new GameClientPacketParser(packetHandler);
    TesterRPCReceiver* testerRPCReceiver = new TesterRPCReceiver();
    
    GameClientObject* gameClientObject = new GameClientObject(m_IOService, packetParser);
    packetHandler->LinkGameClientObject(gameClientObject);
    packetHandler->SetGameClientRPCReceiver(testerRPCReceiver);
    
    gameClientObject->InitializeClient(m_ServerAddress.c_str(), m_ServerPort);
    
    {
        ScopedLock lockList(m_LockList);
        m_GameClientObjectList.push_back(gameClientObject);
        ++m_NumberOfGameClientObjects;
    }
    
    return gameClientObject;
}

} // namespace flownet

