//
//  SpellNode.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 3/22/13.
//
//

#include "Headers.pch"

SpellNode::SpellNode() : m_SpellInfo(), m_CasterID(ActorID_None), m_Destination(), m_LastTickTime(0)
{

}

SpellNode::~SpellNode()
{

}

bool SpellNode::initWithFile(const char* fileName)
{
    if(!CCSprite::initWithFile(fileName))
    {
        return false;
    }
    
    const double degreeVariance = 60;
    const double vertical = 90;
    const float spellInitialDistance = 200;
    const double PI = 4.0 * atan(1.0);
    CCPoint spellStartPoint = CCPointZero;

    BaseScene* scene = static_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
    PlayerNode* firingObject = scene->GetActorLayer()->FindPlayerNode(this->m_CasterID);
    CCPoint targetPoint = PointConverter::Convert(this->m_Destination);
    switch(this->m_SpellInfo.m_StartingPoint)
    {
        case flownet::StartingPoint_Target:
            spellStartPoint = targetPoint;
            break;
        case flownet::StartingPoint_Somewhere:{
            float RandomValue = CCRANDOM_0_1() - 0.5; // offset for random 0~1
            
            float spellStartPointX = targetPoint.x + spellInitialDistance * cos((vertical + (RandomValue * degreeVariance)) * PI / 180);
            float spellStartPointY = targetPoint.y + spellInitialDistance * sin((vertical + (RandomValue * degreeVariance)) * PI / 180);
            spellStartPoint = ccp(spellStartPointX, spellStartPointY);
            break;
        }
        case flownet::StartingPoint_Self:
        default:
            spellStartPoint = firingObject->GetSpellPosition();
            // NOTE : character height adjustment
            spellStartPoint.y += firingObject->GetRect().size.height / 2;
            // NOTE : character width adjustment
            Actor* caster = GameClient::Instance().GetClientStage()->FindActor(this->m_CasterID);
            if(caster->GetLookingDirection() == DIRECTION_LEFT)
            {
                spellStartPoint.x -= firingObject->GetRect().size.width / 2;
            }
            if(caster->GetLookingDirection() == DIRECTION_RIGHT)
            {
                spellStartPoint.x += firingObject->GetRect().size.width / 2;
            }
            break;
    }

    this->setPosition(spellStartPoint);
    
    this->StartSpellAnimation();
    
    scheduleUpdate();
    
    return true;
}

SpellNode* SpellNode::create(SpellInfo spellInfo, ActorID casterID, POINT destination)
{
    SpellNode* spellNode = new SpellNode();
    spellNode->m_SpellInfo = spellInfo;
    spellNode->m_CasterID = casterID;
    spellNode->m_Destination = destination;

    if(spellNode && spellNode->initWithFile("blank.png"))
    {
        return spellNode;
    }
    else
    {
        spellNode->release();
        spellNode = nullptr;
        return nullptr;
    }
}

void SpellNode::update(float deltaTime)
{
    this->m_LastTickTime += deltaTime;
    if( this->m_LastTickTime <= 0.2 )
    {
        return;
    }
    this->m_LastTickTime = 0;
    
    ActorLayer* actorLayer = static_cast<ActorLayer*>(this->getParent());
    if(!actorLayer) ASSERT_DEBUG(actorLayer);
    
    // NOTE : reoder spell's ZOrder based on caster's ZOrder
    ActorNode* actorNode = actorLayer->FindActorNode(this->m_CasterID);
    this->setZOrder(actorNode->getZOrder() + 1);
    
    switch(this->m_SpellInfo.m_StartingPoint)
    {
        case flownet::StartingPoint_Target:
            break;
        case flownet::StartingPoint_Self:
        case flownet::StartingPoint_Somewhere:
        default:{
            CCPoint currentPosition = this->getPosition();
            CCPoint destination = PointConverter::Convert(this->m_Destination);
            // NOTE : character height offset
            if(currentPosition.x != destination.x || currentPosition.y != destination.y)
            {
                float duration = PointConverter::Convert(currentPosition).DistanceTo(PointConverter::Convert(destination)) / this->m_SpellInfo.m_Speed;
                CCMoveTo* moveTo = CCMoveTo::create(duration, destination);
                moveTo->setTag(ActionType_Animation);
        
                this->stopActionByTag(ActionType_Animation);
        
                this->runAction(moveTo);
            }
            break;
        }
    }
        
    
}

void SpellNode::StartSpellAnimation()
{
    BaseScene* scene = static_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
    PlayerNode* firingObject = scene->GetActorLayer()->FindPlayerNode(this->m_CasterID);
    
    float delay = PointConverter::Convert(firingObject->getPosition()).DistanceTo(this->m_Destination) / (this->m_SpellInfo.m_Speed - 1);
    
    CCAnimation* animation = SpellAnimationLoader::Instance()->GetSpellAnimation(this->m_SpellInfo.m_SpellType);
    CCAction* animate = CCAnimate::create(animation);
    animate->setTag(ActionType_Spell);
    this->runAction(animate);
    
    schedule(schedule_selector(SpellNode::Destroy), 0, 0, delay);
}

void SpellNode::Destroy()
{
    this->getParent()->removeChild(this, true);
    delete this;
}