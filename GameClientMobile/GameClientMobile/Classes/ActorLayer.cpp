//
//  ActorLayer.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 3/19/13.
//
//

#include "Headers.pch"

ActorLayer::ActorLayer(): m_ActorNodeSetMap(), m_ItemNodeMap()
{
    
}

ActorLayer::~ActorLayer()
{
    std::for_each(this->m_ActorNodeSetMap.begin(), this->m_ActorNodeSetMap.end(), [this](ActorNodeSetMap::value_type pair){
        ActorNodeSet* actorNodeSet = pair.second;

        if(actorNodeSet->m_ActorNode)
        {
            this->removeChild(actorNodeSet->m_ActorNode, true);
            actorNodeSet->m_ActorNode->release();
        }
        if(actorNodeSet->m_HUDNode)
        {
            this->removeChild(actorNodeSet->m_HUDNode, true);
            actorNodeSet->m_HUDNode->release();
        }
        if(actorNodeSet->m_ShadowNode)
        {
            this->removeChild(actorNodeSet->m_ShadowNode, true);
            actorNodeSet->m_ShadowNode->release();
        }
        if(actorNodeSet->m_HighlightNode)
        {
            this->removeChild(actorNodeSet->m_HighlightNode, true);
            actorNodeSet->m_HighlightNode->release();
        }
        
        delete actorNodeSet;
        
        pair.second = nullptr;
    });
    
    this->m_ActorNodeSetMap.clear();
    
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
        
        ActorNodeSet* actorNodeSet = new ActorNodeSet(playerID);

        this->m_ActorNodeSetMap.insert(ActorNodeSetMap::value_type(playerID, actorNodeSet));
        
        actorNodeSet->m_ActorNode->setPosition(PointConverter::Convert(playerInfo->GetCurrentPosition()));
        
        if(playerID == GameClient::Instance().GetMyActorID())
        {
            actorNodeSet->m_HighlightNode = HighlightNode::create(playerID);
            actorNodeSet->m_HighlightNode->retain();
        }
        
        if(actorNodeSet->m_ActorNode) this->addChild(actorNodeSet->m_ActorNode);
        if(actorNodeSet->m_HUDNode) this->addChild(actorNodeSet->m_HUDNode);
        if(actorNodeSet->m_ShadowNode) this->addChild(actorNodeSet->m_ShadowNode);
        if(actorNodeSet->m_HighlightNode) this->addChild(actorNodeSet->m_HighlightNode);
    });
    
    // NOTE : initialize monsters
    MonsterMap& monsterMap = GameClient::Instance().GetMonsterMap();

    std::for_each(monsterMap.begin(), monsterMap.end(), [this](MonsterMap::value_type monsterInfoPair){
        ActorID monsterID = monsterInfoPair.first;
        Monster* monsterInfo = monsterInfoPair.second;
        
        ActorNodeSet* actorNodeSet = new ActorNodeSet(monsterID);
        
        this->m_ActorNodeSetMap.insert(ActorNodeSetMap::value_type(monsterID, actorNodeSet));
        
        actorNodeSet->m_ActorNode->setPosition(PointConverter::Convert(monsterInfo->GetCurrentPosition()));
        
        if(actorNodeSet->m_ActorNode) this->addChild(actorNodeSet->m_ActorNode);
        if(actorNodeSet->m_HUDNode) this->addChild(actorNodeSet->m_HUDNode);
        if(actorNodeSet->m_ShadowNode) this->addChild(actorNodeSet->m_ShadowNode);
        if(actorNodeSet->m_HighlightNode) this->addChild(actorNodeSet->m_HighlightNode);
    });

    scheduleUpdate();
    
    schedule(schedule_selector(ActorLayer::SortNodes));
    
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

void ActorLayer::DeletePlayer(ActorID playerID)
{
    ActorNodeSetMap::iterator iter = this->m_ActorNodeSetMap.find(playerID);
    
    ASSERT_DEBUG(iter != this->m_ActorNodeSetMap.end());
    
    ActorNodeSet* actorNodeSet = iter->second;
    
    if(actorNodeSet->m_ActorNode) this->removeChild(actorNodeSet->m_ActorNode, true);
    if(actorNodeSet->m_HUDNode) this->removeChild(actorNodeSet->m_HUDNode, true);
    if(actorNodeSet->m_ShadowNode) this->removeChild(actorNodeSet->m_ShadowNode, true);
    if(actorNodeSet->m_HighlightNode) this->removeChild(actorNodeSet->m_HighlightNode, true);

    delete actorNodeSet;
    
    this->m_ActorNodeSetMap.erase(iter);
}

