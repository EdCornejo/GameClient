//
//  PlayerNode.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 3/7/13.
//
//

#include "Headers.pch"

PlayerNode::PlayerNode() {}

PlayerNode::~PlayerNode() {}

bool PlayerNode::init()
{
    if(!ActorNode::init()) return false;
    
    Player* player = GameClient::Instance().GetClientStage()->FindPlayer(this->m_ActorID);
    
    this->m_Skeleton = CCSkeletonAnimation::createWithFile("common/character.json", "player/character.atlas");
    this->m_Skeleton->retain();
    
    
    // set animtaion mixing
    this->m_Skeleton->setMix("idle", "moving", 0.2);
    this->m_Skeleton->setMix("moving", "idle", 0.2);
    
    this->m_Skeleton->setAnchorPoint(CharacterAnchorPoint);
    
    if(player->GetGender() == Gender_Male)
    {
        this->m_Skeleton->setSkin("boy");
    }
    else
    {
        this->m_Skeleton->setSkin("girl");
    }
    this->m_Skeleton->setSlotsToSetupPose();
    
    this->m_Skeleton->setAnimation("idle", true);
    
    this->setScale(0.1);
    
    this->addChild(this->m_Skeleton);
    
    return true;
}

bool PlayerNode::initWithGender(flownet::Gender gender)
{
    if(!ActorNode::init()) return false;
    
    this->m_Skeleton = CCSkeletonAnimation::createWithFile("common/character.json", "player/character.atlas");
    this->m_Skeleton->retain();
    this->m_Skeleton->setMix("idle", "moving", 0.2);
    this->m_Skeleton->setMix("moving", "idle", 0.2);
    
    this->m_Skeleton->setAnchorPoint(CharacterAnchorPoint);
    
    if(gender == Gender_Male)
    {
        this->m_Skeleton->setSkin("boy");
    }
    else
    {
        this->m_Skeleton->setSkin("girl");
    }
    
    this->m_Skeleton->setSlotsToSetupPose();
    this->m_Skeleton->setAnimation("idle", true);
    
    this->setScale(0.1);
    
    this->addChild(this->m_Skeleton);
    
    return true;
}

PlayerNode* PlayerNode::create(flownet::ActorID actorID)
{
    PlayerNode* newNode = new PlayerNode();

    newNode->m_ActorID = actorID;

    if(newNode && newNode->init())
    {
        newNode->autorelease();
        return newNode;
    }
    else
    {
        delete newNode;
        return nullptr;
    }
}

PlayerNode* PlayerNode::create(flownet::Gender gender)
{
    PlayerNode* newNode = new PlayerNode();
    
    if(newNode && newNode->initWithGender(gender))
    {
        newNode->autorelease();
        return newNode;
    }
    else
    {
        delete newNode;
        return nullptr;
    }
}


//void ActorNode::PlayIdleEffect(bool loop)
//{
//    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("common/idle.mp3", loop);
//}

void PlayerNode::PlayMovingEffect(bool loop)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopEffect(this->m_StateEffectID);
    this->m_StateEffectID = CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("player/moving.mp3", loop);
}

//void ActorNode::PlayAttackingEffect(bool loop)
//{
//    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("common/attacking.mp3", loop);
//}
//
//void ActorNode::PlayAttackedEffect(bool loop)
//{
//    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("common/attacked.mp3", loop);
//}
//
//void ActorNode::PlayBeginCastingEffect(bool loop)
//{
//    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("common/begin_casting.mp3", loop);
//}
//
//void ActorNode::PlayRepeatCastingEffect(bool loop)
//{
//    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("common/repeat_casting.mp3", loop);
//}
//
//void ActorNode::PlayEndCastingEffect(bool loop)
//{
//    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("common/end_casting.mp3", loop);
//}
//
//void ActorNode::PlayDeadEffect(bool loop)
//{
//    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("common/dead.mp3", loop);
//}


void PlayerNode::ChangeWand(flownet::ItemType itemType)
{
    std::string itemImageName;
    switch (itemType) {
        case flownet::ItemType_OakWand:
            itemImageName = "oakwand";
            break;
        case flownet::ItemType_WizardStaff:
            itemImageName = "wizard";
            break;
        default:
            itemImageName = "default";
            break;
    }

    this->m_Skeleton->setAttachment("wand", itemImageName.c_str());
}

void PlayerNode::ChangeHat(flownet::ItemType itemType)
{
    std::string itemImageName;
    switch (itemType) {
        case flownet::ItemType_WizardHat:
            itemImageName = "wizard";
            break;
        default:
            itemImageName = "default";
            break;
    }
    this->m_Skeleton->setAttachment("hat_front", itemImageName.c_str());
    this->m_Skeleton->setAttachment("hat_rear", itemImageName.c_str());
}

void PlayerNode::ChangeRobe(flownet::ItemType itemType)
{
    std::string itemImageName;
    switch (itemType) {
        case flownet::ItemType_WizardRobe:
            itemImageName = "wizard";
            break;
        default:
            itemImageName = "default";
            break;
    }
    
    this->m_Skeleton->setAttachment("right_upper_arm", itemImageName.c_str());
    this->m_Skeleton->setAttachment("right_lower_arm-copy", itemImageName.c_str());
    this->m_Skeleton->setAttachment("robe_up", itemImageName.c_str());
    this->m_Skeleton->setAttachment("robe_down", itemImageName.c_str());
    this->m_Skeleton->setAttachment("robe_neck", itemImageName.c_str());
    this->m_Skeleton->setAttachment("left_lower_arm", itemImageName.c_str());
    this->m_Skeleton->setAttachment("left_upper_arm", itemImageName.c_str());
}

void PlayerNode::ChangeCloak(flownet::ItemType itemType)
{
    std::string itemImageName;
    switch (itemType) {
        default:
            itemImageName = "default";
            break;
    }
    this->m_Skeleton->setAttachment("cloak", itemImageName.c_str());
}

void PlayerNode::ChangeShoes(flownet::ItemType itemType)
{
    std::string itemImageName;
    switch (itemType) {
        default:
            itemImageName = "default";
            break;
    }
    this->m_Skeleton->setAttachment("shoes", itemImageName.c_str());
}

void PlayerNode::ChangeRing(flownet::ItemType itemType)
{
    std::string itemImageName;
    switch (itemType) {
        default:
            itemImageName = "default";
            break;
    }
    this->m_Skeleton->setAttachment("ring", itemImageName.c_str());
}

void PlayerNode::ChangeGender(flownet::Gender gender)
{
    if(gender == Gender_Male)
    {
        this->m_Skeleton->setSkin("boy");
    }
    else
    {
        this->m_Skeleton->setSkin("girl");
    }
    
    this->m_Skeleton->setSlotsToSetupPose();
    this->m_Skeleton->setAnimation("idle", true);
}
