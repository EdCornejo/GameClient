    
//
//  InventoryNode.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 4/20/13.
//
//

#include "Headers.pch"

InventoryNode::InventoryNode(): m_IsOpen(false), m_MovingPart(nullptr), m_PinnedPart(nullptr), m_CurrentItemGroup(ItemGroup_None), m_ScrollButton(nullptr), m_SlideButton(nullptr), m_Body(nullptr), m_GoldLabel(nullptr), m_DescriptionLabel(nullptr), m_EquipmentButton(nullptr), m_ConsumeButton(nullptr), m_MaterialButton(nullptr), m_TabMenu(nullptr), m_ItemSlotNodeList(), m_TrackingItemSlotNode(nullptr), m_HighlightedItemSlotNode(nullptr), m_IsTrackingItemSlotMoving(false), m_TrackingItemSlotTouchedTime(0)
{

}

InventoryNode::~InventoryNode()
{
    if(this->m_MovingPart)
    {
        this->m_MovingPart->release();
        this->m_MovingPart = nullptr;
    }
    if(this->m_PinnedPart)
    {
        this->m_PinnedPart->release();
        this->m_PinnedPart = nullptr;
    }
    if(this->m_SlideButton)
    {
        this->m_SlideButton->release();
        this->m_SlideButton = nullptr;
    }
    if(this->m_Body)
    {
        this->m_Body->release();
        this->m_Body = nullptr;
    }
    if(this->m_EquipmentButton)
    {
        this->m_EquipmentButton->release();
        this->m_EquipmentButton = nullptr;
    }
    if(this->m_ConsumeButton)
    {
        this->m_ConsumeButton->release();
        this->m_ConsumeButton = nullptr;
    }
    if(this->m_MaterialButton)
    {
        this->m_MaterialButton->release();
        this->m_MaterialButton = nullptr;
    }
    if(this->m_TabMenu)
    {
        this->m_TabMenu->release();
        this->m_TabMenu = nullptr;
    }
    if(this->m_DescriptionLabel)
    {
        this->m_DescriptionLabel->release();
        this->m_DescriptionLabel = nullptr;
    }
    
    std::for_each(this->m_ItemSlotNodeList.begin(), this->m_ItemSlotNodeList.end(), [](ItemSlotNode* node){
        node->release();
    });
    this->m_ItemSlotNodeList.clear();
}

