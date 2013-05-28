//
//  LoadingLayer.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 5/24/13.
//
//

#include "Headers.pch"

LoadingLayer::LoadingLayer() {}

LoadingLayer::~LoadingLayer() {}

bool LoadingLayer::init()
{
    if(!CCNode::init()) return false;
    
    CCLayerColor* layer = CCLayerColor::create(ccc4(61, 43, 39, 150));
    this->addChild(layer);
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    // TO DO : loading animation comes here
    CCSprite* loadingAnimation = CCSprite::create("loading.png");
    loadingAnimation->setPosition(ccp(winSize.width / 2, winSize.height / 2));
    this->addChild(loadingAnimation);
    
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority - 20, true);
    
    this->setTouchEnabled(true);
    
    return true;
}

bool LoadingLayer::ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{
    return true;
}