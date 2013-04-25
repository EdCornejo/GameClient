//
//  StashNode.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 4/23/13.
//
//

#include "Headers.pch"

ItemInfoPopupLayer::ItemInfoPopupLayer()
//: m_ItemType(ItemType_None), m_Background(nullptr), m_ItemSlotBackground(nullptr), m_ItemImage(nullptr), m_ItemNameLabel(nullptr), m_GoldValueLabel(nullptr), m_FullDescriptionLabel(nullptr), m_AbstractDescriptionLabel(nullptr),
    :m_UseItemMenu(nullptr), m_CloseMenu(nullptr)
{

}

ItemInfoPopupLayer::~ItemInfoPopupLayer()
{
//    if(this->m_Background)
//    {
//        this->m_Background->release();
//        this->m_Background = nullptr;
//    }
//    if(this->m_ItemSlotBackground)
//    {
//        this->m_ItemSlotBackground->release();
//        this->m_ItemSlotBackground = nullptr;
//    }
//    if(this->m_ItemImage)
//    {
//        this->m_ItemImage->release();
//        this->m_ItemImage = nullptr;
//    }
//    if(this->m_ItemNameLabel)
//    {
//        this->m_ItemNameLabel->release();
//        this->m_ItemNameLabel = nullptr;
//    }
//    if(this->m_GoldValueLabel)
//    {
//        this->m_GoldValueLabel->release();
//        this->m_GoldValueLabel = nullptr;
//    }
//    if(this->m_FullDescriptionLabel)
//    {
//        this->m_FullDescriptionLabel->release();
//        this->m_FullDescriptionLabel = nullptr;
//    }
    if(this->m_UseItemMenu)
    {
        this->m_UseItemMenu->release();
        this->m_UseItemMenu = nullptr;
    }
    if(this->m_CloseMenu)
    {
        this->m_CloseMenu->release();
        this->m_CloseMenu = nullptr;
    }
}
    
bool ItemInfoPopupLayer::init()
{
    if(!CCLayer::init()) return false;
    
    const ItemData* data = ItemDataDictionary::Instance().GetItemData(this->m_ItemType);

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    // layer background
    CCSprite* background = CCSprite::create("ui/stash/item_info_background.png");
    background->setPosition(ccp(winSize.width, winSize.height));
    this->addChild(background);
    
    
    const ItemData* itemData = ItemDataDictionary::Instance().GetItemData(this->m_ItemType);
    
    CCSize backgroundSize = GetRect(background).size;

    // item name
    
    CCLabelTTF* itemNameLabel = CCLabelTTF::create("Item Name", "", 12);
    //CCLabelTTF* itemNameLabel = CCLabelTTF::create(itemData->GetItemName() , "", 12);
    //itemNameLabel->setPosition(); // this position will be related to background image size
    background->addChild(itemNameLabel);
    
    // item image background & item image
    // CCPoint imagePosition = somePosition;
    CCPoint imagePosition;
    CCSprite* itemImageBackground = CCSprite::create("ui/inventory/item_slot.png");
    itemImageBackground->setAnchorPoint(CCPointUpperLeft);
    itemImageBackground->setPosition(imagePosition);
    CCSprite* itemImage = ItemImageLoader::GetItemInventoryImage(this->m_ItemType);
    CCSize imageBackgroundSize = GetRect(itemImageBackground).size;
    itemImage->setPosition(ccp(imageBackgroundSize.width / 2, imageBackgroundSize.height / 2));
    
    background->addChild(itemImageBackground);

    // abstract desc
    CCPoint absDescPosition;
    CCLabelTTF* abstractDescription = CCLabelTTF::create("abs desc", "", 12);
    abstractDescription->setPosition(absDescPosition);
    background->addChild(abstractDescription);
    
    // full desc
    CCPoint fullDescPosition;
    CCLabelTTF* fullDescription = CCLabelTTF::create("full desc", "", 12);
    fullDescription->setPosition(fullDescPosition);
    background->addChild(fullDescription);
    
    // gold value
    CCPoint goldLabelPosition;
    CCLabelTTF* goldValueLabel = CCLabelTTF::create("gold value", "", 12);
    goldValueLabel->setPosition(goldLabelPosition);
    background->addChild(goldValueLabel);
    
    // use button
    CCPoint useItemMenuPosition;
    CCMenuItemImage* useItemButton = CCMenuItemImage::create("ui/inventory/use_item_normal.png", "ui/inventory/use_item_active.png", this, menu_selector(ItemInfoPopupLayer::OnUseItemButtonClicked));
    this->m_UseItemMenu = CCMenu::create(useItemButton, NULL);
    this->m_UseItemMenu->setPosition(useItemMenuPosition);
    background->addChild(this->m_UseItemMenu);


    // close button
    CCPoint closeMenuPosition;
    CCMenuItemImage* closeButton = CCMenuItemImage::create("ui/inventory/close_normal.png", "ui/inventory/close_active.png", this, menu_selector(ItemInfoPopupLayer::OnCloseButtonClicked));
    this->m_CloseMenu = CCMenu::create(closeButton, NULL);
    this->m_CloseMenu->setPosition(closeMenuPosition);
    background->addChild(this->m_CloseMenu);
    
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    
    return true;
}

