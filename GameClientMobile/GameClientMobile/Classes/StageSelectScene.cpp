//
//  StageSelectScene.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 5/21/13.
//
//

#include "Headers.pch"

RunningStageInfoMenuItem::RunningStageInfoMenuItem() {}

RunningStageInfoMenuItem::~RunningStageInfoMenuItem() {}

bool RunningStageInfoMenuItem::init(std::string& playerName, CCObject* target, SEL_MenuHandler selector)
{
    if(!CCMenuItemImage::initWithNormalImage("ui/stage_select_scene/running_info_button_normal.png", "ui/stage_select_scene/running_info_button_active.png", NULL, target, selector)) return false;
    
    CCLabelTTF* playerNameLabel = CCLabelTTF::create(playerName.c_str(), "thonburi", 20);
    this->addChild(playerNameLabel);
    
    return true;
}

RunningStageInfoMenuItem* RunningStageInfoMenuItem::create(std::string& playerName, CCObject* target, SEL_MenuHandler selector)
{
    RunningStageInfoMenuItem* menuItem = new RunningStageInfoMenuItem();
    if(menuItem && menuItem->init(playerName, target, selector))
    {
        menuItem->autorelease();
        return menuItem;
    }
    else
    {
        delete menuItem;
        return nullptr;
    }
}


StageInfoLayer::StageInfoLayer() {}

StageInfoLayer::~StageInfoLayer()
{
    if(this->m_Background)
    {
        this->m_Background->release();
        this->m_Background = nullptr;
    }
}

bool StageInfoLayer::init()
{
    if(!CCLayerColor::initWithColor(ccc4(61,43,39, 150))) return false;
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    this->m_Background = CCSprite::create("ui/stage_select_scene/stage_info_background.png");
    this->m_Background->retain();
    this->m_Background->setPosition(ccp(winSize.width / 2, winSize.height / 2));
    
    this->addChild(this->m_Background);
    
    CCSize bodySize = this->m_Background->getContentSize();
    
    CCSprite* previewImage = StageImageLoader::GetStagePreviewImage(this->m_StageType);
    previewImage->setAnchorPoint(CCPointUpperLeft);
    previewImage->setPosition(ccp(20, bodySize.height - 20));
    this->m_Background->addChild(previewImage);
    
    StagePreviewInfoDictionary* infoDic = StagePreviewInfoDictionary::Instance();
    const StagePreviewInfo* previewInfo = infoDic->FindStagePreviewInfo(this->m_StageType);
    
    CCLabelTTF* stageNameLabel = CCLabelTTF::create(previewInfo->m_StageName.c_str(), "thonburi", 20, CCSizeMake(bodySize.width/2, 0), kCCTextAlignmentLeft);
    stageNameLabel->setAnchorPoint(CCPointUpperLeft);
    stageNameLabel->setPosition(ccp(20, 130));
    this->m_Background->addChild(stageNameLabel);
    
    CCLabelTTF* stageRecommendedLevelLabel = CCLabelTTF::create(previewInfo->m_RecommendedLevel.c_str(), "thonburi", 20, CCSizeMake(bodySize.width/2, 0), kCCTextAlignmentLeft);
    stageRecommendedLevelLabel->setAnchorPoint(CCPointUpperLeft);
    stageRecommendedLevelLabel->setPosition(ccp(20, 100));
    this->m_Background->addChild(stageRecommendedLevelLabel);
    
    CCLabelTTF* stageDescriptionLabel = CCLabelTTF::create(previewInfo->m_Description.c_str(), "thonburi", 20, CCSizeMake(bodySize.width/2, 0), kCCTextAlignmentLeft);
    stageDescriptionLabel->setAnchorPoint(CCPointUpperRight);
    stageDescriptionLabel->setPosition(ccp(bodySize.width - 20, bodySize.height - 20));
//    this->m_Background->addChild(stageDescriptionLabel);
    
    CCMenuItemImage* createButton = CCMenuItemImage::create("ui/stage_select_scene/create_button_normal.png", "ui/stage_select_scene/create_button_active.png", this, menu_selector(StageInfoLayer::OnCreateButtonTouch));
    createButton->setAnchorPoint(CCPointLowerLeft);
    CCMenu* menu = CCMenu::create(createButton, NULL);
    menu->setPosition(ccp(20, 50));
    menu->setTouchPriority(kCCMenuHandlerPriority - 1);
    
    this->m_Background->addChild(menu);

    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority - 1, true);

    this->setTouchEnabled(true);
    
    return true;
}

