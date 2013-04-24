//
//  StageCreateScene.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 3/24/13.
//
//

#include "Headers.pch"

StageDetailInfoLayer::StageDetailInfoLayer()
{

}

StageDetailInfoLayer::~StageDetailInfoLayer()
{

}

bool StageDetailInfoLayer::initWithStagePreviewInfo(StagePreviewInfo stagePreviewInfo)
{
    if(!CCLayer::init())
    {
        return false;
    }
    this->m_StagePreviewInfo = stagePreviewInfo;
    this->setContentSize(CCSizeMake(450, 280));
    
    
    
    // TO DO : attach background image
//    std::string detailStageInfoLayerBackgroundFileName = "ui/detail_stage_info_layer_background.png";
//    CCSprite* backgroundImage = CCSprite::create(detailStageInfoLayerBackgroundFileName.c_str());
//    this->addChild(backgroundImage);
    
    // NOTE : attach detail stage preview image
    std::string previewFileName = stagePreviewInfo.m_PreviewImage;
    previewFileName.replace(0, 8, "preview/detail_");
    CCSprite* detailStagePreviewImage = CCSprite::create(previewFileName.c_str());
    
    detailStagePreviewImage->setPosition(ccp(LEFT_SIDE_POSITION_X, DETAIL_PREVIEW_IMAGE_POSITION_Y));
    detailStagePreviewImage->setAnchorPoint(CCPointZero);
    this->addChild(detailStagePreviewImage);
    
    CCSprite* testDot = CCSprite::create("test_dot.png");
    testDot->setAnchorPoint(CCPointZero);
    testDot->setPosition(detailStagePreviewImage->getPosition());
    this->addChild(testDot);
    
    // NOTE : attach stage name label
    CCLabelTTF* stageNameLabel = CCLabelTTF::create();
    stageNameLabel->setPosition(ccp(LEFT_SIDE_POSITION_X, STAGE_NAME_POSITION_Y));
    stageNameLabel->setString(stagePreviewInfo.m_Name.c_str());
    this->addChild(stageNameLabel);
    
    CCSprite* testDot2 = CCSprite::create("test_dot.png");
    testDot2->setAnchorPoint(CCPointZero);
    testDot2->setPosition(stageNameLabel->getPosition());
    this->addChild(testDot2);

    
    // NOTE : attach recommented level label
    CCLabelTTF* recommendedLevelLabel = CCLabelTTF::create();
    recommendedLevelLabel->setPosition(ccp(LEFT_SIDE_POSITION_X, STAGE_LEVEL_POSITION_Y));
    recommendedLevelLabel->setString(stagePreviewInfo.m_RecommendedLevel.c_str());
    this->addChild(recommendedLevelLabel);
    
    // NOTE : attach description
    CCLabelTTF* descriptionLabel = CCLabelTTF::create();
    descriptionLabel->setPosition(ccp(RIGHT_SIDE_POSITION_X, DESCRIPTION_POSITION_Y));
    descriptionLabel->setDimensions(CCSizeMake(190, 200));
    descriptionLabel->setString(stagePreviewInfo.m_Description.c_str());
    this->addChild(descriptionLabel);
    
    // NOTE : attach create button
    CCScale9Sprite* buttonImage = CCScale9Sprite::create(CCRectMake(10, 10, 60, 60), "ui/button.png");
    CCLabelTTF* label = CCLabelTTF::create();
    label->setHorizontalAlignment(kCCTextAlignmentCenter);
    label->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
    label->setString("Create");
    
    CCControlButton* createButton = CCControlButton::create(label, buttonImage);
    createButton->addTargetWithActionForControlEvents(createButton, cccontrol_selector(StageCreateScene::OnStageCreateButtonClick), CCControlEventTouchUpInside);
    createButton->setPosition(ccp(RIGHT_SIDE_POSITION_X, CREATE_BUTTON_POSITION_Y));
    this->addChild(createButton);
    
    CCSprite* testDot3 = CCSprite::create("test_dot.png");
    testDot3->setAnchorPoint(CCPointZero);
    testDot3->setPosition(createButton->getPosition());
    this->addChild(testDot3);

    
    // NOTE : attach close button
    CCMenuItemImage* closeButton = CCMenuItemImage::create("ui/close_button.png", "ui/close_button.png", this, menu_selector(StageCreateScene::OnStageDetailCloseButtonClick));
    closeButton->setPosition(ccp(450, 300));
    this->addChild(closeButton);

    return true;
}