ItemInfoPopupLayer* ItemInfoPopupLayer::create(flownet::ItemType itemType, flownet::ItemID itemID)
{
    ItemInfoPopupLayer* newLayer = new ItemInfoPopupLayer();
    
    newLayer->m_ItemType = itemType;
    newLayer->m_ItemID = itemID;
    
    if(newLayer && newLayer->init())
    {
        newLayer->autorelease();
        return newLayer;
    }
    else
    {
        delete newLayer;
        newLayer = nullptr;
        return nullptr;
    }
}

void ItemInfoPopupLayer::OnUseItemButtonClicked()
{
    // send request for item use
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    
    GameClient::Instance().GetClientObject().SendCSRequestUseItem(GameClient::Instance().GetClientStage()->GetStageID(), GameClient::Instance().GetMyActorID(), this->m_ItemID, InventorySlot_None);
}

void ItemInfoPopupLayer::OnCloseButtonClicked()
{
    // remove this layer right away
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    
    CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
    scene->removeChild(this);
}

bool ItemInfoPopupLayer::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    this->m_UseItemMenu->ccTouchBegan(touch, event);
    this->m_CloseMenu->ccTouchEnded(touch, event);
    
    return true;
}

void ItemInfoPopupLayer::ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{
    this->m_UseItemMenu->ccTouchEnded(touch, event);
    this->m_CloseMenu->ccTouchEnded(touch, event);
}



StashNode::StashNode(): m_CurrentTab(), m_Body(nullptr), m_EquipmentButton(nullptr), m_ConsumeButton(nullptr), m_MaterialButton(nullptr), m_ItemSlotNodeList(), m_ItemInfoPopup(nullptr), m_TrackingItemSlotNode(nullptr), m_IsTrackingItemSlotMoving(false), m_TrackingItemSlotTouchedTime(0)
{

}

StashNode::~StashNode()
{
    if(this->m_Body)
    {
        this->m_Body->release();
        this->m_Body = nullptr;
    }
}

