//
//  ItemNode.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 4/23/13.
//
//

#include "Headers.pch"

ItemNode::ItemNode(): m_ItemType(ItemType_None), m_ItemID(ItemID_None), m_ItemImage(nullptr)
{

}

ItemNode::~ItemNode()
{
    if(this->m_ItemImage)
    {
        this->m_ItemImage->release();
        this->m_ItemImage = nullptr;
    }
}
    
bool ItemNode::init(flownet::ItemType itemType, flownet::ItemID itemID)
{
    this->m_ItemType = itemType;
    this->m_ItemID = itemID;

    this->m_ItemImage = ItemImageLoader::GetItemStageImage(itemType);
    this->m_ItemImage->retain();
    
    CCMoveBy* moveUp = CCMoveBy::create(1, ccp(0, 4));
    CCMoveBy* moveDown = CCMoveBy::create(1, ccp(0, -4));
    CCSequence* sequence = CCSequence::create(moveUp, moveDown, NULL);
    CCRepeatForever* hovering = CCRepeatForever::create(sequence);
    
    this->m_ItemImage->runAction(hovering);
    
    this->addChild(this->m_ItemImage);
    
    return true;
}

ItemNode* ItemNode::create(flownet::ItemType itemType, flownet::ItemID itemID)
{
    ItemNode* itemNode = new ItemNode();
    if(itemNode && itemNode->init(itemType, itemID))
    {
        return itemNode;
    }
    else
    {
        delete itemNode;
        itemNode = nullptr;
        return nullptr;
    }
}

const CCSize& ItemNode::getContentSize()
{
    return this->m_ItemImage->getTextureRect().size;
}