StageInfoLayer* StageInfoLayer::create(flownet::StageType stageType)
{
    StageInfoLayer* layer = new StageInfoLayer();
    layer->m_StageType = stageType;
    if(layer && layer->init())
    {
        layer->autorelease();
        return layer;
    }
    else
    {
        delete layer;
        return nullptr;
    }
}

bool StageInfoLayer::ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event)
{
    CCRect bodyRect = GetRect(this->m_Background);
    bodyRect.origin = this->convertToWorldSpace(bodyRect.origin);
    if(!bodyRect.containsPoint(touch->getLocation()))
    {
        // NOTE : remove this layer if touched outside
        this->removeFromParentAndCleanup(true);
    }

    return true;
}

void StageInfoLayer::OnCreateButtonTouch(CCObject* sender)
{
    GPSPoint gps = GPSPoint(43, 54, 0);
    
    this->removeFromParentAndCleanup(true);
    
    GameClient::Instance().GetClientObject().SendCSRequestCreateStage(this->m_StageType, gps);
}

void StageInfoLayer::OnRunningStageInfoButtonTouch(cocos2d::CCObject *sender)
{
    CCLOG("Join to this room");
    
    this->removeFromParentAndCleanup(true);
    
    CCNode* button = static_cast<CCNode*>(sender);
    StageType stageType = static_cast<StageType>(button->getTag());
    
    GPSPoint gps = GPSPoint(CCRANDOM_0_1() * 40, CCRANDOM_0_1() * 40, 0);
    
    GameClient::Instance().GetClientObject().SendCSRequestJoinRunningStage(stageType, gps);
}

void StageInfoLayer::DisplayRunningStages(flownet::StagePlayInfoList &stageInfoList)
{
    CCMenu* menu = CCMenu::create();
    std::for_each(stageInfoList.begin(), stageInfoList.end(), [this, menu](StagePlayInfo& info)
    {
        std::string stageIDString;
        stageIDString += info.GetStageID();
        RunningStageInfoMenuItem* runningStageJoinButton = RunningStageInfoMenuItem::create(stageIDString, this, menu_selector(StageInfoLayer::OnRunningStageInfoButtonTouch));
        runningStageJoinButton->setTag(info.GetStageType());
        
        menu->addChild(runningStageJoinButton);
        menu->setTouchPriority(kCCMenuHandlerPriority - 1);
    });
    
    this->m_Background->addChild(menu);
}


/////////////////////////////
/////////////////////////////


StageSelectLayer::StageSelectLayer(): m_StageInfoLayer(nullptr), m_Menu(nullptr), m_WorldMapWidth(0), m_WorldMapHeight(0), m_WorldMap(nullptr) {}

StageSelectLayer::~StageSelectLayer()
{
    if(this->m_StageInfoLayer)
    {
        this->m_StageInfoLayer->release();
        this->m_StageInfoLayer = nullptr;
    }
    if(this->m_Menu)
    {
        this->m_Menu->release();
        this->m_Menu = nullptr;
    }
    if(this->m_WorldMap)
    {
        this->m_WorldMap->release();
        this->m_WorldMap = nullptr;
    }
}

