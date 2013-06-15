//
//  GameClientTypes.h
//  StoryOfEren
//
//  Created by SungJinYoo on 6/11/13.
//
//

#ifndef StoryOfEren_GameClientTypes_h
#define StoryOfEren_GameClientTypes_h

namespace flownet {

enum SpellEffectType
{
    SpellEffectType_None = -1,
    SpellEffectType_Teleport,
    SpellEffectType_Max,
};


// NOTE : maybe the number of events will be less than 20 per stage so give a gap between stages about 20
enum AfterEventType
{
    AfterEventType_None = -1,
    /* Intro Stage */
    AfterEventType_AskForTutorial = 0,
    
    /* Clan Community Stage */
    AfterEventType_OpenShop = 20,
    AfterEventType_Max,
};

}

#endif
