//
//  SplashScene.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 4/15/13.
//
//

#include "Headers.pch"

SplashScene::SplashScene(): m_HasRequestSession(false){}
SplashScene::~SplashScene() {}
    
bool SplashScene::init()
{
    if(!BaseScene::init()) return false;
 
    this->m_BackgroundLayer = BackgroundLayer::create("background/loading.png");
    this->m_BackgroundLayer->retain();
    
    this->addChild(this->m_BackgroundLayer);
 
    scheduleUpdate();
    
    return true;
}

void SplashScene::update(float deltaTime)
{
    BaseScene::update(deltaTime);
    
    if(GameClient::Instance().GetClientObject().GetConnectionID() != ConnectionID_NONE && !this->m_HasRequestSession)
    {
        this->m_HasRequestSession = true;
        GameClient::Instance().GetClientObject().SendCSRequestSession(GameClient::Instance().GetDeviceID(), GameClient::Instance().GetClientObject().GetConnectionID());
    }
}