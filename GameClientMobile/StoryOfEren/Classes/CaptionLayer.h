//
//  CaptionLayer.h
//  GameClientMobile
//
//  Created by SungJinYoo on 3/24/13.
//
//

#ifndef __GameClientMobile__CaptionLayer__
#define __GameClientMobile__CaptionLayer__

typedef Vector<std::string>::type CaptionList;

enum CaptionCharacterPosition {
    CaptionCharacterPosition_None = -1,
    CaptionCharacterPosition_LEFT,
    CaptionCharacterPosition_RIGHT,
    CaptionCharacterPosition_Max,
};

struct CaptionInfo {
    CaptionCharacterPosition m_CharacterPosition;
    std::string m_Name;
    std::string m_DisplayName;
    std::string m_State;
    CaptionList m_CaptionList;
    std::string m_AfterEvent;
    
    CaptionInfo(){}
    ~CaptionInfo(){}
};

typedef Vector<CaptionInfo*>::type CaptionInfoList;

class CaptionLayer : public BaseLayer
{
private:
    flownet::StageType m_StageType;
    CCLabelTTF* m_CaptionLabel;
    CCLabelTTF* m_NameLabel;
    CCSprite* m_CharacterImage;
    CaptionInfoList m_CaptionInfoList;
    
    int m_CaptionInfoListIndex;
    int m_CaptionIndex;
    

public:
    CaptionLayer();
    virtual ~CaptionLayer();
    
    virtual bool init();
    static CaptionLayer* create(StageType stageType);
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    
private:
    std::string GetCaptionInfoFileName(flownet::StageType stageType);
    bool LoadCaptionInfoFromFile(const char* fileName, int tier);
    void CaptionEnded();
};

#endif /* defined(__GameClientMobile__CaptionLayer__) */
