//
//  CaptionLayer.h
//  GameClientMobile
//
//  Created by SungJinYoo on 3/24/13.
//
//

#ifndef __GameClientMobile__CaptionLayer__
#define __GameClientMobile__CaptionLayer__

class CaptionLayer : public BaseLayer
{
public:
    CaptionLayer();
    virtual ~CaptionLayer();
    
    virtual bool init();
    virtual bool initWithStageType(StageType stageType);
    static CaptionLayer* create(StageType stageType);
};

#endif /* defined(__GameClientMobile__CaptionLayer__) */
