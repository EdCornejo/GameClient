//
//  StageObjectNode.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 5/28/13.
//
//

#include "Headers.pch"

StageObjectNode::StageObjectNode() {}

StageObjectNode::~StageObjectNode() {}

bool StageObjectNode::init()
{
    if(!CCNode::init()) return false;
    
    StageObject* stageObject = GameClient::Instance().GetClientStage()->FindStageObject(this->m_ActorID);
    
    this->m_Body = StageObjectImageLoader::GetStageObjectImage(stageObject->GetStageObjectType());
    this->m_Body->retain();
    this->m_Body->setAnchorPoint(CharacterAnchorPoint);
    
    this->addChild(this->m_Body);
    
    return true;
}

StageObjectNode* StageObjectNode::create(flownet::ActorID actorID)
{
    StageObjectNode* node = new StageObjectNode();
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

void StageObjectNode::AnimateIdle() {}
void StageObjectNode::AnimateMoving() {}
void StageObjectNode::AnimateAttacking() {}
void StageObjectNode::AnimateAttacked() {}
void StageObjectNode::AnimateBeginCasting() {}
void StageObjectNode::AnimateRepeatCasting() {}
void StageObjectNode::AnimateEndCasting() {}
void StageObjectNode::AnimateFire() {}
void StageObjectNode::AnimateDead() {}