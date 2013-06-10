//
//  NPCImageLoader.cpp
//  StoryOfEren
//
//  Created by SungJinYoo on 6/7/13.
//
//

#include "Headers.pch"

NPCImageLoader::NPCImageLoader() {}

NPCImageLoader::~NPCImageLoader() {}

CCSprite* NPCImageLoader::GetNPCImage(flownet::NPCType npcType)
{
    std::string fileName = "npc/";
    switch (npcType) {
    case flownet::NPCType_Dev:
    default:
        fileName += "dev";
        break;
    }
    
    fileName += ".png";
    
    return CCSprite::create(fileName.c_str());
}