bool InventoryNode::init()
{
    // 좌측에도 스크롤되는 버튼이 하나가 필요
    // change SlideButton to TagButton
    
    this->m_MovingPart = CCNode::create();
    this->m_MovingPart->retain();
    
    this->m_PinnedPart = CCNode::create();
    this->m_PinnedPart->retain();
    
    this->m_SlideButton = CCSprite::create("ui/inventory/inventory_tag_button.png");
    this->m_SlideButton->retain();
    this->m_SlideButton->setAnchorPoint(CCPointLowerLeft);
    this->m_SlideButton->setPosition(ccp(-30, 50));
    this->m_MovingPart->addChild(this->m_SlideButton);
    
    this->m_Body = CCSprite::create("ui/inventory/inventory_background.png");
    this->m_Body->retain();
    this->m_Body->setAnchorPoint(CCPointLowerLeft);
    
    CCRect bodyRect = this->m_Body->getTextureRect();
    
    CCNode* bodyNode = CCNode::create();
    bodyNode->setPosition(-bodyRect.size.width, 0);
    
    // Add tab buttons to body
    this->m_EquipmentButton = CCMenuItemToggle::createWithTarget(this,
                menu_selector(InventoryNode::OnEquipmentButtonClicked),
                CCMenuItemImage::create("ui/inventory/equipment_button_normal.png", "ui/inventory/equipment_button_normal.png"),
                CCMenuItemImage::create("ui/inventory/equipment_button_active.png", "ui/inventory/equipment_button_active.png"),
                NULL);
    this->m_EquipmentButton->retain();
    this->m_ConsumeButton = CCMenuItemToggle::createWithTarget(this,
                menu_selector(InventoryNode::OnConsumeButtonClicked),
                CCMenuItemImage::create("ui/inventory/consume_button_normal.png", "ui/inventory/consume_button_normal.png"),
                CCMenuItemImage::create("ui/inventory/consume_button_active.png", "ui/inventory/consume_button_active.png"),
                NULL);
    this->m_ConsumeButton->retain();
    this->m_MaterialButton = CCMenuItemToggle::createWithTarget(this,
                menu_selector(InventoryNode::OnMaterialButtonClicked),
                CCMenuItemImage::create("ui/inventory/material_button_normal.png", "ui/inventory/material_button_normal.png"),
                CCMenuItemImage::create("ui/inventory/material_button_active.png", "ui/inventory/material_button_active.png"),
                NULL);
    this->m_MaterialButton->retain();
    

    
    this->m_TabMenu = CCMenu::create(this->m_EquipmentButton, this->m_ConsumeButton, this->m_MaterialButton, NULL);
    this->m_TabMenu->retain();
    this->m_TabMenu->alignItemsHorizontallyWithPadding(0);
    this->m_TabMenu->setPosition(ccp(bodyRect.size.width / 2, bodyRect.size.height));
    this->m_TabMenu->setVisible(false);
    
    // NOTE : default tab is consume tab
    this->m_CurrentItemGroup = ItemGroup_Consume;
    this->m_ConsumeButton->setSelectedIndex(1);

    bodyNode->addChild(this->m_TabMenu);
    bodyNode->addChild(this->m_Body);
    // for changing order of render
    
    
    this->m_DescriptionLabel = CCLabelTTF::create("", "thonburi", 7); // TO DO : multi line?
    this->m_DescriptionLabel->retain();
    this->m_DescriptionLabel->setAnchorPoint(CCPointUpperLeft);
    this->m_DescriptionLabel->setPosition(ccp(30, bodyRect.size.height - 24));
    
    this->m_Body->addChild(this->m_DescriptionLabel);

    // add ItemSlotNodes to body
    ItemSlotNode* slotForSize = ItemSlotNode::create(ItemType_None, ItemID_None, InventorySlot_None);
    CCRect slotRect = GetRect(slotForSize);
    
    const float ItemSlotSizeX = slotRect.size.width;
    const float ItemSlotSizeY = slotRect.size.height;
    
    delete slotForSize;
    
    // NOTE : first initialize inventory slot with empty items
    const int perRow = 4;
    for(int i = 0; i < InventorySlot_Max; i++)
    {
        ItemSlotNode* slotNode = ItemSlotNode::create(ItemType_None, ItemID_None, static_cast<InventorySlot>(i));
        slotNode->setAnchorPoint(CCPointUpperLeft);
        CCPoint slotPosition;
        slotPosition.x = ItemSlotPositionX + ((ItemSlotMarginX + ItemSlotSizeX) * (i % perRow));
        slotPosition.y = ItemSlotPositionY - ((ItemSlotMarginY + ItemSlotSizeY) * (i / perRow));
        
        slotNode->setPosition(slotPosition);
        this->m_ItemSlotNodeList.push_back(slotNode);
        slotNode->retain();
        this->m_Body->addChild(slotNode);
    }
    // end of add ItemSlotNodes to body
    
    this->m_MovingPart->addChild(bodyNode);
    
    this->m_ScrollButton = CCSprite::create("ui/inventory/inventory_scroll_button.png");
    this->m_ScrollButton->setAnchorPoint(CCPointLowerLeft);
    this->m_ScrollButton->setPosition(ccp(0, -5));
    this->m_PinnedPart->addChild(this->m_ScrollButton);
    
    this->addChild(m_MovingPart);
    this->addChild(m_PinnedPart);

    // Initialize with actor's inventory
    this->Update();
    
    return true;
}

InventoryNode* InventoryNode::create()
{
    InventoryNode* newInventoryNode = new InventoryNode();
    if(newInventoryNode && newInventoryNode->init())
    {
        newInventoryNode->autorelease();
        return newInventoryNode;
    }
    else
    {
        delete newInventoryNode;
        newInventoryNode = nullptr;
        return nullptr;
    }
}