StageDetailInfoLayer* StageDetailInfoLayer::create(StagePreviewInfo stagePreviewInfo)
{
    StageDetailInfoLayer* layer = new StageDetailInfoLayer();
    if(layer && layer->initWithStagePreviewInfo(stagePreviewInfo))
    {
        layer->autorelease();
        return layer;
    }
    else
    {
        delete layer;
        layer = nullptr;
        return nullptr;
    }
}


StageCreateScene::StageCreateScene(): m_StagePreviewInfoList(), m_CurrentPage(0), m_TotalPages(0), m_StageInfoMenuList(), m_StageDetailInfoLayer(nullptr)
{
    CCDictionary* dict = CCDictionary::createWithContentsOfFile("stageinfo/stageinfos.plist");
    CCArray* stageInfos = static_cast<CCArray*>(dict->objectForKey("stages"));
    CCObject* object;
    CCARRAY_FOREACH(stageInfos, object)
    {
        CCDictionary* stageInfoDict = static_cast<CCDictionary*>(object);
        // TO DO : multi region support comes here
        StagePreviewInfo* stageInfo = new StagePreviewInfo;
        stageInfo->m_Name = static_cast<CCString*>(stageInfoDict->objectForKey("name_kr"))->getCString();
        stageInfo->m_RecommendedLevel = static_cast<CCString*>(stageInfoDict->objectForKey("recommended_level"))->getCString();
        stageInfo->m_PreviewImage = "preview/";
        stageInfo->m_PreviewImage += static_cast<CCString*>(stageInfoDict->objectForKey("preview_image"))->getCString();
        stageInfo->m_StageType = static_cast<StageType>(atoi(static_cast<CCString*>(stageInfoDict->objectForKey("stage_type"))->getCString()));
        stageInfo->m_Description = static_cast<CCString*>(stageInfoDict->objectForKey("description_kr"))->getCString();
        this->m_StagePreviewInfoList.push_back(stageInfo);
    }
    delete dict;
    
    this->m_TotalPages = this->m_StagePreviewInfoList.size() / PER_PAGE;
}

StageCreateScene::~StageCreateScene()
{
    std::for_each(this->m_StagePreviewInfoList.begin(), this->m_StagePreviewInfoList.end(), [](StagePreviewInfo* stagePreviewInfo){
        delete stagePreviewInfo;
    });
    
    if(m_StageDetailInfoLayer)
    {
        m_StageDetailInfoLayer->release();
        m_StageDetailInfoLayer = nullptr;
    }
}

