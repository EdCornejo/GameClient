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
        delete this->m_SpellQuickSlotNode; // TO DO : refactor spellquickslot to auto release object
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
            this->InitializeMenuBar();
            break;
    }
    
    this->setTouchEnabled(true);
    
    return true;
}

UILayer* UILayer::create(StageType stageType)
{
    UILayer* newUILayer = new UILayer();
    newUILayer->m_StageType = stageType;
    
    if(newUILayer && newUILayer->init())
    {
        newUILayer->m_StageType = stageType;
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

}

// NOTE : maybe will be deprecated
void UILayer::ccTouchesBegan(cocos2d::CCSet *touches, cocos2d::CCEvent *event)
{
    CCTouch* touch = (CCTouch*)(touches->anyObject());
    CCPoint destination = touch->getLocation();
    
    ClientPlayer* player = static_cast<ClientPlayer*>(GameClient::Instance().GetClientStage()->FindPlayer(GameClient::Instance().GetMyActorID()));

    BaseScene* scene = static_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
    ActorLayer* actorLayer = scene->GetActorLayer();
    ItemNode* itemNode = actorLayer->FindSelectedItemNode(touch->getLocation());
    if(itemNode)
    {
        GameClient::Instance().GetClientObject().SendCSRequestPickUpItemFromField(GameClient::Instance().GetClientStage()->GetStageID(), GameClient::Instance().GetMyActorID(), itemNode->GetItemID());
        return;
    }

    if(this->m_SelectedSpellType != SpellType_NONE && player->IsStateIdle())
    {
        // NOTE : do spell casting
        player->ChangeToCastingState();
        
        this->m_SpellDestinationPoint = destination;
        GameClient::Instance().GetClientObject().SendCSRequestBeginCast(GameClient::Instance().GetClientStage()->GetStageID(), player->GetActorID(), this->m_SelectedSpellType, PointConverter::Convert(destination));
        return;
    }
}

void UILayer::ccTouchesEnded(CCSet *touches, CCEvent *event)
{
    CCTouch* touch = (CCTouch*)( touches->anyObject() );
    CCPoint destinationPoint = touch->getLocation();
    
    ClientPlayer* player = static_cast<ClientPlayer*>(GameClient::Instance().GetClientStage()->FindPlayer(GameClient::Instance().GetMyActorID()));
    
    if(this->m_SelectedSpellType != SpellType_NONE && player->IsStateCasting())
    {
        // NOTE : do spell firing
        player->ChangeToIdleState();
        
        GameClient::Instance().GetClientObject().SendCSRequestEndCast(GameClient::Instance().GetClientStage()->GetStageID(), player->GetActorID(), this->m_SelectedSpellType, PointConverter::Convert(destinationPoint));
        this->m_SelectedSpellType = SpellType_NONE;
        this->m_SpellDestinationPoint = CCPointZero;
    }
    else
    {
        // NOTE : moving player is the default behavior

        // NOTE : boundary check
        ClientStage* stage = GameClient::Instance().GetClientStage();
        if(destinationPoint.x < PointConverter::ModiXToViewPointX(0)) // NOTE : left boundary check
        {
            destinationPoint.x = PointConverter::ModiXToViewPointX(0);
        }
        if(destinationPoint.x > PointConverter::ModiXToViewPointX(stage->GetWidth())) // NOTE : right boundary check
        {
            destinationPoint.x = PointConverter::ModiXToViewPointX(stage->GetWidth());
        }
        if(destinationPoint.y < PointConverter::ModiYToViewPointY(0)) // NOTE : lower boundary check
        {
            destinationPoint.y = PointConverter::ModiYToViewPointY(0);
        }
        if(destinationPoint.y > PointConverter::ModiYToViewPointY(stage->GetHeight())) // NOTE : upper boundary check
        {
            destinationPoint.y = PointConverter::ModiYToViewPointY(stage->GetHeight());
        }
        
        GameClientObject& clientObject = GameClient::Instance().GetClientObject();
        ActorID myActorID = GameClient::Instance().GetMyActorID();
        
        BaseScene* scene = static_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
        ActorLayer* actorLayer = scene->GetActorLayer();
        
        PlayerNode* node = actorLayer->FindPlayerNode(myActorID);
        
        ASSERT_DEBUG(node != nullptr);
        
        clientObject.SendCSRequestMoveActor(GameClient::Instance().GetClientStage()->GetStageID(), myActorID, PointConverter::Convert(node->getPosition()), PointConverter::Convert(destinationPoint));
    }
}


void UILayer::InitializeSpellQuickSlot()
{
    this->m_SpellQuickSlotNode = new SpellQuickSlotNode();
    this->m_SpellQuickSlotNode->Initialize();
    
    this->m_SpellQuickSlotNode->setPosition(ccp(SpellStackPositionX, SpellStackPositionY));
    
    this->addChild(this->m_SpellQuickSlotNode);
}

void UILayer::InitializeInventory()
{
    this->m_InventoryNode = InventoryNode::create();
    this->m_InventoryNode->retain();
    this->m_InventoryNode->setPosition(ccp(InventoryNode::PositionX, InventoryNode::PositionY));
    
    this->addChild(this->m_InventoryNode);
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

void UILayer::SetSelectedSpellType(flownet::SpellType spellType)
{
    this->m_SelectedSpellType = spellType;
}

void UILayer::UseItem(flownet::ItemID itemID, flownet::InventorySlot inventorySlot)
{
    if(this->m_InventoryNode)
    {
        this->m_InventoryNode->UseItem(itemID, inventorySlot);
    }
    if(this->m_StashNode)
    {
        this->m_StashNode->UseItem(itemID);
    }
}

void UILayer::SwapInventorySlot(flownet::InventorySlot sourceSlotNumber, flownet::InventorySlot destinationSlotNumber)
{
    if(this->m_InventoryNode)
    {
        this->m_InventoryNode->SwapInventorySlot(sourceSlotNumber, destinationSlotNumber);
    }
}