bool InventoryNode::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    CC_UNUSED_PARAM(event);
    
    CCRect slideButtonRect = GetRectForAnchorLowerLeft(this->m_SlideButton);
    slideButtonRect.origin = this->m_MovingPart->convertToWorldSpace(slideButtonRect.origin);
    CCRect scrollButtonRect = GetRectForAnchorLowerLeft(this->m_ScrollButton);
    scrollButtonRect.origin = this->m_PinnedPart->convertToWorldSpace(scrollButtonRect.origin);
    
    if(slideButtonRect.containsPoint(touch->getLocation()))
    {
        this->Slide();
        return true;
    }
    else if(scrollButtonRect.containsPoint(touch->getLocation()))
    {
        this->OnScrollButtonClicked();
        return true;
    }
    
    ItemSlotNode* selectedItemSlot = this->FindSelectedItemSlotNode(touch->getLocation());
    if(this->m_HighlightedItemSlotNode && selectedItemSlot != this->m_HighlightedItemSlotNode)
    {
        this->m_HighlightedItemSlotNode->ResetHighlight();
    }
    
    if(selectedItemSlot)
    {
        this->m_TrackingItemSlotTouchedTime = GameClient::Instance().GetClientTimer().Check();
        this->m_TrackingItemSlotNode = selectedItemSlot;
        return true;
    }
    CCRect bodyRect = GetRectForAnchorLowerLeft(this->m_Body);
    bodyRect.origin = this->m_Body->getParent()->convertToWorldSpace(bodyRect.origin);
    if(bodyRect.containsPoint(touch->getLocation()))
    {
        return true;
    }
    
    return false;
}

void InventoryNode::ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{
    // maybe time checking will be checked in a update logic
    ServerTime currentTime = GameClient::Instance().GetClientTimer().Check();
    
    if(this->m_TrackingItemSlotNode && currentTime - this->m_TrackingItemSlotTouchedTime > ServerTime(TimeToBeganMove))
    {
        this->m_IsTrackingItemSlotMoving = true;
        this->m_TrackingItemSlotNode->TrackTouch(touch);
    }
}

void InventoryNode::ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{
    // if there is a tracking node
    if(this->m_TrackingItemSlotNode)
    {
        ItemSlotNode* selectedItemSlot = this->FindSelectedItemSlotNode(touch->getLocation());
        
        GameClient& client = GameClient::Instance();
        
        CCRect bodyRect = GetRectForAnchorLowerLeft(this->m_Body);
        
        // if the selected item was tracking node itself
        if(selectedItemSlot == this->m_TrackingItemSlotNode)
        {
            // if the selected item was highlighted node use it
            if(selectedItemSlot == this->m_HighlightedItemSlotNode)
            {
                CCLOG("used");
                this->m_HighlightedItemSlotNode->ResetHighlight();
                this->m_HighlightedItemSlotNode = nullptr;
                this->m_DescriptionLabel->setString("");
                if(this->m_CurrentItemGroup == ItemGroup_Consume)
                {
                    client.GetClientObject().SendCSRequestUseItem(client.GetClientStage()->GetStageID(), client.GetMyActorID(), this->m_TrackingItemSlotNode->GetItemID());
                }
                else if(this->m_CurrentItemGroup == ItemGroup_Equipment)
                {
                    client.GetClientObject().SendCSRequestEquipItem(client.GetClientStage()->GetStageID(), client.GetMyActorID(), this->m_TrackingItemSlotNode->GetItemID());
                }
            }
            // if the selected item was not highlighted highlight it
            else
            {
                // if ItemType is not None highlight
                if(this->m_TrackingItemSlotNode->GetItemType() != ItemType_None)
                {
                    this->m_HighlightedItemSlotNode = this->m_TrackingItemSlotNode;
                    this->m_HighlightedItemSlotNode->Highlight();
                    const ItemData* itemData = ItemDataDictionary::Instance().GetItemData(this->m_HighlightedItemSlotNode->GetItemType());
                    this->m_DescriptionLabel->setString(itemData->GetItemDescription().c_str());
                }
            }
        }
        else if(selectedItemSlot && this->m_IsTrackingItemSlotMoving )
        {
            client.GetClientObject().SendCSRequestSwapInventorySlot(client.GetClientStage()->GetStageID(), client.GetMyActorID(), this->m_CurrentItemGroup, this->m_TrackingItemSlotNode->GetInventorySlot(), selectedItemSlot->GetInventorySlot());
            // TO DO : first request to server and change it in a response
        }
        else if(!bodyRect.containsPoint(touch->getLocation()))// drop boundary check modify
        {
            BaseScene* scene = static_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
            ASSERT_DEBUG(scene);
            
            ActorLayer* actorLayer = scene->GetActorLayer();
            ASSERT_DEBUG(actorLayer);
            
            CCPoint itemDropPosition = actorLayer->convertToNodeSpace(touch->getLocation());
            client.GetClientObject().SendCSRequestDropItemToField(client.GetClientStage()->GetStageID(), client.GetMyActorID(), this->m_TrackingItemSlotNode->GetItemID(), PointConverter::Convert(itemDropPosition));
            // TO DO : ask for dropping this item
        }
        else
        {
            // do nothing 
        }

        this->m_IsTrackingItemSlotMoving = false;
        this->m_TrackingItemSlotNode->ResetMoving(); // Reset to ResetMoving, ResetHighlight
        this->m_TrackingItemSlotNode = nullptr;
    }
}

