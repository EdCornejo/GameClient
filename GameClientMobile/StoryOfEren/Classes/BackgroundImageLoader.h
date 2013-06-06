//
//  BackgroundImageLoader.h
//  GameClientMobile
//
//  Created by SungJinYoo on 3/12/13.
//
//

#ifndef __GameClientMobile__BackgroundImageLoader__
#define __GameClientMobile__BackgroundImageLoader__

USING_NS_CC;

class BackgroundImageLoader
{
private:
    BackgroundImageLoader();
    ~BackgroundImageLoader();
    
public:
    static CCSprite* GetBackgroundImage(flownet::StageType stageType);
};


#endif /* defined(__GameClientMobile__BackgroundImageLoader__) */
