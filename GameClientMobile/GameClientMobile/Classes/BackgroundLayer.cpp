//
//  BackgroundLayer.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 3/6/13.
//
//

#include "Headers.pch"

BackgroundLayer::BackgroundLayer() : m_BackgroundParallaxNode(nullptr)
{
    
}

BackgroundLayer::~BackgroundLayer()
{
    if(this->m_BackgroundParallaxNode)
    {
        this->m_BackgroundParallaxNode->release();
        this->m_BackgroundParallaxNode = nullptr;
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
    backgroundImage->setAnchorPoint(CCPointZero);
    
    this->m_BackgroundParallaxNode = CCParallaxNode::create();
    
    this->m_BackgroundParallaxNode->addChild(backgroundImage, 0, ccp(1, 1), CCPointZero);
    this->m_BackgroundParallaxNode->retain();
    
    this->addChild(this->m_BackgroundParallaxNode);
   
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
}

void BackgroundLayer::setPosition(CCPoint newPoint)
{
    this->m_BackgroundParallaxNode->setPosition(newPoint);
}
