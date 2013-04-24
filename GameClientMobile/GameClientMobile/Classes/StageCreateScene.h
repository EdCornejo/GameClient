//
//  StageCreateScene.h
//  GameClientMobile
//
//  Created by SungJinYoo on 3/24/13.
//
//

#ifndef __GameClientMobile__StageCreateScene__
#define __GameClientMobile__StageCreateScene__

class StagePreviewInfo : public CCObject
{
public:
    StagePreviewInfo() : m_Name(), m_RecommendedLevel(), m_PreviewImage(), m_StageType(StageType_NONE), m_Description() {}
    virtual ~StagePreviewInfo() {}
    
    std::string m_Name;
    std::string m_RecommendedLevel;
    std::string m_PreviewImage;
    StageType m_StageType;
    std::string m_Description;
};

typedef std::vector<StagePreviewInfo*> StagePreviewInfoList;

class StageDetailInfoLayer : public CCLayer
{
private:
    enum{
        MARGIN_TOP = 10,
        MARGIN_LEFT = 10,
        LEFT_SIDE_WIDTH = 230,
        LEFT_SIDE_POSITION_X = MARGIN_LEFT,
        DETAIL_PREVIEW_IMAGE_HEIGHT = 154,
        DETAIL_PREVIEW_IMAGE_POSITION_Y = 320 - MARGIN_TOP - DETAIL_PREVIEW_IMAGE_HEIGHT,
        STAGE_NAME_POSITION_Y = DETAIL_PREVIEW_IMAGE_POSITION_Y - MARGIN_TOP,
        STAGE_LEVEL_POSITION_Y = STAGE_NAME_POSITION_Y,
        RIGHT_SIDE_POSITION_X = LEFT_SIDE_POSITION_X + LEFT_SIDE_WIDTH + MARGIN_LEFT,
        RIGHT_SIDE_WIDTH = 190,
        DESCRIPTION_POSITION_Y = 320 - MARGIN_TOP,
        CREATE_BUTTON_HEIGHT = 40,
        CREATE_BUTTON_POSITION_Y = 40 - CREATE_BUTTON_HEIGHT,
    };

    StagePreviewInfo m_StagePreviewInfo;
    
public:
    StageDetailInfoLayer();
    virtual ~StageDetailInfoLayer();
    
    virtual bool initWithStagePreviewInfo(StagePreviewInfo stagePreviewInfo);
    static StageDetailInfoLayer* create(StagePreviewInfo stagePreviewInfo);
};

typedef std::vector<CCMenu*> StageInfoMenuList;

class StageCreateScene : public BaseScene
{
private:
    enum{
        PER_PAGE = 8,
        PER_ROW = 3,
        MARGIN_X = 15,
        MARGIN_Y = 10,
        PREVIEW_IMAGE_SIZE_X = 140,
        PREVIEW_IMAGE_SIZE_Y = 94,
        PREVIEW_IMAGE_OFFSET_Y = 47,
        PREV_BUTTON_POSITION_X = 120,
        PREV_BUTTON_POSITION_Y = 94,
        NEXT_BUTTON_POSITION_X = 360,
        NEXT_BUTTON_POSITION_Y = 94,
        PAGE_LABEL_POSITION_X = 240,
        PAGE_LABEL_POSITION_Y = 94,
    };
    
    StagePreviewInfoList m_StagePreviewInfoList;
    int m_CurrentPage;
    int m_TotalPages;
    StageInfoMenuList m_StageInfoMenuList;
    StageDetailInfoLayer* m_StageDetailInfoLayer;
    
public:
    StageCreateScene();
    virtual ~StageCreateScene();
    
    virtual bool init();
    static StageCreateScene* create();
    virtual void update(float deltaTime) override;

    void OnStageInfoButtonClick(CCObject* sender);
    void OnStageCreateButtonClick(CCObject* sender);
    void OnStageDetailCloseButtonClick(CCObject* sender);
    
public:
    StageDetailInfoLayer* GetStageDetailInfoLayer();
    StageInfoMenuList& GetStageInfoMenuList();
};

#endif /* defined(__GameClientMobile__StageCreateScene__) */
