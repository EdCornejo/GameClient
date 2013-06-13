//
//  UILayer.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 3/6/13.
//
//

#include "Headers.pch"

UILayer::UILayer() : m_StageType(flownet::StageType_NONE), m_SpellQuickSlotNode(nullptr), m_StashNode(nullptr), m_EquipmentNode(nullptr), m_InventoryNode(nullptr), m_MenuBarNode(nullptr), m_ChattingNode(nullptr), m_SelectedSpellType(SpellType_NONE), m_SpellDestinationPoint(), m_LastTouchLocation(), m_LastTouchTime(0)
{
    
}

UILayer::~UILayer()
{
    CCTouchDispatcher* touchDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
    if(this->m_SpellQuickSlotNode)
    {
        this->m_SpellQuickSlotNode->release();
        this->m_SpellQuickSlotNode = nullptr;
    }
    if(this->m_StashNode)
    {
        touchDispatcher->removeDelegate(this->m_StashNode);
        this->m_StashNode->release();
        this->m_StashNode = nullptr;
    }
    if(this->m_EquipmentNode)
    {
        touchDispatcher->removeDelegate(this->m_EquipmentNode);
        this->m_EquipmentNode->release();
        this->m_EquipmentNode = nullptr;
    }
    if(this->m_InventoryNode)
    {
        touchDispatcher->removeDelegate(this->m_InventoryNode);
        this->m_InventoryNode->release();
        this->m_InventoryNode = nullptr;
    }
    if(this->m_MenuBarNode)
    {
        touchDispatcher->removeDelegate(this->m_MenuBarNode);
        this->m_MenuBarNode->release();
        this->m_MenuBarNode = nullptr;
    }
    if(this->m_ChattingNode)
    {
        touchDispatcher->removeDelegate(this->m_ChattingNode);
        this->m_ChattingNode->release();
        this->m_ChattingNode = nullptr;
    }
    CC_SAFE_RELEASE(this->m_ExpBarNode);
}

bool UILayer::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    switch (this->m_StageType) {
        case flownet::StageType_MushroomField:
        case flownet::StageType_SpiderCaveFront:
        case flownet::StageType_SpiderCave:
        default:
            this->InitializeSpellQuickSlot();
            this->InitializeInventory();
            //this->InitializeStash();
            this->InitializeEquipment();
            this->InitializeMenuBar();
            this->InitializeExpBar();
            this->InitializeChatting();

            break;
    }
    
    this->setTouchEnabled(true);
    
    scheduleUpdate();
    
    return true;
}

UILayer* UILayer::create(StageType stageType)
{
    UILayer* newUILayer = new UILayer();
    newUILayer->m_StageType = stageType;
    
    if(newUILayer && newUILayer->init())
    {
        newUILayer->m_StageType = stageType;
        newUILayer->autorelease();
        return newUILayer;
    }
    else
    {
        delete newUILayer;
        newUILayer = nullptr;
        return nullptr;
    }
}


void UILayer::update(float deltaTime)
{
   this->TranslateScreen();
}

// NOTE : touch process order is
//        Spell -> Item -> Move (default)
void UILayer::ccTouchesBegan(cocos2d::CCSet *touches, cocos2d::CCEvent *event)
{
    CCTouch* touch = (CCTouch*)(touches->anyObject());
    CCPoint touchLocation = touch->getLocation();
    
    CCLOG("touch began");
    
    if(this->TouchProcessSpellBegan(touchLocation)) return;
}

void UILayer::ccTouchesEnded(CCSet *touches, CCEvent *event)
{
    CCTouch* touch = (CCTouch*)( touches->anyObject() );
    CCPoint touchLocation = touch->getLocation();
    
    CCLOG("touch end");
    
    if(this->TouchProcessSpellEnded(touchLocation)) return;
    if(this->TouchProcessItem(touchLocation)) return;
    if(this->TouchProcessMove(touchLocation)) return;
}

void UILayer::OnResponse() const
{
    BaseLayer::OnResponse();
    
    if(this->m_MenuBarNode)
    {
        this->m_MenuBarNode->OnResponse();
    }
}

void UILayer::InitializeSpellQuickSlot()
{
    this->m_SpellQuickSlotNode = SpellQuickSlotNode::create();
    this->m_SpellQuickSlotNode->retain();
    this->m_SpellQuickSlotNode->setPosition(ccp(SpellQuickSlotNode::PositionX, SpellQuickSlotNode::PositionY));
    
    this->addChild(this->m_SpellQuickSlotNode);
}