ItemSlotNode* InventoryNode::FindSelectedItemSlotNode(cocos2d::CCPoint touchLocation)
{
    ItemSlotNode* selectedItemSlot = nullptr;
        // NOTE : check for the touch is over the another slot
    for(int i = 0; i < this->m_ItemSlotNodeList.size(); ++i){
        ItemSlotNode* itemSlot = this->m_ItemSlotNodeList[i];
        CCRect itemSlotRect = GetRect(itemSlot);
        itemSlotRect.origin = this->m_Body->convertToWorldSpace(itemSlotRect.origin);
        if(itemSlotRect.containsPoint(touchLocation))
        {
            selectedItemSlot = itemSlot;
            break;
        }
    }
    
    return selectedItemSlot;
}

void InventoryNode::Update()
{
    Actor* actor = GameClient::Instance().GetClientStage()->FindPlayer(GameClient::Instance().GetMyActorID());
    if(actor)
    {
        for(int i = 0; i < InventorySlot_Max; i++)
        {
            const Item* item = actor->GetInventory().FindItem(this->m_CurrentItemGroup, static_cast<InventorySlot>(i));
            if(item)
            {
                this->m_ItemSlotNodeList[i]->ChangeItemTypeAndItemID(item->GetItemType(), item->GetItemID());
            }
            else
            {
                this->m_ItemSlotNodeList[i]->ChangeItemTypeAndItemID(ItemType_None, ItemID_None);
            }
        }
    }
}

void InventoryNode::SwapInventorySlot(flownet::InventorySlot source, flownet::InventorySlot destination)
{
    if(!(source > InventorySlot_None && source < InventorySlot_Max))
    {
        // index over
        ASSERT_DEBUG(false);
    }
    if(!(destination > InventorySlot_None && destination < InventorySlot_Max))
    {
        // index over
        ASSERT_DEBUG(false);
    }
    
    ItemSlotNode* sourceSlot = this->m_ItemSlotNodeList[source];
    ItemSlotNode* destinationSlot = this->m_ItemSlotNodeList[destination];
    
    ASSERT_DEBUG(sourceSlot != nullptr);
    ASSERT_DEBUG(destinationSlot != nullptr);
    
    this->SwapInventorySlot(sourceSlot, destinationSlot);
}

void InventoryNode::OnScrollButtonClicked()
{
    if(!this->m_IsOpen)
    {
        this->Slide();
    }
}

