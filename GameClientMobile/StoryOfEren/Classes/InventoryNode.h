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
    
    CCNode* m_MovingPart;
    CCNode* m_PinnedPart;
    
    flownet::ItemGroup m_CurrentItemGroup;
    CCSprite* m_ScrollButton;
    CCSprite* m_SlideButton;
    CCSprite* m_Body;
    CCLabelTTF* m_GoldLabel;
    CCLabelTTF* m_DescriptionLabel;
    CCMenuItemToggle* m_EquipmentButton;
    CCMenuItemToggle* m_ConsumeButton;
    CCMenuItemToggle* m_MaterialButton;
    CCMenu* m_TabMenu;
    ItemSlotNodeList m_ItemSlotNodeList;
    
    ItemSlotNode* m_TrackingItemSlotNode;
    ItemSlotNode* m_HighlightedItemSlotNode;
    bool m_IsTrackingItemSlotMoving;
    flownet::ServerTime m_TrackingItemSlotTouchedTime;
    

private:
    enum {
        GoldLabelMarginX = 10,
        GoldLabelMarginY = -10,
        ItemSlotMarginX = 5,
        ItemSlotMarginY = 2,
        ItemSlotPositionX = 46,
        ItemSlotPositionY = 82,
        
        TimeToBeganMove = 1000, // 1000 millisec
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
    void Update();

public:
    void SwapInventorySlot(flownet::InventorySlot source, flownet::InventorySlot destination);
    //void AddItem(flownet::ItemType itemType, flownet::ItemID itemID);
    //void EraseItem(flownet::ItemID itemID);
    
private:
    void OnScrollButtonClicked();
    void Slide();
    void ShowTabMenu();
    
    void OnEquipmentButtonClicked(CCObject* sender);
    void OnConsumeButtonClicked(CCObject* sender);
    void OnMaterialButtonClicked(CCObject* sender);
    
private:
    void SwapInventorySlot(ItemSlotNode* lhs, ItemSlotNode* rhs);
    
};

#endif /* defined(__GameClientMobile__InventoryNode__) */