void UILayer::InitializeInventory()
{
    this->m_InventoryNode = InventoryNode::create();
    this->m_InventoryNode->retain();
    this->m_InventoryNode->setPosition(ccp(InventoryNode::PositionX, InventoryNode::PositionY));

    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this->m_InventoryNode, kCCMenuHandlerPriority - 1, true);

    this->addChild(this->m_InventoryNode);
}

void UILayer::InitializeStash()
{
    this->m_StashNode = StashNode::create();
    this->m_StashNode->retain();
    this->m_StashNode->setPosition(ccp(StashNode::PositionX, StashNode::PositionY));
    
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this->m_StashNode, kCCMenuHandlerPriority - 1, true);
    
    this->addChild(this->m_StashNode);
}

void UILayer::InitializeEquipment()
{
    this->m_EquipmentNode = EquipmentNode::create();
    this->m_EquipmentNode->retain();
    this->m_EquipmentNode->setPosition(ccp(EquipmentNode::PositionX, EquipmentNode::PositionY));
    
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this->m_EquipmentNode, kCCMenuHandlerPriority - 1, true);
    
    this->addChild(this->m_EquipmentNode);
}

void UILayer::InitializeMenuBar()
{
    this->m_MenuBarNode = MenuBarNode::create();
    this->m_MenuBarNode->retain();
    this->m_MenuBarNode->setPosition(ccp(MenuBarNode::PositionX, MenuBarNode::PositionY));
    
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this->m_MenuBarNode, kCCMenuHandlerPriority, true);
    
    this->addChild(this->m_MenuBarNode);
}

void UILayer::InitializeChatting()
{
    this->m_ChattingNode = ChattingNode::create();
    this->m_ChattingNode->retain();
    
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this->m_ChattingNode, kCCMenuHandlerPriority - 1, true);

    this->addChild(this->m_ChattingNode);
}

void UILayer::InitializeExpBar()
{
    this->m_ExpBarNode = ExpBarNode::create();
    this->m_ExpBarNode->retain();
    
    this->addChild(this->m_ExpBarNode);
}

InventoryNode* UILayer::GetInventoryNode()
{
    return this->m_InventoryNode;
}

StashNode* UILayer::GetStashNode()
{
    return this->m_StashNode;
}

bool UILayer::TouchProcessSpellBegan(CCPoint touchLocation)
{
    BaseScene* scene = static_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
    ActorLayer* actorLayer = scene->GetActorLayer();
    
    if(!actorLayer) return false;
    
    ClientPlayer* player = static_cast<ClientPlayer*>(GameClient::Instance().GetClientStage()->FindPlayer(GameClient::Instance().GetMyActorID()));

    if(this->m_SelectedSpellType == SpellType_NONE) return false;
    if(player->IsStateAttacking()) return false;
    
    touchLocation = actorLayer->convertToNodeSpace(touchLocation);

    player->ChangeToCastingState();
    this->m_SpellDestinationPoint = touchLocation;
    GameClient::Instance().GetClientObject().SendCSRequestBeginCast(GameClient::Instance().GetClientStage()->GetStageID(), player->GetActorID(), this->m_SelectedSpellType, PointConverter::Convert(touchLocation));
    
    CCLOG("begin request sent");

    return true;
}

bool UILayer::TouchProcessSpellEnded(CCPoint touchLocation)
{
    ClientPlayer* player = static_cast<ClientPlayer*>(GameClient::Instance().GetClientStage()->FindPlayer(GameClient::Instance().GetMyActorID()));
    
    if(this->m_SelectedSpellType == SpellType_NONE)
    {
        return false;
    }
    if(!player->IsStateCasting())
    {
        return false;
    }

    player->ChangeToIdleState();
    GameClient::Instance().GetClientObject().SendCSRequestEndCast(GameClient::Instance().GetClientStage()->GetStageID(), player->GetActorID(), this->m_SelectedSpellType, PointConverter::Convert(this->m_SpellDestinationPoint));
    this->m_SelectedSpellType = SpellType_NONE;
    this->m_SpellDestinationPoint = CCPointZero;
    
    CCLOG("end request sent");
    
    return true;
}

