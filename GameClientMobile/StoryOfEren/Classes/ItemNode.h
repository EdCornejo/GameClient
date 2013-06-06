//
//  ItemNode.h
//  GameClientMobile
//
//  Created by SungJinYoo on 4/23/13.
//
//

#ifndef __GameClientMobile__ItemNode__
#define __GameClientMobile__ItemNode__

class ItemNode : public CCNode
{
private:
    flownet::ItemType m_ItemType;
    flownet::ItemID m_ItemID;
    
    CCSprite* m_ItemImage;
public:
    ItemNode();
    virtual ~ItemNode();
    
    virtual bool init(flownet::ItemType itemType, flownet::ItemID itemID);
    static ItemNode* create(flownet::ItemType itemType, flownet::ItemID itemID);
    
    virtual const CCSize& getContentSize();

public:
    flownet::ItemType GetItemType() { return this->m_ItemType; }
    flownet::ItemID GetItemID() { return this->m_ItemID; }
};

#endif /* defined(__GameClientMobile__ItemNode__) */
