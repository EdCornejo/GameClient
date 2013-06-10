//
//  SpellNode.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 3/22/13.
//
//

#include "Headers.pch"

SpellNode::SpellNode(const flownet::SpellInfo& spellInfo, flownet::ActorID actorID, flownet::POINT destination) : m_SpellInfo(spellInfo), m_CasterID(actorID), m_Destination(destination), m_LastTickTime(0), m_StartingEffectID(0), m_Shadow(nullptr)
{

}

SpellNode::~SpellNode()
{
    if(this->m_Shadow)
    {
        this->m_Shadow->removeFromParent();
        this->m_Shadow->release();
    }
}

bool SpellNode::initWithFile(const char* fileName)
{
    if(!CCSprite::initWithFile(fileName))
    {
        return false;
    }
    
    const double degreeVariance = 60;
    const double vertical = 90;
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
            const float spellInitialDistance = 190; //PointConverter::ModiYToViewPointY(Spell_StartingPoint_Somewhere_Distance);
            float RandomValue = CCRANDOM_0_1() - 0.5; // offset for random 0~1
            
            float spellStartPointX = targetPoint.x + spellInitialDistance * cos((vertical + (RandomValue * degreeVariance)) * PI / 180);
            float spellStartPointY = targetPoint.y + spellInitialDistance * sin((vertical + (RandomValue * degreeVariance)) * PI / 180);
            spellStartPoint = ccp(spellStartPointX, spellStartPointY);
            
            double rotateDegree = -atan2(targetPoint.y - spellStartPoint.y, targetPoint.x - spellStartPoint.x) * 180 / M_PI;
            this->setRotation(rotateDegree);
            
            targetPoint.y += 60;
            
            break;
        }
        case flownet::StartingPoint_Self:
        default:
            spellStartPoint = firingObject->getPosition();
            
            Actor* caster = GameClient::Instance().GetClientStage()->FindActor(this->m_CasterID);
            ASSERT_DEBUG(caster);
            
            // NOTE : adjust fire position to charac
            if(caster->GetLookingDirection() == DIRECTION_LEFT) {
                spellStartPoint.x -= 30;
                spellStartPoint.y += 30;
                this->setRotation(180);
            }
            else { // RIGHT
                spellStartPoint.x += 30;
                spellStartPoint.y += 30;
            }
            
            targetPoint.y += 30;
            this->m_Destination = PointConverter::Convert(targetPoint);
            
//            double rotateDegree = -atan2(targetPoint.y - spellStartPoint.y, targetPoint.x - spellStartPoint.x) * 180 / M_PI;
//            this->setRotation(rotateDegree);
            
            break;
    }
    
    // NOTE : if the spell is nova like need rotation
    switch (this->m_SpellInfo.m_SpellType) {
    case flownet::SpellType_IceFog:
        this->setScaleY(0.45);
        break;
    default:
        break;
    }
    

    this->setPosition(spellStartPoint);
    
    
    // 이것은 예외적으로 layer에서 생성하지 않고 여기서 생성한다.
    // 스펠노드에 차일드로 붙이려고 했지만 너무 많은 코드를 수정해야하는 바람에 일단 이렇게 대체한다.
    // update에서 페어런트에 붙었는지 확인후에 그림자를 추가하는 식으로 대체를 한다
    // 스펠노드에 대대적인 수정이 필요할것임을 직감하였다...
    this->m_Shadow = ShadowNode::create(this);
    this->m_Shadow->retain();
        
    this->StartSpellAnimation();
    
    scheduleUpdate();
    
    return true;
}

SpellNode* SpellNode::create(const SpellInfo& spellInfo, ActorID casterID, POINT destination)
{
    SpellNode* spellNode = new SpellNode(spellInfo, casterID, destination);

    if(spellNode && spellNode->initWithFile("blank.png"))
    {
        spellNode->autorelease();
        return spellNode;
    }
    else
    {
        delete spellNode;
        spellNode = nullptr;
        return nullptr;
    }
}

void SpellNode::update(float deltaTime)
{
    if(!this->m_Shadow->getParent())
    {
        if(this->getParent())
        {
            this->getParent()->addChild(this->m_Shadow);
        }
    }

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
    if(!actorNode) return; // NOTE : actorNode가 제거된 경우 위치 갱신을 하지 않는다
    
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
    
    float delay = PointConverter::Convert(this->getPosition()).DistanceTo(this->m_Destination) / (this->m_SpellInfo.m_Speed);
    delay = delay == 0 ? 3 : delay;
    
    // TO DO : changed this logic ! when spell effect changed
    CCAnimation* animation = SpellAnimationLoader::Instance()->GetSpellAnimation(this->m_SpellInfo.m_SpellType);
    CCAction* animate = CCAnimate::create(animation);
    animate->setTag(ActionType_Spell);
    this->runAction(animate);
    
    CCCallFunc* playStartEffect = CCCallFunc::create(this, callfunc_selector(SpellNode::PlayStartingEffect));
    CCDelayTime* delayTime = CCDelayTime::create(delay);
    CCCallFunc* playEndEffect = CCCallFunc::create(this, callfunc_selector(SpellNode::PlayEndingEffect));
    CCCallFunc* destroy = CCCallFunc::create(this, callfunc_selector(SpellNode::Destroy));
    CCSequence* sequence = CCSequence::create(playStartEffect, delayTime, playEndEffect, destroy, NULL);
    sequence->setTag(ActionType_UI);
    this->runAction(sequence);
}

void SpellNode::Destroy()
{
    this->removeFromParent();
}

std::string SpellNode::GetFileName(SpellType spellType)
{
    std::string effectFileName = "";
    
    switch (this->m_SpellInfo.m_SpellType) {
    case flownet::SpellType_FireBall:
        effectFileName += "fire_burst";
        break;
    case flownet::SpellType_FireBurst:
        effectFileName += "fire_burst";
        break;
    case flownet::SpellType_IceArrow:
    {
        int i = CCRANDOM_0_1();
        effectFileName += "ice_arrow";
        effectFileName.push_back('0' + i);
        break;
    }
    case flownet::SpellType_IceFog:
        effectFileName += "ice_arrow";
        break;
    case flownet::SpellType_Crystalize :
        effectFileName += "ice_arrow";
        break;
    default:
        ASSERT_DEBUG(false);
        break;
    }

    return effectFileName;
}

void SpellNode::PlayStartingEffect()
{
    std::string effectFileName = "sound/effect/spell/" + GetFileName(this->m_SpellInfo.m_SpellType) + "_start.mp3";
    AudioEngine::Instance()->PlayEffect(effectFileName.c_str());
}

void SpellNode::PlayEndingEffect()
{
    std::string effectFileName = "sound/effect/spell/" + GetFileName(this->m_SpellInfo.m_SpellType) + "_end.mp3";
    AudioEngine::Instance()->PlayEffect(effectFileName.c_str());
}