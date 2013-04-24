//
//  BattleScene.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 3/11/13.
//
//

#include "Headers.pch"

using namespace flownet;

bool BattleScene::init()
{
    if(!BaseScene::init())
    {
        return false;
    }
    // test code
    //GameClient::Instance().InitializeTestData();
    // end of test code
    
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

    scheduleUpdate();
    
    return true;
}

void BattleScene::update(float deltaTime)
{
    BaseScene::update(deltaTime);
}
