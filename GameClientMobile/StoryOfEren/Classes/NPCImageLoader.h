//
//  NPCImageLoader.h
//  StoryOfEren
//
//  Created by SungJinYoo on 6/7/13.
//
//

#ifndef __StoryOfEren__NPCImageLoader__
#define __StoryOfEren__NPCImageLoader__

class NPCImageLoader
{
private:
    NPCImageLoader();
    ~NPCImageLoader();

public:
    static CCSprite* GetNPCImage(flownet::NPCType npcType);
    
};

#endif /* defined(__StoryOfEren__NPCImageLoader__) */
