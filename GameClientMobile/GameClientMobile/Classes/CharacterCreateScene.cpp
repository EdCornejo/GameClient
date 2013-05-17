//
//  CharacterCreateScene.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 5/15/13.
//
//

#include "Headers.pch"

bool CharacterCreateSceneTextFieldDelegate::init()
{
    return true;
}


CharacterCreateScene::CharacterCreateScene(): m_SelectedGender(Gender_None), m_BackgroundImage(nullptr), m_PlayerNode(nullptr), m_NameField(nullptr), m_TextFieldDelegate(nullptr), m_MaleButton(nullptr), m_FemaleButton(nullptr) {}

CharacterCreateScene::~CharacterCreateScene()
{
    if(this->m_BackgroundImage)
    {
        this->m_BackgroundImage->release();
        this->m_BackgroundImage = nullptr;
    }
    if(this->m_PlayerNode)
    {
        this->m_PlayerNode->release();
        this->m_PlayerNode = nullptr;
    }
    if(this->m_NameField)
    {
        this->m_NameField->release();
        this->m_NameField = nullptr;
    }
    if(this->m_TextFieldDelegate)
    {
        this->m_TextFieldDelegate->release();
        this->m_TextFieldDelegate = nullptr;
    }
    if(this->m_MaleButton)
    {
        this->m_MaleButton->release();
        this->m_MaleButton = nullptr;
    }
    if(this->m_FemaleButton)
    {
        this->m_FemaleButton->release();
        this->m_FemaleButton = nullptr;
    }

    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool CharacterCreateScene::init() {
    if(!BaseScene::init()) return false;
    
    CCLayerColor* layer = CCLayerColor::create(ccc4(43, 39, 36, 255));
    
    this->m_BackgroundImage = CCSprite::create("ui/character_create_scene/background.png");
    this->m_BackgroundImage->retain();
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    this->m_BackgroundImage->setPosition(ccp(winSize.width / 2, winSize.height / 2));
    layer->addChild(this->m_BackgroundImage);
    CCSize backgroundSize = this->m_BackgroundImage->getTextureRect().size;
    
    this->m_TextFieldDelegate = CharacterCreateSceneTextFieldDelegate::create();
    this->m_TextFieldDelegate->retain();
    
    this->m_NameField = CCTextFieldTTF::textFieldWithPlaceHolder("Character Name", "Thonburi", 20);
    this->m_NameField->retain();
    this->m_NameField->setPosition(ccp(300, 192));
    this->m_NameField->setDelegate(this->m_TextFieldDelegate);
    this->m_BackgroundImage->addChild(this->m_NameField);
    
    this->m_MaleButton = CCMenuItemToggle::createWithTarget(this,
                                                            menu_selector(CharacterCreateScene::OnMaleButtonTouch),
                                                            CCMenuItemImage::create("ui/character_create_scene/male_button_normal.png", "ui/character_create_scene/male_button_normal.png"),
                                                            CCMenuItemImage::create("ui/character_create_scene/male_button_active.png", "ui/character_create_scene/male_button_active.png"),
                                                            NULL);
    this->m_MaleButton->retain();
    
    this->m_FemaleButton = CCMenuItemToggle::createWithTarget(this,
                                                            menu_selector(CharacterCreateScene::OnFemaleButtonTouch),
                                                            CCMenuItemImage::create("ui/character_create_scene/female_button_normal.png", "ui/character_create_scene/female_button_normal.png"),
                                                            CCMenuItemImage::create("ui/character_create_scene/female_button_active.png", "ui/character_create_scene/female_button_active.png"),
                                                            NULL);
    this->m_FemaleButton->retain();
    
    CCMenu* genderMenu = CCMenu::create(this->m_MaleButton, this->m_FemaleButton, NULL);
    genderMenu->alignItemsHorizontallyWithPadding(20);
    genderMenu->setPosition(ccp(backgroundSize.width * 2 / 3, 120));
    this->m_BackgroundImage->addChild(genderMenu);
    
    CCMenuItemImage* backButton = CCMenuItemImage::create("ui/character_create_scene/back_button_normal.png", "ui/character_create_scene/back_button_active.png", this, menu_selector(CharacterCreateScene::OnBackButtonTouch));
    CCMenuItemImage* createButton = CCMenuItemImage::create("ui/character_create_scene/create_button_normal.png", "ui/character_create_scene/create_button_active.png", this, menu_selector(CharacterCreateScene::OnCreateButtonTouch));
    CCMenu* buttonMenu = CCMenu::create(backButton, createButton, NULL);
    
    buttonMenu->setPosition(ccp(backgroundSize.width * 2 / 3, 40));
    buttonMenu->alignItemsHorizontallyWithPadding(20);
    this->m_BackgroundImage->addChild(buttonMenu);
    
    this->addChild(layer);
    
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    
    scheduleUpdate();
    
    return true;
}

void CharacterCreateScene::update(float deltaTime)
{
    BaseScene::update(deltaTime);
}

bool CharacterCreateScene::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    CC_UNUSED_PARAM(event);
    if(this->m_BackgroundImage->getChildren() && this->m_BackgroundImage->getChildren()->count() > 0)
    {
        CCObject* object;
        CCARRAY_FOREACH(this->m_BackgroundImage->getChildren(), object)
        {
            CCTextFieldTTF* textfield = dynamic_cast<CCTextFieldTTF*>(object);
            if(textfield){
                CCRect rect = GetRect(textfield);
                rect.origin = this->m_BackgroundImage->convertToWorldSpace(rect.origin);
                if(rect.containsPoint(touch->getLocation()))
                {
                    textfield->attachWithIME();
                    CCLOG("attach");
                    return true;
                }
            }
        }
    }

    return false;
}

void CharacterCreateScene::OnMaleButtonTouch(cocos2d::CCObject *sender)
{
    if(!this->m_PlayerNode)
    {
        this->m_PlayerNode = PlayerNode::create(Gender_Male);
        this->m_PlayerNode->setScale(0.13);
        this->m_PlayerNode->retain();
        this->m_PlayerNode->setPosition(ccp(105, 80));
        this->m_BackgroundImage->addChild(this->m_PlayerNode);
    }
    this->m_PlayerNode->ChangeGender(Gender_Male);
    
    this->m_MaleButton->setSelectedIndex(1);
    
    if(this->m_SelectedGender == Gender_Male) return;
    
    this->m_FemaleButton->setSelectedIndex(0);
    this->m_SelectedGender = Gender_Male;
}

void CharacterCreateScene::OnFemaleButtonTouch(cocos2d::CCObject *sender)
{
    if(!this->m_PlayerNode)
    {
        this->m_PlayerNode = PlayerNode::create(Gender_Female);
        this->m_PlayerNode->setScale(0.13);
        this->m_PlayerNode->retain();
        this->m_PlayerNode->setPosition(ccp(60, 60));
        this->m_BackgroundImage->addChild(this->m_PlayerNode);
    }
    this->m_PlayerNode->ChangeGender(Gender_Female);

    this->m_FemaleButton->setSelectedIndex(1);
    
    if(this->m_SelectedGender == Gender_Female) return;
    
    this->m_MaleButton->setSelectedIndex(0);
    this->m_SelectedGender = Gender_Female;
}

void CharacterCreateScene::OnBackButtonTouch(cocos2d::CCObject *sender)
{
    // NOTE : there might be no back button, ask with teammates
    CCLOG("back button touch");
}

void CharacterCreateScene::OnCreateButtonTouch(cocos2d::CCObject *sender)
{
    // NOTE : send create request
    CCLOG("create button touch");
}