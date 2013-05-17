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
    
    this->m_Skeleton = CCSkeleton::create("common/character.json", "player/character.atlas");
    
    
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
    
    this->m_Skeleton->setSlotsToBindPose();
    this->m_Skeleton->setAnimation("idle", true);
    
    this->setScale(0.1);
    
    this->addChild(this->m_Skeleton);
    
    return true;
}

bool PlayerNode::initWithGender(flownet::Gender gender)
{
    if(!ActorNode::init()) return false;
    
    this->m_Skeleton = CCSkeleton::create("common/character.json", "player/character.atlas");
    
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
    
    this->m_Skeleton->setSlotsToBindPose();
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

void PlayerNode::ChangeWand(flownet::ItemType itemType)
{
    std::string itemImageName;
    switch (itemType) {
        case flownet::ItemType_OakWand:
            itemImageName = "oakwand";
            break;
        default:
            ASSERT_DEBUG(false);
            break;
    }

    this->m_Skeleton->setAttachment("wand", itemImageName.c_str());
}

void PlayerNode::ChangeHat(flownet::ItemType itemType)
{
    std::string itemImageName;
    switch (itemType) {
        default:
            ASSERT_DEBUG(false);
            break;
    }
    this->m_Skeleton->setAttachment("hat", itemImageName.c_str());
}

void PlayerNode::ChangeRobe(flownet::ItemType itemType)
{
    std::string itemImageName;
    switch (itemType) {
        default:
            //ASSERT_DEBUG(false);
            break;
    }
    
    this->m_Skeleton->setAttachment("right_upper_arm", "wizard");
    this->m_Skeleton->setAttachment("right_lower_arm-copy", "wizard");
    this->m_Skeleton->setAttachment("robe_up", "wizard");
    this->m_Skeleton->setAttachment("robe_down", "wizard");
    this->m_Skeleton->setAttachment("robe_neck", "wizard");
    this->m_Skeleton->setAttachment("left_lower_arm", "wizard");
    this->m_Skeleton->setAttachment("left_upper_arm", "wizard");
}

void PlayerNode::ChangeCloak(flownet::ItemType itemType)
{
    std::string itemImageName;
    switch (itemType) {
        default:
            ASSERT_DEBUG(false);
            break;
    }
    this->m_Skeleton->setAttachment("cloak", itemImageName.c_str());
}

void PlayerNode::ChangeShoes(flownet::ItemType itemType)
{
    std::string itemImageName;
    switch (itemType) {
        default:
            ASSERT_DEBUG(false);
            break;
    }
    this->m_Skeleton->setAttachment("shoes", itemImageName.c_str());
}

void PlayerNode::ChangeRing(flownet::ItemType itemType)
{
    std::string itemImageName;
    switch (itemType) {
        default:
            ASSERT_DEBUG(false);
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
    
    this->m_Skeleton->setSlotsToBindPose();
    this->m_Skeleton->setAnimation("idle", true);
}