bool StashNode::init()
{
    // add background (body)
    this->m_Body = CCSprite::create("ui/stash/body.png");
    this->m_Body->retain();
    this->m_Body->setAnchorPoint(CCPointLowerRight);
    this->m_Body->setPosition(CCPointZero);

    this->addChild(this->m_Body);
    
    // add tab menu 
    this->m_EquipmentButton = CCMenuItemToggle::createWithTarget(this,
                menu_selector(StashNode::OnEquipmentButtonClicked),
                CCMenuItemImage::create("ui/stash/tab_button_normal.png", "ui/stash/tab_button_normal.png"),
                CCMenuItemImage::create("ui/stash/tab_button_active.png", "ui/stash/tab_button_active.png"),
                NULL);
    this->m_EquipmentButton->retain();
    this->m_EquipmentButton->setAnchorPoint(CCPointLowerMid);
    
    this->m_ConsumeButton = CCMenuItemToggle::createWithTarget(this,
                menu_selector(StashNode::OnConsumeButtonClicked),
                CCMenuItemImage::create("ui/stash/tab_button_normal.png", "ui/stash/tab_button_normal.png"),
                CCMenuItemImage::create("ui/stash/tab_button_active.png", "ui/stash/tab_button_active.png"),
                NULL);
    this->m_ConsumeButton->retain();
    this->m_ConsumeButton->setAnchorPoint(CCPointLowerMid);
    
    this->m_MaterialButton = CCMenuItemToggle::createWithTarget(this,
                menu_selector(StashNode::OnMaterialButtonClicked),
                CCMenuItemImage::create("ui/stash/tab_button_normal.png", "ui/stash/tab_button_normal.png"),
                CCMenuItemImage::create("ui/stash/tab_button_active.png", "ui/stash/tab_button_active.png"),
                NULL);
    this->m_MaterialButton->retain();
    this->m_MaterialButton->setAnchorPoint(CCPointLowerMid);
    
    this->m_CurrentTab = "equipment";
    this->m_EquipmentButton->setSelectedIndex(1);
    
    CCMenu* tabMenu = CCMenu::create(this->m_EquipmentButton, this->m_ConsumeButton, this->m_MaterialButton, NULL);
    tabMenu->alignItemsHorizontallyWithPadding(0);

    CCRect bodyRect = this->m_Body->getTextureRect();
    
    tabMenu->setPosition(ccp(bodyRect.size.width / 2, bodyRect.size.height));
    
    this->m_Body->addChild(tabMenu);
    
    // insert item slots
    Actor* actor = GameClient::Instance().GetClientStage()->FindActor(GameClient::Instance().GetMyActorID());
    
    const int perPage = 15;
    const int perRow = 2;
    const int SlotInitialPositionX = 10;
    const int SlotInitialPositionY = bodyRect.size.height - 10;
    
    ItemSlotNode* slotForSize = ItemSlotNode::create(ItemType_None, ItemID_None, InventorySlot_None);
    CCRect slotRect = GetRect(slotForSize);
    
    const float ItemSlotSizeX = slotRect.size.width;
    const float ItemSlotSizeY = slotRect.size.height;
    
    delete slotForSize;

//    int i = 0;

//    const ItemList& equimentItemList = actor->GetStash().GetItemList(ItemGroup_Equipment);
//    std::for_each(equimentItemList.begin(), equimentItemList.end(), [this, &SlotInitialPositionX, &SlotInitialPositionY, &ItemSlotSizeX, &ItemSlotSizeY, &i](Item* item){
//        item->GetItemType();
//        item->GetItemID();
//        ItemSlotNode* node = ItemSlotNode::create(item->GetItemType(), item->GetItemID(), InventorySlot_None);
//        node->setAnchorPoint(CCPointUpperLeft);
//        node->setPosition(ccp(SlotInitialPositionX + ((ItemSlotMargin + ItemSlotSizeX) * (i % perRow)), SlotInitialPositionY - ((ItemSlotMargin + ItemSlotSizeY) * (i / perRow))));
//        i++;
//    });

    ItemDataDictionary::Initialize();

    for(int i = 0; i < 25; i++)
    {
        Item* item = nullptr;
        if( i % 2 == 0) item = ItemFactory::Instance().CreateItem(ItemType_BluePotion9, i);    else item = ItemFactory::Instance().CreateItem(ItemType_RedPotion9, i);
        
        ItemSlotNode* node = ItemSlotNode::create(item->GetItemType(), item->GetItemID(), static_cast<InventorySlot>(i));
        node->setAnchorPoint(CCPointUpperLeft);
        CCPoint position = ccp(SlotInitialPositionX + ((ItemSlotMargin + ItemSlotSizeX) * (i % perRow)), SlotInitialPositionY - ((ItemSlotMargin + ItemSlotSizeY) * (i / perRow)));
        node->setPosition(position);
        
        this->m_Body->addChild(node);
    }
    
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    
    return true;
}

bool StashNode::ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{
    ItemSlotNode* selectedItemSlot = this->FindSelectedItemSlotNode(touch->getLocation());
    if(selectedItemSlot)
    {
        this->m_TrackingItemSlotNode = selectedItemSlot;
        return true;
    }
    
    return false;
}

