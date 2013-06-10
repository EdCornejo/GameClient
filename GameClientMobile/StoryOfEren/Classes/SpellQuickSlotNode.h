//
//  SpellQuickSlotNode.h
//  GameClientMobile
//
//  Created by SungJinYoo on 3/21/13.
//
//

#ifndef __GameClientMobile__SpellQuickSlotNode__
#define __GameClientMobile__SpellQuickSlotNode__

//class SpellQuickSlotItemNode : public CCNode
//{
//public:
//    SpellQuickSlotItemNode(flownet::SpellType spellType);
//    SpellQuickSlotItemNode(flownet::SpellInfo spellInfo);
//    virtual ~SpellQuickSlotItemNode();
//    
//private:
//    flownet::SpellType m_SpellType;
//    CCSprite* m_IconSprite;
//    
//public:
//    CCRect GetRect();
//    flownet::SpellType GetSpellType();
//};
//

class SpellQuickSlotNode : public CCNode, public CCTargetedTouchDelegate
{
    typedef flownet::Map<CCMenuItem*, flownet::SpellType>::type ButtonSpellMap;
public:
    enum
    {
        PositionX = 140,
        PositionY = 320 - 40, // windowSizeHeight - iconsize/2
    };

private:
    ButtonSpellMap m_ButtonSpellMap;
    CCSprite* m_HighlightImage;
    
public:
    SpellQuickSlotNode();
    virtual ~SpellQuickSlotNode();
    
    virtual bool init();
    
    static SpellQuickSlotNode* create();

private:
    void OnSkillTouched(CCObject* sender);
    
    void DisableButton(CCObject* menuItem);
    void EnableButton(CCObject* menuItem);
    void RemoveProgressTimer(CCObject* timer);
    
    void SetHighlight(CCMenuItem* spellIcon);
    
public:
    void ApplyCoolTime(flownet::SpellType spellType);
    void RemoveHighlight();
};

#endif /* defined(__GameClientMobile__SpellQuickSlotNode__) */
