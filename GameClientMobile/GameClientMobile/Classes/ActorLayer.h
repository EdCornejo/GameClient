//
//  ActorLayer.h
//  GameClientMobile
//
//  Created by SungJinYoo on 3/19/13.
//
//

#ifndef __GameClientMobile__ActorLayer__
#define __GameClientMobile__ActorLayer__

class ActorLayer : public BaseLayer
{
    typedef flownet::Map<flownet::ActorID, ActorNode*>::type PlayerNodeMap;
    typedef flownet::Map<flownet::ActorID, ActorNode*>::type MonsterNodeMap;
    typedef flownet::Map<flownet::ItemID, ItemNode*>::type ItemNodeMap;

private:
    PlayerNodeMap m_PlayerNodeMap;
    MonsterNodeMap m_MonsterNodeMap;
    ItemNodeMap m_ItemNodeMap;
    
public:
    ActorLayer();
    virtual ~ActorLayer();
    
public:
    virtual bool init();
    static ActorLayer* create(StageType stageType);

    virtual void update(float deltaTime) override;

// Node Managing
public:
    void AddPlayerNode(ActorID playerID, PlayerNode* playerNode);
    void DeletePlayerNode(ActorID playerID);
    PlayerNode* FindPlayerNode(ActorID playerID);
    
    void AddMonsterNode(ActorID monsterID, MonsterNode* monsterNode);
    void DeleteMonsterNode(ActorID monsterID);
    MonsterNode* FindMonsterNode(ActorID monsterID);
    
    ActorNode* FindActorNode(ActorID actorID);
    ItemNode* FindItemNode(ItemID itemID);
    
    void SortNodes();
// end of Node Managing

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


// spell part
    void FireSpell(flownet::ActorID playerID, flownet::POINT destination, SpellInfo spellInfo);
// end of spell part
    void UseItem(flownet::ActorID playerID, flownet::ItemID itemID, flownet::InventorySlot inventorySlot);
    void AddNewItem(flownet::Item item, flownet::POINT spawnPosition);
    void RemoveItem(CCObject* itemNode);
    void ItemPickuped(flownet::ActorID playerID, flownet::ItemID itemID);

};


#endif /* defined(__GameClientMobile__ActorLayer__) */
