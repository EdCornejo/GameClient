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
    CCSprite* normalImage = CCSprite::create("ui/stage_select_scene/stage_info.png");
    CCSprite* activeImage = CCSprite::create("ui/stage_select_scene/stage_info.png");
    activeImage->setColor(ccBLACK);

    if(!CCMenuItemSprite::initWithNormalSprite(normalImage, activeImage, NULL, target, selector)) return false;
    
    CCSize buttonSize = normalImage->getContentSize();
    
    CCLabelTTF* playerNameLabel = CCLabelTTF::create(playerName.c_str(), "thonburi", 20);
    playerNameLabel->setAnchorPoint(CCPointMidLeft);
    playerNameLabel->setPosition(ccp(10, buttonSize.height / 2));
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


StageInfoLayer::StageInfoLayer(): m_StageType(StageType_NONE), m_Background(nullptr), m_CreateButton(nullptr) {}

StageInfoLayer::~StageInfoLayer()
{
    if(this->m_Background)
    {
        this->m_Background->release();
        this->m_Background = nullptr;
    }
    if(this->m_CreateButton)
    {
        this->m_CreateButton->release();
        this->m_CreateButton = nullptr;
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
    previewImage->setPosition(ccp(52, bodySize.height - 34));
    this->m_Background->addChild(previewImage);
    
    StagePreviewInfoDictionary* infoDic = StagePreviewInfoDictionary::Instance();
    const StagePreviewInfo* previewInfo = infoDic->FindStagePreviewInfo(this->m_StageType);
    
    CCLabelTTF* stageNameLabel = CCLabelTTF::create(previewInfo->m_StageName.c_str(), "thonburi", 14, CCSizeMake(bodySize.width/2, 0), kCCTextAlignmentLeft);
    stageNameLabel->setAnchorPoint(CCPointUpperLeft);
    stageNameLabel->setPosition(ccp(20, 80));
    this->m_Background->addChild(stageNameLabel);

    this->m_CreateButton = CCMenuItemImage::create("ui/stage_select_scene/create_button_normal.png", "ui/stage_select_scene/create_button_active.png", this, menu_selector(StageInfoLayer::OnCreateButtonTouch));
    this->m_CreateButton->retain();
    this->m_CreateButton->setAnchorPoint(CCPointLowerLeft);
    CCMenu* menu = CCMenu::create(this->m_CreateButton, NULL);
    menu->setPosition(ccp(46, 20));
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

void StageInfoLayer::OnResponse()
{
    this->m_CreateButton->setEnabled(true);
}

void StageInfoLayer::OnCreateButtonTouch(CCObject* sender)
{
    GPSPoint gps = GPSPoint(37.566615 + CCRANDOM_0_1(), 126.977958 + CCRANDOM_0_1());

    this->m_CreateButton->setEnabled(false);
    
    this->removeFromParentAndCleanup(true);
    
    GameClient::Instance().GetClientObject().SendCSRequestCreateStage(this->m_StageType, gps);
}

void StageInfoLayer::OnRunningStageInfoButtonTouch(cocos2d::CCObject *sender)
{
    CCLOG("Join to this room");
    
    this->removeFromParentAndCleanup(true);
    
    BaseScene* scene = static_cast<BaseScene*>(CCDirector::sharedDirector()->getRunningScene());
    scene->AddLoadingSpinnerAndBlock();
    
    CCNode* button = static_cast<CCNode*>(sender);
    StageID stageID = static_cast<StageID>(button->getTag());
    
    GPSPoint gps = GPSPoint(37.566615 + CCRANDOM_0_1(), 126.977958 + CCRANDOM_0_1());
    GameClient::Instance().GetClientObject().SendCSRequestJoinRunningStage(stageID, gps);
}

void StageInfoLayer::DisplayRunningStages(flownet::StagePlayInfoList &stageInfoList)
{
    CCMenu* menu = CCMenu::create();

    int listSize = stageInfoList.size() < 4 ? stageInfoList.size() : 4;
    
    for(int i = 0; i < listSize; i++)
    {
        StagePlayInfo& info = stageInfoList[i];
        std::stringstream ss;
        ss << info.GetStageID();
        std::string stageIDString = ss.str();
        RunningStageInfoMenuItem* runningStageJoinButton = RunningStageInfoMenuItem::create(stageIDString, this, menu_selector(StageInfoLayer::OnRunningStageInfoButtonTouch));
        runningStageJoinButton->setTag(info.GetStageID());
        
        menu->addChild(runningStageJoinButton);
        menu->setTouchPriority(kCCMenuHandlerPriority - 1);
    }
    
    menu->alignItemsVertically();
    menu->setAnchorPoint(CCPointUpperLeft);
    menu->setPosition(ccp(256, 102));
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
    
    CCSprite* worldmap = CCSprite::create("ui/stage_select_scene/map.png");
    worldmap->setAnchorPoint(CCPointZero);
    CCSize worldMapSize = worldmap->getContentSize();
    this->m_WorldMapWidth = worldMapSize.width;
    this->m_WorldMapHeight = worldMapSize.height;
    
    // TO DO : read map object position and set
    // 지금은 월드맵 오브젝트 위치를 맵사이즈안의 퍼센티지로 표현하겠음 obejct1 은 0.21, 0.21
    // object2 는 0.23, 0.56임
    // 오브젝트의 태그로 자신의 스테이지 타입을 저장한다
    CCMenuItemImage* object0 = CCMenuItemImage::create("ui/stage_select_scene/object_normal.png", "ui/stage_select_scene/object_active.png", this, menu_selector(StageSelectLayer::OnStageSelectObjectTouch));
    object0->setTag(StageType_Colosseum);
    object0->setPosition(ccp(worldMapSize.width * 0.15, worldMapSize.height * 0.5));
    CCMenuItemImage* object1 = CCMenuItemImage::create("ui/stage_select_scene/object_normal.png", "ui/stage_select_scene/object_active.png", this, menu_selector(StageSelectLayer::OnStageSelectObjectTouch));
    object1->setTag(StageType_MushroomField);
    object1->setPosition(ccp(worldMapSize.width * 0.64, worldMapSize.height * 0.17));
    CCMenuItemImage* object2 = CCMenuItemImage::create("ui/stage_select_scene/object_normal.png", "ui/stage_select_scene/object_active.png", this, menu_selector(StageSelectLayer::OnStageSelectObjectTouch));
    object2->setTag(StageType_WolfForest);
    object2->setPosition(ccp(worldMapSize.width * 0.08, worldMapSize.height * 0.87));
    CCMenuItemImage* object3 = CCMenuItemImage::create("ui/stage_select_scene/small_object_n.png", "ui/stage_select_scene/small_object_a.png", this, menu_selector(StageSelectLayer::OnStageSelectObjectTouch));
    object3->setTag(StageType_Intro);
    object3->setPosition(ccp(worldMapSize.width * 0.5, worldMapSize.height * 0.5));
    CCMenuItemImage* object4 = CCMenuItemImage::create("ui/stage_select_scene/small_object_n.png", "ui/stage_select_scene/small_object_a.png", this, menu_selector(StageSelectLayer::OnStageSelectObjectTouch));
    object4->setTag(StageType_IvoryTower);
    object4->setPosition(ccp(worldMapSize.width * 0.51, worldMapSize.height * 0.51));
    this->m_Menu = CCMenu::create(object0, object1, object2, object3, object4, NULL);
    this->m_Menu->retain();
    this->m_Menu->setPosition(CCPointZero);

    this->m_WorldMap = CCParallaxNode::create();
    this->m_WorldMap->retain();
    this->m_WorldMap->addChild(worldmap, 0, ccp(1, 1), CCPointZero);
    
    
    // 좀더 정확한 초기 위치 조정 코드를 넣어라
//    this->m_WorldMap->setPosition( ccp(-worldMapSize.width / 2, -worldMapSize.height / 2) );
//    this->m_Menu->setPosition( ccp(-worldMapSize.width / 2, -worldMapSize.height / 2) );
    
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

void StageSelectLayer::OnResponse()
{
    BaseLayer::OnResponse();

    this->m_StageInfoLayer->OnResponse();
}

void StageSelectLayer::OnStageSelectObjectTouch(cocos2d::CCObject *sender)
{
    CCNode* node = static_cast<CCNode*>(sender);
    StageType stageType = static_cast<StageType>(node->getTag());

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
    GPSPoint gps = GPSPoint(37.566615 + CCRANDOM_0_1(), 126.977958 + CCRANDOM_0_1());
    GameClient::Instance().GetClientObject().SendCSRequestGetStagePlayInfoList(stageType, gps);
}

void StageSelectLayer::DisplayRunningStages(flownet::StagePlayInfoList& stageInfoList)
{
    if(this->m_StageInfoLayer)
    {
        this->m_StageInfoLayer->DisplayRunningStages(stageInfoList);
    }
}


StageSelectScene::StageSelectScene(): m_StageSelectLayer(nullptr), m_BackButton(nullptr){}

StageSelectScene::~StageSelectScene()
{
    CC_SAFE_RELEASE(this->m_StageSelectLayer);
    CC_SAFE_RELEASE(this->m_BackButton);
}

bool StageSelectScene::init()
{
    if(!BaseScene::init()) return false;
    
    this->m_StageSelectLayer = StageSelectLayer::create();
    this->m_StageSelectLayer->retain();
    this->addChild(this->m_StageSelectLayer);
    
    CCSprite* homeButtonImageNormal = CCSprite::create("ui/stage_select_scene/home_button.png");
    CCSprite* homeButtonImageActive = CCSprite::create("ui/stage_select_scene/home_button.png");
    homeButtonImageActive->setColor(ccWHITE);

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    this->m_BackButton = CCMenuItemSprite::create(homeButtonImageNormal, homeButtonImageActive, this, menu_selector(StageSelectScene::OnBackButtonTouch));
    this->m_BackButton->retain();
    this->m_BackButton->setAnchorPoint(CCPointUpperRight);
    CCMenu* menu = CCMenu::create(this->m_BackButton, NULL);
    menu->setAnchorPoint(CCPointUpperRight);
    menu->setPosition(ccp(winSize.width, winSize.height));
    
    this->addChild(menu);
    
    this->m_UILayer = UILayer::create(StageType_NONE);
    this->m_UILayer->retain();
    
    this->addChild(this->m_UILayer);
    
    AudioEngine::Instance()->PlayBackgroundMusic("sound/bgm/campsite.mp3", true);
    
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

void StageSelectScene::OnResponse()
{
    BaseScene::OnResponse();

    this->m_StageSelectLayer->OnResponse();
}

void StageSelectScene::OnBackButtonTouch(CCObject* object)
{
    GPSPoint gps = GPSPoint(37.566615 + CCRANDOM_0_1(), 126.977958 + CCRANDOM_0_1());
    GameClient::Instance().GetClientObject().SendCSRequestJoinClanCommunityStage(gps);

    this->AddLoadingSpinnerAndBlock();
}
