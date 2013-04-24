//
//  PlayerNode.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 3/7/13.
//
//

#include "Headers.pch"

PlayerNode::PlayerNode(flownet::ActorID actorID) : ActorNode(actorID)
{
    // NOTE : find the player's info and make a skeleton with it
    Player* player = GameClient::Instance().GetClientStage()->FindPlayer(actorID);
    
    this->m_Skeleton = CCSkeleton::create("common/character.json", "player/character.atlas");
    
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

    this->m_Skeleton->setScale(0.3);

    this->addChild(this->m_Skeleton);
}

PlayerNode::PlayerNode(flownet::Actor* actor): ActorNode(actor->GetActorID())
{
    // NOTE : make a skeleton based on the actor's info
    Player* player = static_cast<Player*>(actor);
    
    this->m_Skeleton = CCSkeleton::create("common/character.json", "player/character.atlas");
    this->m_Skeleton->setMix("idle", "moving", 0.2);
    this->m_Skeleton->setMix("moving", "idle", 0.2);

    this->m_Skeleton->setAnchorPoint(CharacterAnchorPoint);
    
    this->addChild(this->m_Skeleton);
}

PlayerNode::~PlayerNode()
{
}

CCSprite* PlayerNode::LoadHighLightImage()
{
    return CCSprite::create("player_highlight_circle.png");
}