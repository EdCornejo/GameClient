//
//  ActorNode.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 3/7/13.
//
//

#include "Headers.pch"

ActorNode::ActorNode(flownet::ActorID actorID): m_ActorID(actorID), m_HUD(nullptr), m_SpellGuideLine(nullptr), m_SpellGuideIcon(nullptr), m_Skeleton(nullptr)
{
    this->InitializeHUD();
}

ActorNode::ActorNode(flownet::Actor* actor): m_ActorID(actor->GetActorID()), m_HUD(nullptr), m_SpellGuideLine(nullptr), m_SpellGuideIcon(nullptr), m_Skeleton(nullptr)
{
    this->InitializeHUD();
}

ActorNode::~ActorNode()
{
    if(this->m_HUD)
    {
        this->m_HUD->release();
        this->m_HUD = nullptr;
    }
    if(this->m_Skeleton)
    {
        this->m_Skeleton->release();
        this->m_Skeleton = nullptr;
    }
    if(this->m_SpellGuideLine)
    {
        this->m_SpellGuideLine->release();
        this->m_SpellGuideLine = nullptr;
    }
    if(this->m_SpellGuideIcon)
    {
        this->m_SpellGuideIcon->release();
        this->m_SpellGuideIcon = nullptr;
    }
}

void ActorNode::HighLight()
{
    CCSprite* highLightImage = this->LoadHighLightImage();
    highLightImage->setAnchorPoint(CharacterAnchorPoint);
    this->addChild(highLightImage);
}

// NOTE : this method called by ActorLayer
void ActorNode::StopAnimationActions()
{
    CCAction* action = nullptr;
    while((action = this->getActionByTag(ActionType_Animation)) != nullptr)
    {
        this->stopAction(action);
    }
}

// NOTE : this method will be deprecated
// NOTE : this method called by AnimateXXX methods
void ActorNode::StopCharacterAnimate()
{
    CCAction* action = nullptr;
//    while((action = this->m_CharacterSprite->getActionByTag(ActionType_Animation)) != nullptr)
//    {
//        this->m_CharacterSprite->stopAction(action);
//    }
}

// NOTE : default looking direction for animation is left
void ActorNode::AnimateIdle()
{
    CCLOG("actor %d animates idle", this->GetActorID());
    
    Actor* actorInfo = this->GetActorInfo();
    if(!actorInfo->IsAlive())
    {
        return;
    }
    
    bool flipX = false;
    
    if(actorInfo->GetLookingDirection() == DIRECTION_LEFT) // actor's looking direction is left
    {
        flipX = false;
    }
    else
    {
        flipX = true;
    }

    this->StopCharacterAnimate();
    this->m_Skeleton->skeleton->flipX = flipX;
    
    // NOTE : check for same animation 
    if(this->m_Skeleton->state->animation != nullptr && strcmp(this->m_Skeleton->state->animation->name, "idle") == 0) return;
    this->m_Skeleton->setAnimation("idle", true);
}

void ActorNode::AnimateMoving()
{
    CCLOG("actor %d animates moving", this->GetActorID());
    
    
    Actor* actorInfo = this->GetActorInfo();
    
    if(!actorInfo->IsAlive())
    {
        this->StopAnimationActions();
        return;
    }
    
    bool flipX = false;
    
    if(actorInfo->GetLookingDirection() == DIRECTION_LEFT) // actor's looking direction is left
    {
        flipX = false;
    }
    else
    {
        flipX = true;
    }

    this->StopCharacterAnimate();
    this->m_Skeleton->skeleton->flipX = flipX;
    
    // NOTE : check for same animation, flipping animation is left behind
    if(this->m_Skeleton->state->animation != nullptr && strcmp(this->m_Skeleton->state->animation->name, "moving") == 0) return;
    
    this->m_Skeleton->setAnimation("moving", true);
}

void ActorNode::AnimateAttacking()
{
    CCLOG("actor %d animates attacking", this->GetActorID());
    
    Actor* actorInfo = this->GetActorInfo();
    
    if(!actorInfo->IsAlive())
    {
        return;
    }

    bool flipX = false;
    
    if(actorInfo->GetLookingDirection() == DIRECTION_LEFT) // actor's looking direction is left
    {
        flipX = false;
    }
    else
    {
        flipX = true;
    }

    this->StopCharacterAnimate();
    this->m_Skeleton->skeleton->flipX = flipX;
    // NOTE : check for same animation
    // NOTE : this->m_Skeleton->state->animation != nullptr &&  is test code
    if(this->m_Skeleton->state->animation != nullptr && strcmp(this->m_Skeleton->state->animation->name, "attacking") == 0) return;
    this->m_Skeleton->setAnimation("attacking", false);
}