bool UILayer::TouchProcessItem(CCPoint touchLocation)
{
    BaseScene* scene = static_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
    ActorLayer* actorLayer = scene->GetActorLayer();
    if(!actorLayer) return false;
    
    touchLocation = actorLayer->convertToNodeSpace(touchLocation);
    ItemNode* itemNode = actorLayer->FindSelectedItemNode(touchLocation);
    if(!itemNode) return false;
    
    GameClient::Instance().GetClientObject().SendCSRequestPickUpItemFromField(GameClient::Instance().GetClientStage()->GetStageID(), GameClient::Instance().GetMyActorID(), itemNode->GetItemID());
    
    return true;
}

bool UILayer::TouchProcessMove(CCPoint touchLocation)
{
    // NOTE : moving player is the default behavior

    BaseScene* scene = static_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
    ActorLayer* actorLayer = scene->GetActorLayer();
    
    if(!actorLayer) return false;
    
    touchLocation = actorLayer->convertToNodeSpace(touchLocation);

    // NOTE : boundary check
    ClientStage* stage = GameClient::Instance().GetClientStage();
    if(touchLocation.x < PointConverter::ModiXToViewPointX(0)) // NOTE : left boundary check
    {
        touchLocation.x = PointConverter::ModiXToViewPointX(0);
    }
    if(touchLocation.x > PointConverter::ModiXToViewPointX(stage->GetWidth())) // NOTE : right boundary check
    {
        touchLocation.x = PointConverter::ModiXToViewPointX(stage->GetWidth());
    }
    if(touchLocation.y < PointConverter::ModiYToViewPointY(0)) // NOTE : lower boundary check
    {
        touchLocation.y = PointConverter::ModiYToViewPointY(0);
    }
    if(touchLocation.y > PointConverter::ModiYToViewPointY(stage->GetHeight())) // NOTE : upper boundary check
    {
        touchLocation.y = PointConverter::ModiYToViewPointY(stage->GetHeight());
    }
    
    ActorID myActorID = GameClient::Instance().GetMyActorID();
    
    PlayerNode* node = actorLayer->FindPlayerNode(myActorID);
    
    ASSERT_DEBUG(node != nullptr);

    const float doubleTouchRadius = 20;
    const ServerTime doubleTouchTimeRange = ServerTime(300); // millisecond
    const ServerTime currentTime = GameClient::Instance().GetClientTimer().Check();
    float distanceToLastTouchLocation = ccpDistance(this->m_LastTouchLocation, touchLocation);
    CCLOG("distance to last touch location %f", distanceToLastTouchLocation);
    
    if(distanceToLastTouchLocation < doubleTouchRadius && (currentTime - this->m_LastTouchTime) < doubleTouchTimeRange)
    {
        POINT dest = PointConverter::Convert(touchLocation);
        CCLOG("teleport to CCPoint %f, %f", touchLocation.x, touchLocation.y);
        CCLOG("teleport to POINT %f, %f", dest.x, dest.y);
        GameClient::Instance().GetClientObject().SendCSRequestTeleportActor(GameClient::Instance().GetClientStage()->GetStageID(), myActorID, PointConverter::Convert(node->getPosition()), PointConverter::Convert(touchLocation));
    }
    else
    {
        GameClient::Instance().GetClientObject().SendCSRequestMoveActor(GameClient::Instance().GetClientStage()->GetStageID(), myActorID, PointConverter::Convert(node->getPosition()), PointConverter::Convert(touchLocation));
    }

    this->m_LastTouchTime = currentTime;
    this->m_LastTouchLocation = touchLocation;
    
    return true;
}   

