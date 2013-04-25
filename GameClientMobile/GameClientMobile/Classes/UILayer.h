//
//  UILayer.h
//  GameClientMobile
//
//  Created by SungJinYoo on 3/6/13.
//
//

#ifndef __GameClientMobile__UILayer__
#define __GameClientMobile__UILayer__

class UILayer : public BaseLayer
{
private:
    enum
    {
        SpellStackPositionX = 10,
        SpellStackPositionY = 320 - 60 - 10, // windowSizeHeight - iconsize - realposition
    };
    
private:
    StageType m_StageType;
    SpellQuickSlotNode* m_SpellQuickSlotNode;
    StashNode* m_StashNode;
    InventoryNode* m_InventoryNode;
    MenuBarNode* m_MenuBarNode;
    SpellType m_SelectedSpellType;
    CCPoint m_SpellDestinationPoint;
    
public:
    UILayer();
    virtual ~UILayer();
    virtual bool init();

    static UILayer* create(StageType stageType);
    virtual void update(float deltaTime);
    
    virtual void ccTouchesBegan(CCSet* touches, CCEvent* event) override;
    virtual void ccTouchesEnded(CCSet* touches, CCEvent* event) override;
    
private:
    void InitializeSpellQuickSlot();
    void InitializeInventory();
    void InitializeMenuBar();
    
public:
    InventoryNode* GetInventoryNode();
    StashNode* GetStashNode();
    
public:
    void SetSelectedSpellType(SpellType spellType);
    void UseItem(flownet::ItemID itemID, flownet::InventorySlot inventorySlot);
    void SwapInventorySlot(flownet::InventorySlot sourceSlotNumber, flownet::InventorySlot destinationSlotNumber);
};

#endif /* defined(__GameClientMobile__UILayer__) */
