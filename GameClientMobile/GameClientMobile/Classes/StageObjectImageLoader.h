//
//  StageObjectImageLoader.h
//  GameClientMobile
//
//  Created by SungJinYoo on 5/28/13.
//
//

#ifndef __GameClientMobile__StageObjectImageLoader__
#define __GameClientMobile__StageObjectImageLoader__

class StageObjectImageLoader
{
private:
    StageObjectImageLoader();
    ~StageObjectImageLoader();
    
public:
    static CCSprite* GetStageObjectImage(flownet::StageObjectType stageObjectType);
};

#endif /* defined(__GameClientMobile__StageObjectImageLoader__) */
