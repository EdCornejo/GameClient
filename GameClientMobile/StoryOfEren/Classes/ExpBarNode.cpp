//
//  ExpBarNode.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 5/27/13.
//
//

#include "Headers.pch"

ExpBarNode::ExpBarNode(): m_Initialized(false), m_LevelLabel(nullptr), m_CurrentExpBar(nullptr)
{

}

ExpBarNode::~ExpBarNode()
{
    CC_SAFE_RELEASE(this->m_LevelLabel);
    CC_SAFE_RELEASE(this->m_CurrentExpBar);
}

bool ExpBarNode::init()
{
    if(!CCNode::init()) return false;
 
    CCSprite* levelBackground = CCSprite::create("ui/exp_bar/level_background.png");
    levelBackground->setAnchorPoint(CCPointUpperLeft);
    this->addChild(levelBackground);
   
    CCSize levelBackgroundSize = levelBackground->getContentSize();
    this->m_LevelLabel = CCLabelTTF::create("0", "thonburi", 12);
    this->m_LevelLabel->retain();
    this->m_LevelLabel->setAnchorPoint(CCPointUpperRight);
    this->m_LevelLabel->setHorizontalAlignment(kCCTextAlignmentRight);
    this->m_LevelLabel->setPosition(ccp(levelBackgroundSize.width - 4, 0));
    this->addChild(this->m_LevelLabel);
    
    CCSprite* background = CCSprite::create("ui/exp_bar/background.png");
    background->setAnchorPoint(CCPointMidLeft);
    background->setPosition(ccp(levelBackgroundSize.width, -background->getContentSize().height / 2));
    this->addChild(background);
    
    this->m_CurrentExpBar = CCSprite::create("ui/exp_bar/exp_bar.png");
    this->m_CurrentExpBar->retain();
    this->m_CurrentExpBar->setAnchorPoint(CCPointMidLeft);
    this->m_CurrentExpBar->setScaleX(0);
    this->m_CurrentExpBar->setPosition(ccp(levelBackgroundSize.width, -this->m_CurrentExpBar->getContentSize().height / 2));
    this->addChild(this->m_CurrentExpBar);
    
    this->setPosition(ccp(0, 320));
    
    scheduleUpdate();
    
    return true;
}

void ExpBarNode::update(float deltaTime)
{
    if(!this->m_Initialized)
    {
        ActorID myActorID = GameClient::Instance().GetMyActorID();
        Actor* actor = GameClient::Instance().GetClientStage()->FindActor(myActorID);
        
        if(!actor) return;
     
        CCString* level = CCString::createWithFormat("%d", actor->GetLevel());
        this->m_LevelLabel->setString(level->getCString());
    
        float scaleFactor = actor->GetExperiencePoint() / actor->GetMaxExperiencePoint();
        this->m_CurrentExpBar->setScaleX(scaleFactor);
        
        this->m_Initialized = true;
        
        this->unscheduleUpdate();
    }
}

void ExpBarNode::Update()
{
    ActorID myActorID = GameClient::Instance().GetMyActorID();
    Actor* actor = GameClient::Instance().GetClientStage()->FindActor(myActorID);
    ASSERT_DEBUG(actor);
    
    CCString* level = CCString::createWithFormat("%d", actor->GetLevel());
    this->m_LevelLabel->setString(level->getCString());
    
    
    float scaleFactor = actor->GetExperiencePoint() / actor->GetMaxExperiencePoint();
    this->m_CurrentExpBar->setScaleX(scaleFactor);
}