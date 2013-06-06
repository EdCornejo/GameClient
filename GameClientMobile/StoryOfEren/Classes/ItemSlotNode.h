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
    enum {
        BackgroundIndex = -10,
    };

private:
    flownet::ItemType m_ItemType;
    flownet::ItemID m_ItemID;
    flownet::InventorySlot m_InventorySlot;
    flownet::EquipmentSlot m_EquipmentSlot;
    
    CCSprite* m_SlotBackground;
    CCSprite* m_ItemPlaceHolder;
    CCSprite* m_ItemImage;
    
public :
    ItemSlotNode();
    virtual ~ItemSlotNode();
    
    virtual bool init(flownet::ItemType itemType, flownet::ItemID itemID, flownet::InventorySlot slotNumber);
    virtual bool init(flownet::ItemType itemType, flownet::ItemID itemID, flownet::EquipmentSlot equipmentSlot);
    static ItemSlotNode* create(flownet::ItemType itemType, flownet::ItemID itemID, flownet::InventorySlot slotNumber);
    static ItemSlotNode* create(flownet::ItemType itemType, flownet::ItemID itemID, flownet::EquipmentSlot equipmentSlot);

    void TrackTouch(CCTouch* touch);
    void ResetMoving();
    void Highlight();
    void ResetHighlight();
    void Empty();
    void ChangeItemTypeAndItemID(flownet::ItemType itemType, flownet::ItemID itemID);

    virtual const CCSize& getContentSize();

    flownet::ItemType GetItemType() { return this->m_ItemType; }
    flownet::ItemID GetItemID() { return this->m_ItemID; }
    flownet::InventorySlot GetInventorySlot() { return this->m_InventorySlot; }
    flownet::EquipmentSlot GetEquipmentSlot() { return this->m_EquipmentSlot; }
};

#endif /* defined(__GameClientMobile__InventorySlotNode__) */
