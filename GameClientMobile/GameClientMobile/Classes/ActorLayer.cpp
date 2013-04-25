//
//  ActorLayer.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 3/19/13.
//
//

#include "Headers.pch"

ActorLayer::ActorLayer(): m_PlayerNodeMap(), m_MonsterNodeMap(), m_ItemNodeMap()
{
    
}

ActorLayer::~ActorLayer()
{
    std::for_each(this->m_PlayerNodeMap.begin(), this->m_PlayerNodeMap.end(), [this](PlayerNodeMap::value_type pair){
        ActorNode* node = pair.second;
        this->removeChild(node, true);
        delete node;
    });
    
    this->m_PlayerNodeMap.clear();
    std::for_each(this->m_MonsterNodeMap.begin(), this->m_MonsterNodeMap.end(), [this](MonsterNodeMap::value_type pair){
        ActorNode* node = pair.second;
        this->removeChild(node, true);
        delete node;
    });
    this->m_MonsterNodeMap.clear();
    
    std::for_each(this->m_ItemNodeMap.begin(), this->m_ItemNodeMap.end(), [this](ItemNodeMap::value_type pair){
        ItemNode* node = pair.second;
        this->removeChild(node);
        delete node;
    });
    this->m_ItemNodeMap.clear();
}

bool ActorLayer::init()
{
    if(!BaseLayer::init())
    {
        return false;
    }
    CCLOG("actor layer created");
    
    // NOTE : initialize players
    PlayerMap& playerMap = GameClient::Instance().GetPlayerMap();
    
    std::for_each(playerMap.begin(), playerMap.end(), [this](PlayerMap::value_type playerInfoPair){
        ActorID playerID = playerInfoPair.first;
        Player* playerInfo = playerInfoPair.second;
        Player* myPlayerInfo = GameClient::Instance().GetClientStage()->FindPlayer(GameClient::Instance().GetMyActorID());

        PlayerNode* newPlayerNode = new PlayerNode(playerID);
        
        newPlayerNode->setPosition(PointConverter::Convert(playerInfo->GetCurrentPosition()));
        
        // NOTE : if this player is me, then highlight it
        if(playerID == myPlayerInfo->GetActorID())
        {
            newPlayerNode->HighLight();
        }
        
        this->AddPlayerNode(playerID, newPlayerNode);
    });
    
    // NOTE : initialize monsters
    MonsterMap& monsterMap = GameClient::Instance().GetMonsterMap();

    std::for_each(monsterMap.begin(), monsterMap.end(), [this](MonsterMap::value_type monsterInfoPair){
        ActorID monsterID = monsterInfoPair.first;
        Monster* monsterInfo = monsterInfoPair.second;
        
        MonsterNode* newMonsterNode = new MonsterNode(monsterID);
        
        newMonsterNode->setPosition(PointConverter::Convert(monsterInfo->GetCurrentPosition()));

        this->AddMonsterNode(monsterID, newMonsterNode);
    });

    scheduleUpdate();
    schedule( schedule_selector(ActorLayer::SortNodes) );
    
    return true;
}

ActorLayer* ActorLayer::create(StageType stageType)
{
    ActorLayer* actorLayer = new ActorLayer();
   
    if(actorLayer && actorLayer->init())
    {
        return actorLayer;
    }
    else
    {
        delete actorLayer;
        actorLayer = nullptr;
        return nullptr;
    }
}

void ActorLayer::update(float deltaTime)
{

}

void ActorLayer::AddPlayerNode(ActorID playerID, PlayerNode* playerNode)
{
    PlayerNodeMap::iterator iter = this->m_PlayerNodeMap.find(playerID);
    
    ASSERT_DEBUG(iter == this->m_PlayerNodeMap.end());
    
    this->m_PlayerNodeMap.insert(PlayerNodeMap::value_type(playerID, playerNode));
    this->addChild(playerNode);
}

