//
//  NPCNode.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 5/28/13.
//
//

#include "Headers.pch"

NPCNode::NPCNode(){}

NPCNode::~NPCNode() {}

bool NPCNode::init()
{
    if(!CCNode::init()) return false;
    
    this->m_Skeleton = CCSkeletonAnimation::createWithFile("common/character.json", "player/character.atlas");
    this->m_Skeleton->retain();
    
    
    // set animtaion mixing
    this->m_Skeleton->setMix("idle", "moving", 0.2);
    this->m_Skeleton->setMix("moving", "idle", 0.2);
    
    this->m_Skeleton->setAnchorPoint(CharacterAnchorPoint);
    
    this->m_Skeleton->setSkin("girl");
    this->m_Skeleton->setSlotsToSetupPose();
    
    this->m_Skeleton->setAnimation("idle", true);
    
    this->setScale(0.1);
    
    this->addChild(this->m_Skeleton);

    
    return true;
}

NPCNode* NPCNode::create(flownet::ActorID actorID)
{
    NPCNode* node = new NPCNode();
    node->m_ActorID = actorID;
    
    if(node && node->init())
    {
        node->autorelease();
        return node;
    }
    else
    {
        delete node;
        return nullptr;
    }
}

