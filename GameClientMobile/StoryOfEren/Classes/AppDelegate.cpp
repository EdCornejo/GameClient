//
//  GameClientMobileAppDelegate.cpp
//  GameClientMobile
//
//  Created by Sinhyub Kim on 2/27/13.
//  Copyright __MyCompanyName__ 2013. All rights reserved.
//

#include "Headers.pch"

USING_NS_CC;
using namespace CocosDenshion;

AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
    pDirector->setOpenGLView(pEGLView);
    
    pEGLView->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, kResolutionShowAll);

    CCSize frameSize = pEGLView->getFrameSize();

    // In this demo, we select resource according to the frame's height.
    // If the resource size is different from design resolution size, you need to set contentScaleFactor.
    // We use the ratio of resource's height to the height of design resolution,
    // this can make sure that the resource's height could fit for the height of design resolution.

    std::vector<std::string> searchPaths;

    if (frameSize.height > mediumResource.size.height)
    {
        searchPaths.push_back(largeResource.directory);
        pDirector->setContentScaleFactor(largeResource.size.height/designResolutionSize.height);
    }
    else if (frameSize.height > iphoneHDResource.size.height)
    {
        searchPaths.push_back(mediumResource.directory);
        pDirector->setContentScaleFactor(mediumResource.size.height/designResolutionSize.height);
    }
    else if (frameSize.height > smallResource.size.height)
    {
        searchPaths.push_back(iphoneHDResource.directory);
        pDirector->setContentScaleFactor(iphoneHDResource.size.height/designResolutionSize.height);
    }
    else
    {
        searchPaths.push_back(smallResource.directory);
        pDirector->setContentScaleFactor(smallResource.size.height/designResolutionSize.height);
    }
    
    searchPaths.push_back("");
    CCFileUtils::sharedFileUtils()->setSearchPaths(searchPaths);

    // display FPS setting
    #ifdef COCOS_FPS_SCREEN_ON
    pDirector->setDisplayStats(true);
    #else
    pDirector->setDisplayStats(false);
    #endif
    
    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    CCScene *pScene = SplashScene::create();

    pDirector->runWithScene(pScene);
    
    LogTerminal::Initialize();
    LogSystem::Initialize("GameClientSystemLog");
    // Initialize GameClient
    flownet::GameClient::Instance().InitializeClient(this);
    this->InitializeConnection();
    flownet::GameClient::Instance().StartClient();

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    // Stop Gameclient
    flownet::GameClient::Instance().TerminateClient();
    
    CCDirector::sharedDirector()->stopAnimation();
    AudioEngine::Instance()->PauseBackgroundMusic();
    AudioEngine::Instance()->PauseAllEffects();
    
    CCUserDefault::sharedUserDefault()->flush();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    // Re start GameClient
    flownet::GameClient::Instance().InitializeClient(this);
    this->InitializeConnection();
    flownet::GameClient::Instance().StartClient();


    CCDirector::sharedDirector()->startAnimation();
    
    AudioEngine::Instance()->ResumeBackgroundMusic();
    AudioEngine::Instance()->ResumeAllEffects();
}

void AppDelegate::InitializeConnection()
{
    std::string serverIP = CCUserDefault::sharedUserDefault()->getStringForKey("yours", "");
    
    if( serverIP.empty() || serverIP.length()==0 )
    {
        GameClient::Instance().GetCFConnection().InitializeClient(FESERVER_CF_CONNECT_ADDRESS, FESERVER_CF_CONNECT_PORT);
        //GameClient::Instance().GetCFConnection().RecvStart();
    }
    else
    {
        GameClient::Instance().GetClientObject().InitializeClient(serverIP.c_str(), SERVER_CONNECT_PORT);
        //GameClient::Instance().GetClientObject().RecvStart();
    }
}

void AppDelegate::DisconnectCFAndConnectCSConnection() const
{
    if(GameClient::Instance().GetCFConnection().IsConnected())
    {
        GameClient::Instance().GetCFConnection().Disconnect();
    }
    if(!GameClient::Instance().GetClientObject().IsConnected())
    {
        std::string serverIP = CCUserDefault::sharedUserDefault()->getStringForKey("yours", "");
        if(serverIP.empty()) ASSERT_DEBUG(!serverIP.empty());
        GameClient::Instance().GetClientObject().InitializeClientWithBlocking(serverIP.c_str(), SERVER_CONNECT_PORT);
        GameClient::Instance().GetClientObject().RecvStart();
    }
}

void AppDelegate::DisconnectCSAndConnectCFConnection() const
{
    if(GameClient::Instance().GetClientObject().IsConnected())
    {
        GameClient::Instance().GetClientObject().Disconnect();
    }
    if(!GameClient::Instance().GetCFConnection().IsConnected())
    {
        GameClient::Instance().GetCFConnection().InitializeClientWithBlocking(FESERVER_CF_CONNECT_ADDRESS, FESERVER_CF_CONNECT_PORT);
        GameClient::Instance().GetCFConnection().RecvStart();
    }
}

void AppDelegate::OnSCProtocolError() const
{

}

void AppDelegate::OnFCProtocolError() const
{
}

void AppDelegate::OnSCResponseConnect(flownet::INT64 connectionID) const
{
    CCLOG("AppDelegate::OnSCResponseConnect >> response connect");
}

