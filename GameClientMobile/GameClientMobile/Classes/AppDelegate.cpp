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
    
    // Initialize GameClient
    flownet::GameClient::Instance().InitializeClient(this);
    flownet::GameClient::Instance().StartClient();
    
    this->InitializeConnection();

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    // Stop Gameclient
    flownet::GameClient::Instance().TerminateClient();
    
    CCDirector::sharedDirector()->stopAnimation();
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->pauseAllEffects();
    
    CCUserDefault::sharedUserDefault()->flush();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    // Re start GameClient
    flownet::GameClient::Instance().InitializeClient(this);
    flownet::GameClient::Instance().StartClient();
    
    this->InitializeConnection();

    CCDirector::sharedDirector()->startAnimation();
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->resumeAllEffects();
}

void AppDelegate::InitializeConnection()
{
    std::string serverIP;
    serverIP = CCUserDefault::sharedUserDefault()->getStringForKey("yours", "");
    
    GameClient::Instance().GetCFConnection().InitializeClient("61.43.139.149", 1990);
    GameClient::Instance().GetClientObject().InitializeClient(serverIP.c_str(), SERVER_CONNECT_PORT);
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
    
    if(sessionID == SessionID_NONE)
    {
//        GameClient::Instance().GetClientObject().Disconnect();
        
//        if(!GameClient::Instance().GetCFConnection().InitializeClientWithBlocking(FESERVER_CF_CONNECT_ADDRESS, FESERVER_CF_CONNECT_PORT))
//        {
//            ASSERT_DEBUG( false );
//        }
        
        if( false == static_cast<ClientDirector*>(CCDirector::sharedDirector())->ChangeScene<AccountScene>() )
        {
            // TO DO : handle error
            ASSERT_DEBUG(false);
        }

        return;
    }
    
    GameClient::Instance().GetClientObject().SendCSRequestRejoinCurrentStage();
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

void AppDelegate::OnFCResponseCreateUserAccount(flownet::UserID userID) const
{
    if(userID != UserID_None)
    {
        CCLOG("AppDelegate::OnFCResponseCreateUserAccount >> User Account Created");
    // TO DO :
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
    if( userID == UserID_None )
    {
        CCLOG("AppDelegate::OnFCResponseLogInUserAccount >> LogInUserAccount Failed. 1) logged-in session is exist  2) useraccount / password missmatch");
        return;
    }
    else
    {
//        GameClient::Instance().GetCFConnection().Disconnect();
        
        GameClient::Instance().SetUserID(userID);
        GameClient::Instance().SetGameServerID(gameServerID);
        GameClient::Instance().SetOTP(otp);
        
        // save ServerIP
        CCUserDefault::sharedUserDefault()->setStringForKey("yours", gameServerIP.c_str());
        CCUserDefault::sharedUserDefault()->flush();
        
        GameClient::Instance().GetClientObject().SendCSRequestLogInWithOTP(GameClient::Instance().GetDeviceID(), userID, otp);
        
//        GameClient::Instance().GetClientObject().InitializeClient(gameServerIP.c_str(), SERVER_CONNECT_PORT);
    }
}

void AppDelegate::OnSCResponseLogInWithOTP(flownet::UserID userID, flownet::ActorID playerID, flownet::SessionID sessionID) const
{
    GameClient::Instance().SetUserID(userID);
    GameClient::Instance().SetMyActorID(playerID);
    GameClient::Instance().SetSessionID(sessionID);    

    if( userID == UserID_None)
    {
        CCLOG("LogInWithOTP FAILED");
        return;
    }
    
    if( playerID == ActorID_None )
    {
        // To Do : have to create new player
        return;
    }
    
    GameClient::Instance().GetClientObject().SendCSRequestRejoinCurrentStage();
    ASSERT_DEBUG(sessionID != SessionID_NONE);
}


void AppDelegate::OnSCResponseLogOutUserAccount(flownet::UserID userID) const
{
    if( userID == UserID_None )
    {
        CCLOG("AppDelegate::OnSCResponseLogOutUserAccount >> LogOut Failed");
    }
    else
    {
        CCUserDefault::sharedUserDefault()->setStringForKey("yours", "");
        CCUserDefault::sharedUserDefault()->flush();
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
    
    if( false == static_cast<ClientDirector*>(CCDirector::sharedDirector())->ChangeScene<BattleScene>() )
    {
        // TO DO : handle error
        ASSERT_DEBUG(false);
    }
}

void AppDelegate::OnSCResponseRunningStages(flownet::StageInfoList stageInfoList) const
{
    // TO DO : display running stages
    if(!stageInfoList.empty())
    {
        StageInfo& stage = stageInfoList[0];
        GameClientObject& gameClientObject = GameClient::Instance().GetClientObject();
        gameClientObject.SendCSRequestJoinRunningStage(stage.m_StageID);
    }
}

void AppDelegate::OnSCResponseJoinRunningStage(flownet::StageID stageID, flownet::Stage stage, flownet::ErrorType errorType) const
{
    if(stageID == StageID_None)
    {
        ASSERT_DEBUG(stageID != StageID_None);
        CCLOGERROR("joining to empty stage!");
    }
    ASSERT_DEBUG(stageID == stage.GetStageID());
    
    if(errorType != ErrorType_None){
        CCLOG("error occured while JoinRunningStage");
        return;
    }
    
    ClientStage* newClientStage = new ClientStage(stage);
    GameClient::Instance().SetClientStage(newClientStage);

    if( false == static_cast<ClientDirector*>(CCDirector::sharedDirector())->ChangeScene<BattleScene>() )
    {
        // TO DO : handle error
        ASSERT_DEBUG(false);
    }
}

void AppDelegate::OnSCResponseExitStage(flownet::StageID stageID, flownet::ActorID playerID) const
{
    ClientStage* clientStage = GameClient::Instance().GetClientStage();
    if(clientStage == nullptr || stageID != clientStage->GetStageID())
    {
        ASSERT_DEBUG(stageID == clientStage->GetStageID());
        return;
    }

}

void AppDelegate::OnSCNotifyExitStage(flownet::StageID stageID, flownet::ActorID playerID) const
{
    ClientStage* clientStage = GameClient::Instance().GetClientStage();
    if(clientStage == nullptr || stageID != clientStage->GetStageID())
    {
        //ASSERT_DEBUG(clientStage != nullptr && stageID == clientStage->GetStageID());
        return;
    }
}

void AppDelegate::OnSCResponseRejoinCurrentStage(flownet::StageID stageID, flownet::Stage stage) const
{
    if(stageID == StageID_None)
    {
        // TO DO : go to main menu
        if( false == static_cast<ClientDirector*>(CCDirector::sharedDirector())->ChangeScene<StageScene>() )
        {
            // TO DO : handle error
            ASSERT_DEBUG(false);
        }
    }
    else
    {
        ClientStage* clientStage = new ClientStage(stage);
        GameClient::Instance().SetClientStage(clientStage);
        
        if( false == static_cast<ClientDirector*>(CCDirector::sharedDirector())->ChangeScene<BattleScene>() )
        {
            // TO DO : handle error
            ASSERT_DEBUG(false);
        }
    }
}

void AppDelegate::OnSCNotifyClearStage(flownet::StageID stageID) const
{
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
    
    UILayer* uiLayer = scene->GetUILayer();
    ASSERT_DEBUG(uiLayer);
    uiLayer->UpdateInventory();
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

void AppDelegate::OnSCNotifyMoveActor(flownet::StageID stageID, flownet::ActorID actorID, flownet::POINT currentPosition, flownet::POINT destinationPosition) const
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

    actorLayer->MoveActor(actorID, currentPosition, destinationPosition);
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
    
    if(actorLayer == nullptr) return;
    
    Actor* actor = GameClient::Instance().GetClientStage()->FindActor(actorID);

    actor->ChangeAttribute(clientStage, actorAttribute, amount);
    
    switch (actorAttribute) {
        case ActorAttribute_HealthPoint:
            actorLayer->ActorTakeDamage(actorID); // TO DO : change this function's naming
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
            break;
        default:
            break;
    
    }
}


void AppDelegate::OnSCNotifyActorAttack(flownet::StageID stageID, flownet::ActorID actorID, flownet::ActorID targetActorID, flownet::AttackType attackType) const
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

//void AppDelegate::OnSCNotifyActorTakeDamage(flownet::StageID stageID, ActorID actorID, FLOAT damageAmount) const
//{
//    ClientStage* clientStage = GameClient::Instance().GetClientStage();
//    if(clientStage == nullptr || stageID != clientStage->GetStageID())
//    {
//        std::cout << "Error clientStage==nullptr" << std::endl;
//        return;
//    }
//
//    BaseScene* scene = static_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
//    ActorLayer* actorLayer = scene->GetActorLayer();
//    
//    if(actorLayer == nullptr) return;
//    
//    if(IsPlayerID(actorID))
//    {
//        static_cast<ClientPlayer*>(GameClient::Instance().GetClientStage()->FindPlayer(actorID))->OnAttacked(damageAmount);
//    }
//    if(IsMonsterID(actorID))
//    {
//        static_cast<ClientMonster*>(GameClient::Instance().GetClientStage()->FindMonster(actorID))->OnAttacked(damageAmount);
//    }
//    
//    // TO DO : display hp bar over the character
//    actorLayer->ActorTakeDamage(actorID);
//}

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

//void AppDelegate::OnSCNotifyFireSpell(flownet::StageID stageID, flownet::ActorID invokerID, flownet::SpellType spellType, flownet::POINT destination) const
//{
//    ClientStage* clientStage = GameClient::Instance().GetClientStage();
//    if(stageID != clientStage->GetStageID())
//    {
//        ASSERT_DEBUG(stageID == clientStage->GetStageID());
//        return;
//    }
//
//    BaseScene* scene = static_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
//    ActorLayer* actorLayer = scene->GetActorLayer();
//    
//    if(actorLayer == nullptr) return;
//
//    actorLayer->ActorFireSpell(invokerID, spellType, destination);
//}
//

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
    // ui + data
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

void AppDelegate::OnSCNotifyUnEquip(flownet::StageID stageID, flownet::ActorID playerID, flownet::ItemID itemID) const
{
    ClientStage* clientStage = GameClient::Instance().GetClientStage();
    if(clientStage == nullptr || stageID != clientStage->GetStageID())
    {
        //ASSERT_DEBUG(clientStage != nullptr && stageID == clientStage->GetStageID());
        return;
    }

}
