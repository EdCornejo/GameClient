//
//  StageImageLoader.h
//  GameClientMobile
//
//  Created by SungJinYoo on 5/22/13.
//
//

#ifndef __GameClientMobile__StageImageLoader__
#define __GameClientMobile__StageImageLoader__

class StageImageLoader
{
private:
    static std::string GetStageName(flownet::StageType stageType);
public:
    static CCSprite* GetStagePreviewImage(flownet::StageType stageType);
    static CCSprite* GetStageBackgroundImage(flownet::StageType stageType); // return type of this might be CCParallaxNode
};

#endif /* defined(__GameClientMobile__StageImageLoader__) */
