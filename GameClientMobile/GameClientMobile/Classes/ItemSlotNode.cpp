//
//  InventorySlotNode.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 4/20/13.
//
//

#include "Headers.pch"

ItemSlotNode::ItemSlotNode(): m_ItemType(ItemType_None), m_ItemID(ItemID_None), m_SlotNumber(InventorySlot_None), m_SlotBackground(nullptr), m_ItemPlaceHolder(nullptr), m_ItemImage(nullptr)
{

}

ItemSlotNode::~ItemSlotNode()
{
    if(this->m_SlotBackground)
    {
        this->m_SlotBackground->release();
        this->m_SlotBackground = nullptr;
    }
    if(this->m_ItemPlaceHolder)
    {
        this->m_ItemPlaceHolder->release();
        this->m_ItemPlaceHolder = nullptr;
    }
    if(this->m_ItemImage)
    {
        this->m_ItemImage->release();
        this->m_SlotBackground = nullptr;
    }
}

bool ItemSlotNode::init(flownet::ItemType itemType, flownet::ItemID itemID, flownet::InventorySlot slotNumber)
{
    this->m_ItemType = itemType;
    this->m_ItemID = itemID;
    this->m_SlotNumber = slotNumber;
    
    this->m_SlotBackground = CCSprite::create("ui/inventory/item_slot_normal.png");
    this->m_SlotBackground->retain();
    
    this->m_ItemImage = ItemImageLoader::GetItemInventoryImage(this->m_ItemType);
    this->m_ItemImage->retain();
    
    this->addChild(this->m_SlotBackground, BackgroundIndex);
    this->addChild(this->m_ItemImage);
    
    return true;
}

ItemSlotNode* ItemSlotNode::create(flownet::ItemType itemType, flownet::ItemID itemID, flownet::InventorySlot slotNumber)
{
    ItemSlotNode* newItemSlotNode = new ItemSlotNode();
    if(newItemSlotNode && newItemSlotNode->init(itemType, itemID, slotNumber))
    {
        return newItemSlotNode;
    }
    else
    {
        delete newItemSlotNode;
        newItemSlotNode = nullptr;
        return nullptr;
    }
}

void ItemSlotNode::TrackTouch(CCTouch *touch)
{
    if(!this->m_ItemPlaceHolder)
    {
        this->m_ItemPlaceHolder = ItemImageLoader::GetItemInventoryImage(this->m_ItemType);
        this->m_ItemPlaceHolder->retain();
        this->m_ItemPlaceHolder->setOpacity(100);
        this->addChild(this->m_ItemPlaceHolder);
    }
    
    CCPoint localPoint = this->convertToNodeSpace(touch->getLocation());
    this->m_ItemImage->setPosition(localPoint);
}

void ItemSlotNode::Empty()
{
    if(this->m_ItemPlaceHolder)
    {
        this->removeChild(this->m_ItemPlaceHolder);
        this->m_ItemPlaceHolder->release();
        this->m_ItemPlaceHolder = nullptr;
    }
    if(this->m_ItemImage)
    {
        this->removeChild(this->m_ItemImage);
        this->m_ItemImage->release();
        this->m_ItemImage = nullptr;
    }
    
    this->m_ItemType = ItemType_None;
    this->m_ItemID = ItemID_None;
}

void ItemSlotNode::ResetMoving()
{
    if(this->m_ItemPlaceHolder)
    {
        this->removeChild(this->m_ItemPlaceHolder);
        this->m_ItemPlaceHolder->release();
        this->m_ItemPlaceHolder = nullptr;
    }
    if(this->m_ItemImage)
    {
        this->m_ItemImage->setPosition(CCPointZero);
    }
}

void ItemSlotNode::Highlight()
{
    if(this->m_SlotBackground)
    {
        this->removeChild(this->m_SlotBackground);
        this->m_SlotBackground->release();
    }
    this->m_SlotBackground = CCSprite::create("ui/inventory/item_slot_active.png");
    this->addChild(this->m_SlotBackground, BackgroundIndex);
    this->m_SlotBackground->retain();
}

void ItemSlotNode::ResetHighlight()
{
    if(this->m_SlotBackground)
    {
        this->removeChild(this->m_SlotBackground);
        this->m_SlotBackground->release();

    }
    this->m_SlotBackground = CCSprite::create("ui/inventory/item_slot_normal.png");
    this->addChild(this->m_SlotBackground, BackgroundIndex);
    this->m_SlotBackground->retain();
}

const CCSize& ItemSlotNode::getContentSize()
{
    return this->m_SlotBackground->getTextureRect().size;
}

void ItemSlotNode::ChangeItemTypeAndItemID(flownet::ItemType itemType, flownet::ItemID itemID)
{
    this->m_ItemType = itemType;
    this->m_ItemID = itemID;

    if(this->m_ItemImage)
    {
        this->removeChild(this->m_ItemImage);
        this->m_ItemImage->release();
        this->m_ItemImage = nullptr;
    }
    
    this->m_ItemImage = ItemImageLoader::GetItemInventoryImage(itemType);
    this->m_ItemImage->retain();
    this->addChild(this->m_ItemImage);
}