void InventoryNode::Slide()
{
    if(this->m_IsOpen)
    {
        CCLOG("slide in");

        CCPoint slidePosition = CCPoint(PositionX, PositionY);
        CCMoveTo* moveIn = CCMoveTo::create(0.2, slidePosition);
        moveIn->setTag(ActionType_UI);
        this->m_MovingPart->stopActionByTag(ActionType_UI);
        this->m_MovingPart->runAction(moveIn);
        this->stopActionByTag(ActionType_UI);
        this->m_TabMenu->setVisible(false);
    }
    else{
        CCLOG("slide out");
        CCPoint slidePosition = CCPoint(PositionX + this->m_Body->getTextureRect().size.width, PositionY);
        CCMoveTo* moveOut = CCMoveTo::create(0.2, slidePosition);
        moveOut->setTag(ActionType_UI);
        this->m_MovingPart->stopActionByTag(ActionType_UI);
        this->m_MovingPart->runAction(moveOut);
        
        CCDelayTime* delay = CCDelayTime::create(0.2);
        CCCallFunc* show = CCCallFunc::create(this, callfunc_selector(InventoryNode::ShowTabMenu));
        CCSequence* sequence = CCSequence::create(delay, show, NULL);
        sequence->setTag(ActionType_UI);
        this->stopActionByTag(ActionType_UI);
        this->runAction(sequence);
    }
    
    this->m_IsOpen = !this->m_IsOpen;
}

void InventoryNode::ShowTabMenu()
{
    this->m_TabMenu->setVisible(true);
}

void InventoryNode::OnEquipmentButtonClicked(CCObject* sender)
{
    CCLOG("equip tab clicked");
    if(this->m_CurrentItemGroup == ItemGroup_Equipment)
    {
        this->m_EquipmentButton->setSelectedIndex(1);
        return;
    }

    // TO DO : unselect tabs
    this->m_ConsumeButton->setSelectedIndex(0);
    this->m_MaterialButton->setSelectedIndex(0);
    this->m_CurrentItemGroup = ItemGroup_Equipment;
    this->m_TrackingItemSlotNode = nullptr;
    this->m_HighlightedItemSlotNode = nullptr;
    this->m_DescriptionLabel->setString("");
    
    this->Update();
}

void InventoryNode::OnConsumeButtonClicked(CCObject* sender)
{
    CCLOG("consume tab clicked");
    if(this->m_CurrentItemGroup == ItemGroup_Consume)
    {
        this->m_ConsumeButton->setSelectedIndex(1);
        return;
    }
    
    this->m_EquipmentButton->setSelectedIndex(0);
    this->m_MaterialButton->setSelectedIndex(0);
    this->m_CurrentItemGroup = ItemGroup_Consume;
    this->m_TrackingItemSlotNode = nullptr;
    this->m_HighlightedItemSlotNode = nullptr;
    this->m_DescriptionLabel->setString("");
    
    this->Update();
}

void InventoryNode::OnMaterialButtonClicked(CCObject* sender)
{
    CCLOG("material tab clicked");
    if(this->m_CurrentItemGroup == ItemGroup_Material)
    {
        this->m_MaterialButton->setSelectedIndex(1);
        return;
    }
    
    this->m_EquipmentButton->setSelectedIndex(0);
    this->m_ConsumeButton->setSelectedIndex(0);
    this->m_CurrentItemGroup = ItemGroup_Material;
    this->m_TrackingItemSlotNode = nullptr;
    this->m_HighlightedItemSlotNode = nullptr;    
    this->m_DescriptionLabel->setString("");
    
    this->Update();
}

void InventoryNode::SwapInventorySlot(ItemSlotNode *lhs, ItemSlotNode *rhs)
{
    ItemType itemType = rhs->GetItemType();
    ItemID itemID = rhs->GetItemID();
    
    rhs->ChangeItemTypeAndItemID(lhs->GetItemType(), lhs->GetItemID());
    lhs->ChangeItemTypeAndItemID(itemType, itemID);
}

