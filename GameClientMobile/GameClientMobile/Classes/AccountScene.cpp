//
//  AccountScene.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 4/10/13.
//r
//

#include "Headers.pch"

AccountLayer::AccountLayer(): m_EmailTextField(nullptr), m_PasswordTextField(nullptr), m_NameTextField(nullptr), m_GenderMaleButton(nullptr), m_GenderFemaleButton(nullptr)
{}
AccountLayer::~AccountLayer()
{
    if(this->m_EmailTextField)
    {
        this->m_EmailTextField->release();
        this->m_EmailTextField = nullptr;
    }
    if(this->m_PasswordTextField)
    {
        this->m_PasswordTextField->release();
        this->m_PasswordTextField = nullptr;
    }
    if(this->m_NameTextField)
    {
        this->m_NameTextField->release();
        this->m_NameTextField = nullptr;
    }
    if(this->m_GenderMaleButton)
    {
        this->m_GenderMaleButton->release();
        this->m_GenderMaleButton = nullptr;
    }
    if(this->m_GenderFemaleButton)
    {
        this->m_GenderFemaleButton->release();
        this->m_GenderFemaleButton = nullptr;
    }
}

bool AccountLayer::init()
{
    if(!BaseLayer::init()) return false;
    
    if(this->IsFirstBoot())
    {
        this->m_EmailTextField = CCTextFieldTTF::textFieldWithPlaceHolder("Email", "Thonburi", 20);
        this->m_EmailTextField->setPosition(ccp(240, 240));
        this->m_EmailTextField->setDelegate(this);
        this->m_EmailTextField->retain();
        
        this->m_PasswordTextField = CCTextFieldTTF::textFieldWithPlaceHolder("Password", "Thonburi", 20);
        this->m_PasswordTextField->setPosition(ccp(240, 200));
        this->m_PasswordTextField->setDelegate(this);
        this->m_PasswordTextField->retain();
        
        this->m_NameTextField = CCTextFieldTTF::textFieldWithPlaceHolder("Character Name", "Thonburi", 20);
        this->m_NameTextField->setPosition(ccp(240, 160));
        this->m_NameTextField->setDelegate(this);
        this->m_NameTextField->retain();
        
        this->addChild(this->m_EmailTextField);
        this->addChild(this->m_PasswordTextField);
        this->addChild(this->m_NameTextField);
        
        this->m_GenderMaleButton = CCMenuItemToggle::createWithTarget(this,
                menu_selector(AccountLayer::OnGenderMaleButtonPressed),
                CCMenuItemImage::create("ui/gender_male_button_normal.png", "ui/gender_male_button_normal.png"),
                CCMenuItemImage::create("ui/gender_male_button_selected.png", "ui/gender_male_button_selected.png"),
                NULL);
        this->m_GenderMaleButton->retain();
        this->m_GenderFemaleButton = CCMenuItemToggle::createWithTarget(this,
                menu_selector(AccountLayer::OnGenderFemaleButtonPressed),
                CCMenuItemImage::create("ui/gender_female_button_normal.png", "ui/gender_female_button_normal.png"),
                CCMenuItemImage::create("ui/gender_female_button_selected.png", "ui/gender_female_button_selected.png"),
                NULL);
        this->m_GenderFemaleButton->retain();
        
        CCMenu* genderMenu = CCMenu::create(this->m_GenderMaleButton, this->m_GenderFemaleButton, NULL);
        genderMenu->alignItemsHorizontally();
        genderMenu->setPosition(ccp(240, 100));
            
        this->addChild(genderMenu);
        
        CCMenuItemImage* registerButton = CCMenuItemImage::create("ui/register_button_normal.png", "ui/register_button_selected.png", this, menu_selector(AccountLayer::OnRegisterButtonPressed));
        CCMenu* registerMenu = CCMenu::create(registerButton, NULL);
        registerMenu->setPosition(ccp(240, 40));

        this->addChild(registerMenu);
    }
    else
    {
        if(!IsLoggedIn())
        {
            this->m_EmailTextField = CCTextFieldTTF::textFieldWithPlaceHolder("Email", "Thonburi", 20);
            this->m_EmailTextField->setPosition(ccp(240, 200));
            this->m_EmailTextField->setDelegate(this);
            this->m_EmailTextField->retain();
            
            this->m_PasswordTextField = CCTextFieldTTF::textFieldWithPlaceHolder("Password", "Thonburi", 20);
            this->m_PasswordTextField->setPosition(ccp(240,160));
            this->m_PasswordTextField->setDelegate(this);
            this->m_PasswordTextField->retain();
            
            this->addChild(this->m_EmailTextField);
            this->addChild(this->m_PasswordTextField);
            
                       
            CCMenuItemImage* loginButton = CCMenuItemImage::create("ui/login_button_normal.png", "ui/login_button_selected.png", this, menu_selector(AccountLayer::OnLoginButtonPressed));
            CCMenu* loginButtonMenu = CCMenu::create(loginButton, NULL);
            loginButtonMenu->setPosition(ccp(240, 40));

            this->addChild(loginButtonMenu);
            // TO DO : popup login window
        }
        else
        {
            // TO DO : goto character window or stage window
            CCDelayTime* delay = CCDelayTime::create(0.5f);
            CCCallFunc* callfunc = CCCallFunc::create(this, callfunc_selector(AccountLayer::SendRejoinRequest));
            CCSequence* sequence = CCSequence::create(delay, callfunc, NULL);
            this->runAction(sequence);
        }
    }

    this->setTouchEnabled(true);
    
    return true;
}

void AccountLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

bool AccountLayer::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    CC_UNUSED_PARAM(event);
    if(this->m_pChildren && this->m_pChildren->count() > 0)
    {
        CCObject* object;
        CCARRAY_FOREACH(this->m_pChildren, object)
        {
            CCTextFieldTTF* textfield = dynamic_cast<CCTextFieldTTF*>(object);
            if(textfield){
                CCRect rect = GetRect(textfield);
                if(rect.containsPoint(touch->getLocation()))
                {
                    textfield->attachWithIME();
                    return true;
                }
            }
        }
    }
    
    return false;
}

bool AccountLayer::IsLoggedIn()
{
    return GameClient::Instance().GetSessionID() != SessionID_NONE;
}

bool AccountLayer::IsFirstBoot()
{
    bool isFirstBoot = CCUserDefault::sharedUserDefault()->getBoolForKey("FirstBoot", true);
    if(isFirstBoot)
    {
        CCUserDefault::sharedUserDefault()->setBoolForKey("FirstBoot", false);
        CCUserDefault::sharedUserDefault()->flush();
    }
    return isFirstBoot;
}

void AccountLayer::SendRejoinRequest()
{
    GameClient::Instance().GetClientObject().SendCSRequestRejoinCurrentStage();
}

void AccountLayer::OnGenderMaleButtonPressed(cocos2d::CCObject *sender)
{
    CCMenuItemToggle* button = dynamic_cast<CCMenuItemToggle*>(sender);
    if(button)
    {
        button->setSelectedIndex(true);
        this->m_GenderFemaleButton->setSelectedIndex(false);
    }
}

void AccountLayer::OnGenderFemaleButtonPressed(cocos2d::CCObject *sender)
{
    CCMenuItemToggle* button = dynamic_cast<CCMenuItemToggle*>(sender);
    if(button)
    {
        button->setSelectedIndex(true);
        this->m_GenderMaleButton->setSelectedIndex(false);
    }
}

void AccountLayer::OnLoginButtonPressed(cocos2d::CCObject *sender)
{
    // TO DO : check form
    GameClient::Instance().GetClientObject().SendCSRequestLogInUserAccount(GameClient::Instance().GetDeviceID(), this->m_EmailTextField->getString() , this->m_PasswordTextField->getString());
}

void AccountLayer::OnRegisterButtonPressed(cocos2d::CCObject* sender)
{
    // TO DO : check form
    Gender selectedGender = Gender_Max;
    if(this->m_GenderMaleButton->getSelectedIndex() == 1)
    {
        selectedGender = Gender_Male;
    }
    if(this->m_GenderFemaleButton->getSelectedIndex() == 1)
    {
        selectedGender = Gender_Female;
    }

    GameClient::Instance().GetClientObject().SendCSRequestCreateUserAccount(GameClient::Instance().GetDeviceID(), this->m_EmailTextField->getString(), this->m_PasswordTextField->getString(), selectedGender, this->m_NameTextField->getString());
}

AccountScene::AccountScene() {}
AccountScene::~AccountScene() {}

bool AccountScene::init()
{
    if(!BaseScene::init()) return false;
    
    this->m_BackgroundLayer = BackgroundLayer::create("background/default.png");
    this->m_BackgroundLayer->retain();
    this->addChild(this->m_BackgroundLayer);
    
    CCLayer* accountLayer = AccountLayer::create();
    this->addChild(accountLayer);
    
    scheduleUpdate();
    
    return true;
}


void AccountScene::update(float deltaTime)
{
    BaseScene::update(deltaTime);
}

