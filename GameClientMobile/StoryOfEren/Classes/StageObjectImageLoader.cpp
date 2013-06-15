//
//  StageObjectImageLoader.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 5/28/13.
//
//

#include "Headers.pch"

StageObjectImageLoader::StageObjectImageLoader() {}

StageObjectImageLoader::~StageObjectImageLoader() {}

CCSprite* StageObjectImageLoader::GetStageObjectImage(flownet::StageObjectType stageObjectType)
{
    std::string imageName = "stage/object/";
    switch (stageObjectType) {
        case flownet::StageObjectType_IcePoll:
            imageName += "ice_poll.png";
            break;
        case flownet::StageObjectType_IceStone1:
            imageName += "ice_stone1.png";
            break;
        case flownet::StageObjectType_IceStone2:
            imageName += "ice_stone2.png";
            break;
        case flownet::StageObjectType_IceStone3:
            imageName += "ice_stone3.png";
            break;
        case flownet::StageObjectType_IceTree:
            imageName += "ice_tree.png";
            break;
        case flownet::StageObjectType_IcePond:
            imageName += "ice_pond.png";
            break;
        case flownet::StageObjectType_ColosseumBase:
            imageName += "colosseum_base.png";
            break;
        case flownet::StageObjectType_ColosseumCenter:
            imageName += "colosseum_center.png";
            break;
        case flownet::StageObjectType_BookShelf:
            imageName +="bookshelf.png";
            break;
        case flownet::StageObjectType_PrivateChest:
            imageName +="private_chest.png";
            break;
        case flownet::StageObjectType_Campfire:
            imageName +="campfire.png";
            break;
        default:
            ASSERT_DEBUG(false);
            break;
    }
    
    return CCSprite::create(imageName.c_str());
}