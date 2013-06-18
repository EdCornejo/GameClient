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
            this->m_Skeleton = CCSkeletonAnimation::createWithFile("common/mushroom.json", "monster/mushroom.atlas");
            this->m_Skeleton->setSkin("purple");
            this->m_Skeleton->setScale(0.095);
            break;
        case flownet::MonsterType_Goblin:

        case flownet::MonsterType_Spider:

        case flownet::MonsterType_KingSpider:
            this->m_Skeleton = CCSkeletonAnimation::createWithFile("common/goblin.json", "monster/goblin.atlas");
            this->m_Skeleton->setSkin("club");
            this->m_Skeleton->setScale(0.3);
            this->m_Skeleton->skeleton->flipX = true;
            break;
        
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
        case flownet::MonsterType_ColosseumGuardian:
            this->m_Skeleton = CCSkeletonAnimation::createWithFile("common/tree.json", "monster/tree.atlas");
            this->m_Skeleton->setSkin("blue_tree");
            this->m_Skeleton->setScale(0.12);
            break;
        default:
            this->m_Skeleton = CCSkeletonAnimation::createWithFile("common/goblin.json", "monster/goblin.atlas");
            this->m_Skeleton->setSkin("club");
            this->m_Skeleton->setScale(0.24);
            this->m_Skeleton->skeleton->flipX = true;
            break;
        
    }
    this->m_Skeleton->retain();
    this->m_Skeleton->setSlotsToSetupPose();
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

void MonsterNode::PlayIdleEffect(bool loop)
{
    Monster* monster = static_cast<Monster*>(this->GetActorInfo());

    if(!monster){
        ASSERT_DEBUG(monster);
        return;
    }
    
    std::string fileName = "sound/effect/monster/";
    
    switch(monster->GetMonsterType())
    {
        case flownet::MonsterType_ColosseumGuardian:
        case flownet::MonsterType_NightmareTree:
            fileName += "tree";
            break;
        case flownet::MonsterType_GrandWolfKing:
        case flownet::MonsterType_Wolf:
        case flownet::MonsterType_YoungWolf:
            fileName += "wolf";
            break;
        case flownet::MonsterType_Goblin:
            fileName += "goblin";
            break;
        case flownet::MonsterType_KingSpider:
        case flownet::MonsterType_Spider:
            fileName += "spider";
            break;
        case flownet::MonsterType_Mushroom:
            fileName += "mushroom";
            break;
    }
    
    fileName += "/idle";

    unsigned int result = AudioEngine::Instance()->PlayEffect((fileName + ".wav").c_str());
    if(result != AudioEngine::MUTE) return;
    
    result = AudioEngine::Instance()->PlayEffect((fileName + ".mp3").c_str());
    if(result != AudioEngine::MUTE) return;
}

void MonsterNode::PlayMovingEffect(bool loop)
{
    Monster* monster = static_cast<Monster*>(this->GetActorInfo());

    if(!monster){
        ASSERT_DEBUG(monster);
        return;
    }
    
    std::string fileName = "sound/effect/monster/";
    
    switch(monster->GetMonsterType())
    {
        case flownet::MonsterType_ColosseumGuardian:
        case flownet::MonsterType_NightmareTree:
            fileName += "tree";
            break;
        case flownet::MonsterType_GrandWolfKing:
        case flownet::MonsterType_Wolf:
        case flownet::MonsterType_YoungWolf:
            fileName += "wolf";
            break;
        case flownet::MonsterType_Goblin:
            fileName += "goblin";
            break;
        case flownet::MonsterType_KingSpider:
        case flownet::MonsterType_Spider:
            fileName += "spider";
            break;
        case flownet::MonsterType_Mushroom:
            fileName += "mushroom";
            break;
    }
    
    fileName += "/moving";

    unsigned int result = AudioEngine::Instance()->PlayEffect((fileName + ".wav").c_str());
    if(result != AudioEngine::MUTE) return;
    
    result = AudioEngine::Instance()->PlayEffect((fileName + ".mp3").c_str());
    if(result != AudioEngine::MUTE) return;
}

