//
//  UILayer.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 3/6/13.
//
//

#include "Headers.pch"

UILayer::UILayer() : m_StageType(flownet::StageType_NONE), m_SpellQuickSlotNode(nullptr), m_StashNode(nullptr), m_InventoryNode(nullptr), m_SelectedSpellType(SpellType_NONE), m_SpellDestinationPoint()
{
    
}

UILayer::~UILayer()
{
    if(this->m_SpellQuickSlotNode)
    {
        this->m_SpellQuickSlotNode->release();
        this->m_SpellQuickSlotNode = nullptr;
    }
    if(this->m_StashNode)
    {
        this->m_StashNode->release();
        this->m_StashNode = nullptr;
    }
    if(this->m_InventoryNode)
    {
        this->m_InventoryNode->release();
        this->m_InventoryNode = nullptr;
    }
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
            this->InitializeStash();
            this->InitializeMenuBar();
            break;
    }
    
    
    CCMenuItemFont* changeCloth = CCMenuItemFont::create("change", this, menu_selector(UILayer::ChangeRobe));
    CCMenu* menu = CCMenu::create(changeCloth, NULL);
    
    this->addChild(menu);
    
    this->setTouchEnabled(true);
    
    scheduleUpdate();
    
    return true;
}

void UILayer::ChangeRobe()
{
    CCLOG("change robe");
    BaseScene* scene = static_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
    PlayerNode* player = scene->GetActorLayer()->FindPlayerNode(GameClient::Instance().GetMyActorID());
    player->ChangeRobe(ItemType_None);
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
    
    if(this->TouchProcessSpellBegan(touchLocation)) return;
}

void UILayer::ccTouchesEnded(CCSet *touches, CCEvent *event)
{
    CCTouch* touch = (CCTouch*)( touches->anyObject() );
    CCPoint touchLocation = touch->getLocation();
    
    if(this->TouchProcessSpellEnded(touchLocation)) return;
    if(this->TouchProcessItem(touchLocation)) return;
    if(this->TouchProcessMove(touchLocation)) return;
}

void UILayer::InitializeSpellQuickSlot()
{
    this->m_SpellQuickSlotNode = SpellQuickSlotNode::create();
    this->m_SpellQuickSlotNode->setPosition(ccp(SpellQuickSlotNode::PositionX, SpellQuickSlotNode::PositionY));
    
    this->addChild(this->m_SpellQuickSlotNode);
}

void UILayer::InitializeInventory()
{
    this->m_InventoryNode = InventoryNode::create();
    this->m_InventoryNode->retain();
    this->m_InventoryNode->setPosition(ccp(InventoryNode::PositionX, InventoryNode::PositionY));
    
    this->addChild(this->m_InventoryNode);
}

void UILayer::InitializeStash()
{
    this->m_StashNode = StashNode::create();
    this->m_StashNode->retain();
    this->m_StashNode->setPosition(ccp(StashNode::PositionX, StashNode::PositionY));
    
    this->addChild(this->m_StashNode);
}

void UILayer::InitializeMenuBar()
{
    this->m_MenuBarNode = MenuBarNode::create();
    this->m_MenuBarNode->retain();
    this->m_MenuBarNode->setPosition(ccp(MenuBarNode::PositionX, MenuBarNode::PositionY));
    
    this->addChild(this->m_MenuBarNode);
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
    
    touchLocation = actorLayer->convertToNodeSpace(touchLocation);

    ClientPlayer* player = static_cast<ClientPlayer*>(GameClient::Instance().GetClientStage()->FindPlayer(GameClient::Instance().GetMyActorID()));

    if(this->m_SelectedSpellType == SpellType_NONE) return false;
    if(!(player->IsStateIdle() || player->IsStateMoving())) return false;

    player->ChangeToCastingState();
    this->m_SpellDestinationPoint = touchLocation;
    GameClient::Instance().GetClientObject().SendCSRequestBeginCast(GameClient::Instance().GetClientStage()->GetStageID(), player->GetActorID(), this->m_SelectedSpellType, PointConverter::Convert(touchLocation));

    return true;
}

bool UILayer::TouchProcessSpellEnded(CCPoint touchLocation)
{
    ClientPlayer* player = static_cast<ClientPlayer*>(GameClient::Instance().GetClientStage()->FindPlayer(GameClient::Instance().GetMyActorID()));
    
    if(this->m_SelectedSpellType == SpellType_NONE) return false;
    if(!player->IsStateCasting()) return false;

    player->ChangeToIdleState();
    GameClient::Instance().GetClientObject().SendCSRequestEndCast(GameClient::Instance().GetClientStage()->GetStageID(), player->GetActorID(), this->m_SelectedSpellType, PointConverter::Convert(this->m_SpellDestinationPoint));
    this->m_SelectedSpellType = SpellType_NONE;
    this->m_SpellDestinationPoint = CCPointZero;
    
    return true;
}

bool UILayer::TouchProcessItem(CCPoint touchLocation)
{
    BaseScene* scene = static_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
    ActorLayer* actorLayer = scene->GetActorLayer();
    if(!actorLayer) return false;
    
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

    GameClient::Instance().GetClientObject().SendCSRequestMoveActor(GameClient::Instance().GetClientStage()->GetStageID(), myActorID, PointConverter::Convert(node->getPosition()), PointConverter::Convert(touchLocation));

    
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