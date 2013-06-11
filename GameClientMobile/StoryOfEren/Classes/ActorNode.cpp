//
//  ActorNode.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 3/7/13.
//
//

#include "Headers.pch"

ShadowNode::ShadowNode() : m_ActorID(ActorID_None), m_SpellNode(nullptr) {}
ShadowNode::~ShadowNode()
{
}
    
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

ShadowNode* ShadowNode::create(SpellNode* spellNode)
{
    ShadowNode* newNode = new ShadowNode();
    newNode->m_SpellNode = spellNode;

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
    if(this->m_ActorID != ActorID_None)
    {
        BaseScene* scene = static_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
        ActorLayer* actorLayer = scene->GetActorLayer();
        
        if(!actorLayer) ASSERT_DEBUG(false);
        
        ActorNode* actor = actorLayer->FindActorNode(this->m_ActorID);
        
        if(!actor) return;
        
        this->setPosition(actor->getPosition());
    }
    else if (this->m_SpellNode)
    {
        CCPoint spellPosition;
        switch(this->m_SpellNode->GetSpellInfo().m_StartingPoint)
        {
            case flownet::StartingPoint_Self :
                spellPosition = this->m_SpellNode->getPosition();
                spellPosition.y -= 30;
                break;
            case flownet::StartingPoint_Somewhere:
                spellPosition = this->m_SpellNode->GetDestination();
                spellPosition.x = this->m_SpellNode->getPosition().x;
                break;
            case flownet::StartingPoint_Target :
            default:
                spellPosition = this->m_SpellNode->GetDestination();
                break;
        }
        
        this->setPosition(spellPosition);
    }
    else
    {
        ASSERT_DEBUG(false);
    }
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


GuideLineNode::GuideLineNode(): m_SpellGuideLine(nullptr), m_SpellGuideIcon(nullptr), m_SpellCastingIcon(nullptr) {}
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
    if(this->m_SpellCastingIcon)
    {
        this->m_SpellCastingIcon->release();
        this->m_SpellCastingIcon = nullptr;
    }
}
    
