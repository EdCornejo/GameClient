//
//  MonsterNode.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 3/7/13.
//
//

#include "Headers.pch"

MonsterNode::MonsterNode() : ActorNode() {}

MonsterNode::~MonsterNode() {}

bool MonsterNode::init()
{
    if(!ActorNode::init()) return false;
    
    Monster* monster = GameClient::Instance().GetClientStage()->FindMonster(this->m_ActorID);
    
    switch(monster->GetMonsterType())
    {
        case flownet::MonsterType_Mushroom:

        case flownet::MonsterType_Goblin:

        case flownet::MonsterType_Spider:

        case flownet::MonsterType_KingSpider:
        
        case flownet::MonsterType_Wolf:
            this->m_Skeleton = CCSkeletonAnimation::createWithFile("common/wolf.json", "monster/wolf.atlas");
            this->m_Skeleton->setSkin("blue_wolf");
            this->m_Skeleton->setScale(0.24);
            break;
        case flownet::MonsterType_YoungWolf:
            this->m_Skeleton = CCSkeletonAnimation::createWithFile("common/wolf.json", "monster/wolf.atlas");
            this->m_Skeleton->setSkin("blue_wolf");
            this->m_Skeleton->setScale(0.15);
            break;
        case flownet::MonsterType_GrandWolfKing:
            this->m_Skeleton = CCSkeletonAnimation::createWithFile("common/white_wolf.json", "monster/white_wolf.atlas");
            this->m_Skeleton->setSkin("white");
            this->m_Skeleton->setScale(0.24);
            break;
        case flownet::MonsterType_NormalTree :
            this->m_Skeleton = CCSkeletonAnimation::createWithFile("common/tree.json", "monster/tree.atlas");
            this->m_Skeleton->setSkin("blue_tree");
            this->m_Skeleton->setScale(0.24);
            break;
        case flownet::MonsterType_NightmareTree :
            this->m_Skeleton = CCSkeletonAnimation::createWithFile("common/tree.json", "monster/tree.atlas");
            this->m_Skeleton->setSkin("blue_tree");
            this->m_Skeleton->setScale(0.24);
            break;
        default:
            this->m_Skeleton = CCSkeletonAnimation::createWithFile("monster/goblins.json", "monster/goblin.atlas");
            this->m_Skeleton->setSkin("goblingirl");
            this->m_Skeleton->setScale(0.24);
            this->m_Skeleton->skeleton->flipX = true;

            break;
        
    }
    this->m_Skeleton->retain();
    this->m_Skeleton->setSlotsToSetupPose();
    this->m_Skeleton->setAnimation("idle", true);
    this->m_Skeleton->setAnchorPoint(CharacterAnchorPoint);

    this->addChild(this->m_Skeleton);
    
    return true;
}

MonsterNode* MonsterNode::create(flownet::ActorID actorID)
{
    MonsterNode* newNode = new MonsterNode();
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