PlayerNode* ActorLayer::FindPlayerNode(ActorID playerID)
{
    return static_cast<PlayerNode*>(this->FindActorNode(playerID));
}

MonsterNode* ActorLayer::FindMonsterNode(ActorID monsterID)
{
    return static_cast<MonsterNode*>(this->FindActorNode(monsterID));
}


ActorNode* ActorLayer::FindActorNode(ActorID actorID)
{
    ActorNodeSet* actorNodeSet = this->FindActorNodeSet(actorID);
    
    return actorNodeSet ? actorNodeSet->m_ActorNode : nullptr;
}

ActorNodeSet* ActorLayer::FindActorNodeSet(ActorID actorID)
{
    ActorNodeSetMap::iterator iter = this->m_ActorNodeSetMap.find(actorID);
    
    if(iter == this->m_ActorNodeSetMap.end()) return nullptr;
    
    return iter->second;
}

ItemNode* ActorLayer::FindItemNode(ItemID itemID)
{
    ItemNodeMap::iterator iter = this->m_ItemNodeMap.find(itemID);
    if(iter == this->m_ItemNodeMap.end()) return nullptr;

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
        
        
        // NOTE : character's z order starts with 20, inc with 20. between actors there are effects. magics or so
        int zOrder = 20;
        std::for_each(actorNodes.begin(), actorNodes.end(), [this, &zOrder](ActorNode* actorNode){
            this->reorderChild(actorNode, zOrder);
            zOrder += 20;
        });
    }
}


// TO DO : if can, merge this two
void ActorLayer::AddNewPlayer(flownet::ClientPlayer player)
{
    player.ChangeToIdleState();
    
    ActorNodeSet* actorNodeSet = new ActorNodeSet(player.GetActorID());

    if(player.GetActorID() == GameClient::Instance().GetMyActorID())
    {
        actorNodeSet->m_HighlightNode = HighlightNode::create(player.GetActorID());
        actorNodeSet->m_HighlightNode->retain();
    }


    actorNodeSet->m_ActorNode->setPosition(PointConverter::Convert(player.GetCurrentPosition()));
    
    ActorNodeSetMap::iterator iter = this->m_ActorNodeSetMap.find(player.GetActorID());
    
    ASSERT_DEBUG(iter == this->m_ActorNodeSetMap.end());
    
    this->m_ActorNodeSetMap.insert(ActorNodeSetMap::value_type(player.GetActorID(), actorNodeSet));
    
    if(actorNodeSet->m_ActorNode) this->addChild(actorNodeSet->m_ActorNode);
    if(actorNodeSet->m_HUDNode)this->addChild(actorNodeSet->m_HUDNode);
    if(actorNodeSet->m_ShadowNode)this->addChild(actorNodeSet->m_ShadowNode);
    if(actorNodeSet->m_HighlightNode)this->addChild(actorNodeSet->m_HighlightNode);
}

void ActorLayer::AddNewMonster(flownet::ClientMonster monster)
{
    monster.ChangeToIdleState();
    
    ActorNodeSet* actorNodeSet = new ActorNodeSet(monster.GetActorID());
    
    actorNodeSet->m_ActorNode->setPosition(PointConverter::Convert(monster.GetCurrentPosition()));
    
    ActorNodeSetMap::iterator iter = this->m_ActorNodeSetMap.find(monster.GetActorID());
    
    ASSERT_DEBUG(iter == this->m_ActorNodeSetMap.end());
    
    this->m_ActorNodeSetMap.insert(ActorNodeSetMap::value_type(monster.GetActorID(), actorNodeSet));
    
    if(actorNodeSet->m_ActorNode) this->addChild(actorNodeSet->m_ActorNode);
    if(actorNodeSet->m_HUDNode) this->addChild(actorNodeSet->m_HUDNode);
    if(actorNodeSet->m_ShadowNode) this->addChild(actorNodeSet->m_ShadowNode);
    if(actorNodeSet->m_HighlightNode) this->addChild(actorNodeSet->m_HighlightNode);
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
        CCLOG("ActorLayer::MoveActor >> ignore actor move request. actor is dead");
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
    }
    
    if(IsPlayerID(actorID))
    {
        ClientPlayer* player = static_cast<ClientPlayer*>(actor);
        player->ChangeToMovingState();
        changeToIdleState = CCCallFunc::create(player, callfunc_selector(ClientPlayer::ChangeToIdleState));
    }
    
    sequence = CCSequence::create(animateMove, actionMove, actionMoveDone, changeToIdleState, NULL);
    
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
        CCLOG("ActorLayer::ActorAttack >> ignore actor's attack request. actor is dead");
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
        CCLOG("ActorLayer::ActorBeginCast >> ignore actor's cast request. actor is dead");
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
    this->UpdateActorLookingDirection(actor, castingObject->getPosition(), PointConverter::Convert(destination));
    CCFiniteTimeAction* animateBeginCasting = CCCallFunc::create(castingObject, callfunc_selector(ActorNode::AnimateBeginCasting));
    CCDelayTime* delay = CCDelayTime::create(0.2);
    CCFiniteTimeAction* animateRepeatCasting = CCCallFunc::create(castingObject, callfunc_selector(ActorNode::AnimateRepeatCasting));
    CCAction* sequence = CCSequence::create(animateBeginCasting, delay, animateRepeatCasting, NULL);
    sequence->setTag(ActionType_Animation);
    castingObject->runAction(sequence);
    
    // TO DO : show spell guide line and marker
    // MUSTTODO
    // castingObject->ShowSpellGuide(spellType, PointConverter::Convert(destination));
}

