//
//  StashNode.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 4/23/13.
//
//

#include "Headers.pch"

StashNode::StashNode(): m_CurrentTab(), m_Body(nullptr), m_EquipmentButton(nullptr), m_ConsumeButton(nullptr), m_MaterialButton(nullptr), m_ItemSlotNodeList(), m_TrackingItemSlotNode(nullptr)
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

//    for(int i = 0; i < 25; i++)
//    {
//        Item* item = nullptr;
//        if( i % 2 == 0) item = ItemFactory::Instance().CreateItem(ItemType_BluePotion9, i);    else item = ItemFactory::Instance().CreateItem(ItemType_RedPotion9, i);
//        
//        ItemSlotNode* node = ItemSlotNode::create(item->GetItemType(), item->GetItemID(), static_cast<InventorySlot>(i));
//        node->setAnchorPoint(CCPointUpperLeft);
//        CCPoint position = ccp(SlotInitialPositionX + ((ItemSlotMargin + ItemSlotSizeX) * (i % perRow)), SlotInitialPositionY - ((ItemSlotMargin + ItemSlotSizeY) * (i / perRow)));
//        node->setPosition(position);
//        
//        this->m_Body->addChild(node);
//    }
    
    
    return true;
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


void StashNode::OnEquipmentButtonClicked(CCObject* sender)
{

}

void StashNode::OnConsumeButtonClicked(CCObject* sender)
{

}

void StashNode::OnMaterialButtonClicked(CCObject* sender)
{

}
