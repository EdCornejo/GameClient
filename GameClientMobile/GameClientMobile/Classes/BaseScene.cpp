//
//  BaseScene.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 3/5/13.
//
//

#include "Headers.pch"

BaseScene::BaseScene() : m_BackgroundLayer(nullptr), m_EffectLayer(nullptr), m_ObjectLayer(nullptr), m_ActorLayer(nullptr), /*m_PlayerLayer(nullptr), m_MonsterLayer(nullptr),*/ m_UILayer(nullptr)
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