void ActorLayer::ActorEndCast(flownet::ActorID invokerActorID, flownet::SpellType spellType, flownet::POINT destination)
{
    Actor* actor = GameClient::Instance().GetClientStage()->FindActor(invokerActorID);
    ActorNode* invokerObject = this->FindPlayerNode(invokerActorID);
    
    SpellInfo spellInfo = SpellDictionary::Instance().FindSpellInfoBySpellType(spellType);
    
    if( !actor->IsAlive() )
    {
        CCLOG("ActorLayer::ActorEndCast >> ignore fire spell request. player is dead");
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
    
    
    // MUSTTODO
    //invokerObject->HideSpellGuide();
    
    this->FireSpell(invokerActorID, destination, spellInfo);
}

void ActorLayer::ActorAttacked(flownet::ActorID attackedActorID, flownet::ActorID attackerActorID)
{
    Actor* actor = GameClient::Instance().GetClientStage()->FindActor(attackedActorID);
    
    if( !actor->IsAlive() )
    {
        CCLOG("ActorLayer::ActorAttacked >> ignore player attacked request. player is dead");
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
    
    ActorNodeSet* actorNodeSet = this->FindActorNodeSet(actorID);
    ASSERT_DEBUG(actorNodeSet != nullptr);
    
    ASSERT_DEBUG(actor->GetMaxHealthPoint() != 0); // just checking for division by zero exception, will be this happened?
    
    actorNodeSet->m_HUDNode->ChangeHealthPointBar(actor->GetHealthPoint() / actor->GetMaxHealthPoint());
}

MonsterNode* ActorLayer::FindSelectedMonsterNode(cocos2d::CCPoint touchLocation)
{
    ActorNode* foundNode = nullptr;
    
    for(ActorNodeSetMap::iterator iter = this->m_ActorNodeSetMap.begin(); iter != this->m_ActorNodeSetMap.end() ; ++iter)
    {
        ActorNodeSet* actorNodeSet = iter->second;
        
        if(!IsMonsterID(actorNodeSet->m_ActorNode->GetActorID())) continue;
        
        CCRect actorNodeRect = actorNodeSet->m_ActorNode->GetRect();
        actorNodeRect.origin = CCPointZero;
        CCPoint actorNodeLocalPoint = actorNodeSet->m_ActorNode->convertToNodeSpace(touchLocation);
        
        actorNodeLocalPoint.x = actorNodeLocalPoint.x + (actorNodeRect.size.width * 0.5);
        
        if(actorNodeRect.containsPoint(actorNodeLocalPoint) && actorNodeSet->m_ActorNode->GetActorInfo()->IsAlive())
        {
            foundNode = actorNodeSet->m_ActorNode;
            break;
        }
    }

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

void ActorLayer::UseItem(flownet::ActorID playerID, flownet::ItemID itemID)
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
    CCJumpTo* jumpTo = CCJumpTo::create(0.3, actor->GetMidPosition(), 30, 1);
    CCDelayTime* delay = CCDelayTime::create(0.3);
    CCCallFuncO* remove = CCCallFuncO::create(this, callfuncO_selector(ActorLayer::RemoveItem), item);
    CCSequence* sequence = CCSequence::create(jumpTo, delay, remove, NULL);
    sequence->setTag(ActionType_Animation);
    item->runAction(sequence);
    // TO DO : display effect for acquiring item
}
