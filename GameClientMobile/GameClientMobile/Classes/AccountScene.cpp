//
//  AccountScene.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 4/10/13.
//r
//

#include "Headers.pch"

AccountLayer::AccountLayer(): m_Delegate(nullptr), m_BackgroundImage(nullptr), m_EmailField(nullptr), m_PasswordField(nullptr), m_PasswordConfirmField(nullptr), m_KeyboardAttachedTextField(nullptr) {}

AccountLayer::~AccountLayer()
{
    if(this->m_Delegate)
    {
        this->m_Delegate->release();
        this->m_Delegate = nullptr;
    }
    if(this->m_BackgroundImage)
    {
        this->m_BackgroundImage->release();
        this->m_BackgroundImage = nullptr;
    }
    if(this->m_EmailField)
    {
        this->m_EmailField->release();
        this->m_EmailField = nullptr;
    }
    if(this->m_PasswordField)
    {
        this->m_PasswordField->release();
        this->m_PasswordField = nullptr;
    }
    if(this->m_PasswordConfirmField)
    {
        this->m_PasswordConfirmField->release();
        this->m_PasswordConfirmField = nullptr;
    }
}

bool AccountLayer::init()
{
    if(!CCLayerColor::initWithColor(ccc4(43, 39, 36, 255))) return false;
    
    this->m_Delegate = AccountSceneTextFieldDelegate::create();
    this->m_Delegate->retain();
    
    if(this->IsFirstBoot())
    {
        this->InitializeWithCreateAccount();
    }
    else
    {
        this->InitializeWithLoginWindow();
    }
    
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    
    this->setTouchEnabled(true);
    
    scheduleUpdate();
    
    return true;

}

bool AccountLayer::ccTouchBegan(CCTouch* touch, CCEvent* event)
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
                    this->m_KeyboardAttachedTextField = textfield;
                    return true;
                }
            }
        }
    }
    
    return false;
}

void AccountLayer::OnBackButtonTouch(cocos2d::CCObject *sender)
{
    this->InitializeWithLoginWindow();
}

void AccountLayer::OnCreateButtonTouch(cocos2d::CCObject *sender)
{
    // TO DO : check form
    GameClient::Instance().GetCFConnection().SendCFRequestCreateUserAccount(GameClient::Instance().GetDeviceID(), this->m_EmailField->getString(), this->m_PasswordField->getString());
}

void AccountLayer::OnNewButtonTouch(cocos2d::CCObject *sender)
{
    this->InitializeWithCreateAccount();
}