void ActorLayer::DeletePlayerNode(ActorID playerID)
{
    PlayerNodeMap::iterator iter = this->m_PlayerNodeMap.find(playerID);
    
    ASSERT_DEBUG(iter != this->m_PlayerNodeMap.end());
    
    ActorNode* playerNode = iter->second;
    
    this->m_PlayerNodeMap.erase(iter);
   
    delete playerNode;
}

PlayerNode* ActorLayer::FindPlayerNode(ActorID playerID)
{
    PlayerNodeMap::iterator iter = this->m_PlayerNodeMap.find(playerID);
    if(iter == this->m_PlayerNodeMap.end())
    {
        return nullptr;
    }
    return static_cast<PlayerNode*>(iter->second);
}

void ActorLayer::AddMonsterNode(ActorID monsterID, MonsterNode* monsterNode)
{
    MonsterNodeMap::iterator iter = this->m_MonsterNodeMap.find(monsterID);
    
    ASSERT_DEBUG(iter == this->m_MonsterNodeMap.end());
    
    this->m_MonsterNodeMap.insert(MonsterNodeMap::value_type(monsterID, monsterNode));
    this->addChild(monsterNode);
}

void ActorLayer::DeleteMonsterNode(ActorID monsterID)
{
    MonsterNodeMap::iterator iter = this->m_MonsterNodeMap.find(monsterID);
    
    ASSERT_DEBUG(iter != this->m_MonsterNodeMap.end());
    
    ActorNode* monsterNode = iter->second;
    
    this->m_MonsterNodeMap.erase(iter);
   
    delete monsterNode;
}

MonsterNode* ActorLayer::FindMonsterNode(ActorID monsterID)
{
    MonsterNodeMap::iterator iter = this->m_MonsterNodeMap.find(monsterID);
    if(iter == this->m_MonsterNodeMap.end())
    {
        return nullptr;
    }
    return static_cast<MonsterNode*>(iter->second);
}


ActorNode* ActorLayer::FindActorNode(ActorID actorID)
{
    if(IsMonsterID(actorID))
    {
        return this->FindMonsterNode(actorID);
    }
    if(IsPlayerID(actorID))
    {
        return this->FindPlayerNode(actorID);
    }
    return nullptr;
}

ItemNode* ActorLayer::FindItemNode(ItemID itemID)
{
    ItemNodeMap::iterator iter = this->m_ItemNodeMap.find(itemID);
    if(iter == this->m_ItemNodeMap.end())
    {
        return nullptr;
    }

    return iter->second;
}

void ActorLayer::UpdateActorLookingDirection(flownet::Actor *actor, cocos2d::CCPoint actorPoint, cocos2d::CCPoint lookingPoint)
{
    if(actorPoint.x > lookingPoint.x) // actor is looking left
    {
        actor->SetLookingDirection(DIRECTION_LEFT);
    }
    else
    {
        actor->SetLookingDirection(DIRECTION_RIGHT);
    }
}

void ActorLayer::SortNodes()
{
    if(this->m_pChildren && this->m_pChildren->count() > 0)
    {
        Vector<ActorNode*>::type actorNodes;
        CCObject* object;
        CCARRAY_FOREACH(this->m_pChildren, object)
        {
            ActorNode* actorNode = dynamic_cast<ActorNode*>(object);
            if(actorNode)
            {
                actorNodes.push_back(actorNode);
            }
        }
        std::sort(actorNodes.begin(), actorNodes.end(), [](ActorNode* lhs, ActorNode* rhs)->bool{
            return lhs->getPositionY() > rhs->getPositionY();
        });
        
        int zOrder = 0;
        std::for_each(actorNodes.begin(), actorNodes.end(), [this, &zOrder](ActorNode* actorNode){
            this->reorderChild(actorNode, zOrder++);
        });
    }
}


void ActorLayer::AddNewPlayer(flownet::ClientPlayer player)
{
    PlayerNode* newObject = new PlayerNode(player.GetActorID());
    newObject->setPosition(PointConverter::Convert(player.GetCurrentPosition()));
    
    newObject->StopAnimationActions();
    player.ChangeToIdleState();
    this->AddPlayerNode(player.GetActorID(), newObject);
}