void AppDelegate::OnSCResponseSession(flownet::UserID userID, flownet::ActorID myPlayerID, flownet::SessionID sessionID) const
{
    CCLOG("AppDelegate::OnSCResponseSession >> session responsed with %d", sessionID);
    GameClient::Instance().SetMyActorID(myPlayerID);
    GameClient::Instance().SetSessionID(sessionID);
    
    // SessionID 가 없는 경우에는 로그인이 안되어있는 상태이기때문에 로그인을 해야한다.
    if(sessionID == SessionID_NONE)
    {
        this->DisconnectCSAndConnectCFConnection();
        
        if( false == static_cast<ClientDirector*>(CCDirector::sharedDirector())->ChangeScene<AccountScene>() )
        {
            // TO DO : handle error
            ASSERT_DEBUG(false);
        }

        return;
    }
    
    // SessionID가 있고, UserID가 있는 경우에는 로그인이 제대로 된것 (보통의 경우에 세션이 있으면 UserID는 당연히 있다) 조건검사 그래서 안한다.
    
    // SessionID가 있고, UserID가 있고, ActorID가 없는 경우에는 케릭터를 생성해야 한다. 케릭터 생성창으로 보내야 함.
    if(sessionID != SessionID_NONE && myPlayerID == ActorID_None)
    {
        if( false == static_cast<ClientDirector*>(CCDirector::sharedDirector())->ChangeScene<CharacterCreateScene>() )
        {
            // TO DO : handle error
            ASSERT_DEBUG(false);
        }
        
        return;
    }
    
    GameClient::Instance().GetClientObject().SendCSRequestRejoinCurrentStage();
}

void AppDelegate::OnSCResponseCreatePlayer(flownet::UserID userID, flownet::ActorID playerID, flownet::SessionID sessionID) const
{
    CCLOG("AppDelegate::OnSCResponseCreatePlayer >> session responsed with %d", sessionID);
    BaseScene* scene = static_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
    ASSERT_DEBUG(scene);
    scene->OnResponse();
    
    GameClient::Instance().SetMyActorID(playerID);
    GameClient::Instance().SetSessionID(sessionID);
    
    if(sessionID == SessionID_NONE)
    {
        if( false == static_cast<ClientDirector*>(CCDirector::sharedDirector())->ChangeScene<AccountScene>() )
        {
            // TO DO : handle error
            ASSERT_DEBUG(false);
        }

        return;
    }
    
    if(sessionID != SessionID_NONE && playerID == ActorID_None)
    {
        if( false == static_cast<ClientDirector*>(CCDirector::sharedDirector())->ChangeScene<CharacterCreateScene>() )
        {
            // TO DO : handle error
            ASSERT_DEBUG(false);
        }
        return;
    }
    
    // TODO : play game video here!
    
    // NOTE : if create character create tutorial stage!
    GPSPoint gps = GPSPoint(37.566615 + CCRANDOM_0_1(), 126.977958 + CCRANDOM_0_1());
    GameClient::Instance().GetClientObject().SendCSRequestCreateStage(StageType_Intro, gps);
}

void AppDelegate::OnSCNotifySCErrorMessage(flownet::SCErrorMessage scErrorMessage, flownet::STRING errorMessage) const
{
    CCLOGERROR("SCErrorMessageCode : %d >> %s", scErrorMessage, errorMessage.c_str());
}

void AppDelegate::OnSCResponseHeartbeat(flownet::INT64 heartbeatCountAck) const
{
    // NOTE : the scene must be type of BaseScene
    CCDirector *pDirector = CCDirector::sharedDirector();
    BaseScene* scene = static_cast<BaseScene*>(pDirector->getRunningScene());
    HeartbeatLayer* heartbeatLayer = scene->GetHeartbeatLayer();
    
    heartbeatLayer->changeHeartbeatText(heartbeatCountAck);
}

void AppDelegate::OnFCResponseConnect(flownet::ConnectionID feConnectionID) const
{

}

void AppDelegate::OnFCNotifyFCErrorMessage(flownet::FCErrorMessage fcErrorMessage, flownet::STRING errorMessage) const
{
    CCLOGERROR("FCErrorMessageCode : %d >> %s", fcErrorMessage, errorMessage.c_str());
}

void AppDelegate::OnFCResponseCreateUserAccount(flownet::UserID userID) const
{
    BaseScene* scene = static_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
    ASSERT_DEBUG(scene);
    scene->OnResponse();
    if(userID != UserID_None)
    {
        CCLOG("AppDelegate::OnFCResponseCreateUserAccount >> User Account Created");
        // TO DO : go back to the account scene
        if( false == static_cast<ClientDirector*>(CCDirector::sharedDirector())->ChangeScene<AccountScene>() )
        {
            // TO DO : handle error
            ASSERT_DEBUG(false);
        }
    }
    else
    {
        CCLOG("AppDelegate::OnFCResponseCreateUserAccount >> Error. Create User Account");
        return;
        // TO DO : if failed alert in current scene
    }
}

void AppDelegate::OnFCResponseLogInUserAccount(flownet::UserID userID, flownet::GameServerID gameServerID, flownet::STRING gameServerIP, flownet::OTP otp) const
{
    BaseScene* scene = static_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
    ASSERT_DEBUG(scene);
    scene->OnResponse();
    
    if( userID == UserID_None )
    {
        CCLOG("AppDelegate::OnFCResponseLogInUserAccount >> LogInUserAccount Failed. 1) logged-in session is exist  2) useraccount / password missmatch");
        return;
    }
    else
    {
        GameClient::Instance().SetUserID(userID);
        GameClient::Instance().SetGameServerID(gameServerID);
        GameClient::Instance().SetOTP(otp);
        
        // save ServerIP
        CCUserDefault::sharedUserDefault()->setStringForKey("yours", gameServerIP.c_str());
        CCUserDefault::sharedUserDefault()->flush();
        
        this->DisconnectCFAndConnectCSConnection();
        
        GameClient::Instance().GetClientObject().SendCSRequestLogInWithOTP(GameClient::Instance().GetDeviceID(), userID, otp);
    }
}

