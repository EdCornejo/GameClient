//
//  GameClientTester.cpp
//  GameClient
//
//  Created by Kim Sinhyub on 4/30/13.
//  Copyright (c) 2013 RoughHands. All rights reserved.
//

#include "GameClientHeaders.pch"

namespace flownet
{

GameClientTester* GameClientTester::_SingletonInstance = 0;

GameClientTester::GameClientTester():
    m_ClientTimer(),
    m_ScheduledTaskWorkerRoutine(m_ClientTimer),
    m_GameTaskWorkerRoutine(),
    m_NetworkWorkerRoutine(),
    m_ThreadController(THREAD_SCHEDULING_TIMESLICE),
    m_RenderingTaskWorkerRoutine(),
    m_GameClientObjectManager(m_NetworkWorkerRoutine.m_IOService),
    m_CFConnectionManager(m_NetworkWorkerRoutine.m_IOService,FESERVER_CF_CONNECT_ADDRESS,FESERVER_CF_CONNECT_PORT),
//    m_ClientPacketHandler(&m_RenderingTaskWorkerRoutine),
//    m_ClientPacketParser(&m_ClientPacketHandler),
//    m_ClientObject(m_NetworkWorkerRoutine.m_IOService,&m_ClientPacketParser),
//    m_FCPacketHandler(&m_RenderingTaskWorkerRoutine),
//    m_FCPacketParser(&m_FCPacketHandler),
//    m_CFConnection(m_NetworkWorkerRoutine.m_IOService,&m_FCPacketParser),
    m_IsInitialized(false),
    m_IsStarted(false),
    m_DeviceID(DeviceID_None),
    m_SessionID(SessionID_NONE),
    m_MyActorID(ActorID_None),
    m_ClientStage(nullptr)
{
    this->m_DeviceID = boost::uuids::random_generator()();
//    m_ClientPacketHandler.LinkGameClientObject(&m_ClientObject);
//    m_FCPacketHandler.LinkCFConnection(&m_CFConnection);
}

GameClientTester::~GameClientTester()
{
    if( m_IsStarted == true )
    {
        m_ThreadController.Stop();
    }
}

GameClientTester& GameClientTester::Instance()
{
    if(_SingletonInstance==nullptr)
    {
        _SingletonInstance= new GameClientTester;
    }
    return *_SingletonInstance;
}

void GameClientTester::DeleteInstance()
{
    if( _SingletonInstance != nullptr )
    {
        delete _SingletonInstance;
    }
    _SingletonInstance = nullptr;
}

// GameClient Network Part
    
void GameClientTester::InitializeClient(GameClientRPCInterface* gameClientRPCReceiver)
{
    ASSERT_RELEASE(m_IsInitialized == false);
    ASSERT_RELEASE(m_IsStarted == false);

    SpellDictionary::Initialize();
    ItemDataDictionary::Initialize();
    
    m_ThreadController.Initialize(DEFAULT_NUMBER_OF_THREADS);
    m_ThreadController.AddWorkerRoutine(&m_ScheduledTaskWorkerRoutine);
    m_ThreadController.AddWorkerRoutine(&m_GameTaskWorkerRoutine);
    m_ThreadController.AddWorkerRoutine(&m_NetworkWorkerRoutine);
    
    #ifdef GAMECLIENTTESTER
        m_ThreadController.AddWorkerRoutine(&m_RenderingTaskWorkerRoutine);
    #endif
    // NOTE :
    // RenderingTaskWorkerRoutine is managed by Cocos2d main thread
    // DO NOT ADD this m_RenderingTaskWorkerRoutine
    
    
    // connect to server
    m_CFConnectionManager.Initialize(500);
//    m_GameClientObjectManager.Initialize(1);
//    m_GameClientObjectManager.Initialize(500);

//    m_ClientObject.InitializeClient(SERVER_CONNECT_ADDRESS, SERVER_CONNECT_PORT);
    
//    m_ClientPacketHandler.SetGameClientRPCReceiver(gameClientRPCReceiver);
    
    m_IsInitialized = true;
}

void GameClientTester::StartClient()
{
    ASSERT_RELEASE(m_IsInitialized == true);
    ASSERT_RELEASE(m_IsStarted == false);
    
    m_ClientTimer.Start();
    m_ThreadController.Start();
}

void GameClientTester::TerminateClient()
{
    DeleteInstance();
}

// End of GameClient Network Part
    
// GameClient Data Part

DeviceID GameClientTester::GetDeviceID()
{
    return this->m_DeviceID;
}

SessionID GameClientTester::GetSessionID()
{
    return this->m_SessionID;
}

ActorID GameClientTester::GetMyActorID()
{
    return this->m_MyActorID;
}

ClientStage* GameClientTester::GetClientStage()
{
    return this->m_ClientStage;
}

PlayerMap& GameClientTester::GetPlayerMap()
{
    return this->m_ClientStage->GetPlayerMap();
}

MonsterMap& GameClientTester::GetMonsterMap()
{
    return this->m_ClientStage->GetMonsterMap();
}

void GameClientTester::SetSessionID(SessionID sessionID)
{
    this->m_SessionID = sessionID;
}

void GameClientTester::SetMyActorID(ActorID actorID)
{
    this->m_MyActorID = actorID;
}

void GameClientTester::SetClientStage(ClientStage *clientStage)
{
    ASSERT_DEBUG(this->m_ClientStage==nullptr);

    this->m_ClientStage = clientStage;
}
 
void GameClientTester::EndStage()
{
    delete this->m_ClientStage;
}
    
// End of GameClient Data Part
    
// test data generation
void GameClientTester::InitializeTestData()
{
    // test data generation code
    ActorID ownerID = 1;
    StageInfo* stageInfo = new StageInfo(StageType_MushroomField, 1, 200, 100, 3);
    Stage* stage = new Stage(*stageInfo, ownerID);
    
    ClientStage* clientStage = new ClientStage(*stage);
    
    ClientPlayer *player1 = new ClientPlayer(1);
    ClientPlayer *player2 = new ClientPlayer(2);
    ClientPlayer *player3 = new ClientPlayer(3);
    
    clientStage->AddPlayer(player1->GetActorID(), player1);
    clientStage->AddPlayer(player2->GetActorID(), player2);
    clientStage->AddPlayer(player3->GetActorID(), player3);
    
    ClientMonster *monster1 = new ClientMonster(MonsterType_Goblin, 1 | ActorID_MonsterFlag, flownet::POINT(3,10));
    ClientMonster *monster2 = new ClientMonster(MonsterType_Goblin, 2 | ActorID_MonsterFlag, flownet::POINT(4, 15));
    ClientMonster *monster3 = new ClientMonster(MonsterType_Goblin, 3 | ActorID_MonsterFlag, flownet::POINT(5, 20));
    ClientMonster *monster4 = new ClientMonster(MonsterType_Goblin, 4 | ActorID_MonsterFlag, flownet::POINT(6, 10));
    ClientMonster *monster5 = new ClientMonster(MonsterType_Goblin, 5 | ActorID_MonsterFlag, flownet::POINT(7, 15));
    ClientMonster *monster6 = new ClientMonster(MonsterType_Goblin, 6 | ActorID_MonsterFlag, flownet::POINT(8, 20));
    ClientMonster *monster7 = new ClientMonster(MonsterType_Goblin, 7 | ActorID_MonsterFlag, flownet::POINT(9,10));
    ClientMonster *monster8 = new ClientMonster(MonsterType_Goblin, 8 | ActorID_MonsterFlag, flownet::POINT(10, 15));
    ClientMonster *monster9 = new ClientMonster(MonsterType_Goblin, 9 | ActorID_MonsterFlag, flownet::POINT(11, 20));
    ClientMonster *monster10 = new ClientMonster(MonsterType_Goblin, 10 | ActorID_MonsterFlag, flownet::POINT(12, 10));
    ClientMonster *monster11 = new ClientMonster(MonsterType_Goblin, 11 | ActorID_MonsterFlag, flownet::POINT(13, 15));
    ClientMonster *monster12 = new ClientMonster(MonsterType_Goblin, 12 | ActorID_MonsterFlag, flownet::POINT(14, 20));
    
    clientStage->AddMonster(monster1->GetActorID(), monster1);
    clientStage->AddMonster(monster2->GetActorID(), monster2);
    clientStage->AddMonster(monster3->GetActorID(), monster3);
    clientStage->AddMonster(monster4->GetActorID(), monster4);
    clientStage->AddMonster(monster5->GetActorID(), monster5);
    clientStage->AddMonster(monster6->GetActorID(), monster6);
    clientStage->AddMonster(monster7->GetActorID(), monster7);
    clientStage->AddMonster(monster8->GetActorID(), monster8);
    clientStage->AddMonster(monster9->GetActorID(), monster9);
    clientStage->AddMonster(monster10->GetActorID(), monster10);
    clientStage->AddMonster(monster11->GetActorID(), monster11);
    clientStage->AddMonster(monster12->GetActorID(), monster12);
    
    GameClientTester::Instance().SetClientStage(clientStage);
    GameClientTester::Instance().SetMyActorID(player1->GetActorID());
    // end of test data generation code

}
// end of test data generation

    
} // namespace flownet