void ActorLayer::AddNewMonster(flownet::ClientMonster monster)
{
    MonsterNode* newObject = new MonsterNode(monster.GetActorID());
    newObject->setPosition(PointConverter::Convert(monster.GetCurrentPosition()));
    
    newObject->StopAnimationActions();
    monster.ChangeToIdleState();
    this->AddMonsterNode(monster.GetActorID(), newObject);
}

void ActorLayer::ChangeTarget(flownet::ActorID monsterID, flownet::ActorID targetPlayerID)
{
    ClientMonster* monster = static_cast<ClientMonster*>(GameClient::Instance().GetClientStage()->FindMonster(monsterID));
    ActorNode* monsterObject = this->FindMonsterNode(monsterID);
    ActorNode* targetObject = this->FindPlayerNode(targetPlayerID);
    
    // To Do : change target
}

void ActorLayer::MoveActor(flownet::ActorID actorID, flownet::POINT currentPosition, flownet::POINT destinationPosition)
{
    Actor* actor = GameClient::Instance().GetClientStage()->FindActor(actorID);
    ActorNode* movingObject = this->FindActorNode(actorID);
    
    if( !actor->IsAlive() )
    {
        CCLOG("ignore actor move request. actor is dead");
        return;
    }

    float distance = destinationPosition.DistanceTo(currentPosition);
    ASSERT_DEBUG(actor->GetMovingSpeed() != 0);
    float duration = distance / actor->GetMovingSpeed() ;

    this->UpdateActorLookingDirection(actor, movingObject->getPosition(), PointConverter::Convert(destinationPosition));
    movingObject->StopAnimationActions();
    
    CCFiniteTimeAction* animateMove = CCCallFunc::create(movingObject, callfunc_selector(ActorNode::AnimateMoving));
    CCFiniteTimeAction* actionMove = CCMoveTo::create(duration, PointConverter::Convert(destinationPosition));
    CCFiniteTimeAction* actionMoveDone = CCCallFuncN::create( movingObject, callfuncN_selector(ActorNode::AnimateIdle));
    CCFiniteTimeAction* changeToIdleState = nullptr;
    CCAction* sequence = nullptr;
    
    if(IsMonsterID(actorID))
    {
        ClientMonster* monster = static_cast<ClientMonster*>(actor);
        monster->ChangeToMovingState();
        changeToIdleState = CCCallFunc::create(monster, callfunc_selector(ClientMonster::ChangeToIdleState));
        sequence = CCSequence::create(animateMove, actionMove, actionMoveDone, changeToIdleState, NULL);
    }
    
    if(IsPlayerID(actorID))
    {
        ClientPlayer* player = static_cast<ClientPlayer*>(actor);
        player->ChangeToMovingState();
        changeToIdleState = CCCallFunc::create(player, callfunc_selector(ClientPlayer::ChangeToIdleState));
        sequence = CCSequence::create(animateMove, actionMove, actionMoveDone, changeToIdleState, NULL);
    }
    
    ASSERT_DEBUG(sequence != nullptr);
    ASSERT_DEBUG(changeToIdleState != nullptr);
    
    // TO DO : show moving point
    
    sequence->setTag(ActionType_Animation);
    movingObject->runAction(sequence);
}

