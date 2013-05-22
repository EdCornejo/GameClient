//
//  StageImageLoader.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 5/22/13.
//
//

#include "Headers.pch"

std::string StageImageLoader::GetStageName(flownet::StageType stageType)
{
    std::string stageName = "stage/";
    switch (stageType) {
        case flownet::StageType_Loading:
            stageName += "loading";
            break;
        case flownet::StageType_Intro :
            stageName += "intro";
            break;
        case flownet::StageType_MushroomField :
            stageName += "mushroom_field";
            break;
        case flownet::StageType_SpiderCaveFront:
            stageName += "spider_cave_front";
            break;
        case flownet::StageType_SpiderCave :
            stageName += "spider_cave";
            break;
        case flownet::StageType_IvoryTower :
            stageName += "ivory_tower";
            break;
        case flownet::StageType_PrivateLibrary :
            stageName += "private_library";
            break;
        case flownet::StageType_GrowingNature :
            stageName += "growing_nature";
            break;
        case flownet::StageType_MotherNature :
            stageName += "mother_nature";
            break;
        case flownet::StageType_WeirdForest :
            stageName += "weird_forest";
            break;
        case flownet::StageType_WolfForest :
            stageName += "wolf_forest";
            break;
        default:
            stageName += "blank";
            break;
    }
    return stageName;
}

CCSprite* StageImageLoader::GetStagePreviewImage(flownet::StageType stageType)
{
    std::string imageFileName = GetStageName(stageType);
    imageFileName += "_preview.png";
    
    return CCSprite::create(imageFileName.c_str());
}

CCSprite* StageImageLoader::GetStageBackgroundImage(flownet::StageType stageType)
{
    std::string imageFileName = GetStageName(stageType);
    imageFileName += "_background.png";
    
    return CCSprite::create(imageFileName.c_str());
}