void AppDelegate::OnSCResponseLogInWithOTP(flownet::UserID userID, flownet::ActorID playerID, flownet::SessionID sessionID) const
{
    GameClient::Instance().SetUserID(userID);
    GameClient::Instance().SetMyActorID(playerID);
    GameClient::Instance().SetSessionID(sessionID);    

    if( userID == UserID_None)
    {
        CCLOGERROR("AppDelegate::OnSCResponseLogInWithOTP >> login with OTP failed");
        this->DisconnectCSAndConnectCFConnection();
        
        if( false == static_cast<ClientDirector*>(CCDirector::sharedDirector())->ChangeScene<AccountScene>() )
        {
            // TO DO : handle error
            ASSERT_DEBUG(false);
        }

        return;
    }
    
    if( playerID == ActorID_None )
    {
        if( false == static_cast<ClientDirector*>(CCDirector::sharedDirector())->ChangeScene<CharacterCreateScene>() )
        {
            // TO DO : handle error
            ASSERT_DEBUG(false);
        }

        return;
    }
    
    GameClient::Instance().GetClientObject().SendCSRequestRejoinCurrentStage();
    ASSERT_DEBUG(sessionID != SessionID_NONE);
}


void AppDelegate::OnSCResponseLogOutUserAccount(flownet::UserID userID) const
{
    BaseScene* scene = static_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
    ASSERT_DEBUG(scene);
    scene->OnResponse();
    
    if( userID == UserID_None )
    {
        CCLOG("AppDelegate::OnSCResponseLogOutUserAccount >> LogOut Failed");
    }
    else
    {
        // NOTE : infos to be deleted = serverIP, gameServerID, stage, sessionID, userID, actorID, OTP
        // NOTE : reconnect to FE server
        CCUserDefault::sharedUserDefault()->setStringForKey("yours", "");
        CCUserDefault::sharedUserDefault()->flush();
        
        GameClient::Instance().Finalize();
        
        this->DisconnectCSAndConnectCFConnection();
        
        if( false == static_cast<ClientDirector*>(CCDirector::sharedDirector())->ChangeScene<AccountScene>() )
        {
            // TO DO : handle error
            ASSERT_DEBUG(false);
        }
    }
}

void AppDelegate::OnSCResponseCreateStage(flownet::StageID stageID, flownet::Stage stage) const
{
    if(stageID == StageID_None)
    {
        ASSERT_DEBUG(stageID != StageID_None);
        CCLOGERROR("AppDelegate::OnSCResponseCreateStage >> stage not created!");
    }

    ASSERT_DEBUG(stageID == stage.GetStageID());
    
    ClientStage* newClientStage = new ClientStage(stage);
    GameClient::Instance().SetClientStage(newClientStage);
    
    if( false == static_cast<ClientDirector*>(CCDirector::sharedDirector())->ChangeScene<StageScene>() )
    {
        // TO DO : handle error
        ASSERT_DEBUG(false);
    }
}

void AppDelegate::OnSCResponseGetStagePlayInfoList(flownet::StagePlayInfoList stageInfoList) const
{
    // TO DO : display running stages
    StageSelectScene* scene = dynamic_cast<StageSelectScene*>(CCDirector::sharedDirector()->getRunningScene());
    if(!scene)
    {
        CCLOG("AppDelegate::OnSCResponeRunningStages >> response ignored. requested but not in stage select scene");
        return;
    }
    
//    if(!stageInfoList.empty())
//    {
        scene->DisplayRunningStages(stageInfoList);
//    }
}

void AppDelegate::OnSCResponseJoinRunningStage(flownet::StageID stageID, flownet::Stage stage, flownet::ErrorType errorType) const
{
    if(stageID == StageID_None)
    {
        CCLOGERROR("joining to empty stage!");
        // TODO : 안내메시지로 참가할 수 없는 스테이지라고 알릴 것
        return;
    }
    ASSERT_DEBUG(stageID == stage.GetStageID());
    
    if(errorType != ErrorType_None){
        CCLOG("error occured while JoinRunningStage");
        return;
    }
    
    ClientStage* newClientStage = new ClientStage(stage);
    GameClient::Instance().SetClientStage(newClientStage);

    if( false == static_cast<ClientDirector*>(CCDirector::sharedDirector())->ChangeScene<StageScene>() )
    {
        // TO DO : handle error
        ASSERT_DEBUG(false);
    }
}

void AppDelegate::OnSCResponseExitStage(flownet::StageID stageID, flownet::ActorID playerID) const
{
    BaseScene* scene = static_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
    ASSERT_DEBUG(scene);
    scene->OnResponse();

    ClientStage* clientStage = GameClient::Instance().GetClientStage();
    if(clientStage == nullptr) return;
    if(stageID != clientStage->GetStageID()) ASSERT_DEBUG(stageID == clientStage->GetStageID());
    
    ASSERT_DEBUG(playerID == GameClient::Instance().GetMyActorID());
    
    ClientStage* emptyStage = new ClientStage(Stage());
    
    GameClient::Instance().SetClientStage(emptyStage);
    
    GPSPoint gps = GPSPoint(37.566615 + CCRANDOM_0_1(), 126.977958 + CCRANDOM_0_1());

    // NOTE : if clan community exit -> goes to stage map
    if(clientStage->GetStageType() == StageType_ClanCommunity)
    {
        ClientDirector* director = static_cast<ClientDirector*>(CCDirector::sharedDirector());
        director->ChangeScene<StageSelectScene>();
        return;
    }
    
    GameClient::Instance().GetClientObject().SendCSRequestJoinClanCommunityStage(gps);
}

void AppDelegate::OnSCNotifyExitStage(flownet::StageID stageID, flownet::ActorID actorID) const
{
    ClientStage* clientStage = GameClient::Instance().GetClientStage();
    if(clientStage == nullptr) return;
    if(stageID != clientStage->GetStageID()) ASSERT_DEBUG(stageID == clientStage->GetStageID());
    
    BaseScene* scene = static_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
    ASSERT_DEBUG(scene);
    
    ActorLayer* actorLayer = scene->GetActorLayer();
    ASSERT_DEBUG(actorLayer);
    
    // TO DO : fix auto release bug here
    
    actorLayer->DeleteActor(actorID);

    Actor* theActor = clientStage->FindActor(actorID);
    if( theActor == nullptr )
    {
        ASSERT_DEBUG(theActor!=nullptr);
    }

    if(IsPlayerID(actorID))
    {
        clientStage->RemovePlayer(actorID);
    }
    else
    {
        clientStage->RemoveMonster(actorID);
    }
    
    delete theActor;
    // TO DO : make merge delete functions
}

