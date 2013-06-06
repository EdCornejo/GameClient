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
    if(!CCNode::init()) return false;
    
    CCSprite* effectImage = SpellImageLoader::GetSpellEffectImage(this->m_SpellAbility);
    if(!effectImage) return false;
    
    effectImage->setAnchorPoint(SpellEffectAnchorPoint);
    this->addChild(effectImage);
    
    
    switch(this->m_SpellAbility)
    {
        case flownet::SpellAbility_Freeze :
            this->m_IsOverTheCharacter = true;
            break;
        case flownet::SpellAbility_Slow :
        default:
            this->m_IsOverTheCharacter = false;
            break;
    }    
    
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
    ASSERT_DEBUG(scene);
    ActorLayer* actorLayer = scene->GetActorLayer();
    ASSERT_DEBUG(actorLayer);

    if(!actorLayer) ASSERT_DEBUG(false);
    
    ActorNode* actor = actorLayer->FindActorNode(this->m_ActorID);
    
    if(!actor) return;
    
    this->setPosition(actor->getPosition());
}

bool SpellEffectNode::IsOverTheCharacter()
{
    return this->m_IsOverTheCharacter;
}