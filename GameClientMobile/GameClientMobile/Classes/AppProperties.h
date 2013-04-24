//
//  AppProperties.h
//  GameClientMobile
//
//  Created by SungJinYoo on 3/7/13.
//
//

#ifndef GameClientMobile_AppProperties_h
#define GameClientMobile_AppProperties_h

#define HEARTBEAT_DEBUG_SCREEN_ON  // ON and OFF
#define COCOS_FPS_SCREEN_ON // ON and OFF

#include "cocos2d.h"

using namespace cocos2d;
using namespace cocos2d::extension;

enum ActionType {
    ActionType_NONE = 0,
    ActionType_Animation,
    ActionType_Effect,
    ActionType_Spell,
    ActionType_UI,
};

static CCPoint CCPointMid = ccp(240, 160);  // this must be related with design resolution size
static CCPoint CharacterAnchorPoint = ccp(0.5, 0);
static CCPoint CCPointUpperLeft = ccp(0, 1);
static CCPoint CCPointUpperRight = ccp(1, 1);
static CCPoint CCPointLowerLeft = ccp(0, 0); // same as CCPointZero
static CCPoint CCPointLowerRight = ccp(1, 0);
static CCPoint CCPointUpperMid = ccp(0.5, 1);
static CCPoint CCPointLowerMid = ccp(0.5, 0); // same as CharacterAnchorPoint


typedef struct tagResource
{
    cocos2d::CCSize size;
    char directory[100];
}Resource;

static Resource smallResource  =  { cocos2d::CCSizeMake(480, 320),   "iphone" /*"iphone"*/ };
static Resource iphoneHDResource = { cocos2d::CCSizeMake(960, 640), "iphonehd" /*"iphone retina"*/};
static Resource mediumResource =  { cocos2d::CCSizeMake(1024, 768),  "iphonehd"/*"ipad"*/   };
static Resource largeResource  =  { cocos2d::CCSizeMake(2048, 1536), "iphonehd"/*"ipadhd"*/ };
static cocos2d::CCSize designResolutionSize = cocos2d::CCSizeMake(480, 320);


static CCRect GetRect(CCNode* node)
{
    CCRect rc;
    rc.origin = node->getPosition();
    rc.size = node->getContentSize();
    rc.origin.x -= rc.size.width / 2;
    rc.origin.y -= rc.size.height / 2;

    return rc;
}

#endif
