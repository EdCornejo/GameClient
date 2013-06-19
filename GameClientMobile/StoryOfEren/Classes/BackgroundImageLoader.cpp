//
//  BackgroundImageLoader.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 3/12/13.
//
//

#include "Headers.pch"

BackgroundImageLoader::BackgroundImageLoader(){}
BackgroundImageLoader::~BackgroundImageLoader(){}

std::string BackgroundImageLoader::GetFileName(flownet::StageType stageType)
{
    std::string fileName;
    switch(stageType)
    {
        case flownet::StageType_Loading:
            fileName = "loading";
            break;
        case flownet::StageType_Intro:
            fileName = "mushroom_field";
            break;
        case flownet::StageType_ClanCommunity:
            fileName = "clan_community";
            break;
        case flownet::StageType_Colosseum:
            fileName = "wolf_forest";
            break;
        case flownet::StageType_MushroomField:
            fileName = "spider_cave_front";
            break;
        case flownet::StageType_SpiderCaveFront:
            fileName = "spider_cave_front";
            break;
        case flownet::StageType_SpiderCave:
            fileName = "spider_cave";
            break;
        case flownet::StageType_IvoryTower:
            fileName = "ivory_tower";
            break;
        case flownet::StageType_PrivateLibrary:
            fileName = "private_library";
            break;
        case flownet::StageType_GrowingNature:
            fileName = "growingnature";
            break;
        case flownet::StageType_MotherNature:
            fileName = "mothernature";
            break;
        case flownet::StageType_WeirdForest:
            fileName = "weirdforest";
            break;
        case flownet::StageType_WolfForest:
            fileName = "wolf_forest";
            break;
        default:
            fileName = "default";
            break;
    }
    
    return fileName;
}

CCSprite* BackgroundImageLoader::GetSkyImage(flownet::StageType stageType)
{
    std::string imageFileName = "stage/background/";
    imageFileName += GetFileName(stageType);
    imageFileName += "_sky.png";
    
    CCSprite* image = CCSprite::create(imageFileName.c_str());
    
    return image;

}

CCSprite* BackgroundImageLoader::GetTerrainImage(flownet::StageType stageType)
{
    std::string imageFileName = "stage/background/";
    imageFileName += GetFileName(stageType);
    imageFileName += "_terrain.png";
    
    CCSprite* image = CCSprite::create(imageFileName.c_str());
    
    return image;
}

CCSprite* BackgroundImageLoader::GetGroundImage(StageType stageType)
{
    std::string imageFileName = "stage/background/";
    imageFileName += GetFileName(stageType);
    imageFileName += "_ground.png";
    
    CCSprite* image = CCSprite::create(imageFileName.c_str());
    
    return image;
}
