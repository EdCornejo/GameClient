//
//  StageSelectScene.h
//  GameClientMobile
//
//  Created by SungJinYoo on 5/21/13.
//
//

#ifndef __GameClientMobile__StageSelectScene__
#define __GameClientMobile__StageSelectScene__

class RunningStageInfoMenuItem : public CCMenuItemImage
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

public:
    StageInfoLayer();
    virtual ~StageInfoLayer();

    virtual bool init();
    
    static StageInfoLayer* create(flownet::StageType stageType);

    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);

private:
    void OnCreateButtonTouch(CCObject* sender);
    void OnRunningStageInfoButtonTouch(CCObject* sender);
    
public:
    void DisplayRunningStages(flownet::StagePlayInfoList& stageInfoList);
};

class StageSelectLayer : public CCLayer
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
    
    virtual bool init();

    CREATE_FUNC(StageSelectLayer);
    
    virtual void ccTouchesMoved(CCSet* touches, CCEvent* event);

private:
    void OnStageSelectObjectTouch(CCObject* sender);
    
public:
    void DisplayRunningStages(flownet::StagePlayInfoList& stageInfoList);
};

class StageSelectScene : public BaseScene
{
private:
    StageSelectLayer* m_StageSelectLayer;

public:
    StageSelectScene();
    virtual ~StageSelectScene();
    
    virtual bool init();
    
    CREATE_FUNC(StageSelectScene);
    
    virtual void update(float deltaTime);
    
    void DisplayRunningStages(flownet::StagePlayInfoList& stageInfoList);
};

#endif /* defined(__GameClientMobile__StageSelectScene__) */
