//
//  EquipmentNode.h
//  GameClientMobile
//
//  Created by SungJinYoo on 5/9/13.
//
//

#ifndef __GameClientMobile__EquipmentNode__
#define __GameClientMobile__EquipmentNode__

class EquipmentNode : public CCNode, public CCTouchDelegate
{
typedef flownet::Vector<ItemSlotNode*>::type ItemSlotNodeList;

private:
    bool m_IsOpen;
    
    CCSprite* m_SlideButton;
    CCSprite* m_Body;
    ActorNode* m_ActorNode;

    ItemSlotNodeList m_ItemSlotNodeList;
    
    ItemSlotNode* m_TrackingItemSlotNode;
    ItemSlotNode* m_HighlightedItemSlotNode;
    flownet::ServerTime m_TrackingItemSlotTouchedTime;
    
private:
    enum {
        ItemSlotMargin = 2,
        ItemSlotPositionX = 50,
        ItemSlotPositionY = 30,
    };

public:
    enum {
        PositionX = 0,
        PositionY = 320
    };
    
public:
    EquipmentNode();
    virtual ~EquipmentNode();
    
    virtual bool init();
    static EquipmentNode* create();
    
public:
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);

public:
    ItemSlotNode* FindSelectedItemSlotNode(CCPoint touchLocation);
    void Update();

private:
    void OnScrollButtonClicked();
    void Slide();
};

#endif /* defined(__GameClientMobile__EquipmentNode__) */
