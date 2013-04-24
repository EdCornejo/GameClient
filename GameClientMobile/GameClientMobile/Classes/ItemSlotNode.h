//
//  InventorySlotNode.h
//  GameClientMobile
//
//  Created by SungJinYoo on 4/20/13.
//
//

#ifndef __GameClientMobile__ItemSlotNode__
#define __GameClientMobile__ItemSlotNode__

class ItemSlotNode : public CCNode
{
private:
    flownet::ItemType m_ItemType;
    flownet::ItemID m_ItemID;
    flownet::InventorySlot m_SlotNumber;
    
    CCSprite* m_SlotBackground;
    CCSprite* m_ItemPlaceHolder;
    CCSprite* m_ItemImage;
    
public :
    ItemSlotNode();
    virtual ~ItemSlotNode();
    
    virtual bool init(flownet::ItemType itemType, flownet::ItemID itemID, flownet::InventorySlot slotNumber);
    static ItemSlotNode* create(flownet::ItemType itemType, flownet::ItemID itemID, flownet::InventorySlot slotNumber);

    void TrackTouch(CCTouch* touch);
    void Reset();
    void Empty();
    void ChangeItemTypeAndItemID(flownet::ItemType itemType, flownet::ItemID itemID);

    virtual const CCSize& getContentSize();

    flownet::ItemType GetItemType() { return this->m_ItemType; }
    flownet::ItemID GetItemID() { return this->m_ItemID; }
    flownet::InventorySlot GetSlotNumber() { return this->m_SlotNumber; }
};

#endif /* defined(__GameClientMobile__InventorySlotNode__) */
