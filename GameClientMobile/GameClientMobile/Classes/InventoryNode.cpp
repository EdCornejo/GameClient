//
//  InventoryNode.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 4/20/13.
//
//

#include "Headers.pch"

InventoryNode::InventoryNode(): m_IsOpen(false), m_CurrentTab(), m_SlideButton(nullptr), m_Body(nullptr), m_GoldLabel(nullptr), m_DescriptionLabel(nullptr), m_EquipmentButton(nullptr), m_ConsumeButton(nullptr), m_MaterialButton(nullptr), m_ItemSlotNodeList(), m_TrackingItemSlotNode(nullptr)
{

}

InventoryNode::~InventoryNode()
{
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
}

bool InventoryNode::init()
{
    this->m_SlideButton = CCMenuItemImage::create("ui/slide_button.png", "ui/slide_button.png", this, menu_selector(InventoryNode::Slide));
    this->m_SlideButton->retain();
    this->m_SlideButton->setAnchorPoint(CCPointLowerLeft);
    CCMenu* menu = CCMenu::create(this->m_SlideButton, NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu);
    
    this->m_Body = CCSprite::create("ui/inventory/body.png");
    this->m_Body->retain();
    this->m_Body->setAnchorPoint(CCPointLowerRight);
    
    // Add tab buttons to body
    this->m_EquipmentButton = CCMenuItemToggle::createWithTarget(this,
                menu_selector(InventoryNode::OnEquipmentButtonClicked),
                CCMenuItemImage::create("ui/inventory/tab_button_normal.png", "ui/inventory/tab_button_normal.png"),
                CCMenuItemImage::create("ui/inventory/tab_button_active.png", "ui/inventory/tab_button_active.png"),
                NULL);
    this->m_EquipmentButton->retain();
    this->m_EquipmentButton->setAnchorPoint(CCPointLowerLeft);
    this->m_ConsumeButton = CCMenuItemToggle::createWithTarget(this,
                menu_selector(InventoryNode::OnConsumeButtonClicked),
                CCMenuItemImage::create("ui/inventory/tab_button_normal.png", "ui/inventory/tab_button_normal.png"),
                CCMenuItemImage::create("ui/inventory/tab_button_active.png", "ui/inventory/tab_button_active.png"),
                NULL);
    this->m_ConsumeButton->retain();
    this->m_ConsumeButton->setAnchorPoint(CCPointLowerLeft);
    this->m_MaterialButton = CCMenuItemToggle::createWithTarget(this,
                menu_selector(InventoryNode::OnMaterialButtonClicked),
                CCMenuItemImage::create("ui/inventory/tab_button_normal.png", "ui/inventory/tab_button_normal.png"),
                CCMenuItemImage::create("ui/inventory/tab_button_active.png", "ui/inventory/tab_button_active.png"),
                NULL);
    this->m_MaterialButton->retain();
    this->m_MaterialButton->setAnchorPoint(CCPointLowerLeft);
    
    CCRect bodyRect = this->m_Body->getTextureRect();
    
    CCMenu* tabMenu = CCMenu::create(this->m_EquipmentButton, this->m_ConsumeButton, this->m_MaterialButton, NULL);
    tabMenu->alignItemsHorizontallyWithPadding(0);
    tabMenu->setPosition(ccp(bodyRect.size.width / 2, bodyRect.size.height));
    
    // NOTE : default tab is consume tab
    this->m_CurrentTab = "consume";
    this->m_ConsumeButton->setSelectedIndex(1);

    this->m_Body->addChild(tabMenu);
    // end of Add tab buttons to body
    
    // add gold label to body
    this->m_GoldLabel = CCLabelTTF::create("3020", "", 10);
    this->m_GoldLabel->retain();
    this->m_GoldLabel->setAnchorPoint(CCPointUpperLeft);
    this->m_GoldLabel->setHorizontalAlignment(kCCTextAlignmentRight);
    this->m_GoldLabel->setPosition(ccp(GoldLabelMarginX, bodyRect.size.height + GoldLabelMarginY));
    this->m_Body->addChild(this->m_GoldLabel);
    // end of add gold label to body
    
    // add description label to body
    // CCSprite* descriptionBackground = CCSprite::create("ui/inventory/description_background.png");
    // descriptionBackground->setAnchorPoint(CCPointLowerLeft);
    // descriptionBackground->setPosition(ccp(DescriptionBackgroundMarginX, DescriptionBackgroundMarginY));

    this->m_DescriptionLabel = CCLabelTTF::create("description", "", 10); // TO DO : multi line?
    this->m_DescriptionLabel->retain();
    this->m_DescriptionLabel->setAnchorPoint(CCPointLowerLeft);
    this->m_DescriptionLabel->setPosition(ccp(DescriptionLabelMarginX, DescriptionLabelMarginY));
    
    // this->m_DescriptionLabel->setAnchorPoint(CCPointUpperLeft);
    // this->m_DescriptionLabel->setPosition(ccp(DescriptionLabelMarginX, DescriptionLabelMarginY));
    // descriptionBackground->addChild(this->m_DescriptionLabel);
    this->m_Body->addChild(this->m_DescriptionLabel);

    // this->m_Body->addChild(descriptionBackground);
    // end of add description label to body
    
    // add ItemSlotNodes to body
    
    ItemSlotNode* slotForSize = ItemSlotNode::create(ItemType_None, ItemID_None, InventorySlot_None);
    CCRect slotRect = GetRect(slotForSize);
    
    const float ItemSlotSizeX = slotRect.size.width;
    const float ItemSlotSizeY = slotRect.size.height;
    
    delete slotForSize;
    
    const int perRow = 4;
    for(int i = 0; i < InventorySlot_Max; i++)
    {
        ItemSlotNode* slotNode = ItemSlotNode::create(ItemType_None, ItemID_None, static_cast<InventorySlot>(i));
        slotNode->setAnchorPoint(CCPointUpperLeft);
        CCPoint slotPosition;
        slotPosition.x = ItemSlotPositionX + ((ItemSlotMargin + ItemSlotSizeX) * (i % perRow));
        slotPosition.y = ItemSlotPositionY - ((ItemSlotMargin + ItemSlotSizeY) * (i / perRow));
        
        slotNode->setPosition(slotPosition);
        this->m_ItemSlotNodeList.push_back(slotNode);
        this->m_Body->addChild(slotNode);
    }

    // end of add ItemSlotNodes to body
    
    this->addChild(this->m_Body);
    
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    
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
    
    ItemSlotNode* selectedItemSlot = this->FindSelectedItemSlotNode(touch->getLocation());
    
    if(selectedItemSlot)
    {
        this->m_TrackingItemSlotNode = selectedItemSlot;
        return true;
    }
    
    return false;
}