void UILayer::TranslateScreen()
{
    const int Border = 160;
    const int LeftTranslateBorder = Border;
    const int RightTranslateBorder = 480 - Border;

    BaseScene* scene = static_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
    ActorLayer* actorLayer = scene->GetActorLayer();
    BackgroundLayer* backgroundLayer = scene->GetBackgroundLayer();
    
    if(!actorLayer) return;
    
    ActorNode* actor = actorLayer->FindPlayerNode(GameClient::Instance().GetMyActorID());
    
    if(!actor) return;
    
    CCPoint positionInScene = actorLayer->convertToWorldSpace(actor->getPosition());
    positionInScene.y = 0;
    
    if(positionInScene.x <= LeftTranslateBorder)
    {
        // move screen to left
        // if the screen is on the leftmost position still there
        positionInScene.x -= LeftTranslateBorder;

        CCPoint deltaPosition = ccpSub(CCPointZero, positionInScene);
        CCPoint actorLayerPosition = actorLayer->getPosition();
        actorLayerPosition = ccpAdd(actorLayerPosition, deltaPosition);
        if(actorLayerPosition.x >= 0)
        {
            actorLayerPosition.x = 0;
        }
        
        backgroundLayer->setPosition(actorLayerPosition);
        actorLayer->setPosition(actorLayerPosition);
    }
    else if(positionInScene.x >= RightTranslateBorder)
    {
        // move screen to right
        // if the scrren is on the rightmost position still there
        positionInScene.x -= RightTranslateBorder;
        
        CCPoint deltaPosition = ccpAdd(CCPointZero, positionInScene);
        CCPoint actorLayerPosition = scene->convertToNodeSpace(actorLayer->getPosition());
        actorLayerPosition = ccpSub(actorLayerPosition, deltaPosition);
        
        float stageWidth = GameClient::Instance().GetClientStage()->GetStageInfo().m_Width;
        stageWidth = -(PointConverter::ModiXToViewPointX(stageWidth) - 450);
        
        if(actorLayerPosition.x <= stageWidth)
        {
            actorLayerPosition.x = stageWidth;
        }

        backgroundLayer->setPosition(actorLayerPosition);
        actorLayer->setPosition(actorLayerPosition);
    }
}

void UILayer::SetSelectedSpellType(flownet::SpellType spellType)
{
    this->m_SelectedSpellType = spellType;
}

void UILayer::SwapInventorySlot(flownet::InventorySlot sourceSlotNumber, flownet::InventorySlot destinationSlotNumber)
{
    if(this->m_InventoryNode)
    {
        this->m_InventoryNode->SwapInventorySlot(sourceSlotNumber, destinationSlotNumber);
    }
}

void UILayer::ApplyCoolTime(flownet::SpellType spellType)
{
    if(this->m_SpellQuickSlotNode)
    {
        this->m_SpellQuickSlotNode->ApplyCoolTime(spellType);
    }
}

void UILayer::RemoveSpellHighlight()
{
    if(this->m_SpellQuickSlotNode)
    {
        this->m_SpellQuickSlotNode->RemoveHighlight();
    }
}

void UILayer::MessageReceived(flownet::ActorID senderID, flownet::STRING senderName, flownet::STRING message)
{
    if(this->m_ChattingNode)
    {
        this->m_ChattingNode->MessageReceived(senderName, message);
    }
}

void UILayer::ShowStageClearMessage()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSprite* clearMessage = CCSprite::create("clear.png");
    clearMessage->setPosition(ccp(winSize.width / 2, winSize.height / 2));
    
    CCBlink* blink = CCBlink::create(1, 2);
    CCDelayTime* delay = CCDelayTime::create(1);
    CCCallFuncO* removeChild = CCCallFuncO::create(this, callfuncO_selector(UILayer::RemoveChild), clearMessage);
    CCSequence* sequence = CCSequence::create(blink, delay, removeChild, NULL);
    clearMessage->runAction(sequence);
    
    this->addChild(clearMessage);
}

void UILayer::ShowTierClearMessage()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSprite* clearMessage = CCSprite::create("next.png");
    clearMessage->setPosition(ccp(winSize.width/2, winSize.height/2));
    
    CCBlink* blink = CCBlink::create(1, 2);
    CCDelayTime* delay = CCDelayTime::create(1);
    CCCallFuncO* removeChild = CCCallFuncO::create(this, callfuncO_selector(UILayer::RemoveChild), clearMessage);
    CCSequence* sequence = CCSequence::create(blink, delay, removeChild, NULL);
    clearMessage->runAction(sequence);
    
    this->addChild(clearMessage);
}


void UILayer::UpdateInventory()
{
    if(this->m_InventoryNode)
    {
        this->m_InventoryNode->Update();
    }
}

void UILayer::UpdateStash()
{
    if(this->m_StashNode)
    {
        this->m_StashNode->Update();
    }
}

void UILayer::UpdateEquipment()
{
    if(this->m_EquipmentNode)
    {
        this->m_EquipmentNode->Update();
    }
}

void UILayer::UpdateExpBar()
{
    if(this->m_ExpBarNode)
    {
        this->m_ExpBarNode->Update();
    }
}

void UILayer::RemoveChild(CCObject* object)
{
    CCNode* node = static_cast<CCNode*>(object);
    this->removeChild(node);
}