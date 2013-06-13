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
    case flownet::StageObjectType_IceColumn:
        imageName += "ice_column.png";
        break;
    case flownet::StageObjectType_IceColumn2:
        imageName += "ice_column2.png";
        break;
    case flownet::StageObjectType_IceRock:
        imageName += "ice_rock.png";
        break;
    case flownet::StageObjectType_IceRock2:
        imageName += "ice_rock2.png";
        break;
    case flownet::StageObjectType_IceStream:
        imageName += "ice_stream.png";
        break;
    case flownet::StageObjectType_IcePond:
        imageName += "ice_pond.png";
        break;
    case flownet::StageObjectType_IceTree:
        imageName += "ice_tree.png";
        break;
    case flownet::StageObjectType_PrivateStorage:
        imageName += "ice_column2.png";
        break;
    case flownet::StageObjectType_PrivateCampFire:
        imageName += "ice_column.png";
        break;
    case flownet::StageObjectType_ColosseumBase:
        imageName += "ice_pond.png";
        break;
    case flownet::StageObjectType_ColosseumCenter:
        imageName += "ice_tree.png";
        break;
    default:
        ASSERT_DEBUG(false);
        break;
    }
    
    return CCSprite::create(imageName.c_str());
}