void MonsterNode::PlayAttackingEffect(bool loop)
{
    Monster* monster = static_cast<Monster*>(this->GetActorInfo());

    if(!monster){
        ASSERT_DEBUG(monster);
        return;
    }
    
    std::string fileName = "sound/effect/monster/";
    
    switch(monster->GetMonsterType())
    {
        case flownet::MonsterType_ColosseumGuardian:
        case flownet::MonsterType_NightmareTree:
            fileName += "tree";
            break;
        case flownet::MonsterType_GrandWolfKing:
        case flownet::MonsterType_Wolf:
        case flownet::MonsterType_YoungWolf:
            fileName += "wolf";
            break;
        case flownet::MonsterType_Goblin:
            fileName += "goblin";
            break;
        case flownet::MonsterType_KingSpider:
        case flownet::MonsterType_Spider:
            fileName += "spider";
            break;
        case flownet::MonsterType_Mushroom:
            fileName += "mushroom";
            break;
    }
    
    fileName += "/attacking";

    unsigned int result = AudioEngine::Instance()->PlayEffect((fileName + ".wav").c_str());
    if(result != AudioEngine::MUTE) return;
    
    result = AudioEngine::Instance()->PlayEffect((fileName + ".mp3").c_str());
    if(result != AudioEngine::MUTE) return;
}

void MonsterNode::PlayAttackedEffect(bool loop)
{
    Monster* monster = static_cast<Monster*>(this->GetActorInfo());

    if(!monster){
        ASSERT_DEBUG(monster);
        return;
    }
    
    std::string fileName = "sound/effect/monster/";
    
    switch(monster->GetMonsterType())
    {
        case flownet::MonsterType_ColosseumGuardian:
        case flownet::MonsterType_NightmareTree:
            fileName += "tree";
            break;
        case flownet::MonsterType_GrandWolfKing:
        case flownet::MonsterType_Wolf:
        case flownet::MonsterType_YoungWolf:
            fileName += "wolf";
            break;
        case flownet::MonsterType_Goblin:
            fileName += "goblin";
            break;
        case flownet::MonsterType_KingSpider:
        case flownet::MonsterType_Spider:
            fileName += "spider";
            break;
        case flownet::MonsterType_Mushroom:
            fileName += "mushroom";
            break;
    }
    
    fileName += "/attacked";

    unsigned int result = AudioEngine::Instance()->PlayEffect((fileName + ".wav").c_str());
    if(result != AudioEngine::MUTE) return;
    
    result = AudioEngine::Instance()->PlayEffect((fileName + ".mp3").c_str());
    if(result != AudioEngine::MUTE) return;

}

void MonsterNode::PlayBeginCastingEffect(bool loop)
{
    Monster* monster = static_cast<Monster*>(this->GetActorInfo());

    if(!monster){
        ASSERT_DEBUG(monster);
        return;
    }
    
    std::string fileName = "sound/effect/monster/";
    
    switch(monster->GetMonsterType())
    {
        case flownet::MonsterType_ColosseumGuardian:
        case flownet::MonsterType_NightmareTree:
            fileName += "tree";
            break;
        case flownet::MonsterType_GrandWolfKing:
        case flownet::MonsterType_Wolf:
        case flownet::MonsterType_YoungWolf:
            fileName += "wolf";
            break;
        case flownet::MonsterType_Goblin:
            fileName += "goblin";
            break;
        case flownet::MonsterType_KingSpider:
        case flownet::MonsterType_Spider:
            fileName += "spider";
            break;
        case flownet::MonsterType_Mushroom:
            fileName += "mushroom";
            break;
    }
    
    fileName += "/begin_casting";

    unsigned int result = AudioEngine::Instance()->PlayEffect((fileName + ".wav").c_str());
    if(result != AudioEngine::MUTE) return;
    
    result = AudioEngine::Instance()->PlayEffect((fileName + ".mp3").c_str());
    if(result != AudioEngine::MUTE) return;

}

