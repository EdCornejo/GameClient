//
//  BaseLayer.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 3/10/13.
//
//

#include "Headers.pch"

BaseLayer::BaseLayer() : m_StageType(StageType_NONE) {}

BaseLayer::~BaseLayer(){}

bool BaseLayer::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    this->m_StageType = StageType_NONE;
    
    return true;
}

bool BaseLayer::initWithStageType(StageType stageType)
{
    if(!CCLayer::init())
    {
        return false;
    }
    this->m_StageType = stageType;
    
    return true;
}

void BaseLayer::OnResponse() const
{

}