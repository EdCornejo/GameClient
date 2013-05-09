//
//  ActorNode.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 3/7/13.
//
//

#include "Headers.pch"

ShadowNode::ShadowNode() {}
ShadowNode::~ShadowNode(){}
    
bool ShadowNode::init()
{
    CCSprite* shadowImage = CCSprite::create("actor/shadow.png");
    this->addChild(shadowImage);
    
    scheduleUpdate();
    
    return true;
}
ShadowNode* ShadowNode::create(flownet::ActorID actorID)
{
    ShadowNode* newNode = new ShadowNode();
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
    
void ShadowNode::update(float deltaTime)
{
    BaseScene* scene = static_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
    ActorLayer* actorLayer = scene->GetActorLayer();

    if(!actorLayer) ASSERT_DEBUG(false);
    
    ActorNode* actor = actorLayer->FindActorNode(this->m_ActorID);
    
    if(!actor) return;
    
    this->setPosition(actor->getPosition());
}


HighlightNode::HighlightNode(): m_ActorID(ActorID_None) {}
HighlightNode::~HighlightNode() {}
    
bool HighlightNode::init()
{
    CCSprite* highlightImage = CCSprite::create("actor/highlight_circle.png");
    this->addChild(highlightImage);
    
    scheduleUpdate();
    
    return true;
}

HighlightNode* HighlightNode::create(flownet::ActorID actorID)
{
    HighlightNode* newNode = new HighlightNode();
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
    
void HighlightNode::update(float deltaTime)
{
    BaseScene* scene = static_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
    ActorLayer* actorLayer = scene->GetActorLayer();
    
    if(!actorLayer) ASSERT_DEBUG(false);
    
    ActorNode* actor = actorLayer->FindActorNode(this->m_ActorID);

    if(!actor) return;

    this->setPosition(actor->getPosition());
}


GuideLineNode::GuideLineNode(): m_SpellGuideLine(nullptr), m_SpellGuideIcon(nullptr){}
GuideLineNode::~GuideLineNode()
{
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
    
bool GuideLineNode::init()
{
    float distance = ccpDistance(this->m_Source, this->m_Destination);
    
    ASSERT_DEBUG(distance != 0);
    
    float scaleFactor = distance / 60; // NOTE 60 is health bar size
    
    // NOTE : our screen is landscape
    double rotateDegree = -atan2(this->m_Destination.y - this->m_Source.y, this->m_Destination.x - this->m_Source.x) * 180 / M_PI;
    
    if(this->m_SpellGuideLine){
        this->removeChild(this->m_SpellGuideLine);
        this->m_SpellGuideLine->release();
    }
    if(this->m_SpellGuideIcon){
        this->removeChild(this->m_SpellGuideIcon);
        this->m_SpellGuideIcon->release();
    }
    
    this->m_SpellGuideLine = CCSprite::create("ui/spell/spell_guide_line.png");
    this->m_SpellGuideLine->retain();
    this->m_SpellGuideLine->setAnchorPoint(CCPointZero);
    this->m_SpellGuideLine->setPosition(this->m_Source);
    this->m_SpellGuideLine->setScaleX(scaleFactor);
    this->m_SpellGuideLine->setRotation(rotateDegree);
    
    
    this->m_SpellGuideIcon = CCSprite::create("ui/spell/spell_position_marker.png"); // SpellImageLoader::GetSpellGuideImage(this->m_SpellType);
    this->m_SpellGuideIcon->retain();
    this->m_SpellGuideIcon->setPosition(this->m_Destination);
    
    this->addChild(this->m_SpellGuideLine);
    this->addChild(this->m_SpellGuideIcon);
    
    return true;
}


GuideLineNode* GuideLineNode::create(flownet::SpellType spellType, CCPoint source, CCPoint destination)
{
    GuideLineNode* newNode = new GuideLineNode();
    newNode->m_SpellType = spellType;
    newNode->m_Source = source;
    newNode->m_Destination = destination;
    
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


HUDNode::HUDNode(): m_ActorID(ActorID_None), m_RemainHealthPointBar(nullptr), m_GreenBar(nullptr), m_YellowBar(nullptr), m_RedBar(nullptr), m_DamagedHealthPointBar(nullptr){}
HUDNode::~HUDNode()
{
    if(this->m_RemainHealthPointBar)
    {
        this->m_RemainHealthPointBar->release();
        this->m_RemainHealthPointBar = nullptr;
    }
    if(this->m_GreenBar)
    {
        this->m_GreenBar->release();
        this->m_GreenBar = nullptr;
    }
    if(this->m_YellowBar)
    {
        this->m_YellowBar->release();
        this->m_YellowBar = nullptr;
    }
    if(this->m_RedBar)
    {
        this->m_RedBar->release();
        this->m_RedBar = nullptr;
    }
    if(this->m_DamagedHealthPointBar)
    {
        this->m_DamagedHealthPointBar->release();
        this->m_DamagedHealthPointBar = nullptr;
    }
}
    
bool HUDNode::init()
{
    CCSprite* background = CCSprite::create("ui/hud/background.png");
    this->addChild(background);

    this->m_DamagedHealthPointBar = CCSprite::create("ui/hud/health_damaged.png");
    this->m_DamagedHealthPointBar->setAnchorPoint(ccp(0, 0.5));
    this->m_DamagedHealthPointBar->setPosition(ccp(-this->m_DamagedHealthPointBar->getTextureRect().size.width / 2, 0));
    this->m_DamagedHealthPointBar->retain();
    this->addChild(this->m_DamagedHealthPointBar);
    
    this->m_GreenBar = CCSprite::create("ui/hud/health_remain_green.png");
    this->m_GreenBar->setAnchorPoint(ccp(0, 0.5));
    this->m_GreenBar->setPosition(ccp(-this->m_GreenBar->getTextureRect().size.width / 2, 0));
    this->m_GreenBar->retain();
    
    this->m_YellowBar = CCSprite::create("ui/hud/health_remain_yellow.png");
    this->m_YellowBar->setAnchorPoint(ccp(0, 0.5));
    this->m_YellowBar->setPosition(ccp(-this->m_YellowBar->getTextureRect().size.width / 2, 0));
    this->m_YellowBar->retain();
    
    this->m_RedBar = CCSprite::create("ui/hud/health_remain_red.png");
    this->m_RedBar->setAnchorPoint(ccp(0, 0.5));
    this->m_RedBar->setPosition(ccp(-this->m_RedBar->getTextureRect().size.width / 2, 0));
    this->m_RedBar->retain();
    
    this->m_RemainHealthPointBar = this->m_GreenBar;
    this->m_RemainHealthPointBar->retain();
    this->addChild(this->m_RemainHealthPointBar);
    
    this->setVisible(false);
    
    scheduleUpdate();
    
    return true;
}


HUDNode* HUDNode::create(flownet::ActorID actorID)
{
    HUDNode* newNode = new HUDNode();
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
    
void HUDNode::update(float deltaTime)
{
    BaseScene* scene = static_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
    ActorLayer* actorLayer = scene->GetActorLayer();
    
    if(!actorLayer) ASSERT_DEBUG(false);
    
    ActorNode* actor = actorLayer->FindActorNode(this->m_ActorID);

    if(!actor) return;

    // setPosition needs offset
    this->setPosition(ccpAdd(actor->getPosition(), ccp(PositionX, PositionY)));
}

void HUDNode::ChangeHealthPointBar(float scaleFactor)
{
    this->m_DamagedHealthPointBar->stopActionByTag(ActionType_UI);
    this->stopActionByTag(ActionType_UI);
    
    if(scaleFactor >= 0.5)
    {
        if(this->m_GreenBar != this->m_RemainHealthPointBar)
        {
            this->removeChild(this->m_RemainHealthPointBar);
            this->m_RemainHealthPointBar->release();
            this->m_RemainHealthPointBar = this->m_GreenBar;
            this->addChild(this->m_RemainHealthPointBar);
            this->m_RemainHealthPointBar->retain();
        }
    }
    else if(scaleFactor >= 0.25)
    {
        if(this->m_RemainHealthPointBar != this->m_YellowBar)
        {
            this->removeChild(this->m_RemainHealthPointBar);
            this->m_RemainHealthPointBar->release();
            this->m_RemainHealthPointBar = this->m_YellowBar;
            this->addChild(this->m_RemainHealthPointBar);
            this->m_RemainHealthPointBar->retain();
        }
    }
    else
    {
        if(this->m_RemainHealthPointBar != this->m_RedBar)
        {
            this->removeChild(this->m_RemainHealthPointBar);
            this->m_RemainHealthPointBar->release();
            this->m_RemainHealthPointBar = this->m_RedBar;
            this->addChild(this->m_RemainHealthPointBar);
            this->m_RemainHealthPointBar->retain();
        }
    }
    
    this->m_RemainHealthPointBar->setScaleX(scaleFactor);

    CCScaleTo* scaleTo = CCScaleTo::create(0.5, scaleFactor, 1);
    scaleTo->setTag(ActionType_UI);
    this->m_DamagedHealthPointBar->runAction(scaleTo);
    
    CCFiniteTimeAction* showHUD = CCCallFunc::create(this, callfunc_selector(HUDNode::ShowHUD));
    CCDelayTime* delay = CCDelayTime::create(3);
    CCFiniteTimeAction* hideHUD = CCCallFunc::create(this, callfunc_selector(HUDNode::HideHUD));
    CCSequence* sequence = CCSequence::create(showHUD, delay, hideHUD, NULL);
    sequence->setTag(ActionType_UI);
    
    this->runAction(sequence);
}

void HUDNode::ShowHUD()
{
    this->setVisible(true);
}

void HUDNode::HideHUD()
{
    this->setVisible(false);
}



ActorNode::ActorNode(): m_ActorID(ActorID_None), m_Skeleton(nullptr) {}

ActorNode::~ActorNode()
{
    if(this->m_Skeleton)
    {
        this->m_Skeleton->release();
        this->m_Skeleton = nullptr;
    }
}

bool ActorNode::init()
{
    return true;
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

void ActorNode::ChangeEquipment(flownet::EquipmentSlot equipmentSlot, flownet::ItemType itemType)
{
    switch (equipmentSlot) {
        case flownet::EquipmentSlot_Wand:
            this->ChangeWand(itemType);
            break;
        case flownet::EquipmentSlot_Hat:
            this->ChangeHat(itemType);
            break;
        case flownet::EquipmentSlot_Robe:
            this->ChangeRobe(itemType);
            break;
        case flownet::EquipmentSlot_Cloak:
            this->ChangeCloak(itemType);
            break;
        case flownet::EquipmentSlot_Shoes:
            this->ChangeShoes(itemType);
            break;
        case flownet::EquipmentSlot_Ring:
            this->ChangeRing(itemType);
            break;
//        case flownet::EquipmentSlot_Hair:
//            this->ChangeHair(itemType);
//            break;
        default:
            ASSERT_DEBUG(false);
            break;
    }
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

float ActorNode::getScale()
{
    return this->m_Skeleton->getScale();
}

void ActorNode::setScale(float scaleFactor)
{
    this->m_Skeleton->setScale(scaleFactor);
}

CCPoint ActorNode::GetSpellPosition()
{
    CCPoint position = this->getPosition();
    
    Skeleton_updateWorldTransform(this->m_Skeleton->skeleton);
    const Bone* bone = this->m_Skeleton->findBone("spell_position");
    if(bone)
    {
        position.x += bone->worldX * this->m_Skeleton->getScale();
        position.y += bone->worldY * this->m_Skeleton->getScale();
    }
    return position;
}

CCPoint ActorNode::GetTopPosition()
{
    CCPoint position = this->getPosition();
    
    Skeleton_updateWorldTransform(this->m_Skeleton->skeleton);
    const Bone* bone = this->m_Skeleton->findBone("top_position");
    if(bone)
    {
        position.x += bone->worldX;
        position.y += bone->worldY;
    }
    return position;
}

CCPoint ActorNode::GetMidPosition()
{
    CCPoint position = this->getPosition();
    
    Skeleton_updateWorldTransform(this->m_Skeleton->skeleton);
    const Bone* bone = this->m_Skeleton->findBone("mid_position");
    if(bone)
    {
        position.x += bone->worldX;
        position.y += bone->worldY;
    }
    return position;
}

void ActorNode::ChangeWand(flownet::ItemType itemType) { }

void ActorNode::ChangeHat(flownet::ItemType itemType) { }

void ActorNode::ChangeRobe(flownet::ItemType itemType) { }

void ActorNode::ChangeCloak(flownet::ItemType itemType) { }

void ActorNode::ChangeShoes(flownet::ItemType itemType) { }

void ActorNode::ChangeRing(flownet::ItemType itemType) { }

