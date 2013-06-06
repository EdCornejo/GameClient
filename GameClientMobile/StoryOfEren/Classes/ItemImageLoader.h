//
//  ItemImageLoader.h
//  GameClientMobile
//
//  Created by SungJinYoo on 4/22/13.
//
//

#ifndef __GameClientMobile__ItemImageLoader__
#define __GameClientMobile__ItemImageLoader__

class ItemImageLoader
{
private:
    static std::string GetImageFileName(flownet::ItemType itemType);

public:
    static CCSprite* GetItemInventoryImage(flownet::ItemType itemType);
    static CCSprite* GetItemStageImage(flownet::ItemType itemType);
};

#endif /* defined(__GameClientMobile__ItemImageLoader__) */