void MonsterNode::PlayRepeatCastingEffect(bool loop)
{
    Monster* monster = static_cast<Monster*>(this->GetActorInfo());

    if(!monster){
        ASSERT_DEBUG(monster);
        return;
    }
    
    std::string fileName = "sound/effect/monster/";
    
    switch(monster->GetMonsterType())
    {
        case flownet::MonsterType_ColosseumGuardian:
        case flownet::MonsterType_NightmareTree:
            fileName += "tree";
            break;
        case flownet::MonsterType_GrandWolfKing:
        case flownet::MonsterType_Wolf:
        case flownet::MonsterType_YoungWolf:
            fileName += "wolf";
            break;
        case flownet::MonsterType_Goblin:
            fileName += "goblin";
            break;
        case flownet::MonsterType_KingSpider:
        case flownet::MonsterType_Spider:
            fileName += "spider";
            break;
        case flownet::MonsterType_Mushroom:
            fileName += "mushroom";
            break;
    }
    
    fileName += "/repeat_casting";

    unsigned int result = AudioEngine::Instance()->PlayEffect((fileName + ".wav").c_str());
    if(result != AudioEngine::MUTE) return;
    
    result = AudioEngine::Instance()->PlayEffect((fileName + ".mp3").c_str());
    if(result != AudioEngine::MUTE) return;

}

void MonsterNode::PlayEndCastingEffect(bool loop)
{
    Monster* monster = static_cast<Monster*>(this->GetActorInfo());

    if(!monster){
        ASSERT_DEBUG(monster);
        return;
    }
    
    std::string fileName = "sound/effect/monster/";
    
    switch(monster->GetMonsterType())
    {
        case flownet::MonsterType_ColosseumGuardian:
        case flownet::MonsterType_NightmareTree:
            fileName += "tree";
            break;
        case flownet::MonsterType_GrandWolfKing:
        case flownet::MonsterType_Wolf:
        case flownet::MonsterType_YoungWolf:
            fileName += "wolf";
            break;
        case flownet::MonsterType_Goblin:
            fileName += "goblin";
            break;
        case flownet::MonsterType_KingSpider:
        case flownet::MonsterType_Spider:
            fileName += "spider";
            break;
        case flownet::MonsterType_Mushroom:
            fileName += "mushroom";
            break;
    }
    
    fileName += "/end_casting";

    unsigned int result = AudioEngine::Instance()->PlayEffect((fileName + ".wav").c_str());
    if(result != AudioEngine::MUTE) return;
    
    result = AudioEngine::Instance()->PlayEffect((fileName + ".mp3").c_str());
    if(result != AudioEngine::MUTE) return;

}

void MonsterNode::PlayDeadEffect(bool loop)
{
    Monster* monster = static_cast<Monster*>(this->GetActorInfo());

    if(!monster){
        ASSERT_DEBUG(monster);
        return;
    }
    
    std::string fileName = "sound/effect/monster/";
    
    switch(monster->GetMonsterType())
    {
        case flownet::MonsterType_ColosseumGuardian:
        case flownet::MonsterType_NightmareTree:
            fileName += "tree";
            break;
        case flownet::MonsterType_GrandWolfKing:
        case flownet::MonsterType_Wolf:
        case flownet::MonsterType_YoungWolf:
            fileName += "wolf";
            break;
        case flownet::MonsterType_Goblin:
            fileName += "goblin";
            break;
        case flownet::MonsterType_KingSpider:
        case flownet::MonsterType_Spider:
            fileName += "spider";
            break;
        case flownet::MonsterType_Mushroom:
            fileName += "mushroom";
            break;
    }
    
    fileName += "/dead";

    unsigned int result = AudioEngine::Instance()->PlayEffect((fileName + ".wav").c_str());
    if(result != AudioEngine::MUTE) return;
    
    result = AudioEngine::Instance()->PlayEffect((fileName + ".mp3").c_str());
    if(result != AudioEngine::MUTE) return;

}
