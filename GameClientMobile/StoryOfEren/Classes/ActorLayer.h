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

struct ActorNodeSet : public CCNode {
    int m_ZOrder;
    ActorID m_ActorID;
    ActorNode* m_ActorNode;
    HUDNode* m_HUDNode;
    ShadowNode* m_ShadowNode;
    HighlightNode* m_HighlightNode;
    GuideLineNode* m_GuideLineNode;
    ChatBalloonNode* m_ChatBalloonNode;
    SpellEffectNodeMap m_SpellEffectNodeMap;

    
    ActorNodeSet(): m_ZOrder(0), m_ActorID(ActorID_None), m_ActorNode(nullptr), m_HUDNode(nullptr), m_ShadowNode(nullptr), m_HighlightNode(nullptr), m_GuideLineNode(nullptr), m_ChatBalloonNode(nullptr), m_SpellEffectNodeMap()
    {
     
    }
    
    ~ActorNodeSet()
    {
        this->RemoveActorNode();
        this->RemoveHUDNode();
        this->RemoveShadowNode();
        this->RemoveHighlightNode();
        this->RemoveGuideLineNode();
        this->RemoveChatBalloonNode();
//        std::for_each(this->m_SpellEffectNodeMap.begin(), this->m_SpellEffectNodeMap.end(), [this](SpellEffectNodeMap::value_type pair){
//            pair.second->release();
//        });
//        this->m_SpellEffectNodeMap.clear();
    }

    bool init()
    {
        this->AddActorNode(this->m_ActorID);
        this->AddChatBalloonNode(this->m_ActorID);
        
        return true;
    }
    
    static ActorNodeSet* create(flownet::ActorID actorID)
    {
        ActorNodeSet* nodeSet = new ActorNodeSet();
        nodeSet->m_ActorID = actorID;
        
        if(nodeSet && nodeSet->init())
        {
            nodeSet->autorelease();
            return nodeSet;
        }
        else
        {
            delete nodeSet;
            return nullptr;
        }
    }
    
    void SetZOrder(int zOrder)
    {
        this->m_ZOrder = zOrder;
        if(this->m_ActorNode) this->m_ActorNode->setZOrder(zOrder);
        if(this->m_HUDNode) this->m_HUDNode->setZOrder(zOrder + 9);
        if(this->m_ShadowNode) this->m_ShadowNode->setZOrder(zOrder - 9);
        if(this->m_HighlightNode) this->m_HighlightNode->setZOrder(zOrder - 8);
        if(this->m_ChatBalloonNode) this->m_ChatBalloonNode->setZOrder(zOrder + 9);

        std::for_each(this->m_SpellEffectNodeMap.begin(), this->m_SpellEffectNodeMap.end(), [this, zOrder](SpellEffectNodeMap::value_type pair){
            SpellEffectNode* node = pair.second;
            if(node->IsOverTheCharacter())
                node->setZOrder(zOrder+1);
            else
                node->setZOrder(zOrder-1);
        });
    }
    
    int GetZOrder()
    {
        return this->m_ZOrder;
    }
    
    void SetVisible(bool visibility)
    {
        if(this->m_ActorNode) this->m_ActorNode->setVisible(visibility);
        if(this->m_HUDNode) this->m_HUDNode->setVisible(visibility);
        if(this->m_ShadowNode) this->m_ShadowNode->setVisible(visibility);
        if(this->m_HighlightNode) this->m_HighlightNode->setVisible(visibility);
        if(this->m_ChatBalloonNode) this->m_ChatBalloonNode->setVisible(visibility);
    }
    
    bool IsVisible()
    {
        if(this->m_ActorNode) this->m_ActorNode->isVisible();
    }
    
