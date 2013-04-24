//
//  InventoryNode.h
//  GameClientMobile
//
//  Created by SungJinYoo on 4/20/13.
//
//

#ifndef __GameClientMobile__InventoryNode__
#define __GameClientMobile__InventoryNode__

class InventoryNode : public CCNode, public CCTouchDelegate
{
typedef flownet::Vector<ItemSlotNode*>::type ItemSlotNodeList;

private:
    bool m_IsOpen;
    std::string m_CurrentTab;
    CCMenuItem* m_SlideButton;
    CCSprite* m_Body;
    CCLabelTTF* m_GoldLabel;
    CCLabelTTF* m_DescriptionLabel;
    CCMenuItemToggle* m_EquipmentButton;
    CCMenuItemToggle* m_ConsumeButton;
    CCMenuItemToggle* m_MaterialButton;
    ItemSlotNodeList m_ItemSlotNodeList;
    
    ItemSlotNode* m_TrackingItemSlotNode;

private:
    enum {
        GoldLabelMarginX = 10,
        GoldLabelMarginY = -10,
        DescriptionLabelMarginX = 10,
        DescriptionLabelMarginY = 10,
        ItemSlotMargin = 2,
        ItemSlotPositionX = 120,
        ItemSlotPositionY = 90,
    };

public:
    enum {
        PositionX = 0,
        PositionY = 0
    };
    
public:
    InventoryNode();
    virtual ~InventoryNode();
    
    virtual bool init();
    static InventoryNode* create();
    
public:
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);

public:
    ItemSlotNode* FindSelectedItemSlotNode(CCPoint touchLocation);

public:
    void UseItem(flownet::ItemID itemID, flownet::InventorySlot inventorySlot);
    void SwapInventorySlot(flownet::InventorySlot source, flownet::InventorySlot destination);

private:
    void Slide();
    
    void OnEquipmentButtonClicked(CCObject* sender);
    void OnConsumeButtonClicked(CCObject* sender);
    void OnMaterialButtonClicked(CCObject* sender);
    
private:
    void SwapInventorySlot(ItemSlotNode* lhs, ItemSlotNode* rhs);
};

#endif /* defined(__GameClientMobile__InventoryNode__) */