void ActorNode::AnimateAttacked()
{
    CCLOG("actor %d animates attacked", this->GetActorID());

    Actor* actorInfo = this->GetActorInfo();
    
    if(!actorInfo->IsAlive())
    {
        return;
    }

    bool flipX = false;
    
    if(actorInfo->GetLookingDirection() == DIRECTION_LEFT) // actor's looking direction is left
    {
        flipX = false;
    }
    else
    {
        flipX = true;
    }

    this->StopCharacterAnimate();
    this->m_Skeleton->skeleton->flipX = flipX;
     
    // NOTE : check for same animation
    if(this->m_Skeleton->state->animation != nullptr && strcmp(this->m_Skeleton->state->animation->name, "attacked") == 0) return;
    this->m_Skeleton->setAnimation("attacked", false);
}

void ActorNode::AnimateBeginCasting()
{
    CCLOG("actor %d animates begin casting", this->GetActorID());
    
    Actor* actorInfo = this->GetActorInfo();
    
    if(!actorInfo->IsAlive())
    {
        return;
    }

    bool flipX = false;
    
    if(actorInfo->GetLookingDirection() == DIRECTION_LEFT) // actor's looking direction is left
    {
        flipX = false;
    }
    else
    {
        flipX = true;
    }

    this->StopCharacterAnimate();
    this->m_Skeleton->skeleton->flipX = flipX;
    // NOTE : check for same animation
    if(this->m_Skeleton->state->animation != nullptr && strcmp(this->m_Skeleton->state->animation->name, "begin_casting") == 0) return;

    this->m_Skeleton->setAnimation("begin_casting", false);
}

void ActorNode::AnimateRepeatCasting()
{
    CCLOG("actor %d animates repeat casting", this->GetActorID());
    
    Actor* actorInfo = this->GetActorInfo();
    
    if(!actorInfo->IsAlive())
    {
        return;
    }

    bool flipX = false;
    
    if(actorInfo->GetLookingDirection() == DIRECTION_LEFT) // actor's looking direction is left
    {
        flipX = false;
    }
    else
    {
        flipX = true;
    }

    this->StopCharacterAnimate();
    this->m_Skeleton->skeleton->flipX = flipX;
     // NOTE : check for same animation
    if(this->m_Skeleton->state->animation != nullptr && strcmp(this->m_Skeleton->state->animation->name, "casting") == 0) return;

    this->m_Skeleton->setAnimation("repeat_casting", true);
}

void ActorNode::AnimateEndCasting()
{
    CCLOG("actor %d animates end casting", this->GetActorID());
    
    Actor* actorInfo = this->GetActorInfo();
    
    if(!actorInfo->IsAlive())
    {
        return;
    }

    bool flipX = false;
    
    if(actorInfo->GetLookingDirection() == DIRECTION_LEFT) // actor's looking direction is left
    {
        flipX = false;
    }
    else
    {
        flipX = true;
    }

    this->StopCharacterAnimate();
    this->m_Skeleton->skeleton->flipX = flipX;
    // NOTE : check for same animation
    if(this->m_Skeleton->state->animation != nullptr && strcmp(this->m_Skeleton->state->animation->name, "end_casting") == 0) return;

    this->m_Skeleton->setAnimation("end_casting", false);
}

void ActorNode::AnimateFire()
{
    CCLOG("actor %d animates fire", this->GetActorID());
    
    Actor* actorInfo = this->GetActorInfo();
    
    if(!actorInfo->IsAlive())
    {
        return;
    }
    
    bool flipX = false;
    
    if(actorInfo->GetLookingDirection() == DIRECTION_LEFT) // actor's looking direction is left
    {
        flipX = false;
    }
    else
    {
        flipX = true;
    }

    this->StopCharacterAnimate();
    this->m_Skeleton->skeleton->flipX = flipX;
     // NOTE : check for same animation
    if(this->m_Skeleton->state->animation != nullptr && strcmp(this->m_Skeleton->state->animation->name, "fire") == 0) return;
    this->m_Skeleton->setAnimation("fire", false);
}

void ActorNode::AnimateDead()
{
    CCLOG("actor %d animates dead", this->GetActorID());
    
    Actor* actorInfo = this->GetActorInfo();

    bool flipX = false;
    
    if(actorInfo->GetLookingDirection() == DIRECTION_LEFT) // actor's looking direction is left
    {
        flipX = false;
    }
    else
    {
        flipX = true;
    }

    this->StopCharacterAnimate();
    this->m_Skeleton->skeleton->flipX = flipX;
    
    // NOTE : check for same animation
    if(this->m_Skeleton->state->animation != nullptr && strcmp(this->m_Skeleton->state->animation->name, "dead") == 0) return;

    this->m_Skeleton->setAnimation("dead", false);
}

void ActorNode::ChangeWeapon(WeaponType weaponType)
{
    std::string weaponName = "stick";

    this->m_Skeleton->setAttachment("weapon", weaponName.c_str());
}

void ActorNode::ChangeHat(HatType hatType)
{

}

void ActorNode::ChangeCloth(ClothType clothType)
{

}

flownet::ActorID ActorNode::GetActorID()
{
    return this->m_ActorID;
}

flownet::Actor* ActorNode::GetActorInfo()
{
    return GameClient::Instance().GetClientStage()->FindActor(this->m_ActorID);
}

