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
    StageType m_StageType;
    SpellQuickSlotNode* m_SpellQuickSlotNode;
    StashNode* m_StashNode;
    EquipmentNode* m_EquipmentNode;
    InventoryNode* m_InventoryNode;
    MenuBarNode* m_MenuBarNode;
    ChattingNode* m_ChattingNode;
    ExpBarNode* m_ExpBarNode;
    SystemMessageWindowNode* m_SystemMessageWindowNode;
    CCPoint m_SpellDestinationPoint;
    CCPoint m_LastTouchLocation;
    ServerTime m_LastTouchTime;
    
public:
    UILayer();
    virtual ~UILayer();
    virtual bool init() override;

    static UILayer* create(StageType stageType);
    virtual void update(float deltaTime) override;
    
    virtual void ccTouchesBegan(CCSet* touches, CCEvent* event) override;
    virtual void ccTouchesEnded(CCSet* touches, CCEvent* event) override;
    
    virtual void OnResponse() override;
    virtual void OnLoad() override;
    
private:
    void InitializeSpellQuickSlot();
    void InitializeInventory();
    void InitializeStash();
    void InitializeEquipment();
    void InitializeMenuBar();
    void InitializeChatting();
    void InitializeExpBar();
    void InitializeSystemMessageWindow();
    
public:
    InventoryNode* GetInventoryNode();
    StashNode* GetStashNode();

private:
    bool TouchProcessSpellBegan(CCPoint touchLocation);
    bool TouchProcessSpellEnded(CCPoint touchLocation);
    bool TouchProcessItem(CCPoint touchLocation);
    bool TouchProcessMove(CCPoint touchLocation);
    void TranslateScreen();
    
public:
    void SwapInventorySlot(flownet::InventorySlot sourceSlotNumber, flownet::InventorySlot destinationSlotNumber);
    void ApplyCoolTime(flownet::SpellType spellType);
    void RemoveSpellHighlight();
    void MessageReceived(flownet::ActorID senderID, flownet::STRING senderName, flownet::STRING message);
    void SystemMessageReceived(std::string message);
    void ShowStageClearMessage();
    void ShowTierClearMessage();
    
    void UpdateInventory();
    void UpdateStash();
    void UpdateEquipment();
    void UpdateExpBar();
    
private:
    void RemoveChild(CCObject* object);
};

#endif /* defined(__GameClientMobile__UILayer__) */