void ActorLayer::ActorAttack(flownet::ActorID attackerActorID, flownet::ActorID targetActorID)
{
    Actor* actor = GameClient::Instance().GetClientStage()->FindActor(attackerActorID);
    ActorNode* attackingObject = this->FindActorNode(attackerActorID);
    ActorNode* targetObject = this->FindActorNode(targetActorID);
    
    if( !actor->IsAlive() )
    {
        CCLOG("ignore actor's attack request. actor is dead");
        return;
    }

    CCFiniteTimeAction* changeToIdleState = nullptr;
    if(IsMonsterID(attackerActorID))
    {
        ClientMonster* monster = static_cast<ClientMonster*>(actor);
        monster->ChangeToAttackingState();
        changeToIdleState = CCCallFunc::create(monster, callfunc_selector(ClientMonster::ChangeToIdleState));
    }
    
    if(IsPlayerID(attackerActorID))
    {
        ClientPlayer* player = static_cast<ClientPlayer*>(actor);
        player->ChangeToAttackingState();
        changeToIdleState = CCCallFunc::create(player, callfunc_selector(ClientPlayer::ChangeToIdleState));
    }
    
    ASSERT_DEBUG(actor->GetAttackSpeed() != 0);

    float attackDuration = 1 / actor->GetAttackSpeed();

    attackingObject->StopAnimationActions();
    this->UpdateActorLookingDirection(actor, attackingObject->getPosition(), targetObject->getPosition());
    CCFiniteTimeAction* animateAttack = CCCallFunc::create(attackingObject, callfunc_selector(ActorNode::AnimateAttacking));
    CCFiniteTimeAction* timeDuration = CCDelayTime::create(attackDuration);
    CCFiniteTimeAction* animateIdle = CCCallFunc::create(attackingObject, callfunc_selector(ActorNode::AnimateIdle));
    CCAction* sequence = CCSequence::create(animateAttack, timeDuration, animateIdle, changeToIdleState, NULL);
    sequence->setTag(ActionType_Animation);
    attackingObject->runAction(sequence);
}

void ActorLayer::ActorBeginCast(flownet::ActorID casterActorID, flownet::SpellType spellType, flownet::POINT destination)
{
    Actor* actor = GameClient::Instance().GetClientStage()->FindActor(casterActorID);
    ActorNode* castingObject = this->FindPlayerNode(casterActorID);

    if( !actor->IsAlive() )
    {
        CCLOG("ignore actor's cast request. actor is dead");
        return;
    }
    
    if(IsMonsterID(casterActorID))
    {
        ClientMonster* monster = static_cast<ClientMonster*>(actor);
        monster->ChangeToCastingState();
    }
    
    if(IsPlayerID(casterActorID))
    {
        ClientPlayer*player = static_cast<ClientPlayer*>(actor);
        player->ChangeToCastingState();
    }
    
    castingObject->StopAnimationActions();
    CCFiniteTimeAction* animateBeginCasting = CCCallFunc::create(castingObject, callfunc_selector(ActorNode::AnimateBeginCasting));
    CCDelayTime* delay = CCDelayTime::create(0.2);
    CCFiniteTimeAction* animateRepeatCasting = CCCallFunc::create(castingObject, callfunc_selector(ActorNode::AnimateRepeatCasting));
    CCAction* sequence = CCSequence::create(animateBeginCasting, delay, animateRepeatCasting, NULL);
    sequence->setTag(ActionType_Animation);
    castingObject->runAction(sequence);
    
    // TO DO : show spell guide line and marker
    castingObject->ShowSpellGuide(spellType, PointConverter::Convert(destination));
}