    void AddActorNode(flownet::ActorID actorID)
    {
        this->RemoveActorNode();
        if(IsPlayerID(actorID))
        {
            this->m_ActorNode = PlayerNode::create(actorID);
            this->m_ActorNode->Reload(); // 플레이어는 리로드 해줘야함
        }
        else if(IsMonsterID(actorID))
        {
            this->m_ActorNode = MonsterNode::create(actorID);
        }
        else if(IsNPCID(actorID))
        {
            this->m_ActorNode = NPCNode::create(actorID);
        }
        else if(IsStageObjectID(actorID))
        {
            this->m_ActorNode = StageObjectNode::create(actorID);
        }
        else
        {
            ASSERT_DEBUG(false);
        }
        this->m_ActorNode->retain();
        
        CCLOG("this->m_ActorNode %ld, %p", actorID, m_ActorNode);
    }
    
    void RemoveActorNode()
    {
        if(this->m_ActorNode)
        {
            this->m_ActorNode->removeFromParentAndCleanup(true);
            this->m_ActorNode->release();
            this->m_ActorNode = nullptr;
        }
    }
    
    void AddHUDNode(flownet::ActorID actorID)
    {
        this->RemoveHUDNode();
        this->m_HUDNode = HUDNode::create(actorID);
        this->m_HUDNode->retain();
    }
    
    void RemoveHUDNode()
    {
        if(this->m_HUDNode)
        {
            this->m_HUDNode->removeFromParentAndCleanup(true);
            this->m_HUDNode->release();
            this->m_HUDNode = nullptr;
        }
    }

    void AddShadowNode(flownet::ActorID actorID)
    {
        this->RemoveShadowNode();
        this->m_ShadowNode = ShadowNode::create(actorID);
        this->m_ShadowNode->retain();
    }
    
    void RemoveShadowNode()
    {
        if(this->m_ShadowNode)
        {
            this->m_ShadowNode->removeFromParentAndCleanup(true);
            this->m_ShadowNode->release();
            this->m_ShadowNode = nullptr;
        }   
    }

    void AddHighlightNode(flownet::ActorID actorID)
    {
        this->RemoveHighlightNode();
        this->m_HighlightNode = HighlightNode::create(actorID);
        this->m_HighlightNode->retain();
    }
    
    void RemoveHighlightNode()
    {
        if(this->m_HighlightNode)
        {
            this->m_HighlightNode->removeFromParentAndCleanup(true);
            this->m_HighlightNode->release();
            this->m_HighlightNode = nullptr;
        }
    }

    void AddGuideLineNode(flownet::SpellType spellType, CCPoint destination)
    {
        this->RemoveGuideLineNode();
        int zOrder = this->GetZOrder();
        CCPoint source = this->m_ActorNode->getPosition();
        this->m_GuideLineNode = GuideLineNode::create(spellType, source, destination);
        this->m_GuideLineNode->retain();
        this->m_GuideLineNode->setZOrder(zOrder - 1); // TO DO : change here
    }
    
    void RemoveGuideLineNode()
    {
        if(this->m_GuideLineNode)
        {
            this->m_GuideLineNode->removeFromParent();
            this->m_GuideLineNode->release();
            this->m_GuideLineNode = nullptr;
        }
    }
    
    void AddChatBalloonNode(flownet::ActorID senderID)
    {
        this->RemoveChatBalloonNode();
        int zOrder = this->GetZOrder();
        CCPoint source = this->m_ActorNode->getPosition();
        this->m_ChatBalloonNode = ChatBalloonNode::create(senderID);
        this->m_ChatBalloonNode->retain();
        this->m_ChatBalloonNode->setZOrder(zOrder + 9);
    }
    
    void RemoveChatBalloonNode()
    {
        if(this->m_ChatBalloonNode)
        {
            this->m_ChatBalloonNode->removeFromParent();
            this->m_ChatBalloonNode->release();
            this->m_ChatBalloonNode = nullptr;
        }
    }
    
