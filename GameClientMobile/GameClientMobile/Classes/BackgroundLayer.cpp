//
//  BackgroundLayer.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 3/6/13.
//
//

#include "Headers.pch"

BackgroundLayer::BackgroundLayer() : m_BackgroundImage(nullptr)
{
    
}

BackgroundLayer::~BackgroundLayer()
{
    if(m_BackgroundImage)
    {
        m_BackgroundImage->release();
        m_BackgroundImage = nullptr;
    }
}

bool BackgroundLayer::initWithBackgroundFileName(const char* fileName)
{
    if(!BaseLayer::init()) return false;
    
    return true;
}

bool BackgroundLayer::initWithStageType(StageType stageType)
{
    if(!BaseLayer::initWithStageType(stageType))
    {
        return false;
    }
    CCSprite* backgroundImage = BackgroundImageLoader::GetBackgroundImage(stageType);
    
    this->AttachBackgroundImage(backgroundImage);
   
    return true;
}

BackgroundLayer* BackgroundLayer::create(const char* fileName)
{
    BackgroundLayer* backgroundLayer = new BackgroundLayer();
    if(backgroundLayer && backgroundLayer->initWithBackgroundFileName(fileName))
    {
        return backgroundLayer;
    }
    else
    {
        delete backgroundLayer;
        backgroundLayer = nullptr;
        return nullptr;
    }
}

BackgroundLayer* BackgroundLayer::create(StageType stageType)
{
    BackgroundLayer* backgroundLayer = new BackgroundLayer();
    
    if(backgroundLayer && backgroundLayer->initWithStageType(stageType))
    {
        return backgroundLayer;
    }
    else
    {
        delete backgroundLayer;
        backgroundLayer = nullptr;
        return nullptr;
    }
}

void BackgroundLayer::update(float deltaTime)
{
    
}

void BackgroundLayer::AttachBackgroundImage(CCSprite* backgroundImage)
{
    if(this->m_BackgroundImage)
    {
        this->removeChild(this->m_BackgroundImage);
        this->m_BackgroundImage->release();
        this->m_BackgroundImage = nullptr;
    }
    this->m_BackgroundImage = backgroundImage;
    this->m_BackgroundImage->retain();
    this->m_BackgroundImage->setPosition(CCPointMid);
    
    this->addChild(this->m_BackgroundImage);
}
