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
    
    static std::string GetFileName(flownet::StageType stageType);
    
public:
    static CCSprite* GetSkyImage(flownet::StageType stageType);
    static CCSprite* GetTerrainImage(flownet::StageType stageType);
    static CCSprite* GetGroundImage(flownet::StageType stageType);
};


#endif /* defined(__GameClientMobile__BackgroundImageLoader__) */
