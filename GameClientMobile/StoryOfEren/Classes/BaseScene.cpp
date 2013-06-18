//
//  BaseScene.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 3/5/13.
//
//

#include "Headers.pch"

BaseScene::BaseScene() : m_BackgroundLayer(nullptr), m_EffectLayer(nullptr), m_ObjectLayer(nullptr), m_ActorLayer(nullptr), m_UILayer(nullptr), m_CaptionLayer(nullptr), m_LoadingLayer(nullptr), m_LastGPSTime(0)
{
    CCLOG("Base scene created");
}

BaseScene::~BaseScene()
{
    CC_SAFE_RELEASE(this->m_BackgroundLayer);
    CC_SAFE_RELEASE(this->m_EffectLayer);
    CC_SAFE_RELEASE(this->m_ObjectLayer);
    CC_SAFE_RELEASE(this->m_ActorLayer);
    CC_SAFE_RELEASE(this->m_UILayer);
    CC_SAFE_RELEASE(this->m_CaptionLayer);
    CC_SAFE_RELEASE(this->m_LoadingLayer);
}

bool BaseScene::init()
{
    if(!CCScene::init())
    {
        return false;
    }
    
    ClientStage* stage = GameClient::Instance().GetClientStage();
    
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
    if(this->m_HeartbeatLayer)
    {
        this->m_HeartbeatLayer->retain();
        this->addChild(m_HeartbeatLayer, 10);
    }
    
    this->InitializeGPSInfo();
    
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
    
    this->UpdateGPSInfo();
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

void BaseScene::OnResponse()
{
    if(this->m_BackgroundLayer)
    {
        this->m_BackgroundLayer->OnResponse();
    }
    if(this->m_EffectLayer)
    {
        this->m_EffectLayer->OnResponse();
    }
    if(this->m_ObjectLayer)
    {
        this->m_ObjectLayer->OnResponse();
    }
    if(this->m_ActorLayer)
    {
        this->m_ActorLayer->OnResponse();
    }
    if(this->m_UILayer)
    {
        this->m_UILayer->OnResponse();
    }
    
    this->RemoveLoadingSpinnerAndUnblock();
}

void BaseScene::OnLoad()
{
    if(this->m_BackgroundLayer)
    {
        this->m_BackgroundLayer->OnLoad();
    }
    if(this->m_EffectLayer)
    {
        this->m_EffectLayer->OnLoad();
    }
    if(this->m_ObjectLayer)
    {
        this->m_ObjectLayer->OnLoad();
    }
    if(this->m_ActorLayer)
    {
        this->m_ActorLayer->OnLoad();
    }
    if(this->m_UILayer)
    {
        this->m_UILayer->OnLoad();
    }
    if(this->m_CaptionLayer)
    {
        this->m_CaptionLayer->OnLoad();
    }
}

void BaseScene::OnClearStage()
{

}

void BaseScene::OnClearTier()
{

}

void BaseScene::InitializeGPSInfo() {
    CCLabelTTF* label = CCLabelTTF::create("gps info", "thonburi", 13);
    label->setTag(753);
    label->setZOrder(333);
    label->setPosition(ccp(240,306));
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
            
            label->setString(CCString::createWithFormat("gps %3.3f, %3.3f", gps.latitude, gps.longitude)->getCString());
        }
    }
}