void AppDelegate::OnSCResponseRejoinCurrentStage(flownet::StageID stageID, flownet::Stage stage) const
{
    if(stageID == StageID_None)
    {
//        if( false == static_cast<ClientDirector*>(CCDirector::sharedDirector())->ChangeScene<StageSelectScene>() )
//        {
//            // TO DO : handle error
//            ASSERT_DEBUG(false);
//        }
        // NOTE : goto clan community
        GPSPoint gps = GPSPoint(37.566615 + CCRANDOM_0_1(), 126.977958 + CCRANDOM_0_1());
        GameClient::Instance().GetClientObject().SendCSRequestJoinClanCommunityStage(gps);
    }
    else
    {
        ClientStage* clientStage = new ClientStage(stage);
        GameClient::Instance().SetClientStage(clientStage);
        
        if( false == static_cast<ClientDirector*>(CCDirector::sharedDirector())->ChangeScene<StageScene>() )
        {
            // TO DO : handle error
            ASSERT_DEBUG(false);
        }
    }
}

void AppDelegate::OnSCNotifyClearStage(flownet::StageID stageID) const
{
    ClientStage* clientStage = GameClient::Instance().GetClientStage();
    if(!clientStage) return;
    if(stageID != clientStage->GetStageID()) ASSERT_DEBUG(stageID == clientStage->GetStageID());
    
    BaseScene* scene = static_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
    ASSERT_DEBUG(scene);
    
    UILayer* uiLayer = scene->GetUILayer();
    ASSERT_DEBUG(uiLayer);
    
    uiLayer->ShowStageClearMessage();
    
    // TO DO : end the stage
    CCLOG("clear stage notify recv");
    

}

void AppDelegate::OnSCNotifySpawnPlayer(flownet::StageID stageID, flownet::Player player) const
{
    ClientStage* clientStage = GameClient::Instance().GetClientStage();
    if(clientStage == nullptr || stageID != clientStage->GetStageID())
    {
        //ASSERT_DEBUG(clientStage != nullptr && stageID == clientStage->GetStageID());
        return;
    }

    BaseScene* scene = static_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
    
    ClientPlayer* newPlayer = new ClientPlayer(player);
    GameClient::Instance().GetClientStage()->AddPlayer(newPlayer->GetActorID(), newPlayer);

    ActorLayer* actorLayer = scene->GetActorLayer();
    ASSERT_DEBUG(actorLayer);
    actorLayer->AddNewPlayer(player);

    if(player.GetActorID() == GameClient::Instance().GetMyActorID())
    {
        UILayer* uiLayer = scene->GetUILayer();
        ASSERT_DEBUG(uiLayer);

        uiLayer->UpdateStash();
        uiLayer->UpdateInventory();
        uiLayer->UpdateEquipment();
    }
}

void AppDelegate::OnSCNotifySpawnMonster(flownet::StageID stageID, flownet::Monster monster) const
{
    ClientStage* clientStage = GameClient::Instance().GetClientStage();
    if(clientStage == nullptr || stageID != clientStage->GetStageID())
    {
        //ASSERT_DEBUG(clientStage != nullptr && stageID == clientStage->GetStageID());
        return;
    }
    
    BaseScene* scene = static_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());

    ClientMonster* newMonster = new ClientMonster(monster);
    GameClient::Instance().GetClientStage()->AddMonster(newMonster->GetActorID(), newMonster);
    scene->GetActorLayer()->AddNewMonster(monster);
}

void AppDelegate::OnSCNotifySpawnNPC(flownet::StageID stageID, flownet::NPC newNPC) const
{
    ClientStage* clientStage = GameClient::Instance().GetClientStage();
    if(!clientStage) return;
    if(stageID != clientStage->GetStageID()) ASSERT_DEBUG(stageID == clientStage->GetStageID());
    
    BaseScene* scene = static_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
    
    NPC* pNewNPC = new NPC(newNPC);
    GameClient::Instance().GetClientStage()->AddNPC(pNewNPC->GetActorID(), pNewNPC);
    ActorLayer* actorLayer = scene->GetActorLayer();
    ASSERT_DEBUG(actorLayer);
    actorLayer->AddNewNPC(newNPC);
}

void AppDelegate::OnSCNotifySpawnStageObject(flownet::StageID stageID, flownet::StageObject newStageObject) const
{
    ClientStage* clientStage = GameClient::Instance().GetClientStage();
    if(!clientStage) return;
    if(stageID != clientStage->GetStageID()) ASSERT_DEBUG(stageID == clientStage->GetStageID());
    
    BaseScene* scene = static_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
    
    StageObject* pNewStageObject = new StageObject(newStageObject);
    GameClient::Instance().GetClientStage()->AddStageObject(pNewStageObject->GetActorID(), pNewStageObject);
    
    ActorLayer* actorLayer = scene->GetActorLayer();
    ASSERT_DEBUG(actorLayer);
    actorLayer->AddNewStageObject(newStageObject);
}

void AppDelegate::OnSCNotifyMoveActor(flownet::StageID stageID, flownet::ActorID actorID, flownet::POINT currentPosition, flownet::POINT destinationPosition) const
{
    ClientStage* clientStage = GameClient::Instance().GetClientStage();
    if(!clientStage) return;
    if(stageID != clientStage->GetStageID()) ASSERT_DEBUG(stageID == clientStage->GetStageID());
    
    BaseScene* scene = static_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
    ActorLayer* actorLayer = scene->GetActorLayer();
    
    if(actorLayer == nullptr) return;

    actorLayer->MoveActor(actorID, currentPosition, destinationPosition);
}

