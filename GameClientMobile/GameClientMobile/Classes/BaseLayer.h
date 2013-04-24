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
    typedef Map<INT64, CCNode*>::type NodeMap;
    
private:
    NodeMap m_NodeMap;
    
protected:
    StageType m_StageType;

public:
    BaseLayer();
    virtual ~BaseLayer();
    
public:
    virtual bool init();
    virtual bool initWithStageType(StageType stageType);
    
    virtual void update(float deltaTime) override;

public:
    void AddNode(INT64 nodeID, CCNode* node);
    void RemoveNode(INT64 nodeID);
    CCNode* FindNodeByNodeID(INT64 nodeID);
};

#endif /* defined(__GameClientMobile__BaseLayer__) */
