//
//  StageScene.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 3/11/13.
//
//

#include "Headers.pch"

StageScene::StageScene() : m_StageInfoList()
{
    
}

StageScene::~StageScene()
{
    std::for_each(m_StageInfoList.begin(), m_StageInfoList.end(), [](StageInfo stageInfo){
        delete &stageInfo;
    });
}

bool StageScene::init()
{
    if(!BaseScene::init())
    {
        return false;
    }
    
    this->m_BackgroundLayer = BackgroundLayer::create(StageType_NONE);
    this->m_BackgroundLayer->retain();
    
    CCSprite* backgroundImage = CCSprite::create("background/mushroom_field.png");
    this->m_BackgroundLayer->AttachBackgroundImage(backgroundImage);
    
    CCMenu* menu = CCMenu::create();
    CCMenuItemFont* menuitem1 = CCMenuItemFont::create("Create", this, menu_selector(StageScene::OnCreateButtonClick));
    CCMenuItemFont* menuitem2 = CCMenuItemFont::create("Join", this, menu_selector(StageScene::OnJoinButtonClick));

    menu->setPosition(CCPointMid);
    menu->addChild(menuitem1);
    menu->addChild(menuitem2);
    
    menu->alignItemsHorizontallyWithPadding(20);
    
    this->addChild(this->m_BackgroundLayer);
    this->addChild(menu);
    
    scheduleUpdate();
    
    return true;
}

void StageScene::update(float deltaTime)
{
    BaseScene::update(deltaTime);
}

void StageScene::AddStageInfo(StageInfo* stageInfo)
{
    m_StageInfoList.push_back(*stageInfo);
}

void StageScene::OnCreateButtonClick(cocos2d::CCObject *sender)
{
    GameClientObject& clientObject = GameClient::Instance().GetClientObject();
    
    clientObject.SendCSRequestCreateStage(StageType_WolfForest);
}

void StageScene::OnJoinButtonClick(cocos2d::CCObject *sender)
{
    GameClientObject& clientObject = GameClient::Instance().GetClientObject();
    clientObject.SendCSRequestRunningStages();
}