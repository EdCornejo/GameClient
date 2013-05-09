//
//  SpellQuickSlotNode.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 3/21/13.
//
//

#include "Headers.pch"

SpellQuickSlotNode::SpellQuickSlotNode(): m_ButtonSpellMap(), m_SpellTimerMap()
{

}

SpellQuickSlotNode::~SpellQuickSlotNode()
{
    std::for_each(this->m_SpellTimerMap.begin(), this->m_SpellTimerMap.end(), [this](SpellTimerMap::value_type pair){
        pair.second->release();
        pair.second = nullptr;
    });
    
    this->m_SpellTimerMap.clear();
}

bool SpellQuickSlotNode::init()
{
    Player* player = GameClient::Instance().GetClientStage()->FindPlayer(GameClient::Instance().GetMyActorID());
    // TO DO : initialize with player's spell info

    std::vector<SpellType> spellList = {SpellType_FireBall, SpellType_FireBurst, SpellType_IceArrow, SpellType_WaterFire, SpellType_NONE};
    
    CCMenu* menu = CCMenu::create();
    
    std::for_each(spellList.begin(), spellList.end(), [this, &menu](SpellType spellType){
        CCSprite* spellIconImage = SpellImageLoader::GetSpellQuickSlotImage(spellType);
        CCSprite* spellIconImageDisabled = SpellImageLoader::GetSpellQuickSlotImage(spellType);
        spellIconImageDisabled->setColor(ccGRAY);
        
        CCMenuItemSprite* menuItem = CCMenuItemSprite::create(spellIconImage, spellIconImage, spellIconImageDisabled, this, menu_selector(SpellQuickSlotNode::OnSkillTouched));
        
        this->m_ButtonSpellMap.insert(ButtonSpellMap::value_type(menuItem, spellType));
        menuItem->retain();
        menu->addChild(menuItem);
        
        CCSprite* icon = SpellImageLoader::GetSpellQuickSlotImage(spellType);
        CCProgressTimer* timer = CCProgressTimer::create(icon);
        timer->retain();
        
        this->m_SpellTimerMap.insert(SpellTimerMap::value_type(spellType, timer));
    });
    
    menu->alignItemsHorizontallyWithPadding(10);
    menu->setPosition(CCPointZero);

    this->addChild(menu);
    
    return true;
}

SpellQuickSlotNode* SpellQuickSlotNode::create()
{
    SpellQuickSlotNode* newNode = new SpellQuickSlotNode();
    if(newNode && newNode->init())
    {
        newNode->autorelease();
        return newNode;
    }
    else
    {
        delete newNode;
        return nullptr;
    }
}

void SpellQuickSlotNode::OnSkillTouched(cocos2d::CCObject *sender)
{
    ButtonSpellMap::iterator iter = this->m_ButtonSpellMap.find(static_cast<CCMenuItem*>(sender));
    if (iter == this->m_ButtonSpellMap.end())
    {
        return;
    }
    
    flownet::SpellType spellType = iter->second;
    
    UILayer* uiLayer = static_cast<UILayer*>(this->getParent());
    if(!uiLayer)
    {
        CCLOGERROR("SpellQuickSlotNode::OnSkillTouched >> no UILayer exists");
        ASSERT_DEBUG(uiLayer != nullptr);
    }
    
    // TO DO : do nothing on empty skill slot
    // TO DO : set selected highlight on MenuItem's position
    uiLayer->SetSelectedSpellType(spellType);
}

void SpellQuickSlotNode::DisableButton(CCObject* object)
{
    CCMenuItem* menuItem = static_cast<CCMenuItem*>(object);
    menuItem->setEnabled(false);
}

void SpellQuickSlotNode::EnableButton(CCObject* object)
{
    CCMenuItem* menuItem = static_cast<CCMenuItem*>(object);
    menuItem->setEnabled(true);
}

void SpellQuickSlotNode::AddProgressTimer(CCObject* object)
{
    CCProgressTimer* timer = static_cast<CCProgressTimer*>(object);
    this->addChild(timer);
}

void SpellQuickSlotNode::RemoveProgressTimer(CCObject* object)
{
    CCProgressTimer* timer = static_cast<CCProgressTimer*>(object);
    this->removeChild(timer);
}

void SpellQuickSlotNode::ApplyCoolTime(flownet::SpellType spellType)
{
    SpellTimerMap::iterator iter = this->m_SpellTimerMap.find(spellType);
    if(iter == this->m_SpellTimerMap.end())
    {
        return;
    }
    
    CCProgressTimer* timer = iter->second;
    
    SpellInfo spellInfo = SpellDictionary::Instance().FindSpellInfoBySpellType(spellType);
    
    std::for_each(this->m_ButtonSpellMap.begin(), this->m_ButtonSpellMap.end(), [this, spellInfo, timer](ButtonSpellMap::value_type pair){
        if(pair.second == spellInfo.m_SpellType)
        {
    
            // progress with spell cooltime. now fixed to 5 sec
            CCCallFuncO* disable = CCCallFuncO::create(this, callfuncO_selector(SpellQuickSlotNode::DisableButton), pair.first);
            CCDelayTime* delay = CCDelayTime::create(5);
            CCCallFuncO* enable = CCCallFuncO::create(this, callfuncO_selector(SpellQuickSlotNode::EnableButton), pair.first);
            
            CCSequence* sequence = CCSequence::create(disable, delay, enable, NULL);
            pair.first->runAction(sequence);
            
            CCPoint menuPosition = pair.first->getParent()->getPosition();
            CCPoint buttonPosition = pair.first->getPosition();
            
            timer->setPosition(ccpAdd(menuPosition, buttonPosition));
        }
    });
    
    CCCallFuncO* addChild = CCCallFuncO::create(this, callfuncO_selector(SpellQuickSlotNode::AddProgressTimer), iter->second);
    CCDelayTime* delay = CCDelayTime::create(5);
    CCCallFuncO* removeChild = CCCallFuncO::create(this, callfuncO_selector(SpellQuickSlotNode::RemoveProgressTimer), iter->second);
    CCSequence* sequence = CCSequence::create(addChild, delay, removeChild, NULL);

    this->runAction(sequence);
    
    CCProgressTo* progress = CCProgressTo::create(5, 100);
    timer->runAction(progress);

}
