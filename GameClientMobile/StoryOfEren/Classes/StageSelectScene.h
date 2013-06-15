//
//  StageSelectScene.h
//  GameClientMobile
//
//  Created by SungJinYoo on 5/21/13.
//
//

#ifndef __GameClientMobile__StageSelectScene__
#define __GameClientMobile__StageSelectScene__

class RunningStageInfoMenuItem : public CCMenuItemSprite
{
public:
    RunningStageInfoMenuItem();
    virtual ~RunningStageInfoMenuItem();
    
    virtual bool init(std::string& playerName, CCObject* target, SEL_MenuHandler selector);
    
    static RunningStageInfoMenuItem* create(std::string& playerName, CCObject* target, SEL_MenuHandler selector);
};

class StageInfoLayer : public CCLayerColor
{
private:
    flownet::StageType m_StageType;
    CCSprite* m_Background;
    CCMenuItemImage* m_CreateButton;

public:
    StageInfoLayer();
    virtual ~StageInfoLayer();

    virtual bool init() override;
    
    static StageInfoLayer* create(flownet::StageType stageType);

    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event) override;
    
    void OnResponse();

private:
    void OnCreateButtonTouch(CCObject* sender);
    void OnRunningStageInfoButtonTouch(CCObject* sender);
    
public:
    void DisplayRunningStages(flownet::StagePlayInfoList& stageInfoList);
};

class StageSelectLayer : public BaseLayer
{
private:
    StageInfoLayer* m_StageInfoLayer;
    CCMenu* m_Menu;
    int m_WorldMapWidth;
    int m_WorldMapHeight;
    CCParallaxNode* m_WorldMap;
    
public :
    StageSelectLayer();
    virtual ~StageSelectLayer();
    
    virtual bool init() override;

    CREATE_FUNC(StageSelectLayer);
    
    virtual void ccTouchesMoved(CCSet* touches, CCEvent* event) override;
    
    virtual void OnResponse() override;

private:
    void OnStageSelectObjectTouch(CCObject* sender);
    
public:
    void DisplayRunningStages(flownet::StagePlayInfoList& stageInfoList);
};

class StageSelectScene : public BaseScene
{
private:
    StageSelectLayer* m_StageSelectLayer;
    CCMenuItemSprite* m_BackButton;

public:
    StageSelectScene();
    virtual ~StageSelectScene();
    
    virtual bool init() override;
    
    CREATE_FUNC(StageSelectScene);
    
    virtual void update(float deltaTime) override;
    
    void DisplayRunningStages(flownet::StagePlayInfoList& stageInfoList);
    
    virtual void OnResponse() override;
    
private:
    void OnBackButtonTouch(CCObject* object);
};

#endif /* defined(__GameClientMobile__StageSelectScene__) */
