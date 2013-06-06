//
//  LoadingLayer.h
//  GameClientMobile
//
//  Created by SungJinYoo on 5/24/13.
//
//

#ifndef __GameClientMobile__LoadingLayer__
#define __GameClientMobile__LoadingLayer__

class LoadingLayer : public BaseLayer
{
public:
    LoadingLayer();
    virtual ~LoadingLayer();
    
    virtual bool init() override;
    
    CREATE_FUNC(LoadingLayer);
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event) override;
};

#endif /* defined(__GameClientMobile__LoadingLayer__) */