void AppDelegate::OnSCNotifyKnockBackActor(flownet::StageID stageID, flownet::ActorID playerID, flownet::POINT currentPosition, flownet::POINT knockBackDestination) const
{
    ClientStage* clientStage = GameClient::Instance().GetClientStage();
    if(!clientStage) return;
    if(stageID != clientStage->GetStageID()) ASSERT_DEBUG(stageID == clientStage->GetStageID());
    
    BaseScene* scene = static_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
    ActorLayer* actorLayer = scene->GetActorLayer();
    
    if(!actorLayer) return;
    
    actorLayer->KnockBackActor(playerID, currentPosition, knockBackDestination);
}

void AppDelegate::OnSCNotifyActorAttributeChanged(flownet::StageID stageID, flownet::ActorID actorID, flownet::ActorAttribute actorAttribute, flownet::FLOAT amount) const
{
    ClientStage* clientStage = GameClient::Instance().GetClientStage();
    if(clientStage == nullptr || stageID != clientStage->GetStageID())
    {
        //ASSERT_DEBUG(clientStage != nullptr && stageID == clientStage->GetStageID());
        return;
    }
    
    BaseScene* scene = static_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
    ActorLayer* actorLayer = scene->GetActorLayer();
    UILayer* uiLayer = scene->GetUILayer();
    
    
    Actor* actor = GameClient::Instance().GetClientStage()->FindActor(actorID);
    if(!actor) return;
    
    actor->ChangeAttribute(clientStage, actorAttribute, amount);
    
    switch (actorAttribute) {
        case ActorAttribute_HealthPoint:
            ASSERT_DEBUG(actorLayer);
            actorLayer->ActorTakeDamage(actorID); // TO DO : change this function's naming
            break;
        case ActorAttribute_ManaPoint:
            ASSERT_DEBUG(actorLayer);
            actorLayer->ActorConsumedMana(actorID);
            break;
        case ActorAttribute_AttackPower:
            break;
        case ActorAttribute_SpellPower:
            break;
        case ActorAttribute_AttackDefence:
            break;
        case ActorAttribute_SpellResistance:
            break;
        case ActorAttribute_MovingSpeed:
            break;
        case ActorAttribute_AttackSpeed:
            break;
        case ActorAttribute_CastingSpeed:
            break;
        case ActorAttribute_ExperiencePoint:
        case ActorAttribute_Level :
            ASSERT_DEBUG(uiLayer);
            uiLayer->UpdateExpBar();
            break;
        default:
            break;
    
    }
}

void AppDelegate::OnSCNotifyResetActorAttributeAmplifier(flownet::StageID stageID, flownet::ActorID actorID) const
{
    ClientStage* clientStage = GameClient::Instance().GetClientStage();
    if(!clientStage) return;
    if(stageID != clientStage->GetStageID()) ASSERT_DEBUG(stageID == clientStage->GetStageID());
    
}


void AppDelegate::OnSCNotifyActorAttack(flownet::StageID stageID, flownet::ActorID actorID, flownet::ActorID targetActorID, flownet::AttackType attackType) const
{
    ClientStage* clientStage = GameClient::Instance().GetClientStage();
    if(!clientStage) return;
    if(stageID != clientStage->GetStageID()) ASSERT_DEBUG(stageID == clientStage->GetStageID());
    
    BaseScene* scene = static_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
    ActorLayer* actorLayer = scene->GetActorLayer();
    
    if(actorLayer == nullptr) return;
    
    actorLayer->ActorAttack(actorID, targetActorID);
}

void AppDelegate::OnSCNotifyActorAttacked(flownet::StageID stageID, flownet::ActorID actorID, flownet::ActorID attackerActorID, flownet::AttackType attackType) const
{
    ClientStage* clientStage = GameClient::Instance().GetClientStage();
    if(clientStage == nullptr || stageID != clientStage->GetStageID())
    {
        //ASSERT_DEBUG(clientStage != nullptr && stageID == clientStage->GetStageID());
        return;
    }

    BaseScene* scene = static_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
    ActorLayer* actorLayer = scene->GetActorLayer();
    
    if(actorLayer == nullptr) return;
    
    actorLayer->ActorAttacked(actorID, attackerActorID);
}

void AppDelegate::OnSCNotifyActorDead(flownet::StageID stageID, flownet::ActorID actorID) const
{
    ClientStage* clientStage = GameClient::Instance().GetClientStage();
    if(clientStage == nullptr || stageID != clientStage->GetStageID())
    {
        //ASSERT_DEBUG(clientStage != nullptr && stageID == clientStage->GetStageID());
        return;
    }

    BaseScene* scene = static_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
    ActorLayer* actorLayer = scene->GetActorLayer();
    
    if(actorLayer == nullptr) return;
    
    actorLayer->ActorDead(actorID);
}

void AppDelegate::OnSCNotifyChangeTarget(flownet::StageID stageID, flownet::ActorID actorID, flownet::ActorID targetID) const
{
    ClientStage* clientStage = GameClient::Instance().GetClientStage();
    if(clientStage == nullptr || stageID != clientStage->GetStageID())
    {
        //ASSERT_DEBUG(clientStage != nullptr && stageID == clientStage->GetStageID());
        return;
    }
   
    BaseScene* scene = static_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
    ActorLayer* actorLayer = scene->GetActorLayer();
    
    if( actorLayer == nullptr ) return;
    
    // NOTE : these two are mutual exclusive
    if(IsPlayerID(actorID))
    {
        ClientPlayer* player = static_cast<ClientPlayer*>(GameClient::Instance().GetClientStage()->FindPlayer(actorID));
        if(player == nullptr)
        {
            ASSERT_DEBUG(player != nullptr);
            return;
        }
        player->ChangeTarget(GameClient::Instance().GetClientStage(), targetID);
    }
    if(IsMonsterID(actorID))
    {
        ClientMonster* monster = static_cast<ClientMonster*>(GameClient::Instance().GetClientStage()->FindMonster(actorID));
        if(monster == nullptr)
        {
            ASSERT_DEBUG(monster != nullptr);
            return;
        }
        monster->ChangeTarget(GameClient::Instance().GetClientStage(), targetID);
    }
    
    actorLayer->ChangeTarget(actorID, targetID);
}