bool StageSelectLayer::init()
{
    if(!CCLayer::init()) return false;
    
    CCSprite* worldmap = CCSprite::create("ui/stage_select_scene/worldmap.gif");
    worldmap->setAnchorPoint(CCPointZero);
    CCSize worldMapSize = worldmap->getContentSize();
    this->m_WorldMapWidth = worldMapSize.width;
    this->m_WorldMapHeight = worldMapSize.height;
    
    // TO DO : read map object position and set
    // 지금은 월드맵 오브젝트 위치를 맵사이즈안의 퍼센티지로 표현하겠음 obejct1 은 0.21, 0.21
    // object2 는 0.23, 0.56임
    // 오브젝트의 태그로 자신의 스테이지 타입을 저장한다
    CCMenuItemImage* object1 = CCMenuItemImage::create("ui/stage_select_scene/object_normal.png", "ui/stage_select_scene/object_active.png", this, menu_selector(StageSelectLayer::OnStageSelectObjectTouch));
    object1->setTag(StageType_MushroomField);
    object1->setPosition(ccp(worldMapSize.width * 0.21, worldMapSize.height * 0.21));
    CCMenuItemImage* object2 = CCMenuItemImage::create("ui/stage_select_scene/object_normal.png", "ui/stage_select_scene/object_active.png", this, menu_selector(StageSelectLayer::OnStageSelectObjectTouch));
    object2->setTag(StageType_WolfForest);
    object2->setPosition(ccp(worldMapSize.width * 0.23, worldMapSize.height * 0.56));
    this->m_Menu = CCMenu::create(object1, object2, NULL);
    this->m_Menu->retain();
    this->m_Menu->setPosition(CCPointZero);

    this->m_WorldMap = CCParallaxNode::create();
    this->m_WorldMap->retain();
    this->m_WorldMap->addChild(worldmap, 0, ccp(1, 1), CCPointZero);

    this->addChild(this->m_WorldMap);
    this->addChild(this->m_Menu);
    
    this->setTouchEnabled(true);
    
    return true;
}

void StageSelectLayer::ccTouchesMoved(cocos2d::CCSet *touches, cocos2d::CCEvent *event)
{
    CCTouch *touch = (CCTouch*)touches->anyObject();
    CCPoint diff = touch->getDelta();
    
    CCPoint currentPos = this->m_WorldMap->getPosition();
    
    CCPoint newPos = ccpAdd(currentPos, diff);
    
    if(newPos.x >= 0)
    {
        newPos.x = 0;
    }
    if(newPos.x - 480 <= -this->m_WorldMapWidth) // + 480 is screen size
    {
        newPos.x = -this->m_WorldMapWidth + 480;
    }
    if(newPos.y >= 0)
    {
        newPos.y = 0;
    }
    if(newPos.y - 320 <= -this->m_WorldMapHeight)
    {
        newPos.y = -this->m_WorldMapHeight + 320;
    }
    
    this->m_WorldMap->setPosition( newPos );
    this->m_Menu->setPosition( newPos );
}

void StageSelectLayer::OnStageSelectObjectTouch(cocos2d::CCObject *sender)
{
    CCNode* node = static_cast<CCNode*>(sender);
    StageType stageType = static_cast<StageType>(node->getTag());
    
    // TO DO : popup a new layer or scene
    if(this->m_StageInfoLayer)
    {
        this->m_StageInfoLayer->removeFromParentAndCleanup(true);
        this->m_StageInfoLayer->release();
        this->m_StageInfoLayer = nullptr;
    }
    
    this->m_StageInfoLayer = StageInfoLayer::create(stageType);
    this->m_StageInfoLayer->retain();
    
    this->addChild(this->m_StageInfoLayer);
    
    // TO DO : request running stage infos
    GPSPoint gps = GPSPoint(CCRANDOM_0_1() * 40, CCRANDOM_0_1() * 40, 0);
    GameClient::Instance().GetClientObject().SendCSRequestGetStagePlayInfoList(stageType, gps);
}

void StageSelectLayer::DisplayRunningStages(flownet::StagePlayInfoList& stageInfoList)
{
    if(this->m_StageInfoLayer)
    {
        this->m_StageInfoLayer->DisplayRunningStages(stageInfoList);
    }
}


StageSelectScene::StageSelectScene(): m_StageSelectLayer(nullptr){}

StageSelectScene::~StageSelectScene()
{
    if(this->m_StageSelectLayer)
    {
        this->m_StageSelectLayer->release();
        this->m_StageSelectLayer = nullptr;
    }
}

bool StageSelectScene::init()
{
    if(!BaseScene::init()) return false;
    
    this->m_StageSelectLayer = StageSelectLayer::create();
    this->m_StageSelectLayer->retain();
    this->addChild(this->m_StageSelectLayer);
    
    scheduleUpdate();
    
    return true;
}

void StageSelectScene::update(float deltaTime)
{
    BaseScene::update(deltaTime);
}

void StageSelectScene::DisplayRunningStages(flownet::StagePlayInfoList &stageInfoList)
{
    if(this->m_StageSelectLayer)
    {
        this->m_StageSelectLayer->DisplayRunningStages(stageInfoList);
    }
}
