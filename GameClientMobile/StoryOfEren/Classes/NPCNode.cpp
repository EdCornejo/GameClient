//
//  NPCNode.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 5/28/13.
//
//

#include "Headers.pch"

NPCNode::NPCNode(){}

NPCNode::~NPCNode()
{
    CC_SAFE_RELEASE(this->m_Body);
}

bool NPCNode::init()
{
    if(!CCNode::init()) return false;
    
    NPC* npc = GameClient::Instance().GetClientStage()->FindNPC(this->m_ActorID);
    
    this->m_Body = NPCImageLoader::GetNPCImage(npc->GetNPCType());
    this->m_Body->retain();
    
    this->addChild(this->m_Body);
    
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


void NPCNode::AnimateIdle() {}
void NPCNode::AnimateMoving() {}
void NPCNode::AnimateAttacking() {}
void NPCNode::AnimateAttacked() {}
void NPCNode::AnimateBeginCasting() {}
void NPCNode::AnimateRepeatCasting() {}
void NPCNode::AnimateEndCasting() {}
void NPCNode::AnimateFire() {}
void NPCNode::AnimateDead() {}