void ActorLayer::ActorEndCast(flownet::ActorID invokerActorID, flownet::SpellType spellType, flownet::POINT destination)
{
    Actor* actor = GameClient::Instance().GetClientStage()->FindActor(invokerActorID);
    ActorNode* invokerObject = this->FindPlayerNode(invokerActorID);
    
    SpellInfo spellInfo = SpellDictionary::Instance().FindSpellInfoBySpellType(spellType);
    
    if( !actor->IsAlive() )
    {
        CCLOG("ignore fire spell request. player is dead");
        return;
    }
   
    CCFiniteTimeAction* changeToIdleState = nullptr;
    
    if(IsMonsterID(invokerActorID))
    {
        ClientMonster* monster = static_cast<ClientMonster*>(actor);
        changeToIdleState = CCCallFunc::create(monster, callfunc_selector(ClientMonster::ChangeToIdleState));
    }
    
    if(IsPlayerID(invokerActorID))
    {
        ClientPlayer* player = static_cast<ClientPlayer*>(actor);
        changeToIdleState = CCCallFunc::create(player, callfunc_selector(ClientPlayer::ChangeToIdleState));
    }
    
    this->UpdateActorLookingDirection(actor, invokerObject->getPosition(), PointConverter::Convert(destination));
    invokerObject->StopAnimationActions();
    CCFiniteTimeAction* animateFire = CCCallFunc::create(invokerObject, callfunc_selector(ActorNode::AnimateFire));
    CCFiniteTimeAction* timeDuration = CCDelayTime::create(Player_Fire_Duration);
    CCFiniteTimeAction* animateFireDone = CCCallFunc::create(invokerObject, callfunc_selector(ActorNode::AnimateIdle));
    CCAction* sequence = CCSequence::create(animateFire, timeDuration, animateFireDone, changeToIdleState, NULL);
    sequence->setTag(ActionType_Animation);
    invokerObject->runAction(sequence);
    
    invokerObject->HideSpellGuide();
    
    this->FireSpell(invokerActorID, destination, spellInfo);
}

void ActorLayer::ActorAttacked(flownet::ActorID attackedActorID, flownet::ActorID attackerActorID)
{
    Actor* actor = GameClient::Instance().GetClientStage()->FindActor(attackedActorID);
    
    if( !actor->IsAlive() )
    {
        CCLOG("ignore player attacked request. player is dead");
        return;
    }

    if(actor->GetActorState() == ActorState_Idle)
    {
        ActorNode* attackedObject = this->FindActorNode(attackedActorID);
        attackedObject->StopAnimationActions();
        CCFiniteTimeAction* animateAttacked = CCCallFunc::create(attackedObject, callfunc_selector(ActorNode::AnimateAttacked));
        CCFiniteTimeAction* timeDuration = CCDelayTime::create(Player_Stiff_Duration);
        CCFiniteTimeAction* animateAttackedDone = CCCallFunc::create(attackedObject, callfunc_selector(ActorNode::AnimateIdle));
        CCAction* sequence = CCSequence::create(animateAttacked, timeDuration, animateAttackedDone, NULL);
        sequence->setTag(ActionType_Animation);
        attackedObject->runAction(sequence);
    }
}

void ActorLayer::ActorDead(flownet::ActorID deadActorID, bool afterDelete)
{
    Actor* actor = GameClient::Instance().GetClientStage()->FindActor(deadActorID);
    MonsterNode* deadObject = this->FindMonsterNode(deadActorID);
    
    if(IsMonsterID(deadActorID))
    {
        static_cast<ClientMonster*>(actor)->ChangeToDeadState();
    }
    
    if(IsPlayerID(deadActorID))
    {
        static_cast<ClientPlayer*>(actor)->ChangeToDeadState();
    }

    deadObject->StopAnimationActions();
    CCFiniteTimeAction* animateDead = CCCallFunc::create(deadObject, callfunc_selector(ActorNode::AnimateDead));
    CCAction* sequence = CCSequence::create(animateDead, NULL);
    sequence->setTag(ActionType_Animation);
    deadObject->runAction(sequence);

}

void ActorLayer::ActorTakeDamage(flownet::ActorID actorID)
{
    Actor* actor = GameClient::Instance().GetClientStage()->FindActor(actorID);
    ASSERT_DEBUG(actor != nullptr);
    
    ActorNode* actorNode = this->FindActorNode(actorID);
    ASSERT_DEBUG(actorNode != nullptr);
    
    ASSERT_DEBUG(actor->GetMaxHealthPoint() != 0); // just checking for division by zero exception, will be this happened?
    // TO DO : make him to set the max health point now
    
    actorNode->ChangeHealthPointBar(actor->GetHealthPoint() / actor->GetMaxHealthPoint());
}

