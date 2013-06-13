//
//  LoadingScene.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 3/13/13.
//
//

#include "Headers.pch"

LoadingScene::LoadingScene() : m_LoadingSprite(nullptr)
{
    
}

LoadingScene::~LoadingScene()
{
    
}

bool LoadingScene::init()
{
    if(!BaseScene::init())
    {
        return false;
    }
    
    this->m_LoadingSprite = BackgroundImageLoader::GetGroundImage(StageType_Loading);
    this->m_LoadingSprite->retain();
    this->setPosition(CCPointMid);
    this->addChild(this->m_LoadingSprite);
    
    scheduleUpdate();
    
    return true;
}

LoadingScene* LoadingScene::create()
{
    LoadingScene* loadingScene = new LoadingScene();
    if(loadingScene && loadingScene->init())
    {
        return loadingScene;
    }
    else
    {
        delete loadingScene;
        loadingScene = nullptr;
        return nullptr;
    }
}

void LoadingScene::update(float deltaTime)
{
    BaseScene::update(deltaTime);
}

