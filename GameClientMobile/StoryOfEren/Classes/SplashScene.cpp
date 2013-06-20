//
//  SplashScene.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 4/15/13.
//
//

#include "Headers.pch"

SplashScene::SplashScene(): m_HasRequestSession(false), m_HasRequestConnect(false){}
SplashScene::~SplashScene()
{
}
    
bool SplashScene::init()
{
    if(!BaseScene::init()) return false;
 
    this->m_BackgroundLayer = BackgroundLayer::create("ui/background/background.png");
    this->m_BackgroundLayer->retain();
    
    this->addChild(this->m_BackgroundLayer);
    
    scheduleUpdate();
    
    return true;
}

void SplashScene::update(float deltaTime)
{
    BaseScene::update(deltaTime);
    
    std::string serverIP = CCUserDefault::sharedUserDefault()->getStringForKey("yours", "");
    
    if(!serverIP.empty() && !this->m_HasRequestSession)
    //if(GameClient::Instance().GetClientObject().GetConnectionID() != ConnectionID_NONE && !this->m_HasRequestSession)
    {
        sleep(1);
        if( GameClient::Instance().GetClientObject().IsConnected() == true )
        {
            GameClient::Instance().GetClientObject().SendCSRequestSession(GameClient::Instance().GetDeviceID(), GameClient::Instance().GetClientObject().GetConnectionID(), GameClient::Instance().GetSessionID());
            this->m_HasRequestSession = true;
        }
    }
    else if(serverIP.empty() && !this->m_HasRequestSession)
    //if(GameClient::Instance().GetCFConnection().GetConnectionID() != ConnectionID_NONE && !this->m_HasRequestSession)
    {
        sleep(1);
        ClientDirector* director = static_cast<ClientDirector*>(CCDirector::sharedDirector());
        director->ChangeScene<AccountScene>();

        this->m_HasRequestSession = true;
    }
}