void AppDelegate::OnSCNotifyBeginCast(flownet::StageID stageID, flownet::ActorID actorID, flownet::SpellType spellType, flownet::POINT destination) const
{
    ClientStage* clientStage = GameClient::Instance().GetClientStage();
    if(clientStage == nullptr || stageID != clientStage->GetStageID())
    {
        //ASSERT_DEBUG(clientStage != nullptr && stageID == clientStage->GetStageID());
        return;
    }

    BaseScene* scene = static_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
    ActorLayer* actorLayer = scene->GetActorLayer();
    
    if(actorLayer == nullptr) return;
    
    actorLayer->ActorBeginCast(actorID, spellType, destination);
}

void AppDelegate::OnSCNotifyEndCast(flownet::StageID stageID, flownet::ActorID actorID, flownet::SpellType spellType, flownet::POINT destination) const
{
    ClientStage* clientStage = GameClient::Instance().GetClientStage();
    if(clientStage == nullptr || stageID != clientStage->GetStageID())
    {
        //ASSERT_DEBUG(clientStage != nullptr && stageID == clientStage->GetStageID());
        return;
    }

    BaseScene* scene = static_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
    ActorLayer* actorLayer = scene->GetActorLayer();
    
    if(actorLayer == nullptr) return;

    actorLayer->ActorEndCast(actorID, spellType, destination);
    
    UILayer* uiLayer = scene->GetUILayer();
    
    if(actorID == GameClient::Instance().GetMyActorID())
    {
        if(!uiLayer)
        {
            CCLOGERROR("AppDelegate::OnSCNotifyEndCast >> no UILayer exists");
            ASSERT_DEBUG(uiLayer != nullptr);
        }
        uiLayer->ApplyCoolTime(spellType);
    }
}

void AppDelegate::OnSCResponseDropItemToField(flownet::StageID stageID, flownet::ActorID playerID, flownet::ItemID itemID) const
{
    ClientStage* clientStage = GameClient::Instance().GetClientStage();
    if(clientStage == nullptr || stageID != clientStage->GetStageID())
    {
        //ASSERT_DEBUG(clientStage != nullptr && stageID == clientStage->GetStageID());
        return;
    }
    
    if(playerID != GameClient::Instance().GetMyActorID()) return;
    
    Actor* actor = clientStage->FindActor(playerID);
    actor->DropItemToField(clientStage, itemID, POINT(0, 0));
    
    BaseScene* scene = static_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
    UILayer* uiLayer = scene->GetUILayer();
    if(uiLayer)
    {
        // TO DO : check the item is removed when dropped
        uiLayer->UpdateStash();
        uiLayer->UpdateInventory();
    }
}

void AppDelegate::OnSCNotifySpawnItem(flownet::StageID stageID, flownet::Item item, flownet::POINT spawnPosition) const
{
    ClientStage* clientStage = GameClient::Instance().GetClientStage();
    if(clientStage == nullptr || stageID != clientStage->GetStageID())
    {
        //ASSERT_DEBUG(clientStage != nullptr && stageID == clientStage->GetStageID());
        return;
    }
    
    clientStage->GetStash().CreateAndAddItem(item.GetItemType(), item.GetItemID());
    
    BaseScene* scene = static_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
    scene->GetActorLayer()->AddNewItem(item, spawnPosition);
}

void AppDelegate::OnSCNotifyPickUpItemFromField(flownet::StageID stageID, flownet::ActorID playerID, flownet::ItemID itemID) const
{
    ClientStage* clientStage = GameClient::Instance().GetClientStage();
    if(clientStage == nullptr || stageID != clientStage->GetStageID())
    {
        //ASSERT_DEBUG(clientStage != nullptr && stageID == clientStage->GetStageID());
        return;
    }
    
    Actor* actor = GameClient::Instance().GetClientStage()->FindActor(playerID);
    actor->PickUpItemFromField(clientStage, itemID);
    
    BaseScene* scene = static_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
    ActorLayer* actorLayer = scene->GetActorLayer();
    if(!actorLayer)
    {
        CCLOGERROR("AppDelegate::OnSCNotifyPickUpItemFromField >> no ActorLayer exist");
        ASSERT_DEBUG(actorLayer != nullptr);
    }

    actorLayer->PickupItemFromField(playerID, itemID);

    if(playerID == GameClient::Instance().GetMyActorID())
    {
        scene->GetUILayer()->UpdateStash();
        scene->GetUILayer()->UpdateInventory();
    }
}

void AppDelegate::OnSCNotifyAddItemToStash(flownet::StageID stageID, flownet::ActorID playerID, flownet::Item item) const
{
    ClientStage* clientStage = GameClient::Instance().GetClientStage();
    if(clientStage == nullptr || stageID != clientStage->GetStageID())
    {
        //ASSERT_DEBUG(clientStage != nullptr && stageID == clientStage->GetStageID());
        return;
    }
    
    if(playerID != GameClient::Instance().GetMyActorID()) return;
    
    Actor* actor = clientStage->FindActor(playerID);
    actor->GetStash().CreateAndAddItem(item.GetItemType(), item.GetItemID());


    // 상점, 스테이지 만들어졋을때임.
    
    // BaseScene* scene->
    // TO DO : ui work left
    // TO DO : notify I've got an item!
}

void AppDelegate::OnSCNotifyRemoveItemFromStash(flownet::StageID stageID, flownet::ActorID playerID, flownet::ItemID itemID) const
{
    ClientStage* clientStage = GameClient::Instance().GetClientStage();
    if(clientStage == nullptr || stageID != clientStage->GetStageID())
    {
        //ASSERT_DEBUG(clientStage != nullptr && stageID == clientStage->GetStageID());
        return;
    }
    
    
    // 사용 혹은 뭐에 대한 데이터처리용
    
    // TO DO : reask to him
}

