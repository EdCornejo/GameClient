//
//  SpellQuickSlotNode.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 3/21/13.
//
//

#include "Headers.pch"

SpellQuickSlotItemNode::SpellQuickSlotItemNode(SpellType spellType): m_SpellType(spellType), m_IconSprite(nullptr)
{
    this->m_IconSprite = SpellImageLoader::GetSpellQuickSlotImage(spellType);
    this->m_IconSprite->retain();
    this->m_IconSprite->setAnchorPoint(CCPointZero);
    this->addChild(this->m_IconSprite);
}

SpellQuickSlotItemNode::~SpellQuickSlotItemNode()
{
    if(this->m_IconSprite)
    {
        this->m_IconSprite->release();
        this->m_IconSprite = nullptr;
    }
}

CCRect SpellQuickSlotItemNode::GetRect()
{
    return this->m_IconSprite->getTextureRect();
}

SpellType SpellQuickSlotItemNode::GetSpellType()
{
    return this->m_SpellType;
}

SpellQuickSlotNode::SpellQuickSlotNode() : m_SpellQuickSlotItemList()
{

}

SpellQuickSlotNode::~SpellQuickSlotNode()
{
    std::for_each(this->m_SpellQuickSlotItemList.begin(), this->m_SpellQuickSlotItemList.end(), [](SpellQuickSlotItemNode* node){
        delete node;
    });
    
    this->m_SpellQuickSlotItemList.clear();
}

void SpellQuickSlotNode::Initialize()
{
    // NOTE : initialize with my character info
    Player* player = GameClient::Instance().GetClientStage()->FindPlayer(GameClient::Instance().GetMyActorID());
    
    int index = 0;
    
    std::vector<SpellType> spellList = {SpellType_FireBall, SpellType_FireBurst, SpellType_IceArrow, SpellType_WaterFire};
    
    std::for_each(spellList.begin(), spellList.end(), [this, &index](SpellType spellType){
        SpellQuickSlotItemNode* node = new SpellQuickSlotItemNode(spellType);
        node->setPosition(ccp((SpellIconMargin + SpellIconSize) * index, 0));
        this->m_SpellQuickSlotItemList.push_back(node);
        this->addChild(node);
        index++;
    });
    
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}



bool SpellQuickSlotNode::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    CC_UNUSED_PARAM(event);
    CCPoint touchLocation = touch->getLocation();
    
    SpellQuickSlotItemNode* selectedSpellItemNode = this->FindSpellQuickSlotItemNode(touchLocation);
    
    if(selectedSpellItemNode)
    {
        // spell touched !
        UILayer* uiLayer = dynamic_cast<UILayer*>(this->getParent());
        if(!uiLayer) ASSERT_DEBUG(uiLayer != nullptr);
        
        uiLayer->SetSelectedSpellType(selectedSpellItemNode->GetSpellType());
        return true;
    }

    return false;
}

SpellQuickSlotItemNode* SpellQuickSlotNode::FindSpellQuickSlotItemNode(cocos2d::CCPoint touchLocation)
{
    SpellQuickSlotItemNode* foundNode = nullptr;
    std::for_each(this->m_SpellQuickSlotItemList.begin(), this->m_SpellQuickSlotItemList.end(), [this, touchLocation, &foundNode](SpellQuickSlotItemNode* node){
        CCRect nodeRect = node->GetRect();
        nodeRect.origin = CCPointZero;
        CCPoint nodeLocalPoint = node->convertToNodeSpace(touchLocation);
    
        if(nodeRect.containsPoint(nodeLocalPoint))
        {
            foundNode = node;
        }
    });
    
    return foundNode;
}