void StashNode::ccTouchMoved(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{
    ServerTime currentTime = GameClient::Instance().GetClientTimer().Check();
    if(this->m_TrackingItemSlotNode && currentTime - this->m_TrackingItemSlotTouchedTime > ServerTime(TimeToBeganMove))
    {
        this->m_IsTrackingItemSlotMoving = true;
        this->m_TrackingItemSlotNode->TrackTouch(touch);
    }
}

void StashNode::ccTouchEnded(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{
    if(this->m_TrackingItemSlotNode)
    {
        ItemSlotNode* selectedStashItemSlot = this->FindSelectedItemSlotNode(touch->getLocation());
        ItemSlotNode* selectedInventoryItemSlot = static_cast<UILayer*>(this->getParent())->GetInventoryNode()->FindSelectedItemSlotNode(touch->getLocation());
        GameClient& client = GameClient::Instance();
        
        CCRect bodyRect = GetRect(this->m_Body);
        
        if(selectedStashItemSlot == this->m_TrackingItemSlotNode)
        {
            
            // TO DO : display popup window
        }
        else if(selectedInventoryItemSlot)
        {
            client.GetClientObject().SendCSRequestRegisterStashItemToInventory(client.GetClientStage()->GetStageID(), client.GetMyActorID(), this->m_TrackingItemSlotNode->GetItemID(), selectedInventoryItemSlot->GetSlotNumber());

            // TO DO : register stash item to inventory
        }
        
        this->m_IsTrackingItemSlotMoving = false;
        this->m_TrackingItemSlotNode->ResetMoving();
        this->m_TrackingItemSlotNode = nullptr;
    }
}

ItemSlotNode* StashNode::FindSelectedItemSlotNode(cocos2d::CCPoint touchLocation)
{
    ItemSlotNode* selectedItemSlot = nullptr;
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

void StashNode::AddItem(flownet::ItemType itemType, flownet::ItemID itemID)
{
    for(int i = 0; i < this->m_ItemSlotNodeList.size(); ++i)
    {
        ItemSlotNode* itemSlot = this->m_ItemSlotNodeList[i];
        if(itemSlot->GetItemType() == ItemType_None && itemSlot->GetItemID() == ItemID_None)
        {
            itemSlot->ChangeItemTypeAndItemID(itemType, itemID);
            break;
        }
    }
}

void StashNode::EraseItem(flownet::ItemID itemID)
{
    for(int i = 0; i < this->m_ItemSlotNodeList.size(); ++i)
    {
        ItemSlotNode* itemSlot = this->m_ItemSlotNodeList[i];
        
        if(itemID == itemSlot->GetItemID())
        {
            itemSlot->Empty();
            break;
        }
    }
}


void StashNode::OnEquipmentButtonClicked(CCObject* sender)
{
    if(this->m_CurrentTab.compare("equipment") == 0)
    {
        this->m_EquipmentButton->setSelectedIndex(1);
        return;
    }
    
    this->m_ConsumeButton->setSelectedIndex(0);
    this->m_MaterialButton->setSelectedIndex(0);
    this->m_CurrentTab = "equipment";
}

void StashNode::OnConsumeButtonClicked(CCObject* sender)
{
    if(this->m_CurrentTab.compare("consume") == 0)
    {
        this->m_ConsumeButton->setSelectedIndex(1);
        return;
    }
    
    this->m_EquipmentButton->setSelectedIndex(0);
    this->m_MaterialButton->setSelectedIndex(0);
    this->m_CurrentTab = "consume";
}

void StashNode::OnMaterialButtonClicked(CCObject* sender)
{
    if(this->m_CurrentTab.compare("material") == 0)
    {
        this->m_MaterialButton->setSelectedIndex(1);
        return;
    }
    
    this->m_EquipmentButton->setSelectedIndex(0);
    this->m_ConsumeButton->setSelectedIndex(0);
    this->m_CurrentTab = "material";
}

void StashNode::ShowItemInfoPopup(flownet::ItemType itemType, flownet::ItemID itemID)
{
    this->m_ItemInfoPopup = ItemInfoPopupLayer::create(itemType, itemID);
    this->m_ItemInfoPopup->retain();
    
    BaseScene* scene = static_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
    scene->addChild(this->m_ItemInfoPopup);
}