    // NOTE : return value of this function used for adding to parent
    SpellEffectNode* AddSpellEffectNode(flownet::ActorID actorID, flownet::SpellAbility spellAbility)
    {
        SpellEffectNode* spellEffectNode = SpellEffectNode::create(actorID, spellAbility);
        if(!spellEffectNode) return nullptr;
        spellEffectNode->retain();
    
        this->m_SpellEffectNodeMap.insert(SpellEffectNodeMap::value_type(spellAbility, spellEffectNode));
        
        return spellEffectNode;
    }
    
    // NOTE : return value of this function used for removing from parent
    SpellEffectNode* RemoveSpellEffectNode(flownet::SpellAbility spellAbility)
    {
        SpellEffectNodeMap::iterator iter = this->m_SpellEffectNodeMap.find(spellAbility);
        if(iter == this->m_SpellEffectNodeMap.end()) return nullptr;
        this->m_SpellEffectNodeMap.erase(iter);

        iter->second->release();
        
        return iter->second;
    }
    
    void Hide()
    {
        if(this->m_ActorNode) this->m_ActorNode->setVisible(false);
        if(this->m_HUDNode) this->m_HUDNode->setVisible(false);
        if(this->m_HighlightNode) this->m_HighlightNode->setVisible(false);
        if(this->m_GuideLineNode) this->m_GuideLineNode->setVisible(false);
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
    
//    void DeleteActorNode(CCObject* actorNodeSet);
    
    void SortNodes();

// Util functions
    void UpdateActorLookingDirection(Actor* actor, CCPoint actorPoint, CCPoint lookingPoint);

// Actor part
    void ChangeTarget(flownet::ActorID monsterID, flownet::ActorID targetPlayerID);

    MonsterNode* FindSelectedMonsterNode(CCPoint touchLocation);
    ItemNode* FindSelectedItemNode(CCPoint touchLocation);
    
    void AddNewPlayer(flownet::ClientPlayer player);
    void AddNewMonster(flownet::ClientMonster monster);
    void AddNewNPC(flownet::NPC npc);
    void AddNewStageObject(flownet::StageObject stageObject);
    void MoveActor(flownet::ActorID actorID, flownet::POINT currentPosition, flownet::POINT destinationPosition);
    void TeleportActor(flownet::ActorID actorID, flownet::POINT currentPosition, flownet::POINT destinationPosition);
    void KnockBackActor(flownet::ActorID actorID, flownet::POINT currentPosition, flownet::POINT knockbackDestination);
    void ActorAttack(flownet::ActorID attackerActorID, flownet::ActorID targetActorID);
    void ActorBeginCast(flownet::ActorID casterActorID, flownet::SpellType spellType, flownet::POINT destination);
    void ActorEndCast(flownet::ActorID invokerActorID, flownet::SpellType spellType, flownet::POINT destination);
    void ActorAttacked(flownet::ActorID attackedActorID, flownet::ActorID attackerActorID);
    void ActorDead(flownet::ActorID deadActorID, bool afterDelete = false); // TO DO : when delete is default after die, change this false to true
    void ActorTakeDamage(flownet::ActorID actorID);
    void ActorConsumedMana(flownet::ActorID actorID);
    void ActorRunOutOfMana(flownet::ActorID actorID);
    void ActorTeamChanged(flownet::ActorID actorID);

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
    
    void MessageReceived(flownet::ActorID senderID, flownet::STRING message);
    
private:
    void AddSpellGuideLine(flownet::ActorID actorID, flownet::SpellType spellType, flownet::POINT destination);
    void RemoveSpellGuideLine(flownet::ActorID actorID);
    
    void ChangeActorStateToIdle(CCObject* object);
    void ChangeActorStateToAttacking(CCObject* object);
    void ChangeActorStateToCasting(CCObject* object);
    void ChangeActorStateToDead(CCObject* object);
    void ChangeActorStateToMoving(CCObject* object);
};


#endif /* defined(__GameClientMobile__ActorLayer__) */
