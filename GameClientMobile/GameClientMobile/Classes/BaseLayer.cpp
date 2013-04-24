//
//  BaseLayer.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 3/10/13.
//
//

#include "Headers.pch"

BaseLayer::BaseLayer() : m_StageType(StageType_NONE), m_NodeMap()
{

}

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

void BaseLayer::update(float deltaTime)
{

}

void BaseLayer::AddNode(INT64 nodeID, cocos2d::CCNode *node)
{
    NodeMap::iterator iter = this->m_NodeMap.find(nodeID);
    if(iter != this->m_NodeMap.end())
    {
        // Duplicate item
        ASSERT_DEBUG(false);
    }
    
    this->m_NodeMap.insert(NodeMap::value_type(nodeID, node));
    this->addChild(node);
}

void BaseLayer::RemoveNode(INT64 nodeID)
{
    NodeMap::iterator iter = this->m_NodeMap.find(nodeID);
    if(iter == this->m_NodeMap.end())
    {
        // No item matches;
        ASSERT_DEBUG(false);
    }
    
    this->m_NodeMap.erase(iter);
    this->removeChild(iter->second);
}

CCNode* BaseLayer::FindNodeByNodeID(INT64 nodeID)
{
    NodeMap::iterator iter = this->m_NodeMap.find(nodeID);
    if(iter == this->m_NodeMap.end())
        return nullptr;
    return iter->second;
}