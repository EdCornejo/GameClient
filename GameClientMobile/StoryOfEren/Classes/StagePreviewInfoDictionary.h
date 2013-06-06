//
//  StagePreviewInfoDictionary.h
//  GameClientMobile
//
//  Created by SungJinYoo on 5/22/13.
//
//

#ifndef __GameClientMobile__StagePreviewInfoDictionary__
#define __GameClientMobile__StagePreviewInfoDictionary__

// NOTE : this is a singleton object
struct StagePreviewInfo
{
    std::string m_StageName;
    std::string m_RecommendedLevel;
    std::string m_Description;
  
    StagePreviewInfo(): m_StageName(), m_RecommendedLevel(), m_Description() {}
    StagePreviewInfo(std::string stageName, std::string recommendedLevel, std::string description):m_StageName(stageName), m_RecommendedLevel(recommendedLevel), m_Description(description) {}
    virtual ~StagePreviewInfo() {}
};

typedef flownet::Map<flownet::StageType, StagePreviewInfo*>::type StagePreviewInfoMap;

class StagePreviewInfoDictionary
{
private:
    static StagePreviewInfoDictionary* instance;

    StagePreviewInfoMap m_StagePreviewInfoMap;

    StagePreviewInfoDictionary();
    ~StagePreviewInfoDictionary();
public:
    static StagePreviewInfoDictionary* Instance();
    
    const StagePreviewInfo* FindStagePreviewInfo(flownet::StageType stageType);
};

#endif /* defined(__GameClientMobile__StagePreviewInfoDictionary__) */
