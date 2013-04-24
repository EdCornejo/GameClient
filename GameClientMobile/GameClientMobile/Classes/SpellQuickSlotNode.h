//
//  SpellQuickSlotNode.h
//  GameClientMobile
//
//  Created by SungJinYoo on 3/21/13.
//
//

#ifndef __GameClientMobile__SpellQuickSlotNode__
#define __GameClientMobile__SpellQuickSlotNode__

class SpellQuickSlotItemNode : public CCNode
{
public:
    SpellQuickSlotItemNode(flownet::SpellType spellType);
    SpellQuickSlotItemNode(flownet::SpellInfo spellInfo);
    virtual ~SpellQuickSlotItemNode();
    
private:
    flownet::SpellType m_SpellType;
    CCSprite* m_IconSprite;
    
public:
    CCRect GetRect();
    flownet::SpellType GetSpellType();
};

class SpellQuickSlotNode : public CCNode, public CCTargetedTouchDelegate
{
    typedef std::vector<SpellQuickSlotItemNode*> SpellQuickSlotItemList;
private:
    enum {
        SpellIconMargin = 10,
        SpellIconSize = 60,
    };
    
private:
    SpellQuickSlotItemList m_SpellQuickSlotItemList;

public:
    SpellQuickSlotNode();
    virtual ~SpellQuickSlotNode();
    
    void Initialize();
    
public:
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    
private:
    SpellQuickSlotItemNode* FindSpellQuickSlotItemNode(CCPoint touchLocation);
};

#endif /* defined(__GameClientMobile__SpellQuickSlotNode__) */
