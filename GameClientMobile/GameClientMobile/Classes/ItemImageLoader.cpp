//
//  ItemImageLoader.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 4/22/13.
//
//

#include "Headers.pch"

std::string ItemImageLoader::GetImageFileName(flownet::ItemType itemType)
{
    std::string imageName;
    switch (itemType) {
        case ItemType_RedPotion9:
            imageName = "red_potion";
            break;
        case ItemType_BluePotion9:
            imageName = "blue_potion";
            break;
        default:
            imageName = "item_none";
    }
    return imageName;
}

CCSprite* ItemImageLoader::GetItemInventoryImage(flownet::ItemType itemType)
{
    std::string imageName = "items/";
    imageName += GetImageFileName(itemType);
    imageName += "_inven.png";
    
    CCSprite* image = CCSprite::create(imageName.c_str());
    ASSERT_DEBUG(image != nullptr);
    
    CCLOG("%s", imageName.c_str());
    
    return image;
}

CCSprite* ItemImageLoader::GetItemStageImage(flownet::ItemType itemType)
{
    std::string imageName = "items/";
    imageName += GetImageFileName(itemType);
    imageName += "_stage.png";
    
    CCSprite* image = CCSprite::create(imageName.c_str());
    ASSERT_DEBUG(image != nullptr);
    
    return image;
}
