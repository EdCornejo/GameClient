//
//  SpellEffectNode.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 5/13/13.
//
//

#include "Headers.pch"

SpellEffectNode::SpellEffectNode() {}
SpellEffectNode::~SpellEffectNode(){}
    
bool SpellEffectNode::init()
{
    CCSprite* shadowImage = CCSprite::create("actor/shadow.png");
    this->setAnchorPoint(CharacterAnchorPoint);
    this->addChild(shadowImage);
    
    scheduleUpdate();
    
    return true;
}
SpellEffectNode* SpellEffectNode::create(flownet::ActorID actorID, flownet::SpellAbility spellAbility)
{
    SpellEffectNode* newNode = new SpellEffectNode();
    newNode->m_ActorID = actorID;
    newNode->m_SpellAbility = spellAbility;

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
    
void SpellEffectNode::update(float deltaTime)
{
    BaseScene* scene = static_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
    ActorLayer* actorLayer = scene->GetActorLayer();

    if(!actorLayer) ASSERT_DEBUG(false);
    
    ActorNode* actor = actorLayer->FindActorNode(this->m_ActorID);
    
    if(!actor) return;
    
    this->setPosition(actor->getPosition());
}
