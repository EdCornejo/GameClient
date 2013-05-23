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
    std::string imageFileName;
    
    switch(stageType)
    {
        case flownet::StageType_Intro:
            imageFileName = "background/mushroom_field.png";
            break;
        case flownet::StageType_MushroomField:
            imageFileName = "background/mushroom_field.png";
            break;
        case flownet::StageType_SpiderCaveFront:
            imageFileName = "background/spider_cave_front.png";
            break;
        case flownet::StageType_SpiderCave:
            imageFileName = "background/spider_cave.png";
            break;
        case flownet::StageType_IvoryTower:
            imageFileName = "background/ivorytower_background.png";
            break;
        case flownet::StageType_PrivateLibrary:
            imageFileName = "background/privatelibrary_background.png";
            break;
        case flownet::StageType_GrowingNature:
            imageFileName = "background/growingnature_background.png";
            break;
        case flownet::StageType_MotherNature:
            imageFileName = "background/mothernature_background.png";
            break;
        case flownet::StageType_WeirdForest:
            imageFileName = "background/weirdforest_background.png";
            break;
        case flownet::StageType_Loading:
            imageFileName = "background/loading.png";
            break;
        case flownet::StageType_WolfForest:
            imageFileName = "background/wolf_forest.png";
            break;
        default:
            imageFileName = "background/default.png";
    }
    
    CCSprite* image = CCSprite::create(imageFileName.c_str());
    
    ASSERT_DEBUG(image != nullptr);
    
    return image;
}