MonsterNode* ActorLayer::FindSelectedMonsterNode(cocos2d::CCPoint touchLocation)
{
    ActorNode* foundNode = nullptr;
    std::for_each(this->m_MonsterNodeMap.begin(), this->m_MonsterNodeMap.end(), [this, touchLocation, &foundNode](MonsterNodeMap::value_type monsterNodeMapPair){
        ActorNode* actorNode = monsterNodeMapPair.second;
        
        CCRect actorNodeRect = actorNode->GetRect();
        actorNodeRect.origin = CCPointZero;
        CCPoint actorNodeLocalPoint = actorNode->convertToNodeSpace(touchLocation);
        // NOTE : offset adding to the localPoint. ActorNodes have anchor point on 0.5, 0
        actorNodeLocalPoint.x = actorNodeLocalPoint.x + (actorNodeRect.size.width * 0.5);
    
        if(actorNodeRect.containsPoint(actorNodeLocalPoint) && actorNode->GetActorInfo()->IsAlive())
        {
            foundNode = actorNode;
        }
    });
    
    return static_cast<MonsterNode*>(foundNode);
}

ItemNode* ActorLayer::FindSelectedItemNode(cocos2d::CCPoint touchLocation)
{
    ItemNode* foundNode = nullptr;
    std::for_each(this->m_ItemNodeMap.begin(), this->m_ItemNodeMap.end(), [this, touchLocation, &foundNode](ItemNodeMap::value_type itemNodeMapPair){
        ItemNode* itemNode = itemNodeMapPair.second;
        
        CCRect itemNodeRect = GetRect(itemNode);
    
        if(itemNodeRect.containsPoint(touchLocation) && GameClient::Instance().GetClientStage()->FindActor(GameClient::Instance().GetMyActorID())->IsAlive())
        {
            foundNode = itemNode;
        }
    });
    
    return foundNode;
}

void ActorLayer::FireSpell(flownet::ActorID playerID, flownet::POINT destination, flownet::SpellInfo spellInfo)
{
    SpellNode* spellObject = SpellNode::create(spellInfo, playerID, destination);
    this->addChild(spellObject);
}

void ActorLayer::UseItem(flownet::ActorID playerID, flownet::ItemID itemID, flownet::InventorySlot inventorySlot)
{
    // TO DO : make some effect on ActorNode
}

void ActorLayer::AddNewItem(flownet::Item item, flownet::POINT spawnPosition)
{
    ItemNode* itemNode = ItemNode::create(item.GetItemType(), item.GetItemID());
    // 중복된 것인지 한번 체크해 볼 필요는 있는듯
    this->m_ItemNodeMap.insert(ItemNodeMap::value_type(item.GetItemID(), itemNode));
    
    itemNode->setPosition(PointConverter::Convert(spawnPosition));
    this->addChild(itemNode);
}

void ActorLayer::RemoveItem(CCObject* object)
{
    ItemNode* itemNode = dynamic_cast<ItemNode*>(object);
    if(!itemNode)
    {
        ASSERT_DEBUG(itemNode != nullptr);
    }

    this->m_ItemNodeMap.erase(itemNode->GetItemID());
    this->removeChild(itemNode, true);
    delete itemNode;
}

void ActorLayer::PickupItemFromField(flownet::ActorID playerID, flownet::ItemID itemID)
{
    ActorNode* actor = this->FindActorNode(playerID);
    ItemNode* item = this->FindItemNode(itemID);
    CCJumpTo* jumpTo = CCJumpTo::create(0.3, actor->getPosition(), 30, 1);
    CCDelayTime* delay = CCDelayTime::create(0.3);
    CCCallFuncO* remove = CCCallFuncO::create(this, callfuncO_selector(ActorLayer::RemoveItem), item);
    CCSequence* sequence = CCSequence::create(jumpTo, delay, remove, NULL);
    sequence->setTag(ActionType_Animation);
    item->runAction(sequence);
    // TO DO : display effect for acquiring item
}