void AccountLayer::OnLoginButtonTouch(cocos2d::CCObject *sender)
{
    CCLOG("login button touch");
    // TO DO : check form
    if( GameClient::Instance().GetCFConnection().IsConnected() == false )
    {
        CCMessageBox("GameServer is not ready. Wait for a minute.", "LogIn Error");
        return;
    }
    
    GameClient::Instance().GetCFConnection().SendCFRequestLogInUserAccount(GameClient::Instance().GetDeviceID(), this->m_EmailField->getString() , this->m_PasswordField->getString());
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

void AccountLayer::InitializeWithLoginWindow()
{
    this->Reset();

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    this->m_BackgroundImage = CCSprite::create("ui/account_scene/login_background.png");
    this->m_BackgroundImage->retain();
    this->m_BackgroundImage->setPosition(ccp(winSize.width / 2, winSize.height / 2));
    this->addChild(this->m_BackgroundImage);
    
    this->m_EmailField = CCTextFieldTTF::textFieldWithPlaceHolder("ID", CCSize(160, 24), kCCTextAlignmentLeft, "thonburi", 20);
    this->m_EmailField->retain();
    this->m_EmailField->setPosition(ccp(180, 114));
    this->m_EmailField->setDelegate(this->m_Delegate);
    this->m_BackgroundImage->addChild(this->m_EmailField);
    
    this->m_PasswordField = CCTextFieldTTF::textFieldWithPlaceHolder("Password", CCSize(160, 24), kCCTextAlignmentLeft, "thonburi", 20);
    this->m_PasswordField->retain();
    this->m_PasswordField->setPosition(ccp(180, 76));
    this->m_PasswordField->setDelegate(this->m_Delegate);
    this->m_BackgroundImage->addChild(this->m_PasswordField);
    
    CCMenuItemImage* newButton = CCMenuItemImage::create("ui/account_scene/new_button_normal.png", "ui/account_scene/new_button_active.png", this, menu_selector(AccountLayer::OnNewButtonTouch));
    CCMenuItemImage* loginButton = CCMenuItemImage::create("ui/account_scene/login_button_normal.png", "ui/account_scene/login_button_active.png", this, menu_selector(AccountLayer::OnLoginButtonTouch));
    
    CCMenu* menu = CCMenu::create(newButton, loginButton, NULL);
    menu->setPosition(ccp(174, 34));
    menu->alignItemsHorizontallyWithPadding(20);
    this->m_BackgroundImage->addChild(menu);
}

void AccountLayer::InitializeWithCreateAccount()
{
    this->Reset();
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    this->m_BackgroundImage = CCSprite::create("ui/account_scene/new_background.png");
    this->m_BackgroundImage->retain();
    this->m_BackgroundImage->setPosition(ccp(winSize.width / 2, winSize.height / 2));
    this->addChild(this->m_BackgroundImage);
    
    this->m_EmailField = CCTextFieldTTF::textFieldWithPlaceHolder("ID", CCSize(170, 24), kCCTextAlignmentLeft, "thonburi", 20);
    this->m_EmailField->retain();
    this->m_EmailField->setPosition(ccp(260, 170));
    this->m_EmailField->setDelegate(this->m_Delegate);
    this->m_BackgroundImage->addChild(this->m_EmailField);
    
    this->m_PasswordField = CCTextFieldTTF::textFieldWithPlaceHolder("Password", CCSize(170, 24), kCCTextAlignmentLeft, "thonburi", 20);
    this->m_PasswordField->retain();
    this->m_PasswordField->setPosition(ccp(260, 130));
    this->m_PasswordField->setDelegate(this->m_Delegate);
    this->m_BackgroundImage->addChild(this->m_PasswordField);
    
    this->m_PasswordConfirmField = CCTextFieldTTF::textFieldWithPlaceHolder("Confirm", CCSize(170, 24), kCCTextAlignmentLeft, "thonburi", 20);
    this->m_PasswordConfirmField->retain();
    this->m_PasswordConfirmField->setPosition(ccp(260, 103));
    this->m_PasswordConfirmField->setDelegate(this->m_Delegate);
    this->m_BackgroundImage->addChild(this->m_PasswordConfirmField);
    
    CCMenuItemImage* backButton = CCMenuItemImage::create("ui/account_scene/back_button_normal.png", "ui/account_scene/back_button_active.png", this, menu_selector(AccountLayer::OnBackButtonTouch));
    CCMenuItemImage* createButton = CCMenuItemImage::create("ui/account_scene/create_button_normal.png", "ui/account_scene/create_button_active.png", this, menu_selector(AccountLayer::OnCreateButtonTouch));
    
    CCMenu* menu = CCMenu::create(backButton, createButton, NULL);
    menu->setPosition(ccp(260, 40));
    menu->alignItemsHorizontallyWithPadding(20);
    this->m_BackgroundImage->addChild(menu);

}

void AccountLayer::Reset()
{
    if(this->m_BackgroundImage)
    {
        this->removeChild(this->m_BackgroundImage);
        this->m_BackgroundImage->release();
        this->m_BackgroundImage = nullptr;
    }
    if(this->m_EmailField)
    {
        this->m_EmailField->release();
        this->m_EmailField = nullptr;
    }
    if(this->m_PasswordField)
    {
        this->m_PasswordField->release();
        this->m_PasswordField = nullptr;
    }
    if(this->m_PasswordConfirmField)
    {
        this->m_PasswordConfirmField->release();
        this->m_PasswordConfirmField = nullptr;
    }
}

bool AccountSceneTextFieldDelegate::init()
{
    return true;
}


AccountScene::AccountScene(){}

AccountScene::~AccountScene(){}

bool AccountScene::init()
{
    if(!BaseScene::init()) return false;
    
    AccountLayer* layer = AccountLayer::create();
    
    this->addChild(layer);
    
    scheduleUpdate();
    
    return true;
}

void AccountScene::update(float deltaTime)
{
    BaseScene::update(deltaTime);
}
