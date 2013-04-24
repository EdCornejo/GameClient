//
//  MonsterNode.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 3/7/13.
//
//

#include "Headers.pch"

MonsterNode::MonsterNode(flownet::ActorID actorID) : ActorNode(actorID)
{
    Monster* monster = GameClient::Instance().GetClientStage()->FindMonster(actorID);
    
    switch(monster->GetMonsterType())
    {
        case flownet::MonsterType_Mushroom:

        case flownet::MonsterType_Goblin:

        case flownet::MonsterType_Spider:

        case flownet::MonsterType_KingSpider:
        default:
            this->m_Skeleton = CCSkeleton::create("monster/goblins.json", "monster/goblin.atlas");
            this->m_Skeleton->setSkin("goblingirl");
            this->m_Skeleton->setSlotsToBindPose();
            this->m_Skeleton->setScale(0.3);
            this->m_Skeleton->skeleton->flipX = true;

            break;
        
    }
    this->m_Skeleton->setAnimation("idle", true);
    this->m_Skeleton->setAnchorPoint(CharacterAnchorPoint);
    this->addChild(this->m_Skeleton);
}

MonsterNode::MonsterNode(flownet::Actor* actor): ActorNode(actor)
{
    Monster* monster = static_cast<Monster*>(actor);
    
    switch(monster->GetMonsterType())
    {
        case flownet::MonsterType_Mushroom:

        case flownet::MonsterType_Goblin:

        case flownet::MonsterType_Spider:

        case flownet::MonsterType_KingSpider:
        default:
            this->m_Skeleton = CCSkeleton::create("monster/goblins.json", "monster/goblin.atlas");
            break;
        
    }
    
    this->m_Skeleton->setAnchorPoint(CharacterAnchorPoint);
    this->addChild(this->m_Skeleton);
}

MonsterNode::~MonsterNode()
{

}

CCSprite* MonsterNode::LoadHighLightImage()
{
    return CCSprite::create("monster_highlight_circle.png");
}

