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

CCSprite* BackgroundImageLoader::GetBackgroundImage(StageType stageType)
{
    std::string imageFileName = "stage/background/";
    
    switch(stageType)
    {
        case flownet::StageType_Intro:
            imageFileName += "mushroom_field.png";
            break;
        case flownet::StageType_MushroomField:
            imageFileName += "mushroom_field.png";
            break;
        case flownet::StageType_SpiderCaveFront:
            imageFileName += "spider_cave_front.png";
            break;
        case flownet::StageType_SpiderCave:
            imageFileName += "spider_cave.png";
            break;
        case flownet::StageType_IvoryTower:
            imageFileName += "ivorytower_background.png";
            break;
        case flownet::StageType_PrivateLibrary:
            imageFileName += "privatelibrary_background.png";
            break;
        case flownet::StageType_GrowingNature:
            imageFileName += "growingnature_background.png";
            break;
        case flownet::StageType_MotherNature:
            imageFileName += "mothernature_background.png";
            break;
        case flownet::StageType_WeirdForest:
            imageFileName += "weirdforest_background.png";
            break;
        case flownet::StageType_Loading:
            imageFileName += "loading.png";
            break;
        case flownet::StageType_WolfForest:
            imageFileName += "wolf_forest.png";
            break;
        default:
            imageFileName += "default.png";
    }
    
    CCSprite* image = CCSprite::create(imageFileName.c_str());
    
    ASSERT_DEBUG(image != nullptr);
    
    return image;
}
