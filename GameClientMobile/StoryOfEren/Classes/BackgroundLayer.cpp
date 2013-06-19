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
    
    this->m_BackgroundParallaxNode = CCParallaxNode::create();
    this->m_BackgroundParallaxNode->retain();
    
    CCSprite* image = CCSprite::create(fileName);
    CCSize imageSize = image->getContentSize();
    this->m_BackgroundParallaxNode->addChild(image, 0, ccp(1, 1), ccp(imageSize.width / 2, imageSize.height / 2));
    
    this->addChild(this->m_BackgroundParallaxNode);
    
    return true;
}

bool BackgroundLayer::initWithStageType(StageType stageType)
{
    if(!BaseLayer::initWithStageType(stageType))
    {
        return false;
    }

    this->m_BackgroundParallaxNode = CCParallaxNode::create();
    this->m_BackgroundParallaxNode->retain();
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCPoint topLeftPoint = ccp(0, winSize.height);
    
    CCSprite* skyImage = BackgroundImageLoader::GetSkyImage(stageType);
    if(skyImage)
    {
        skyImage->setAnchorPoint(CCPointUpperLeft);
        this->m_BackgroundParallaxNode->addChild(skyImage, 0, ccp(0.3, 0), topLeftPoint);
    }
    CCSprite* terrainImage = BackgroundImageLoader::GetTerrainImage(stageType);
    if(terrainImage)
    {
        terrainImage->setAnchorPoint(CCPointUpperLeft);
        this->m_BackgroundParallaxNode->addChild(terrainImage, 1, ccp(0.6, 0), topLeftPoint);
    }
    CCSprite* groundImage = BackgroundImageLoader::GetGroundImage(stageType);
    if(groundImage)
    {
        groundImage->setAnchorPoint(CCPointZero);
        this->m_BackgroundParallaxNode->addChild(groundImage, 2, ccp(1, 1), CCPointZero);
    }
    
    this->addChild(this->m_BackgroundParallaxNode);
   
    return true;
}

BackgroundLayer* BackgroundLayer::create(const char* fileName)
{
    BackgroundLayer* backgroundLayer = new BackgroundLayer();
    if(backgroundLayer && backgroundLayer->initWithBackgroundFileName(fileName))
    {
        backgroundLayer->autorelease();
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
        backgroundLayer->autorelease();
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
