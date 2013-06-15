//
//  BaseLayer.h
//  GameClientMobile
//
//  Created by SungJinYoo on 3/10/13.
//
//

#ifndef __GameClientMobile__BaseLayer__
#define __GameClientMobile__BaseLayer__

USING_NS_CC;
using namespace flownet;

class BaseLayer : public CCLayer
{
protected:
    StageType m_StageType;

public:
    BaseLayer();
    virtual ~BaseLayer();
    
public:
    virtual bool init() override;
    virtual bool initWithStageType(StageType stageType);

    virtual void OnResponse();
    virtual void OnLoad();
};

#endif /* defined(__GameClientMobile__BaseLayer__) */
