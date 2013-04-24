//
//  StashNode.h
//  GameClientMobile
//
//  Created by SungJinYoo on 4/23/13.
//
//

#ifndef __GameClientMobile__StashNode__
#define __GameClientMobile__StashNode__

class StashNode : public CCNode
{
typedef flownet::Vector<ItemSlotNode*>::type ItemSlotNodeList;

private:
    std::string m_CurrentTab;
    CCSprite* m_Body;
    CCMenuItemToggle* m_EquipmentButton;
    CCMenuItemToggle* m_ConsumeButton;
    CCMenuItemToggle* m_MaterialButton;
    ItemSlotNodeList m_ItemSlotNodeList;
    
    ItemSlotNode* m_TrackingItemSlotNode;

public:
    enum {
        PositionX = 480,
        PositionY = 0,
        ItemSlotMargin = 2,
    };

public:
    StashNode();
    virtual ~StashNode();
    
    virtual bool init();

    CREATE_FUNC(StashNode);
    
public:
    ItemSlotNode* FindSelectedItemSlotNode(CCPoint touchLocation);
    
private:
    void OnEquipmentButtonClicked(CCObject* sender);
    void OnConsumeButtonClicked(CCObject* sender);
    void OnMaterialButtonClicked(CCObject* sender);
};

#endif /* defined(__GameClientMobile__StashNode__) */
