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
    
    CCSprite* background = CCSprite::create("ui/exp_bar/background.png");
    background->setAnchorPoint(CCPointMidLeft);
    background->setPosition(ccp(0, -background->getContentSize().height / 2));
    this->addChild(background);
    
    this->m_CurrentExpBar = CCSprite::create("ui/exp_bar/exp_bar.png");
    this->m_CurrentExpBar->retain();
    this->m_CurrentExpBar->setAnchorPoint(CCPointMidLeft);
    this->m_CurrentExpBar->setPosition(ccp(0, -this->m_CurrentExpBar->getContentSize().height / 2));
    this->addChild(this->m_CurrentExpBar);
    
    this->m_LevelLabel = CCLabelTTF::create("Lv. 0", "thonburi", 20);
    this->m_LevelLabel->retain();
    this->m_LevelLabel->setAnchorPoint(CCPointUpperLeft);
    this->m_LevelLabel->setPosition(ccp(4,4));
    this->addChild(this->m_LevelLabel);
    
    this->setPosition(ccp(0, 320
    ));
    
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
     
        CCString* level = CCString::createWithFormat("Lv. %d", actor->GetLevel());
        this->m_LevelLabel->setString(level->getCString());
    
        float scaleFactor = CCRANDOM_0_1();
        //float scaleFactor = actor->GetExperiencePoint() / actor->GetMaxExperiencePoint();
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
    
    CCString* level = CCString::createWithFormat("Lv. %d", actor->GetLevel());
    this->m_LevelLabel->setString(level->getCString());
    
    
    float scaleFactor = actor->GetExperiencePoint() / actor->GetMaxExperiencePoint();
    this->m_CurrentExpBar->setScaleX(scaleFactor);
}