//
//  StashNode.h
//  GameClientMobile
//
//  Created by SungJinYoo on 4/23/13.
//
//

#ifndef __GameClientMobile__StashNode__
#define __GameClientMobile__StashNode__

class ItemInfoPopupLayer : public CCLayer
{
private:
    flownet::ItemType m_ItemType;
    flownet::ItemID m_ItemID;
    
//    CCSprite* m_Background;
//    CCSprite* m_ItemSlotBackground;
//    CCSprite* m_ItemImage;
//    CCLabelTTF* m_ItemNameLabel;
//    CCLabelTTF* m_GoldValueLabel;
//    CCLabelTTF* m_FullDescriptionLabel;
//    CCLabelTTF* m_AbstractDescriptionLabel;
    CCMenu* m_UseItemMenu;
    CCMenu* m_CloseMenu;
    
public:
    ItemInfoPopupLayer();
    virtual ~ItemInfoPopupLayer();
    
    virtual bool init();

    static ItemInfoPopupLayer* create(flownet::ItemType itemType, flownet::ItemID itemID);

public:
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);

public:
    void OnUseItemButtonClicked();
    void OnCloseButtonClicked();
};




class StashNode : public CCNode, public CCTouchDelegate
{
typedef flownet::Vector<ItemSlotNode*>::type ItemSlotNodeList;

private:
    enum {
        PerPage = 12,
        PerRow = 3,
    };

private:
    CCSprite* m_Body;
    flownet::ItemGroup  m_CurrentItemGroup;
    int                 m_CurrentPage;
    CCMenuItemToggle*   m_EquipmentButton;
    CCMenuItemToggle*   m_ConsumeButton;
    CCMenuItemToggle*   m_MaterialButton;
    ItemSlotNodeList    m_ItemSlotNodeList;
    
    ItemInfoPopupLayer* m_ItemInfoPopup;
    
    ItemSlotNode*       m_TrackingItemSlotNode;
    bool                m_IsTrackingItemSlotMoving;
    flownet::ServerTime m_TrackingItemSlotTouchedTime;

public:
    enum {
        PositionX = 480,
        PositionY = 0,
        ItemSlotMarginX = 10,
        ItemSlotMarginY = 6,
        
        TimeToBeganMove = 1000,
    };

public:
    StashNode();
    virtual ~StashNode();
    
    virtual bool init();

    CREATE_FUNC(StashNode);

public:
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    
public:
    ItemSlotNode* FindSelectedItemSlotNode(CCPoint touchLocation);
//    void AddItem(flownet::ItemType itemType, flownet::ItemID itemID);
//    void EraseItem(flownet::ItemID itemID);

    void Update();
    
private:
    void OnEquipmentButtonClicked(CCObject* sender);
    void OnConsumeButtonClicked(CCObject* sender);
    void OnMaterialButtonClicked(CCObject* sender);
    
    void ShowItemInfoPopup(flownet::ItemType itemType, flownet::ItemID itemID);
};

#endif /* defined(__GameClientMobile__StashNode__) */
