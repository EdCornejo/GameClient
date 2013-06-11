//
//  SpellEffectNode.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 5/13/13.
//
//

#include "Headers.pch"

SpellEffectNode::SpellEffectNode(): m_IsOverTheCharacter(false), m_ActorID(ActorID_None), m_SpellAbility(SpellAbility_None), m_SpellEffectType(SpellEffectType_None) {}
SpellEffectNode::~SpellEffectNode(){}
    
bool SpellEffectNode::init()
{
    if(!CCNode::init()) return false;
    
    if(this->m_SpellAbility != SpellAbility_None)
    {
        CCSprite* effectImage = SpellImageLoader::GetSpellEffectImage(this->m_SpellAbility);
        
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
    }
    else if (this->m_SpellEffectType != SpellEffectType_None)
    {
        CCSprite* effectImage = CCSprite::create("blank.png");
        
        effectImage->setAnchorPoint(SpellEffectAnchorPoint);
        this->addChild(effectImage);
    
        // NOTE : if effect has animations play here
        CCSequence* sequence = nullptr;
        switch(this->m_SpellEffectType)
        {
            case flownet::SpellEffectType_Teleport:{
                CCAnimation* animation = SpellAnimationLoader::Instance()->GetSpellAnimation(this->m_SpellEffectType);
                CCFiniteTimeAction* animate = CCAnimate::create(animation);
                CCFiniteTimeAction* animateReverse = CCAnimate::create(animation);
                animateReverse = animateReverse->reverse();
                sequence = CCSequence::create(animate, animateReverse, NULL);
                break;
            }
            default:
                break;
        }
        
        if(sequence)
        {
            CCCallFunc* selfDestroy = CCCallFunc::create(this, callfunc_selector(SpellEffectNode::Destroy));
            sequence = CCSequence::create(sequence, selfDestroy, NULL);
            sequence->setTag(ActionType_Animation);
            effectImage->runAction(sequence);
        }
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

SpellEffectNode* SpellEffectNode::create(flownet::ActorID actorID, flownet::SpellEffectType spellEffectType)
{
    SpellEffectNode* newNode = new SpellEffectNode();
    newNode->m_ActorID = actorID;
    newNode->m_SpellEffectType = spellEffectType;
    
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

void SpellEffectNode::Destroy()
{
    this->removeFromParent();
}
