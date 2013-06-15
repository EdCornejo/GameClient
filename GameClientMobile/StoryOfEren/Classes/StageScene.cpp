//
//  StageScene.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 3/11/13.
//
//

#include "Headers.pch"

using namespace flownet;

StageScene::StageScene(){}
StageScene::~StageScene()
{

}

bool StageScene::init()
{
    if(!BaseScene::init()) return false;
    
    ClientStage* stage = GameClient::Instance().GetClientStage();
    
    this->m_BackgroundLayer = BackgroundLayer::create(stage->GetStageType());
    if(this->m_BackgroundLayer)
    {
        this->m_BackgroundLayer->retain();
        this->addChild(this->m_BackgroundLayer);
    }
    
    this->m_ActorLayer = ActorLayer::create(stage->GetStageType());
    if(this->m_ActorLayer)
    {
        this->m_ActorLayer->retain();
        this->addChild(this->m_ActorLayer);
    }
    
    this->m_UILayer = UILayer::create(stage->GetStageType());
    if(this->m_UILayer)
    {
        this->m_UILayer->retain();
        this->addChild(this->m_UILayer);
    }
    
    this->m_CaptionLayer = CaptionLayer::create(stage->GetStageType(), stage->GetCurrentTier());
    if(this->m_CaptionLayer)
    {
        this->m_CaptionLayer->retain();
        this->addChild(this->m_CaptionLayer, 9);
    }
    
    AudioEngine::Instance()->PlayBackgroundMusic("sound/bgm/seal.mp3", true);

    scheduleUpdate();
    
    return true;
}

void StageScene::update(float deltaTime)
{
    BaseScene::update(deltaTime);
}

void StageScene::OnClearTier()
{
    BaseScene::OnClearTier();
    
    if(this->m_CaptionLayer)
    {
        this->m_CaptionLayer->removeFromParent();
        this->m_CaptionLayer->release();
        this->m_CaptionLayer = nullptr;
    }
    
    if(!this->m_CaptionLayer)
    {
        Stage* stage = GameClient::Instance().GetClientStage();
        this->m_CaptionLayer = CaptionLayer::create(stage->GetStageType(), stage->GetCurrentTier());
        if(this->m_CaptionLayer)
        {
            this->m_CaptionLayer->retain();
            this->m_CaptionLayer->ccTouchBegan(nullptr, nullptr);
            this->addChild(this->m_CaptionLayer, 9);
        }
    }
}
