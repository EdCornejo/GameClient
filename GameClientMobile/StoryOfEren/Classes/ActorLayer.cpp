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

        std::for_each(actorNodeSet->m_SpellEffectNodeMap.begin(), actorNodeSet->m_SpellEffectNodeMap.end(), [this](SpellEffectNodeMap::value_type pair){
            SpellEffectNode* node = pair.second;
            this->removeChild(node, true);
        });

        if(actorNodeSet->m_ActorNode)
        {
            this->removeChild(actorNodeSet->m_ActorNode, true);
        }
        if(actorNodeSet->m_HUDNode)
        {
            this->removeChild(actorNodeSet->m_HUDNode, true);
        }
        if(actorNodeSet->m_ShadowNode)
        {
            this->removeChild(actorNodeSet->m_ShadowNode, true);
        }
        if(actorNodeSet->m_HighlightNode)
        {
            this->removeChild(actorNodeSet->m_HighlightNode, true);
        }
                
        delete actorNodeSet;

        pair.second = nullptr;
    });
    
    this->m_ActorNodeSetMap.clear();
    
    // TO DO : check this code
    std::for_each(this->m_ItemNodeMap.begin(), this->m_ItemNodeMap.end(), [this](ItemNodeMap::value_type pair){
        ItemNode* node = pair.second;
        this->removeChild(node, true);
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
    
    // NOTE : initialize players
    PlayerMap& playerMap = GameClient::Instance().GetPlayerMap();
    
    std::for_each(playerMap.begin(), playerMap.end(), [this](PlayerMap::value_type playerInfoPair){
        ActorID playerID = playerInfoPair.first;
        Player* playerInfo = playerInfoPair.second;
        
        ActorNodeSet* actorNodeSet = ActorNodeSet::create(playerID);
        actorNodeSet->retain();

        this->m_ActorNodeSetMap.insert(ActorNodeSetMap::value_type(playerID, actorNodeSet));
        
        actorNodeSet->m_ActorNode->setPosition(PointConverter::Convert(playerInfo->GetCurrentPosition()));
        
        if(playerID == GameClient::Instance().GetMyActorID())
        {
            // actorNodeSet->AddHighlightNode(playerID);
        }
        
        if(actorNodeSet->m_ActorNode)
        {
            this->addChild(actorNodeSet->m_ActorNode);
            actorNodeSet->m_ActorNode->InitializeAnimation();
        }
        if(actorNodeSet->m_HUDNode) this->addChild(actorNodeSet->m_HUDNode);
        if(actorNodeSet->m_ShadowNode) this->addChild(actorNodeSet->m_ShadowNode);
        if(actorNodeSet->m_HighlightNode) this->addChild(actorNodeSet->m_HighlightNode);
        if(actorNodeSet->m_ChatBalloonNode) this->addChild(actorNodeSet->m_ChatBalloonNode);
    });
    
    // NOTE : initialize monsters
    MonsterMap& monsterMap = GameClient::Instance().GetMonsterMap();

    std::for_each(monsterMap.begin(), monsterMap.end(), [this](MonsterMap::value_type monsterInfoPair){
        ActorID monsterID = monsterInfoPair.first;
        Monster* monsterInfo = monsterInfoPair.second;
        
        ActorNodeSet* actorNodeSet = ActorNodeSet::create(monsterID);
        actorNodeSet->retain();
        
        this->m_ActorNodeSetMap.insert(ActorNodeSetMap::value_type(monsterID, actorNodeSet));
        
        actorNodeSet->m_ActorNode->setPosition(PointConverter::Convert(monsterInfo->GetCurrentPosition()));
        
        if(actorNodeSet->m_ActorNode)
        {
            this->addChild(actorNodeSet->m_ActorNode);
            actorNodeSet->m_ActorNode->InitializeAnimation();
        }
        if(actorNodeSet->m_HUDNode) this->addChild(actorNodeSet->m_HUDNode);
        if(actorNodeSet->m_ShadowNode) this->addChild(actorNodeSet->m_ShadowNode);
        if(actorNodeSet->m_HighlightNode) this->addChild(actorNodeSet->m_HighlightNode);
        if(actorNodeSet->m_ChatBalloonNode) this->addChild(actorNodeSet->m_ChatBalloonNode);
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
        actorLayer->autorelease();
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

void ActorLayer::DeleteActor(ActorID playerID)
{
    // TO DO : release bug fix
    ActorNodeSetMap::iterator iter = this->m_ActorNodeSetMap.find(playerID);
    
    ASSERT_DEBUG(iter != this->m_ActorNodeSetMap.end());
    
    ActorNodeSet* actorNodeSet = iter->second;
    
    if(actorNodeSet->m_ActorNode) this->removeChild(actorNodeSet->m_ActorNode, true);
    if(actorNodeSet->m_HUDNode) this->removeChild(actorNodeSet->m_HUDNode, true);
    if(actorNodeSet->m_ShadowNode) this->removeChild(actorNodeSet->m_ShadowNode, true);
    if(actorNodeSet->m_HighlightNode) this->removeChild(actorNodeSet->m_HighlightNode, true);
    std::for_each(actorNodeSet->m_SpellEffectNodeMap.begin(), actorNodeSet->m_SpellEffectNodeMap.end(), [this](SpellEffectNodeMap::value_type pair){
        pair.second->release();
        this->removeChild(pair.second, true);
    });

    actorNodeSet->release();
    
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

//void ActorLayer::DeleteActorNode(CCObject* object)
//{
//    ActorNodeSet* actorNodeSet = static_cast<ActorNodeSet*>(object);
//    this->DeleteActor(actorNodeSet->m_ActorID);
//}

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
    Vector<ActorNodeSet*>::type actorNodeSets;
    
    std::for_each(this->m_ActorNodeSetMap.begin(), this->m_ActorNodeSetMap.end(), [this, &actorNodeSets](ActorNodeSetMap::value_type pair){
        ActorNodeSet* nodeSet = pair.second;
        
        actorNodeSets.push_back(nodeSet);
    });

    std::sort(actorNodeSets.begin(), actorNodeSets.end(), [](ActorNodeSet* lhs, ActorNodeSet* rhs)->bool{
        if(!lhs->m_ActorNode || !rhs->m_ActorNode) return false;
        
        return lhs->m_ActorNode->getPositionY() > rhs->m_ActorNode->getPositionY();
    });

    // NOTE : character's z order starts with 20, inc with 20. between actors there are effects. magics or so
    int zOrder = 20;
    std::for_each(actorNodeSets.begin(), actorNodeSets.end(), [this, &zOrder](ActorNodeSet* actorNodeSet){
        actorNodeSet->SetZOrder(zOrder);
        if(actorNodeSet->m_ActorNode) this->reorderChild(actorNodeSet->m_ActorNode, actorNodeSet->m_ActorNode->getZOrder());
        if(actorNodeSet->m_HUDNode) this->reorderChild(actorNodeSet->m_HUDNode, actorNodeSet->m_HUDNode->getZOrder());
        if(actorNodeSet->m_ShadowNode) this->reorderChild(actorNodeSet->m_ShadowNode, actorNodeSet->m_ShadowNode->getZOrder());
        if(actorNodeSet->m_HighlightNode) this->reorderChild(actorNodeSet->m_HighlightNode, actorNodeSet->m_HighlightNode->getZOrder());
        if(actorNodeSet->m_GuideLineNode) this->reorderChild(actorNodeSet->m_GuideLineNode, actorNodeSet->m_GuideLineNode->getZOrder());
        if(actorNodeSet->m_ActorNode) this->reorderChild(actorNodeSet->m_ActorNode, actorNodeSet->m_ActorNode->getZOrder());
        zOrder += 20;
    });
}


// TO DO : if can, merge this two
void ActorLayer::AddNewPlayer(flownet::ClientPlayer player)
{
    player.ChangeToIdleState();
    
    ActorNodeSet* actorNodeSet = ActorNodeSet::create(player.GetActorID());
    actorNodeSet->retain();
    actorNodeSet->AddHUDNode(player.GetActorID());
    actorNodeSet->AddShadowNode(player.GetActorID());
    actorNodeSet->AddHighlightNode(player.GetActorID());

    #ifdef OBSERVER_ENABLE
    if(player.GetActorID() == 1)
    {
        actorNodeSet->SetVisible(false);
        //actorNodeSet->AddHighlightNode(player.GetActorID());
    }
    #endif

    actorNodeSet->m_ActorNode->setPosition(PointConverter::Convert(player.GetCurrentPosition()));
    
    ActorNodeSetMap::iterator iter = this->m_ActorNodeSetMap.find(player.GetActorID());
    
    ASSERT_DEBUG(iter == this->m_ActorNodeSetMap.end());

    if(actorNodeSet->m_ActorNode)
    {
        this->addChild(actorNodeSet->m_ActorNode);
        actorNodeSet->m_ActorNode->InitializeAnimation();
    }
    if(actorNodeSet->m_HUDNode) this->addChild(actorNodeSet->m_HUDNode);
    if(actorNodeSet->m_ShadowNode) this->addChild(actorNodeSet->m_ShadowNode);
    if(actorNodeSet->m_HighlightNode) this->addChild(actorNodeSet->m_HighlightNode);
    if(actorNodeSet->m_ChatBalloonNode) this->addChild(actorNodeSet->m_ChatBalloonNode);
    
    this->m_ActorNodeSetMap.insert(ActorNodeSetMap::value_type(player.GetActorID(), actorNodeSet));
}

void ActorLayer::AddNewMonster(flownet::ClientMonster monster)
{
    monster.ChangeToIdleState();
    
    ActorNodeSet* actorNodeSet = ActorNodeSet::create(monster.GetActorID());
    actorNodeSet->retain();
    actorNodeSet->AddHUDNode(monster.GetActorID());
    actorNodeSet->AddShadowNode(monster.GetActorID());
    actorNodeSet->AddHighlightNode(monster.GetActorID());

    actorNodeSet->m_ActorNode->setPosition(PointConverter::Convert(monster.GetCurrentPosition()));
    
    ActorNodeSetMap::iterator iter = this->m_ActorNodeSetMap.find(monster.GetActorID());
    
    ASSERT_DEBUG(iter == this->m_ActorNodeSetMap.end());
    
    if(actorNodeSet->m_ActorNode)
    {
        this->addChild(actorNodeSet->m_ActorNode);
        actorNodeSet->m_ActorNode->InitializeAnimation();
    }
    if(actorNodeSet->m_HUDNode) this->addChild(actorNodeSet->m_HUDNode);
    if(actorNodeSet->m_ShadowNode) this->addChild(actorNodeSet->m_ShadowNode);
    if(actorNodeSet->m_HighlightNode) this->addChild(actorNodeSet->m_HighlightNode);
    if(actorNodeSet->m_ChatBalloonNode) this->addChild(actorNodeSet->m_ChatBalloonNode);
    
    this->m_ActorNodeSetMap.insert(ActorNodeSetMap::value_type(monster.GetActorID(), actorNodeSet));
}

void ActorLayer::AddNewNPC(flownet::NPC npc)
{
    ActorNodeSet* actorNodeSet = ActorNodeSet::create(npc.GetActorID());
    actorNodeSet->retain();
    actorNodeSet->AddShadowNode(npc.GetActorID());
    actorNodeSet->AddHighlightNode(npc.GetActorID());
    
    actorNodeSet->m_ActorNode->setPosition(PointConverter::Convert(npc.GetCurrentPosition()));
    
    ActorNodeSetMap::iterator iter = this->m_ActorNodeSetMap.find(npc.GetActorID());
    
    ASSERT_DEBUG(iter == this->m_ActorNodeSetMap.end());
    
    if(actorNodeSet->m_ActorNode)
    {
        this->addChild(actorNodeSet->m_ActorNode);
        actorNodeSet->m_ActorNode->InitializeAnimation();
    }
    if(actorNodeSet->m_HUDNode) this->addChild(actorNodeSet->m_HUDNode);
    if(actorNodeSet->m_ShadowNode) this->addChild(actorNodeSet->m_ShadowNode);
    if(actorNodeSet->m_HighlightNode) this->addChild(actorNodeSet->m_HighlightNode);
    if(actorNodeSet->m_ChatBalloonNode) this->addChild(actorNodeSet->m_ChatBalloonNode);
    
    this->m_ActorNodeSetMap.insert(ActorNodeSetMap::value_type(npc.GetActorID(), actorNodeSet));
}

void ActorLayer::AddNewStageObject(flownet::StageObject stageObject)
{
    ActorNodeSet* actorNodeSet = ActorNodeSet::create(stageObject.GetActorID());
    actorNodeSet->retain();
    actorNodeSet->AddHighlightNode(stageObject.GetActorID());
    
    actorNodeSet->m_ActorNode->setPosition(PointConverter::Convert(stageObject.GetCurrentPosition()));
    
    ActorNodeSetMap::iterator iter = this->m_ActorNodeSetMap.find(stageObject.GetActorID());
    
    ASSERT_DEBUG(iter == this->m_ActorNodeSetMap.end());

    if(actorNodeSet->m_ActorNode)
    {
        this->addChild(actorNodeSet->m_ActorNode);
        actorNodeSet->m_ActorNode->InitializeAnimation();
    }
    if(actorNodeSet->m_HUDNode) this->addChild(actorNodeSet->m_HUDNode);
    if(actorNodeSet->m_ShadowNode) this->addChild(actorNodeSet->m_ShadowNode);
    if(actorNodeSet->m_HighlightNode) this->addChild(actorNodeSet->m_HighlightNode);
    if(actorNodeSet->m_ChatBalloonNode) this->addChild(actorNodeSet->m_ChatBalloonNode);
    
    this->m_ActorNodeSetMap.insert(ActorNodeSetMap::value_type(stageObject.GetActorID(), actorNodeSet));
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
    ASSERT_DEBUG(actor);
    ActorNode* movingObject = this->FindActorNode(actorID);
    ASSERT_DEBUG(movingObject);
    
    if( !actor->IsAlive() )
    {
        CCLOG("ActorLayer::MoveActor >> ignore actor move request. actor is dead");
        return;
    }
    
    float distance = destinationPosition.DistanceTo(currentPosition); // NOTE : using cocos2d object point because of distance is a bit diffence with server's
    
    ASSERT_DEBUG(actor->GetMovingSpeed() != 0);
    float duration = distance / actor->GetMovingSpeed();
    
    this->UpdateActorLookingDirection(actor, movingObject->getPosition(), PointConverter::Convert(destinationPosition));
    movingObject->StopAnimationActions();
    
    CCFiniteTimeAction* animateMove = CCCallFunc::create(movingObject, callfunc_selector(ActorNode::AnimateMoving));
    CCFiniteTimeAction* actionMove = CCMoveTo::create(duration, PointConverter::Convert(destinationPosition));
    CCFiniteTimeAction* actionMoveDone = CCCallFunc::create( movingObject, callfunc_selector(ActorNode::AnimateIdle));
    CCFiniteTimeAction* changeToIdleState = CCCallFuncN::create(this, callfuncN_selector(ActorLayer::ChangeActorStateToIdle));
    CCAction* sequence = CCSequence::create(animateMove, actionMove, actionMoveDone, changeToIdleState, NULL);
    
    // TO DO : show moving point
    sequence->setTag(ActionType_Animation);
    movingObject->runAction(sequence);
}

void ActorLayer::TeleportActor(flownet::ActorID actorID, flownet::POINT currentPosition, flownet::POINT destinationPosition)
{
    Actor* actor = GameClient::Instance().GetClientStage()->FindActor(actorID);
    ASSERT_DEBUG(actor);
    ActorNode* movingObject = this->FindActorNode(actorID);
    ASSERT_DEBUG(movingObject);
    
    if( !actor->IsAlive() )
    {
        CCLOG("ActorLayer::MoveActor >> ignore actor move request. actor is dead");
        return;
    }
    
    this->UpdateActorLookingDirection(actor, movingObject->getPosition(), PointConverter::Convert(destinationPosition));
    movingObject->StopAnimationActions();
    
    // NOTE : 자신한테 텔레포트 이펙트를 붙인채 싱크에 맞춰 이동할 수 있도록 한다
    // NOTE : teleport effect node는 자동 소멸 할 수 있도록 플래그를 켠다
    SpellEffectNode* effectNode = SpellEffectNode::create(actorID, SpellEffectType_Teleport);
    this->addChild(effectNode);
    
    CCFiniteTimeAction* animateMove = CCCallFunc::create(movingObject, callfunc_selector(ActorNode::AnimateMoving));
    CCDelayTime* beforeDelay = CCDelayTime::create(0.1);
    CCFiniteTimeAction* actionMove = CCMoveTo::create(0, PointConverter::Convert(destinationPosition));
    CCDelayTime* afterDelay = CCDelayTime::create(0.1);
    CCFiniteTimeAction* actionMoveDone = CCCallFunc::create( movingObject, callfunc_selector(ActorNode::AnimateIdle));
    CCFiniteTimeAction* changeToIdleState = CCCallFuncN::create(this, callfuncN_selector(ActorLayer::ChangeActorStateToIdle));
    CCAction* sequence = CCSequence::create(animateMove, beforeDelay, actionMove, afterDelay, actionMoveDone, changeToIdleState, NULL);
    
    // TO DO : show moving point
    sequence->setTag(ActionType_Animation);
    movingObject->runAction(sequence);
}

void ActorLayer::KnockBackActor(flownet::ActorID actorID, flownet::POINT currentPosition, flownet::POINT knockbackDestination)
{
    ActorNode* knockbackObject = this->FindActorNode(actorID);
    ASSERT_DEBUG(knockbackObject);

    const float KNOCKBACK_DURATION = currentPosition.DistanceTo(knockbackDestination) / Actor_KnockBack_Speed;

    knockbackObject->StopAnimationActions();

    CCFiniteTimeAction* animateAttacked = CCCallFunc::create(knockbackObject, callfunc_selector(ActorNode::AnimateAttacked));
    CCFiniteTimeAction* actionMove = CCEaseOut::create(CCMoveTo::create(KNOCKBACK_DURATION, PointConverter::Convert(knockbackDestination)), 4.f);
    CCFiniteTimeAction* actionMoveDone = CCCallFunc::create( knockbackObject, callfunc_selector(ActorNode::AnimateIdle));
    CCSequence* sequence = CCSequence::create(animateAttacked, actionMove, actionMoveDone, NULL);
    sequence->setTag(ActionType_Animation);
    knockbackObject->runAction(sequence);
}

void ActorLayer::ActorAttack(flownet::ActorID attackerActorID, flownet::ActorID targetActorID)
{
    Actor* actor = GameClient::Instance().GetClientStage()->FindActor(attackerActorID);
    ActorNode* attackingObject = this->FindActorNode(attackerActorID);
    ActorNode* targetObject = this->FindActorNode(targetActorID);
    
    if(!actor)
    {
        return;
    }
    
    ASSERT_DEBUG(attackingObject);
    ASSERT_DEBUG(targetObject);
    
    if( !actor->IsAlive() )
    {
        CCLOG("ActorLayer::ActorAttack >> ignore actor's attack request. actor is dead");
        return;
    }

    ASSERT_DEBUG(actor->GetAttackSpeed() != 0);

    float attackDuration = 1 / actor->GetAttackSpeed();

    attackingObject->StopAnimationActions();
    this->UpdateActorLookingDirection(actor, attackingObject->getPosition(), targetObject->getPosition());
    CCFiniteTimeAction* animateAttack = CCCallFunc::create(attackingObject, callfunc_selector(ActorNode::AnimateAttacking));
    CCFiniteTimeAction* timeDuration = CCDelayTime::create(attackDuration);
    CCFiniteTimeAction* animateIdle = CCCallFunc::create(attackingObject, callfunc_selector(ActorNode::AnimateIdle));
    CCFiniteTimeAction* changeToIdleState = CCCallFuncN::create(this, callfuncN_selector(ActorLayer::ChangeActorStateToIdle));
    CCAction* sequence = CCSequence::create(animateAttack, timeDuration, animateIdle, changeToIdleState, NULL);
    sequence->setTag(ActionType_Animation);
    attackingObject->runAction(sequence);
}

void ActorLayer::ActorBeginCast(flownet::ActorID casterActorID, flownet::SpellType spellType, flownet::POINT destination)
{
    Actor* actor = GameClient::Instance().GetClientStage()->FindActor(casterActorID);
    ActorNode* castingObject = this->FindActorNode(casterActorID);

    if(!actor)
    {
        ASSERT_DEBUG(actor);
        return;
    }

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
    this->AddSpellGuideLine(casterActorID, spellType, destination);
}

void ActorLayer::ActorEndCast(flownet::ActorID invokerActorID, flownet::SpellType spellType, flownet::POINT destination)
{
    Actor* actor = GameClient::Instance().GetClientStage()->FindActor(invokerActorID);
    ActorNode* invokerObject = this->FindPlayerNode(invokerActorID);
    
    SpellInfo spellInfo = SpellDictionary::Instance().FindSpellInfoBySpellType(spellType);
    
    if(!actor)
    {
        ASSERT_DEBUG(actor);
        return;
    }
    
    if( !actor->IsAlive() )
    {
        CCLOG("ActorLayer::ActorEndCast >> ignore fire spell request. player is dead");
        return;
    }
    
    this->UpdateActorLookingDirection(actor, invokerObject->getPosition(), PointConverter::Convert(destination));
    invokerObject->StopAnimationActions();
    CCFiniteTimeAction* animateFire = CCCallFunc::create(invokerObject, callfunc_selector(ActorNode::AnimateFire));
    CCFiniteTimeAction* timeDuration = CCDelayTime::create(Player_Fire_Duration);
    CCFiniteTimeAction* animateFireDone = CCCallFunc::create(invokerObject, callfunc_selector(ActorNode::AnimateIdle));
    CCAction* sequence = CCSequence::create(animateFire, timeDuration, animateFireDone, NULL);
    sequence->setTag(ActionType_Animation);
    invokerObject->runAction(sequence);
    
    
    this->RemoveSpellGuideLine(invokerActorID);
    
    this->FireSpell(invokerActorID, destination, spellInfo);
}

void ActorLayer::ActorAttacked(flownet::ActorID attackedActorID, flownet::ActorID attackerActorID)
{
    Actor* actor = GameClient::Instance().GetClientStage()->FindActor(attackedActorID);
    
    if(!actor)
    {    
        return;
    }
    
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
    ActorNodeSet* actorNodeSet = this->FindActorNodeSet(deadActorID);
    ActorNode* deadObject = actorNodeSet->m_ActorNode;
    
    if(!actor)
    {
        ASSERT_DEBUG(actor);
        return;
    }
    if(!actorNodeSet)
    {
        ASSERT_DEBUG(actorNodeSet);
        return;
    }
    if(!deadObject)
    {
        ASSERT_DEBUG(deadObject);
        return;
    }
    
    if(IsMonsterID(deadActorID))
    {
        static_cast<ClientMonster*>(actor)->ChangeToDeadState();
    }
    
    if(IsPlayerID(deadActorID))
    {
        static_cast<ClientPlayer*>(actor)->ChangeToDeadState();
    }

    deadObject->StopAnimationActions();
    CCAction* sequence = nullptr;
    CCFiniteTimeAction* animateDead = CCCallFunc::create(deadObject, callfunc_selector(ActorNode::AnimateDead));
    CCBlink* blink = CCBlink::create(3, 6);
    CCDelayTime* delay = CCDelayTime::create(2);
    CCCallFunc* hide = CCCallFunc::create(actorNodeSet, callfunc_selector(ActorNodeSet::Hide));
    sequence = CCSequence::create(animateDead, blink, delay, hide, NULL);
    sequence->setTag(ActionType_Animation);
    deadObject->runAction(sequence);

}

void ActorLayer::ActorTakeDamage(flownet::ActorID actorID)
{
    Actor* actor = GameClient::Instance().GetClientStage()->FindActor(actorID);
    if(!actor)
    {
        ASSERT_DEBUG(actor);
        return;
    }
    
    ActorNodeSet* actorNodeSet = this->FindActorNodeSet(actorID);
    if(!actorNodeSet)
    {
        ASSERT_DEBUG(actorNodeSet != nullptr);
        return;
    }

    if(actor->GetMaxHealthPoint() == 0)
    {
        ASSERT_DEBUG(actor->GetMaxHealthPoint() != 0); // just checking for division by zero exception, will be this happened?
        return;
    }
    
    actorNodeSet->m_HUDNode->ChangeHealthPointBar(actor->GetHealthPoint() / actor->GetMaxHealthPoint());
}

void ActorLayer::ActorConsumedMana(flownet::ActorID actorID)
{
    Actor* actor = GameClient::Instance().GetClientStage()->FindActor(actorID);
    if(!actor)
    {
        ASSERT_DEBUG(actor);
        return;
    }
    
    ActorNodeSet* actorNodeSet = this->FindActorNodeSet(actorID);
    if(!actorNodeSet)
    {
        ASSERT_DEBUG(actorNodeSet);
        return;
    }
    
    if(actor->GetMaxHealthPoint() == 0)
    {
        ASSERT_DEBUG(actor->GetMaxManaPoint() != 0);
        return;
    }
    
    actorNodeSet->m_HUDNode->ChangeManaPointBar(actor->GetManaPoint() / actor->GetMaxManaPoint());
}

void ActorLayer::ActorRunOutOfMana(flownet::ActorID actorID)
{
    ActorNode* actorNode = this->FindActorNode(actorID);
    if(!actorNode)
    {
        ASSERT_DEBUG(actorNode);
        return;
    }
    
    CCPoint currentPosition = actorNode->getPosition();
    CCMoveTo* moveRight = CCMoveTo::create(0.01, ccp(currentPosition.x + 2, currentPosition.y));
    CCMoveTo* moveLeft = CCMoveTo::create(0.01, ccp(currentPosition.x - 2, currentPosition.y));
    CCSequence* sequence = CCSequence::create(moveRight, moveLeft, NULL);
    CCRepeat* repeat = CCRepeat::create(sequence, 5);
    actorNode->runAction(repeat);
}

void ActorLayer::ActorTeamChanged(flownet::ActorID actorID)
{
    ActorNodeSet* actorNodeSet = this->FindActorNodeSet(actorID);
    if(!actorNodeSet)
    {
        ASSERT_DEBUG(actorNodeSet);
        return;
    }
    
    actorNodeSet->AddHighlightNode(actorID);
    
    if(!actorNodeSet->m_HighlightNode)
    {
        ASSERT_DEBUG(actorNodeSet->m_HighlightNode);
        actorNodeSet->RemoveHighlightNode();
        return;
    }
    
    this->addChild(actorNodeSet->m_HighlightNode);
}

void ActorLayer::ActorNodeReload(flownet::ActorID actorID)
{
    ActorNode* actorNode = this->FindActorNode(actorID);
    if(!actorNode)
    {
        ASSERT_DEBUG(actorNode != nullptr);
        return;
    }

    actorNode->Reload();
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
    CCLOG("fire spell");
    SpellNode* spellObject = SpellNode::create(spellInfo, playerID, destination);
    this->addChild(spellObject);
}

void ActorLayer::AddSpellEffect(flownet::ActorID actorID, flownet::SpellAbility spellAbility)
{
    ActorNodeSet* actorNodeSet = this->FindActorNodeSet(actorID);
    ASSERT_DEBUG(actorNodeSet);
    
    SpellEffectNode* node = actorNodeSet->AddSpellEffectNode(actorID, spellAbility);
    
    if(node)
    {
        this->addChild(node);
    }
}

void ActorLayer::RemoveSpellEffect(flownet::ActorID actorID, flownet::SpellAbility spellAbility)
{
    ActorNodeSet* actorNodeSet = this->FindActorNodeSet(actorID);
    ASSERT_DEBUG(actorNodeSet);

    SpellEffectNode* node = actorNodeSet->RemoveSpellEffectNode(spellAbility);
    if(node)
    {
        this->removeChild(node, true);
    }
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

void ActorLayer::MessageReceived(flownet::ActorID senderID, flownet::STRING message)
{
    ActorNodeSet* actorNodeSet = this->FindActorNodeSet(senderID);
    ASSERT_DEBUG(actorNodeSet->m_ChatBalloonNode);
    
    actorNodeSet->m_ChatBalloonNode->ChangeMessage(message);
}

void ActorLayer::AddSpellGuideLine(flownet::ActorID actorID, flownet::SpellType spellType, flownet::POINT destination)
{
    ActorNodeSet* actorNodeSet = this->FindActorNodeSet(actorID);
    actorNodeSet->AddGuideLineNode(spellType, PointConverter::Convert(destination));
    this->addChild(actorNodeSet->m_GuideLineNode);
}

void ActorLayer::RemoveSpellGuideLine(flownet::ActorID actorID)
{
    ActorNodeSet* actorNodeSet = this->FindActorNodeSet(actorID);
    actorNodeSet->RemoveGuideLineNode();
}

void ActorLayer::ChangeActorStateToIdle(CCObject* object)
{
    ActorNode* actorNode = static_cast<ActorNode*>(object);

    if(IsPlayerID(actorNode->GetActorID()))
    {
        ClientPlayer* clientPlayer = static_cast<ClientPlayer*>(GameClient::Instance().GetClientStage()->FindActor(actorNode->GetActorID()));
        clientPlayer->ChangeToIdleState();
    }
    else if(IsMonsterID(actorNode->GetActorID()))
    {
        ClientMonster* clientMonser = static_cast<ClientMonster*>(GameClient::Instance().GetClientStage()->FindActor(actorNode->GetActorID()));
        clientMonser->ChangeToIdleState();
    }
}

void ActorLayer::ChangeActorStateToAttacking(CCObject* object)
{
    ActorNode* actorNode = static_cast<ActorNode*>(object);
    if(IsPlayerID(actorNode->GetActorID()))
    {
        ClientPlayer* clientPlayer = static_cast<ClientPlayer*>(GameClient::Instance().GetClientStage()->FindActor(actorNode->GetActorID()));
        clientPlayer->ChangeToAttackingState();
    }
    else if(IsMonsterID(actorNode->GetActorID()))
    {
        ClientMonster* clientMonser = static_cast<ClientMonster*>(GameClient::Instance().GetClientStage()->FindActor(actorNode->GetActorID()));
        clientMonser->ChangeToAttackingState();
    }

}

void ActorLayer::ChangeActorStateToCasting(CCObject* object)
{
    ActorNode* actorNode = static_cast<ActorNode*>(object);
    if(IsPlayerID(actorNode->GetActorID()))
    {
        ClientPlayer* clientPlayer = static_cast<ClientPlayer*>(GameClient::Instance().GetClientStage()->FindActor(actorNode->GetActorID()));
        clientPlayer->ChangeToCastingState();
    }
    else if(IsMonsterID(actorNode->GetActorID()))
    {
        ClientMonster* clientMonser = static_cast<ClientMonster*>(GameClient::Instance().GetClientStage()->FindActor(actorNode->GetActorID()));
        clientMonser->ChangeToCastingState();
    }

}

void ActorLayer::ChangeActorStateToDead(CCObject* object)
{
    ActorNode* actorNode = static_cast<ActorNode*>(object);
    if(IsPlayerID(actorNode->GetActorID()))
    {
        ClientPlayer* clientPlayer = static_cast<ClientPlayer*>(GameClient::Instance().GetClientStage()->FindActor(actorNode->GetActorID()));
        clientPlayer->ChangeToDeadState();
    }
    else if(IsMonsterID(actorNode->GetActorID()))
    {
        ClientMonster* clientMonser = static_cast<ClientMonster*>(GameClient::Instance().GetClientStage()->FindActor(actorNode->GetActorID()));
        clientMonser->ChangeToDeadState();
    }

}

void ActorLayer::ChangeActorStateToMoving(CCObject* object)
{
    ActorNode* actorNode = static_cast<ActorNode*>(object);
    if(IsPlayerID(actorNode->GetActorID()))
    {
        ClientPlayer* clientPlayer = static_cast<ClientPlayer*>(GameClient::Instance().GetClientStage()->FindActor(actorNode->GetActorID()));
        clientPlayer->ChangeToMovingState();
    }
    else if(IsMonsterID(actorNode->GetActorID()))
    {
        ClientMonster* clientMonser = static_cast<ClientMonster*>(GameClient::Instance().GetClientStage()->FindActor(actorNode->GetActorID()));
        clientMonser->ChangeToMovingState();
    }

}
