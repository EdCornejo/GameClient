//
//  ActorLayer.h
//  GameClientMobile
//
//  Created by SungJinYoo on 3/19/13.
//
//

#ifndef __GameClientMobile__ActorLayer__
#define __GameClientMobile__ActorLayer__

typedef Map<flownet::SpellAbility, SpellEffectNode*>::type SpellEffectNodeMap;

struct ActorNodeSet : public CCObject {
    ActorID m_ActorID;
    ActorNode* m_ActorNode;
    HUDNode* m_HUDNode;
    ShadowNode* m_ShadowNode;
    HighlightNode* m_HighlightNode;
    GuideLineNode* m_GuideLineNode;
    SpellEffectNodeMap m_SpellEffectNodeMap;

    
    ActorNodeSet(flownet::ActorID actorID): m_ActorID(actorID), m_ActorNode(nullptr), m_HUDNode(nullptr), m_ShadowNode(nullptr), m_HighlightNode(nullptr), m_GuideLineNode(nullptr), m_SpellEffectNodeMap()
    {
        if(flownet::IsPlayerID(actorID))
        {
            this->m_ActorNode = PlayerNode::create(actorID);
        }
        else
        {
            this->m_ActorNode = MonsterNode::create(actorID);
        }
        this->m_ActorNode->retain();
        
        this->m_HUDNode = HUDNode::create(actorID);
        this->m_HUDNode->retain();
        
        this->m_ShadowNode = ShadowNode::create(actorID);
        this->m_ShadowNode->retain();
    }
    
    ~ActorNodeSet()
    {
        if(this->m_ActorNode) this->m_ActorNode->release();
        if(this->m_HUDNode) this->m_HUDNode->release();
        if(this->m_ShadowNode) this->m_ShadowNode->release();
        if(this->m_HighlightNode) this->m_HighlightNode->release();
        if(this->m_GuideLineNode) this->m_GuideLineNode->release();
        std::for_each(this->m_SpellEffectNodeMap.begin(), this->m_SpellEffectNodeMap.end(), [this](SpellEffectNodeMap::value_type pair){            
            pair.second->release();
        });
        
        this->m_SpellEffectNodeMap.clear();
        
        this->m_ActorNode = nullptr;
        this->m_HUDNode = nullptr;
        this->m_ShadowNode = nullptr;
        this->m_HighlightNode = nullptr;
        this->m_GuideLineNode = nullptr;
    }
    
    void SetZOrder(int zOrder)
    {
        this->m_ActorNode->setZOrder(zOrder);
        this->m_HUDNode->setZOrder(zOrder + 9);
        this->m_ShadowNode->setZOrder(zOrder - 9);
        this->m_HighlightNode->setZOrder(zOrder - 8);
        std::for_each(this->m_SpellEffectNodeMap.begin(), this->m_SpellEffectNodeMap.end(), [this, zOrder](SpellEffectNodeMap::value_type pair){
            pair.second->setZOrder(zOrder+1);
        });
    }
    
    int GetZOrder(int zOrder)
    {
        return this->m_ActorNode->getZOrder();
    }
};


class ActorLayer : public BaseLayer
{
    typedef flownet::Map<flownet::ActorID, ActorNodeSet*>::type ActorNodeSetMap;
    typedef flownet::Map<flownet::ItemID, ItemNode*>::type ItemNodeMap;

private:
    ActorNodeSetMap m_ActorNodeSetMap;
    ItemNodeMap m_ItemNodeMap;
    
public:
    ActorLayer();
    virtual ~ActorLayer();
    
public:
    virtual bool init();
    static ActorLayer* create(StageType stageType);

    virtual void update(float deltaTime) override;

public:
    void DeleteActor(ActorID playerID);
    PlayerNode* FindPlayerNode(ActorID playerID);
    
    MonsterNode* FindMonsterNode(ActorID monsterID);
    
    ActorNode* FindActorNode(ActorID actorID);
    ActorNodeSet* FindActorNodeSet(ActorID actorID);
    ItemNode* FindItemNode(ItemID itemID);
    
    void DeleteActorNode(CCObject* actorNodeSet);
    
    void SortNodes();

// Util functions
    void UpdateActorLookingDirection(Actor* actor, CCPoint actorPoint, CCPoint lookingPoint);

// Actor part
    void ChangeTarget(flownet::ActorID monsterID, flownet::ActorID targetPlayerID);

    MonsterNode* FindSelectedMonsterNode(CCPoint touchLocation);
    ItemNode* FindSelectedItemNode(CCPoint touchLocation);
    
    void AddNewPlayer(flownet::ClientPlayer player);
    void AddNewMonster(flownet::ClientMonster monster);
    void MoveActor(flownet::ActorID actorID, flownet::POINT currentPosition, flownet::POINT destinationPosition);
    void ActorAttack(flownet::ActorID attackerActorID, flownet::ActorID targetActorID);
    void ActorBeginCast(flownet::ActorID casterActorID, flownet::SpellType spellType, flownet::POINT destination);
    void ActorEndCast(flownet::ActorID invokerActorID, flownet::SpellType spellType, flownet::POINT destination);
    void ActorAttacked(flownet::ActorID attackedActorID, flownet::ActorID attackerActorID);
    void ActorDead(flownet::ActorID deadActorID, bool afterDelete = false); // TO DO : when delete is default after die, change this false to true
    void ActorTakeDamage(flownet::ActorID actorID);

    void ActorNodeReload(flownet::ActorID actorID);

// spell part
    void FireSpell(flownet::ActorID playerID, flownet::POINT destination, SpellInfo spellInfo);
    
    void AddSpellEffect(flownet::ActorID actorID, flownet::SpellAbility spellAbility);
    void RemoveSpellEffect(flownet::ActorID actorID, flownet::SpellAbility spellAbility);
// end of spell part

    void UseItem(flownet::ActorID playerID, flownet::ItemID itemID);
    void AddNewItem(flownet::Item item, flownet::POINT spawnPosition);
    void RemoveItem(CCObject* itemNode);
    void PickupItemFromField(flownet::ActorID playerID, flownet::ItemID itemID);
};


#endif /* defined(__GameClientMobile__ActorLayer__) */
