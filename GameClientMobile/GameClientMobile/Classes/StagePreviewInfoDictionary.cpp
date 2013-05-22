//
//  StagePreviewInfoDictionary.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 5/22/13.
//
//

#include "Headers.pch"

StagePreviewInfoDictionary* StagePreviewInfoDictionary::instance = nullptr;

StagePreviewInfoDictionary::StagePreviewInfoDictionary()
{
    CCDictionary* dict = CCDictionary::createWithContentsOfFile("stageinfos.plist");
    CCArray* stageInfos = static_cast<CCArray*>(dict->objectForKey("stages"));
    CCObject* object;
    CCARRAY_FOREACH(stageInfos, object)
    {
        CCDictionary* stageInfoDict = static_cast<CCDictionary*>(object);
        // TO DO : multi region support comes here
        StagePreviewInfo* stageInfo = new StagePreviewInfo();
        stageInfo->m_StageName = static_cast<CCString*>(stageInfoDict->objectForKey("name_kr"))->getCString();
        stageInfo->m_RecommendedLevel = static_cast<CCString*>(stageInfoDict->objectForKey("recommended_level"))->getCString();
        stageInfo->m_Description = static_cast<CCString*>(stageInfoDict->objectForKey("description_kr"))->getCString();
        
        flownet::StageType stageType = static_cast<StageType>(atoi(static_cast<CCString*>(stageInfoDict->objectForKey("stage_type"))->getCString()));
        
        this->m_StagePreviewInfoMap.insert(StagePreviewInfoMap::value_type(stageType, stageInfo));
    }
    delete dict;
}

StagePreviewInfoDictionary::~StagePreviewInfoDictionary()
{
    std::for_each(this->m_StagePreviewInfoMap.begin(), this->m_StagePreviewInfoMap.end(), [](StagePreviewInfoMap::value_type pair){
        delete pair.second;
        pair.second = nullptr;
    });
    
    this->m_StagePreviewInfoMap.clear();
}

StagePreviewInfoDictionary* StagePreviewInfoDictionary::Instance()
{
    if(!instance)
    {
        instance = new StagePreviewInfoDictionary();
    }
    return instance;
}

const StagePreviewInfo* StagePreviewInfoDictionary::FindStagePreviewInfo(flownet::StageType stageType)
{
    StagePreviewInfoMap::iterator iter = this->m_StagePreviewInfoMap.find(stageType);
    if(iter == this->m_StagePreviewInfoMap.end()) ASSERT_DEBUG(iter != this->m_StagePreviewInfoMap.end());
    
    return iter->second;
}