bool StageCreateScene::init()
{
    if(!BaseScene::init())
    {
        return false;
    }
    
    // impl. pagination
    const int currentPageStartIndex = this->m_CurrentPage * PER_PAGE;
    int currentPageEndIndex = (this->m_CurrentPage + 1) * PER_PAGE;
    if(currentPageEndIndex > this->m_StagePreviewInfoList.size())
    {
        currentPageEndIndex = this->m_StagePreviewInfoList.size();
    }

    int rowItemCount = 0;
    int currentRow = 0;
    CCMenu* menu = nullptr;
    for(int itemIndex = currentPageStartIndex; itemIndex < currentPageEndIndex; itemIndex++)
    {
        if(menu == nullptr)
        {
            menu = CCMenu::create(NULL);
        }
        StagePreviewInfo* stageInfo = this->m_StagePreviewInfoList[itemIndex];
        
        CCMenuItemImage* stageButton = CCMenuItemImage::create(stageInfo->m_PreviewImage.c_str(), stageInfo->m_PreviewImage.c_str(), this, menu_selector(StageCreateScene::OnStageInfoButtonClick));
        stageButton->setTag(itemIndex);
        menu->addChild(stageButton);
        
        if(itemIndex != 0 && itemIndex % PER_ROW == 0)
        {
            menu->setPosition(ccp(240, 320 - (PREVIEW_IMAGE_SIZE_Y * currentRow) - MARGIN_Y - PREVIEW_IMAGE_OFFSET_Y));
            menu->alignItemsHorizontallyWithPadding(MARGIN_X);
            this->m_StageInfoMenuList.push_back(menu);
            this->addChild(menu);
            
            menu = nullptr;
            currentRow++;
        }
        rowItemCount++;
    }
    if(menu != nullptr && menu->getChildrenCount() > 0)
    {
        menu->setPosition(ccp(240, 320 - (PREVIEW_IMAGE_SIZE_Y * currentRow) - MARGIN_Y - PREVIEW_IMAGE_OFFSET_Y));
        menu->alignItemsHorizontallyWithPadding(MARGIN_X);
        this->m_StageInfoMenuList.push_back(menu);
        this->addChild(menu);
    }
    
    scheduleUpdate();
    
    return true;
}

StageCreateScene* StageCreateScene::create()
{
    StageCreateScene* stageCreateScene = new StageCreateScene();
    if(stageCreateScene && stageCreateScene->init())
    {
        stageCreateScene->autorelease();
        return stageCreateScene;
    }
    else
    {
        delete stageCreateScene;
        stageCreateScene = nullptr;
        return nullptr;
    }
}

void StageCreateScene::update(float deltaTime)
{
    BaseScene::update(deltaTime);
}

void StageCreateScene::OnStageInfoButtonClick(CCObject* sender)
{
    CCMenuItemImage* stageButton = static_cast<CCMenuItemImage*>(sender);
    const int stageInfoIndex = stageButton->getTag();
    StagePreviewInfo* stageInfo = this->m_StagePreviewInfoList[stageInfoIndex];
    
    this->m_StageDetailInfoLayer = StageDetailInfoLayer::create(*stageInfo);
    this->m_StageDetailInfoLayer->retain();
    this->m_StageDetailInfoLayer->setPosition(ccp(15, 15));
    
    std::for_each(this->m_StageInfoMenuList.begin(), this->m_StageInfoMenuList.end(), [](CCMenu* menu){
        CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(menu);
    });
    
    this->addChild(this->m_StageDetailInfoLayer);
    

}

void StageCreateScene::OnStageCreateButtonClick(cocos2d::CCObject *sender)
{
    CCMenuItemImage* createButton = static_cast<CCMenuItemImage*>(sender);
    GameClientObject& clientObject = GameClient::Instance().GetClientObject();
    clientObject.SendCSRequestCreateStage(static_cast<StageType>(createButton->getTag()));
    
    static_cast<ClientDirector*>(CCDirector::sharedDirector())->ChangeScene<StageCreateScene, LoadingScene>();
}

void StageCreateScene::OnStageDetailCloseButtonClick(cocos2d::CCObject *sender)
{
    StageCreateScene* scene = static_cast<StageCreateScene*>(this->getParent());
    std::for_each(scene->m_StageInfoMenuList.begin(), scene->m_StageInfoMenuList.end(), [](CCMenu* menu){
        CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(menu, kCCMenuHandlerPriority, true);
    });
    
    scene->removeChild(this->m_StageDetailInfoLayer);
    scene->m_StageDetailInfoLayer->release();
    scene->m_StageDetailInfoLayer = nullptr;
}

StageDetailInfoLayer* StageCreateScene::GetStageDetailInfoLayer()
{
    return this->m_StageDetailInfoLayer;
}

StageInfoMenuList& StageCreateScene::GetStageInfoMenuList()
{
    return this->m_StageInfoMenuList;
}