void AppDelegate::OnSCResponseRegisterStashItemToInventory(flownet::StageID stageID, flownet::ActorID playerID, flownet::ItemID itemID, flownet::ItemGroup itemGroup, flownet::InventorySlot slotNumber) const
{
    ClientStage* clientStage = GameClient::Instance().GetClientStage();
    if(clientStage == nullptr || stageID != clientStage->GetStageID())
    {
        //ASSERT_DEBUG(clientStage != nullptr && stageID == clientStage->GetStageID());
        return;
    }

    if(playerID != GameClient::Instance().GetMyActorID())
    {
        ASSERT_DEBUG(false);
        return;
    }
    
    Actor* actor = clientStage->FindActor(playerID);
    actor->RegisterStashItemToInventory(clientStage, itemID, itemGroup, slotNumber);
    
    // TO DO : display on inventory
}

void AppDelegate::OnSCResponseUnRegisterStashItemFromInventory(flownet::StageID stageID, flownet::ActorID playerID, flownet::ItemID itemID, flownet::ItemGroup itemGroup, flownet::InventorySlot slotNumber) const
{
    ClientStage* clientStage = GameClient::Instance().GetClientStage();
    if(clientStage == nullptr || stageID != clientStage->GetStageID())
    {
        //ASSERT_DEBUG(clientStage != nullptr && stageID == clientStage->GetStageID());
        return;
    }
    
    if(playerID != GameClient::Instance().GetMyActorID())
    {
        ASSERT_DEBUG(false);
        return;
    }
    
    Actor* actor = clientStage->FindActor(playerID);
    actor->UnRegisterStashItemFromInventory(clientStage, itemID, itemGroup, slotNumber);
    // TO DO : undisplay on inventory
}

void AppDelegate::OnSCResponseSwapInventorySlot(flownet::StageID stageID, flownet::ActorID playerID, flownet::ItemGroup itemGroup, flownet::InventorySlot sourceSlotNumber, flownet::InventorySlot destinationSlotNumber) const
{
    ClientStage* clientStage = GameClient::Instance().GetClientStage();
    if(clientStage == nullptr || stageID != clientStage->GetStageID())
    {
        //ASSERT_DEBUG(clientStage != nullptr && stageID == clientStage->GetStageID());
        return;
    }

    BaseScene* scene = static_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
    scene->GetUILayer()->SwapInventorySlot(sourceSlotNumber, destinationSlotNumber);
}

void AppDelegate::OnSCNotifyUseItem(flownet::StageID stageID, flownet::ActorID playerID, flownet::ItemID itemID) const
{
    // TO DO : refactor this error detect code
    //          divide into clientstage == nullptr -> return;
    //          stageID != clientStage->GetStageID() -> ASSERT
    ClientStage* clientStage = GameClient::Instance().GetClientStage();
    if(clientStage == nullptr || stageID != clientStage->GetStageID())
    {
        //ASSERT_DEBUG(clientStage != nullptr && stageID == clientStage->GetStageID());
        return;
    }
    
    Player* player = clientStage->FindPlayer(playerID);
    if(!player)
    {
        CCLOGERROR("AppDelegate::OnSCNotifyUseItem >> use item notify on non exist player");
        ASSERT_DEBUG(player != nullptr);
    }
    player->UseItem(clientStage, itemID);


    BaseScene* scene = static_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
    ActorLayer* actorLayer = scene->GetActorLayer();
    
    if(!actorLayer)
    {
        CCLOGERROR("AppDelegate::OnSCNotifyUseItem >> no ActorLayer exist");
        ASSERT_DEBUG(actorLayer != nullptr);
    }
    actorLayer->UseItem(playerID, itemID);

    if(playerID == GameClient::Instance().GetMyActorID())
    {
        UILayer* uiLayer = scene->GetUILayer();
        if(!uiLayer)
        {
            CCLOGERROR("AppDelegate::OnSCNotifyUseItem >> no UILayer exist");
            ASSERT_DEBUG(uiLayer != nullptr);
        }
        
        uiLayer->UpdateStash();
        uiLayer->UpdateInventory();
    }
}

void AppDelegate::OnSCNotifyEquipItem(flownet::StageID stageID, flownet::ActorID playerID, flownet::ItemID itemID, flownet::EquipmentSlot equipedSlot) const
{
    ClientStage* clientStage = GameClient::Instance().GetClientStage();
    if(clientStage == nullptr) return;
    if(stageID != clientStage->GetStageID()) ASSERT_DEBUG(stageID == clientStage->GetStageID());

    Actor* actor = clientStage->FindPlayer(playerID);
    if(!actor)
    {
        CCLOGERROR("AppDelegate::OnSCNotifyEquipItem >> actor %d does not exists", playerID);
        ASSERT_DEBUG(actor);
    }
    
    if(!actor->EquipItem(clientStage, equipedSlot, itemID))
    {
        CCLOGERROR("AppDelegate::OnSCNotifyEquipItem >> failed to actor->EquipItem");
        ASSERT_DEBUG(false);
    }
    
    BaseScene* scene = static_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
    ActorLayer* actorLayer = scene->GetActorLayer();
    if(actorLayer)
    {
        actorLayer->ActorNodeReload(playerID);
    }
    
    if(playerID == GameClient::Instance().GetMyActorID())
    {
        UILayer* uiLayer = scene->GetUILayer();
        if(uiLayer)
        {
            uiLayer->UpdateInventory();
            uiLayer->UpdateStash();
            uiLayer->UpdateEquipment();
        }
    }
}

void AppDelegate::OnSCNotifyUnEquipItem(flownet::StageID stageID, flownet::ActorID playerID, flownet::EquipmentSlot unequipedSlot) const
{
    // TO DO : replace all error prevention code to this
    ClientStage* clientStage = GameClient::Instance().GetClientStage();
    if(clientStage == nullptr) return;
    if(stageID != clientStage->GetStageID()) ASSERT_DEBUG(stageID == clientStage->GetStageID());
    // end of prevention
    
    Actor* actor = clientStage->FindPlayer(playerID);
    if(!actor)
    {
        CCLOGERROR("AppDelegate::OnSCNotifyUnEquipItem >> actor %d does not exists", playerID);
        ASSERT_DEBUG(actor);
    }
    
    actor->UnEquipItem(clientStage, unequipedSlot);
    
    BaseScene* scene = static_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
    ActorLayer* actorLayer = scene->GetActorLayer();
    if(actorLayer)
    {
        // TO DO : update actor's visual
        actorLayer->ActorNodeReload(playerID);
    }
    
    if(playerID == GameClient::Instance().GetMyActorID())
    {
        UILayer* uiLayer = scene->GetUILayer();
        if(uiLayer)
        {
            // TO DO : update stash?, inven?, equip
            uiLayer->UpdateInventory();
            uiLayer->UpdateStash();
            uiLayer->UpdateEquipment();
        }
    }
}