CCRect ActorNode::GetRect()
{
    return CCRectMake(0, 0, 0, 0);
//    if(this->m_CharacterSprite)
//    {
//        return this->m_CharacterSprite->getTextureRect();
//    }
//    
//    return CCRectMake(0, 0, 0, 0);
}

// NOTE : scaleFactor is CurrentHP / MaxHP
void ActorNode::ChangeHealthPointBar(float scaleFactor)
{
    ASSERT_DEBUG(this->m_HUD != nullptr);
    
    CCSprite* hpBar = static_cast<CCSprite*>(this->m_HUD->getChildByTag(HP_BAR_REMAIN_TAG));
    ASSERT_DEBUG(hpBar != nullptr);
    

    this->stopActionByTag(ActionType_UI);

    CCFiniteTimeAction* showHUD = CCCallFunc::create(this, callfunc_selector(ActorNode::ShowHUD));
    CCFiniteTimeAction* changeBarScale = CCScaleTo::create(0.5, scaleFactor, 1);
    CCDelayTime* delay = CCDelayTime::create(3);
    CCFiniteTimeAction* hideHUD = CCCallFunc::create(this, callfunc_selector(ActorNode::HideHUD));
    CCSequence* sequence = CCSequence::create(showHUD, changeBarScale, delay, hideHUD, NULL);
    sequence->setTag(ActionType_UI);
    
    hpBar->runAction(sequence);
}

// NOTE : scaleFactor is CurrentMP / MaxMP
void ActorNode::ChangeManaPointBar(float scaleFactor)
{
    ASSERT_DEBUG(this->m_HUD != nullptr);
    
    CCSprite* mpBar = static_cast<CCSprite*>(this->m_HUD->getChildByTag(MP_BAR_REMAIN_TAG));
    ASSERT_DEBUG(mpBar != nullptr);
    
    this->stopActionByTag(ActionType_UI);

    CCFiniteTimeAction* showHUD = CCCallFunc::create(this, callfunc_selector(ActorNode::ShowHUD));
    CCFiniteTimeAction* changeBarScale = CCScaleTo::create(0.5, scaleFactor, 1);
    CCDelayTime* delay = CCDelayTime::create(3);
    CCFiniteTimeAction* hideHUD = CCCallFunc::create(this, callfunc_selector(ActorNode::HideHUD));
    CCSequence* sequence = CCSequence::create(showHUD, changeBarScale, delay, hideHUD, NULL);
    sequence->setTag(ActionType_UI);
    
    mpBar->runAction(sequence);
}

void ActorNode::InitializeHUD()
{
    this->m_HUD = CCSprite::create("blank.png");
    this->m_HUD->setPosition(CCPointZero);
    
    CCSprite* hpBar = CCSprite::create("ui/hud/health_bar_remain.png");
    hpBar->setAnchorPoint(CCPointZero);
    hpBar->setPosition(ccp(-30, 0));
    this->m_HUD->addChild(hpBar, 0, HP_BAR_REMAIN_TAG);
    this->m_HUD->setVisible(false);
    this->addChild(this->m_HUD);

}

void ActorNode::ShowSpellGuide(SpellType spellType, CCPoint destination)
{
    CCPoint invokerPoint = this->getPosition();
    CCPoint markerPoint = this->convertToNodeSpace(destination);
    
    // NOTE : the screen is rotated in 90 degrees... so x and y is oposite
    float distance = ccpDistance(invokerPoint, destination);
    
    ASSERT_DEBUG(distance != 0);
    
    float scaleFactor = distance / 60; /* 60 is guide lines width */
    double rotateDegree = -atan2(destination.y - invokerPoint.y, destination.x - invokerPoint.x) * 180 / M_PI;
    
    if(this->m_SpellGuideLine)
    {
        this->removeChild(this->m_SpellGuideLine);
    }
    if(this->m_SpellGuideIcon)
    {
        this->removeChild(this->m_SpellGuideIcon);
    }
    
    this->m_SpellGuideLine = CCSprite::create("ui/spell/spell_guide_line.png");
    this->m_SpellGuideLine->setAnchorPoint(CCPointZero);
    this->m_SpellGuideLine->setScaleX(scaleFactor);
    this->m_SpellGuideLine->setRotation(rotateDegree);
    
    this->m_SpellGuideIcon = CCSprite::create("ui/spell/spell_position_marker.png");
    this->m_SpellGuideIcon->setPosition(markerPoint);
    
    this->addChild(this->m_SpellGuideLine);
    this->addChild(this->m_SpellGuideIcon);
}

void ActorNode::HideSpellGuide()
{   
    if(this->m_SpellGuideLine)
    {
        this->removeChild(this->m_SpellGuideLine);
    }
    if(this->m_SpellGuideIcon)
    {
        this->removeChild(this->m_SpellGuideIcon);
    }

}

void ActorNode::ShowHUD()
{
    this->m_HUD->setVisible(true);
}


void ActorNode::HideHUD()
{
    this->m_HUD->setVisible(false);
}
