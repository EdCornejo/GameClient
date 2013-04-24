//
//  GameClient.cpp
//  GameClient
//
//  Created by Sinhyub Kim on 2/13/13.
//  Copyright (c) 2013 RoughHands. All rights reserved.
//

#include "GameClientHeaders.pch"

namespace flownet
{

GameClient* GameClient::_SingletonInstance = 0;

GameClient::GameClient():
    m_ClientTimer(),
    m_ScheduledTaskWorkerRoutine(m_ClientTimer),
    m_GameTaskWorkerRoutine(),
    m_RPCTaskWorkerRoutine(),
    m_NetworkWorkerRoutine(),
    m_ThreadController(THREAD_SCHEDULING_TIMESLICE),
    m_ClientPacketHandler(&m_RPCTaskWorkerRoutine),
    m_ClientPacketParser(&m_ClientPacketHandler),
    m_ClientObject(m_NetworkWorkerRoutine.m_IOService,&m_ClientPacketParser),
    m_IsInitialized(false),
    m_IsStarted(false)
{

}

GameClient::~GameClient()
{
    if( m_IsStarted == true )
    {
        m_ThreadController.Stop();
    }
}

GameClient& GameClient::Instance()
{
    if(_SingletonInstance==nullptr)
    {
        _SingletonInstance= new GameClient;
    }
    return *_SingletonInstance;
}

void GameClient::DeleteInstance()
{
    if( _SingletonInstance != nullptr )
    {
        delete _SingletonInstance;
    }
    _SingletonInstance = nullptr;
}


void GameClient::InitializeClient(GameClientRPCInterface* gameClientRPCReceiver)
{
    ASSERT_RELEASE(m_IsInitialized == false);
    ASSERT_RELEASE(m_IsStarted == false);
    m_ThreadController.Initialize(1);
    m_ThreadController.AddWorkerRoutine(&m_ScheduledTaskWorkerRoutine);
    m_ThreadController.AddWorkerRoutine(&m_GameTaskWorkerRoutine);
    m_ThreadController.AddWorkerRoutine(&m_RPCTaskWorkerRoutine);
    m_ThreadController.AddWorkerRoutine(&m_NetworkWorkerRoutine);

    // connect to server
    m_ClientObject.InitializeClient(SERVER_CONNECT_ADDRESS, SERVER_CONNECT_PORT);
    m_ClientPacketHandler.SetGameClientRPCReceiver(gameClientRPCReceiver);
    
    m_IsInitialized = true;
}

void GameClient::StartClient()
{
    ASSERT_RELEASE(m_IsInitialized == true);
    ASSERT_RELEASE(m_IsStarted == false);
    
    m_ClientTimer.Start();
    m_ThreadController.Start();
}

void GameClient::TerminateClient()
{
    DeleteInstance();
}

} // namespace flownet