bool GuideLineNode::init()
{
    float distance = ccpDistance(this->m_Source, this->m_Destination);
    
    ASSERT_DEBUG(distance != 0);
    
    float scaleFactor = distance / 60; // NOTE 60 is health bar size
    
    // NOTE : our screen is landscape
    double rotateDegree = -atan2(this->m_Destination.y - this->m_Source.y, this->m_Destination.x - this->m_Source.x) * 180 / M_PI;
    
    this->m_SpellGuideLine = CCSprite::create("ui/guide_line/spell_guide_line.png");
    this->m_SpellGuideLine->retain();
    this->m_SpellGuideLine->setAnchorPoint(CCPointZero);
    this->m_SpellGuideLine->setPosition(this->m_Source);
    this->m_SpellGuideLine->setScaleX(scaleFactor);
    this->m_SpellGuideLine->setRotation(rotateDegree);
    
    this->m_SpellCastingIcon = SpellImageLoader::GetSpellGuideImage(this->m_SpellType);
    this->m_SpellCastingIcon->retain();
    this->m_SpellCastingIcon->setScaleY(0.4);
    this->m_SpellCastingIcon->setPosition(this->m_Source);
    
    
    this->m_SpellGuideIcon = SpellImageLoader::GetSpellGuideImage(this->m_SpellType);
    this->m_SpellGuideIcon->retain();
    this->m_SpellGuideIcon->setScaleY(0.4);
    this->m_SpellGuideIcon->setPosition(this->m_Destination);
    
    this->addChild(this->m_SpellGuideLine);
    this->addChild(this->m_SpellCastingIcon);
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


HUDNode::HUDNode(): m_ActorID(ActorID_None), m_NameLabel(nullptr), m_RemainHealthPointBar(nullptr), m_RemainManaPointBar(nullptr) {}
HUDNode::~HUDNode()
{
    CC_SAFE_RELEASE(this->m_NameLabel);
    CC_SAFE_RELEASE(this->m_RemainHealthPointBar);
    CC_SAFE_RELEASE(this->m_RemainManaPointBar);
}
    
bool HUDNode::init()
{
    if(IsPlayerID(this->m_ActorID))
    {
        Player* player = GameClient::Instance().GetClientStage()->FindPlayer(this->m_ActorID);
        this->m_NameLabel = CCLabelTTF::create(player->GetPlayerName().c_str(), "thonburi", 12);
        this->m_NameLabel->setColor(ccc3(0, 0, 0));
        this->m_NameLabel->setPosition(ccp(0, 10));
        this->m_NameLabel->retain();
        this->addChild(this->m_NameLabel);
        
        // DISCUSS : do i have to fix the emblem position?
        CCSize nameLabelSize = this->m_NameLabel->getContentSize();
        CCSprite* emblem = CCSprite::create("ui/emblem/emblem.png");
        emblem->setPosition(ccp(-nameLabelSize.width / 2 - 10, 10));
        this->addChild(emblem);
    }
    
    CCSprite* statusBarBackground = CCSprite::create("ui/hud/background.png");
    this->addChild(statusBarBackground);
    
    CCSprite* healthBarBackground = CCSprite::create("ui/hud/max.png");
    healthBarBackground->setPosition(ccp(-healthBarBackground->getTextureRect().size.width / 2, 1.5));
    healthBarBackground->setAnchorPoint(ccp(0, 0.5));
    this->addChild(healthBarBackground);
    this->m_RemainHealthPointBar = CCSprite::create("ui/hud/health_remain.png");
    this->m_RemainHealthPointBar->retain();
    this->m_RemainHealthPointBar->setPosition(ccp(-this->m_RemainHealthPointBar->getTextureRect().size.width / 2, 1.5));
    this->m_RemainHealthPointBar->setAnchorPoint(ccp(0, 0.5));
    this->addChild(this->m_RemainHealthPointBar);

    CCSprite* manaBarBackground = CCSprite::create("ui/hud/max.png");
    manaBarBackground->setPosition(ccp(-manaBarBackground->getTextureRect().size.width / 2, -1.5));
    manaBarBackground->setAnchorPoint(ccp(0, 0.5));
    this->addChild(manaBarBackground);
    this->m_RemainManaPointBar = CCSprite::create("ui/hud/mana_remain.png");
    this->m_RemainManaPointBar->retain();
    this->m_RemainManaPointBar->setPosition(ccp(-this->m_RemainManaPointBar->getTextureRect().size.width / 2, -1.5));
    this->m_RemainManaPointBar->setAnchorPoint(ccp(0, 0.5));
    this->addChild(this->m_RemainManaPointBar);
    
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
    this->m_RemainHealthPointBar->setScaleX(scaleFactor);
}

void HUDNode::ChangeManaPointBar(float scaleFactor)
{
    this->m_RemainManaPointBar->setScaleX(scaleFactor);
}




ChatBalloonNode::ChatBalloonNode(): m_ActorID(ActorID_None), m_LastInputTime(0), m_MessageLabel(nullptr) {}

ChatBalloonNode::~ChatBalloonNode()
{
    CC_SAFE_RELEASE(this->m_MessageLabel);
}

bool ChatBalloonNode::init()
{
    if(!CCNode::init()) return false;
    
    CCSprite* background = CCSprite::create("ui/chatting/chat_balloon.png");
    background->setOpacity(150);
    background->setAnchorPoint(CCPointLowerMid);
    this->addChild(background);
    
    const int paddingX = 4;
    const int paddingY = 4;
    CCSize backgroundSize = background->getContentSize();
    backgroundSize.width -= paddingX * 2;
    backgroundSize.height -= paddingY * 2;
    
    this->m_MessageLabel = CCLabelTTF::create("", "thonburi", 12, backgroundSize, kCCTextAlignmentLeft, kCCVerticalTextAlignmentCenter);
    this->m_MessageLabel->retain();
    this->m_MessageLabel->setColor(ccBLACK);
    this->m_MessageLabel->setAnchorPoint(CCPointLowerLeft);
    this->m_MessageLabel->setPosition(ccp(paddingX, paddingY * 2));
    background->addChild(this->m_MessageLabel);
    
    scheduleUpdate();
    
    return true;
}

ChatBalloonNode* ChatBalloonNode::create(flownet::ActorID actorID)
{
    ChatBalloonNode* node = new ChatBalloonNode();
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
    
void ChatBalloonNode::update(float deltaTime)
{
    if(this->m_LastInputTime < GameClient::Instance().GetClientTimer().Check())
    {
        this->setVisible(false);
        return;
    }

    BaseScene* scene = static_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
    ASSERT_DEBUG(scene);
    
    ActorLayer* actorLayer = scene->GetActorLayer();
    ASSERT_DEBUG(actorLayer);
    
    ActorNode* actorNode = actorLayer->FindActorNode(this->m_ActorID);
    ASSERT_DEBUG(actorNode);
    
    CCPoint actorPosition = actorNode->getPosition();
    actorPosition.y += 60;
    
    this->setPosition(actorPosition);
}

void ChatBalloonNode::ChangeMessage(flownet::STRING message)
{
    this->m_LastInputTime = GameClient::Instance().GetClientTimer().Check();
    this->m_LastInputTime += ServerTime(6000);

    this->m_MessageLabel->setString(message.c_str());
    
    this->setVisible(true);
}









ActorNode::ActorNode(): m_ActorID(ActorID_None), m_Skeleton(nullptr), m_StateEffectID(0) {}

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

void ActorNode::InitializeAnimation()
{
    Actor* actor = GameClient::Instance().GetClientStage()->FindActor(this->m_ActorID);
    switch (actor->GetActorState()) {
        case flownet::ActorState_Attacking:
            this->AnimateAttacking();
            break;
        case flownet::ActorState_Casting:
            this->AnimateBeginCasting();
            break;
        case flownet::ActorState_Dead:
            this->AnimateDead();
            break;
        case flownet::ActorState_Moving:
            this->AnimateMoving();
            break;
        case flownet::ActorState_Idle:
        default:
            this->AnimateIdle();
            break;
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
    CCLOGINFO("actor %d animates idle", this->GetActorID());
    
    Actor* actorInfo = this->GetActorInfo();
    if(!actorInfo) return;
    
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
    if(this->m_Skeleton->states[0]->animation != nullptr && strcmp(this->m_Skeleton->states[0]->animation->name, "idle") == 0) return;
    this->m_Skeleton->setAnimation("idle", true);
}

void ActorNode::AnimateMoving()
{
    CCLOGINFO("actor %d animates moving", this->GetActorID());
    
    
    Actor* actorInfo = this->GetActorInfo();
    
    if(!actorInfo) return;
    
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
    if(this->m_Skeleton->states[0]->animation != nullptr && strcmp(this->m_Skeleton->states[0]->animation->name, "moving") == 0) return;
    this->m_Skeleton->setAnimation("moving", true);
}

void ActorNode::AnimateAttacking()
{
    CCLOGINFO("actor %d animates attacking", this->GetActorID());
    
    Actor* actorInfo = this->GetActorInfo();
    
    if(!actorInfo) return;
    
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
    if(this->m_Skeleton->states[0]->animation != nullptr && strcmp(this->m_Skeleton->states[0]->animation->name, "attacking") == 0) return;
    this->m_Skeleton->setAnimation("attacking", false);
}

void ActorNode::AnimateAttacked()
{
    CCLOGINFO("actor %d animates attacked", this->GetActorID());

    Actor* actorInfo = this->GetActorInfo();
    if(!actorInfo) return;
    
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
    if(this->m_Skeleton->states[0]->animation != nullptr && strcmp(this->m_Skeleton->states[0]->animation->name, "attacked") == 0) return;
    this->m_Skeleton->setAnimation("attacked", false);
}

void ActorNode::AnimateBeginCasting()
{
    CCLOGINFO("actor %d animates begin casting", this->GetActorID());
    
    Actor* actorInfo = this->GetActorInfo();
    if(!actorInfo) return;
    
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
    if(this->m_Skeleton->states[0]->animation != nullptr && strcmp(this->m_Skeleton->states[0]->animation->name, "begin_casting") == 0) return;
    this->m_Skeleton->setAnimation("begin_casting", false);
}

void ActorNode::AnimateRepeatCasting()
{
    CCLOGINFO("actor %d animates repeat casting", this->GetActorID());
    
    Actor* actorInfo = this->GetActorInfo();
    if(!actorInfo) return;
    
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
    if(this->m_Skeleton->states[0]->animation != nullptr && strcmp(this->m_Skeleton->states[0]->animation->name, "repeat_casting") == 0) return;
    this->m_Skeleton->setAnimation("repeat_casting", true);
}

void ActorNode::AnimateEndCasting()
{
    CCLOGINFO("actor %d animates end casting", this->GetActorID());
    
    Actor* actorInfo = this->GetActorInfo();
    if(!actorInfo) return;
    
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
    if(this->m_Skeleton->states[0]->animation != nullptr && strcmp(this->m_Skeleton->states[0]->animation->name, "end_casting") == 0) return;
    this->m_Skeleton->setAnimation("end_casting", false);
}

void ActorNode::AnimateFire()
{
    CCLOGINFO("actor %d animates fire", this->GetActorID());
    
    Actor* actorInfo = this->GetActorInfo();
    if(!actorInfo) return;
    
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
    if(this->m_Skeleton->states[0]->animation != nullptr && strcmp(this->m_Skeleton->states[0]->animation->name, "fire") == 0) return;
    this->m_Skeleton->setAnimation("fire", false);
}

void ActorNode::AnimateDead()
{
    CCLOGINFO("actor %d animates dead", this->GetActorID());
    
    Actor* actorInfo = this->GetActorInfo();
    if(!actorInfo) return;
    
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
    if(this->m_Skeleton->states[0]->animation != nullptr && strcmp(this->m_Skeleton->states[0]->animation->name, "dead") == 0) return;
    this->m_Skeleton->setAnimation("dead", false);
}

void ActorNode::PlayIdleEffect(bool loop)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopEffect(this->m_StateEffectID);
    this->m_StateEffectID = CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("common/idle.mp3", loop);
}

void ActorNode::PlayMovingEffect(bool loop)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopEffect(this->m_StateEffectID);
    this->m_StateEffectID = CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("common/moving.mp3", loop);
}

void ActorNode::PlayAttackingEffect(bool loop)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopEffect(this->m_StateEffectID);
    this->m_StateEffectID = CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("common/attacking.mp3", loop);
}

