//
//  BaseScene.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 3/5/13.
//
//

#include "Headers.pch"

BaseScene::BaseScene() : m_BackgroundLayer(nullptr), m_EffectLayer(nullptr), m_ObjectLayer(nullptr), m_ActorLayer(nullptr), /*m_PlayerLayer(nullptr), m_MonsterLayer(nullptr),*/ m_UILayer(nullptr), m_LoadingLayer(nullptr), m_LastGPSTime(0)
{
    CCLOG("Base scene created");
}

BaseScene::~BaseScene()
{
    if(m_BackgroundLayer)
    {
        m_BackgroundLayer->release();
        m_BackgroundLayer = nullptr;
    }
    if(m_EffectLayer)
    {
        m_EffectLayer->release();
        m_EffectLayer = nullptr;
    }
    if(m_ObjectLayer)
    {
        m_ObjectLayer->release();
        m_ObjectLayer = nullptr;
    }
    if(m_ActorLayer)
    {
        m_ActorLayer->release();
        m_ActorLayer = nullptr;
    }
    if(m_UILayer)
    {
        m_UILayer->release();
        m_UILayer = nullptr;
    }
    if(m_LoadingLayer)
    {
        m_LoadingLayer->release();
        m_LoadingLayer = nullptr;
    }
    CCLOG("Base scene des");
}

bool BaseScene::init()
{
    if(!CCScene::init())
    {
        return false;
    }
    
    // load animations
    SpellAnimationLoader::Instance();
    
    // NOTE : all animation file comes here
    std::vector<std::string> animationPListFileList = {
        "spell_animations.plist",
        };
    std::for_each(animationPListFileList.begin(), animationPListFileList.end(), [](std::string plistFileName){
        CCAnimationCache::sharedAnimationCache()->addAnimationsWithFile(plistFileName.c_str());
    });

    
    this->m_HeartbeatLayer = HeartbeatLayer::create();
    this->m_HeartbeatLayer->retain();
    
    //this->InitializeGPSInfo();
    
    this->addChild(m_HeartbeatLayer, 10);
    
    return true;
}

void BaseScene::update(float deltaTime)
{
    ClientDirector* clientDirector = static_cast<ClientDirector*>(CCDirector::sharedDirector());
    if( clientDirector->IsRenderingWorkerRoutinePaused() )
    {
        clientDirector->ResumeRenderingTaskWorkerRoutine();
    }
    RenderingTaskWorkerRoutine& renderingTaskWorkerRoutine = GameClient::Instance().GetRenderingTaskWorkerRoutine();
    renderingTaskWorkerRoutine.Run(THREAD_SCHEDULING_TIMESLICE);
    
    //this->UpdateGPSInfo();
}

BackgroundLayer* BaseScene::GetBackgroundLayer() const
{
    return this->m_BackgroundLayer;
}

EffectLayer* BaseScene::GetEffectLayer() const
{
    return this->m_EffectLayer;
}

ObjectLayer* BaseScene::GetObjectLayer() const
{
    return this->m_ObjectLayer;
}

ActorLayer* BaseScene::GetActorLayer() const
{
    return this->m_ActorLayer;
}
//PlayerLayer* BaseScene::GetPlayerLayer() const
//{
//    return this->m_PlayerLayer;
//}
//
//MonsterLayer* BaseScene::GetMonsterLayer() const
//{
//    return this->m_MonsterLayer;
//}


UILayer* BaseScene::GetUILayer() const
{
    return this->m_UILayer;
}

HeartbeatLayer* BaseScene::GetHeartbeatLayer() const
{
    return this->m_HeartbeatLayer;
}

void BaseScene::AddLoadingSpinnerAndBlock()
{
    this->RemoveLoadingSpinnerAndUnblock();
    
    this->m_LoadingLayer = LoadingLayer::create();
    this->m_LoadingLayer->retain();
    this->addChild(this->m_LoadingLayer);
}

void BaseScene::RemoveLoadingSpinnerAndUnblock() const
{
    if(this->m_LoadingLayer)
    {
        this->m_LoadingLayer->removeFromParent();
        this->m_LoadingLayer->release();
        this->m_LoadingLayer = nullptr;
    }
}

void BaseScene::OnResponse() const
{
    if(m_BackgroundLayer)
    {
        m_BackgroundLayer->OnResponse();
    }
    if(m_EffectLayer)
    {
        m_EffectLayer->OnResponse();
    }
    if(m_ObjectLayer)
    {
        m_ObjectLayer->OnResponse();
    }
    if(m_ActorLayer)
    {
        m_ActorLayer->OnResponse();
    }
    if(m_UILayer)
    {
        m_UILayer->OnResponse();
    }
    
    this->RemoveLoadingSpinnerAndUnblock();
}

void BaseScene::InitializeGPSInfo() {
    CCLabelTTF* label = CCLabelTTF::create("gps info", "thonburi", 13);
    label->setTag(753);
    label->setZOrder(333);
    label->setPosition(ccp(240,160));
    this->addChild(label);
}

void BaseScene::UpdateGPSInfo() {
    ServerTime currentTime = GameClient::Instance().GetClientTimer().Check();
    if(currentTime - this->m_LastGPSTime > ServerTime(5000)) {
        this->m_LastGPSTime = currentTime;
        CCLabelTTF* label = static_cast<CCLabelTTF*>(this->getChildByTag(753));
        if(label) {
            GPSPoint gps = GPS::GetCurrentGPSPoint();
            
            if(GPS::GetCurrentAddress().empty()) return;
            
            label->setString(CCString::createWithFormat("gps %f, %f, %s", gps.latitude, gps.longitude, GPS::GetCurrentAddress().c_str())->getCString());
        }
    }
}