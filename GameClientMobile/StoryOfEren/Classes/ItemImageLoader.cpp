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
        case ItemType_VioletPotion9:
            imageName = "violet_potion";
            break;
        case ItemType_OakWand:
            imageName = "oak_wand";
            break;
        case ItemType_CrystalWand:
            imageName = "crystal_wand";
            break;
        case ItemType_MoonWand :
            imageName = "moon_wand";
            break;
        case ItemType_WizardHat_Boy :
            imageName = "wizard_hat_boy";
            break;
        case ItemType_WizardHat_Girl:
            imageName = "wizard_hat_girl";
            break;
        case ItemType_WizardRobe_Boy :
            imageName = "wizard_robe_boy";
            break;
        case ItemType_WizardRobe_Girl :
            imageName = "wizard_robe_girl";
            break;
        case ItemType_Fedora_Girl :
            imageName = "fedora_girl";
            break;
        case ItemType_SpellBook:
            imageName = "spell_book";
            break;
        case ItemType_SpellBookPage:
            imageName = "spell_book_page";
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
    imageName += ".png";
    
    CCSprite* image = CCSprite::create(imageName.c_str());
    ASSERT_DEBUG(image != nullptr);
    
    return image;
}

CCSprite* ItemImageLoader::GetItemStageImage(flownet::ItemType itemType)
{
    std::string imageName = "items/";
    imageName += GetImageFileName(itemType);
    imageName += ".png";
    
    CCSprite* image = CCSprite::create(imageName.c_str());
    ASSERT_DEBUG(image != nullptr);
    
    return image;
}