void ActorNode::PlayAttackedEffect(bool loop)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("common/attacked.mp3", loop);
}

void ActorNode::PlayBeginCastingEffect(bool loop)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopEffect(this->m_StateEffectID);
    this->m_StateEffectID = CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("common/begin_casting.mp3", loop);
}

void ActorNode::PlayRepeatCastingEffect(bool loop)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopEffect(this->m_StateEffectID);
    this->m_StateEffectID = CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("common/repeat_casting.mp3", loop);
}

void ActorNode::PlayEndCastingEffect(bool loop)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopEffect(this->m_StateEffectID);
    this->m_StateEffectID = CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("common/end_casting.mp3", loop);
}

void ActorNode::PlayDeadEffect(bool loop)
{
    CocosDenshion::SimpleAudioEngine::sharedEngine()->stopEffect(this->m_StateEffectID);
    this->m_StateEffectID = CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("common/dead.mp3", loop);
}



void ActorNode::Reload()
{
    Actor* actor = GameClient::Instance().GetClientStage()->FindPlayer(this->m_ActorID);
    flownet::EquipmentList& equipmentList = actor->GetEquipmentList();
    equipmentList.ForAllItemSlots([this, actor](EquipmentSlot equipmentSlot, ItemID itemID){
        if(itemID == ItemID_None)
        {
            this->ChangeEquipment(equipmentSlot, ItemType_None);
        }
        else
        {
            const Item* item = actor->GetStash().FindItem(itemID);
            this->ChangeEquipment(equipmentSlot, item->GetItemType());
        }
    });
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
        position.x += bone->worldX * this->m_Skeleton->getScale();
        position.y += bone->worldY * this->m_Skeleton->getScale();
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
        position.x += bone->worldX * this->m_Skeleton->getScale();
        position.y += bone->worldY * this->m_Skeleton->getScale();
    }
    return position;
}

void ActorNode::ChangeWand(flownet::ItemType itemType) { }

void ActorNode::ChangeHat(flownet::ItemType itemType) { }

void ActorNode::ChangeRobe(flownet::ItemType itemType) { }

void ActorNode::ChangeCloak(flownet::ItemType itemType) { }

void ActorNode::ChangeShoes(flownet::ItemType itemType) { }

void ActorNode::ChangeRing(flownet::ItemType itemType) { }

