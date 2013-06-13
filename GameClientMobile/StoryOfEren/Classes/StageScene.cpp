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
    this->m_BackgroundLayer->retain();
    
    this->m_ActorLayer = ActorLayer::create(stage->GetStageType());
    this->m_ActorLayer->retain();
    
    this->m_UILayer = UILayer::create(stage->GetStageType());
    this->m_UILayer->retain();

    this->addChild(this->m_BackgroundLayer);
    this->addChild(this->m_ActorLayer);
    this->addChild(this->m_UILayer);
    
    // NOTE : usage of caption layer is different from any other layers
    // the return value may me null when stage has no script
    // so check for instance is intialized and add as child
    CaptionLayer* layer = CaptionLayer::create(stage->GetStageType());
    if(layer) this->addChild(layer);
    
    AudioEngine::Instance()->PlayBackgroundMusic("sound/bgm/seal.mp3", true);

    scheduleUpdate();
    
    return true;
}

void StageScene::update(float deltaTime)
{
    BaseScene::update(deltaTime);
}