void InventoryNode::ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{
    if(this->m_TrackingItemSlotNode)
    {
        this->m_TrackingItemSlotNode->TrackTouch(touch);
    }
}

void InventoryNode::ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{
    if(this->m_TrackingItemSlotNode)
    {
        this->m_TrackingItemSlotNode->Reset();
    
        ItemSlotNode* selectedItemSlot = this->FindSelectedItemSlotNode(touch->getLocation());
        
        GameClient& client = GameClient::Instance();
        
        if(selectedItemSlot == this->m_TrackingItemSlotNode)
        {
            CCLOG("used");
            client.GetClientObject().SendCSRequestUseItem(client.GetClientStage()->GetStageID(), client.GetMyActorID(), this->m_TrackingItemSlotNode->GetItemID(), this->m_TrackingItemSlotNode->GetSlotNumber());
            // TO DO : ask for using this item
        }
        else if(selectedItemSlot)
        {
            client.GetClientObject().SendCSRequestSwapInventorySlot(client.GetClientStage()->GetStageID(), client.GetMyActorID(), this->m_TrackingItemSlotNode->GetSlotNumber(), selectedItemSlot->GetSlotNumber());
            // first request to server and change it in a response
        }
        else
        {
            CCLOG("dropped");
            client.GetClientObject().SendCSRequestDropItemToField(client.GetClientStage()->GetStageID(), client.GetMyActorID(), this->m_TrackingItemSlotNode->GetSlotNumber(), PointConverter::Convert(touch->getLocation()));
            
            // TO DO : ask for dropping this item
        }
        
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

void InventoryNode::UseItem(flownet::ItemID itemID, flownet::InventorySlot inventorySlot)
{
    if( this->m_ItemSlotNodeList.size() <= inventorySlot )
    {
        ASSERT_DEBUG(this->m_ItemSlotNodeList.size() > inventorySlot );
        return;
    }
    if( this->m_ItemSlotNodeList[inventorySlot]->GetItemID() != itemID )
    {
        ASSERT_DEBUG(this->m_ItemSlotNodeList[inventorySlot]->GetItemID() != itemID);
        return;
    }
    
    this->m_ItemSlotNodeList[inventorySlot]->Empty();
//    std::for_each(this->m_ItemSlotNodeList.begin(), this->m_ItemSlotNodeList.end(), [&itemID](ItemSlotNode* node){
//        if(node->GetItemID() == itemID)
//        {
//            node->Empty();
//        }
//    });
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

void InventoryNode::Slide()
{
    if(this->m_IsOpen)
    {
        CCLOG("slide in");

        CCPoint slidePosition = CCPoint(PositionX, PositionY);
        CCMoveTo* moveIn = CCMoveTo::create(0.2, slidePosition);
        moveIn->setTag(ActionType_UI);
        this->stopActionByTag(ActionType_UI);
        this->runAction(moveIn);
    }
    else{
        CCLOG("slide out");
        CCPoint slidePosition = CCPoint(PositionX + this->m_Body->getTextureRect().size.width, PositionY);
        CCMoveTo* moveOut = CCMoveTo::create(0.2, slidePosition);
        moveOut->setTag(ActionType_UI);
        this->stopActionByTag(ActionType_UI);
        this->runAction(moveOut);
        // TO DO : slide out
    }
    
    this->m_IsOpen = !this->m_IsOpen;
}

void InventoryNode::OnEquipmentButtonClicked(CCObject* sender)
{
    CCLOG("equip tab clicked");
    if(this->m_CurrentTab.compare("equipment") == 0)
    {
        this->m_EquipmentButton->setSelectedIndex(1);
        return;
    }

    // TO DO : unselect tabs
    this->m_ConsumeButton->setSelectedIndex(0);
    this->m_MaterialButton->setSelectedIndex(0);
    this->m_CurrentTab = "equipment";
    
    
    // TO DO : change slots to equipment inven items

}

void InventoryNode::OnConsumeButtonClicked(CCObject* sender)
{
    CCLOG("consume tab clicked");
    if(this->m_CurrentTab.compare("consume") == 0)
    {
        this->m_ConsumeButton->setSelectedIndex(1);
        return;
    }
    
    this->m_EquipmentButton->setSelectedIndex(0);
    this->m_MaterialButton->setSelectedIndex(0);
    this->m_CurrentTab = "consume";
    // TO DO : change slot to consume inven items
}

void InventoryNode::OnMaterialButtonClicked(CCObject* sender)
{
    CCLOG("material tab clicked");
    if(this->m_CurrentTab.compare("material") == 0)
    {
        this->m_MaterialButton->setSelectedIndex(1);
        return;
    }
    
    this->m_EquipmentButton->setSelectedIndex(0);
    this->m_ConsumeButton->setSelectedIndex(0);
    this->m_CurrentTab = "material";
    // TO DO : change slots to material inven items
}

void InventoryNode::SwapInventorySlot(ItemSlotNode *lhs, ItemSlotNode *rhs)
{
    ItemType itemType = rhs->GetItemType();
    ItemID itemID = rhs->GetItemID();
    
    rhs->ChangeItemTypeAndItemID(lhs->GetItemType(), lhs->GetItemID());
    lhs->ChangeItemTypeAndItemID(itemType, itemID);
}