void AppDelegate::OnSCNotifySendMessageToStagePlayers(flownet::StageID stageID, flownet::ActorID playerID, flownet::STRING message) const
{
    ClientStage* clientStage = GameClient::Instance().GetClientStage();
    if(clientStage == nullptr) return;
    if(stageID != clientStage->GetStageID()) ASSERT_DEBUG(stageID == clientStage->GetStageID());
    
    Player* player = GameClient::Instance().GetClientStage()->FindPlayer(playerID);
    
    GameClient::Instance().AddChatMessageLog(playerID, player->GetPlayerName(), message);
    
    BaseScene* scene = static_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
    ASSERT_DEBUG(scene);
    
    UILayer* uiLayer = scene->GetUILayer();
    ASSERT_DEBUG(uiLayer);
    
    uiLayer->MessageReceived(playerID, player->GetPlayerName(), message);
    
    ActorLayer* actorLayer = scene->GetActorLayer();
    ASSERT_DEBUG(actorLayer);
    
    actorLayer->MessageReceived(playerID, message);
}

void AppDelegate::OnSCNotifyApplySpellAbility(flownet::StageID stageID, flownet::ActorID targetID, flownet::ActorID invokerID, flownet::SpellAbility spellAbility, flownet::FLOAT amount) const
{
    ClientStage* clientStage = GameClient::Instance().GetClientStage();
    if(clientStage == nullptr) return;
    if(stageID != clientStage->GetStageID()) ASSERT_DEBUG(stageID == clientStage->GetStageID());
    // TO DO : show spell effects
    
    Actor* actor = clientStage->FindActor(targetID);
    ASSERT_DEBUG(actor);
    Actor* invoker = clientStage->FindActor(invokerID);
    ASSERT_DEBUG(invoker);
    
    SpellAbilityDataList& activeSpellAbilityDataList = actor->GetSpellAbilityDataList();
    bool found = false;
    std::for_each(activeSpellAbilityDataList.begin(), activeSpellAbilityDataList.end(), [this, &found, spellAbility](SpellAbilityData& spellAbilityData){
        if(spellAbilityData.m_SpellAbility == spellAbility){
            found = true;
        }
    });
    
    actor->ApplySpellAbilityForClient(spellAbility, amount);
    
    BaseScene* scene = static_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
    ASSERT_DEBUG(scene);
    
    ActorLayer* actorLayer = scene->GetActorLayer();
    ASSERT_DEBUG(actorLayer);
            
    if(!found)
    {
        actorLayer->AddSpellEffect(targetID, spellAbility);
    }
}

void AppDelegate::OnSCNotifyClearSpellAbility(flownet::StageID stageID, flownet::ActorID targetID, flownet::SpellAbility spellAbility, flownet::FLOAT amount) const
{
    ClientStage* clientStage = GameClient::Instance().GetClientStage();
    if(clientStage == nullptr) return;
    if(stageID != clientStage->GetStageID()) ASSERT_DEBUG(stageID == clientStage->GetStageID());
    // TO DO : remove spell effects
    
    Actor* actor = clientStage->FindActor(targetID);
    ASSERT_DEBUG(actor);
    
    actor->ClearSpellAbilityForClient(spellAbility);
    
    BaseScene* scene = static_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
    ASSERT_DEBUG(scene);
    
    ActorLayer* actorLayer = scene->GetActorLayer();
    ASSERT_DEBUG(actorLayer);
    
    // NOTE : check for same ability left
    SpellAbilityDataList& activeSpellAbilityDataList = actor->GetSpellAbilityDataList();
    bool found = false;
    std::for_each(activeSpellAbilityDataList.begin(), activeSpellAbilityDataList.end(), [this, &found, spellAbility](SpellAbilityData& spellAbilityData){
        if(spellAbilityData.m_SpellAbility == spellAbility){
            found = true;
        }
    });
    
    if(!found)
    {
        actorLayer->RemoveSpellEffect(targetID, spellAbility);
    }
}

void AppDelegate::OnSCNotifySetFreeze(flownet::StageID stageID, flownet::ActorID targetID, flownet::BOOL isFreezed) const
{
    ClientStage* clientStage = GameClient::Instance().GetClientStage();
    if(clientStage == nullptr) return;
    if(stageID != clientStage->GetStageID()) ASSERT_DEBUG(stageID == clientStage->GetStageID());
    
    // TO DO : change actor status
    Actor* actor = clientStage->FindActor(targetID);
    ASSERT_DEBUG(actor);
    
    actor->SetFreeze(clientStage, isFreezed);
}

void AppDelegate::OnSCNotifyRunOutMovingMana(StageID stageID, ActorID actorID) const
{
    ClientStage* clientStage = GameClient::Instance().GetClientStage();
    if(clientStage == nullptr) return;
    if(stageID != clientStage->GetStageID()) ASSERT_DEBUG(stageID == clientStage->GetStageID());
    
    BaseScene* scene = static_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());;
    ASSERT_DEBUG(scene);
    
    ActorLayer* actorLayer = scene->GetActorLayer();
    ASSERT_DEBUG(actorLayer);
    actorLayer->ActorRunOutOfMana(actorID);
}

void AppDelegate::OnSCNotifyRunOutSpellMana(StageID stageID, ActorID actorID) const
{
    // To Do : implement this handler
    
}