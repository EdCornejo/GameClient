//
//  CaptionLayer.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 3/24/13.
//
//

#include "Headers.pch"

CaptionLayer::CaptionLayer()
{

}

CaptionLayer::~CaptionLayer()
{

}

bool CaptionLayer::init()
{
    if(!BaseLayer::init())
    {
        return false;
    }
    
    return true;
}

bool CaptionLayer::initWithStageType(flownet::StageType stageType)
{
    if(!BaseLayer::initWithStageType(stageType))
    {
        return false;
    }
    
    switch(this->m_StageType)
    {
        case flownet::StageType_NONE:
        default:
            break;
    }
    
    
    return true;
}

CaptionLayer* CaptionLayer::create(flownet::StageType stageType)
{
    CaptionLayer* captionLayer = new CaptionLayer();
    if(captionLayer && captionLayer->initWithStageType(stageType))
    {
        return captionLayer;
    }
    else
    {
        delete captionLayer;
        captionLayer = nullptr;
        return nullptr;
    }
}