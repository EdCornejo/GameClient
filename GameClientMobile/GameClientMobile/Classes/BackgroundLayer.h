//
//  BackgroundLayer.h
//  GameClientMobile
//
//  Created by SungJinYoo on 3/6/13.
//
//

#ifndef __GameClientMobile__BackgroundLayer__
#define __GameClientMobile__BackgroundLayer__

USING_NS_CC;
using namespace flownet;

class BackgroundLayer : public BaseLayer
{
private:
    CCSprite* m_BackgroundImage;
    
public:
    BackgroundLayer();
    virtual ~BackgroundLayer();
    
public:
    bool initWithBackgroundFileName(const char* fileName);
    bool initWithStageType(StageType stageType);

    static BackgroundLayer* create(const char* fileName);
    static BackgroundLayer* create(StageType stageType);
    
    virtual void update(float deltaTime);

    void AttachBackgroundImage(CCSprite* backgroundImage);
};

#endif /* defined(__GameClientMobile__BackgroundLayer__) */
