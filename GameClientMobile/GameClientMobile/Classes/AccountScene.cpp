//
//  AccountScene.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 4/10/13.
//r
//

#include "Headers.pch"


bool AccountSceneTextFieldDelegate::init()
{
    return true;
}

/////////////////////////////////////////////

AccountLayer::AccountLayer(): m_Delegate(nullptr), m_BackgroundImage(nullptr), m_EmailField(nullptr), m_PasswordField(nullptr), m_PasswordConfirmField(nullptr), m_KeyboardAttachedTextField(nullptr), m_CreateButton(nullptr), m_LoginButton(nullptr) {}

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
    if(this->m_CreateButton)
    {
        this->m_CreateButton->release();
        this->m_CreateButton = nullptr;
    }
    if(this->m_LoginButton)
    {
        this->m_LoginButton->release();
        this->m_LoginButton = nullptr;
    }
}

bool AccountLayer::init()
{
    if(!CCLayerColor::initWithColor(ccc4(43, 39, 36, 255)))
    {
        ASSERT_DEBUG(false);
        return false;
    }
    
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

void AccountLayer::OnResponse() const
{
    // TO DO : call this when parent class changed to BaseLayer
    // BaseLayer::OnResponse();
    
    if(this->m_CreateButton)
    {
        this->m_CreateButton->setEnabled(true);
    }
    if(this->m_LoginButton)
    {
        this->m_LoginButton->setEnabled(true);
    }
}

void AccountLayer::OnBackButtonTouch(cocos2d::CCObject *sender)
{
    this->InitializeWithLoginWindow();
}

void AccountLayer::OnCreateButtonTouch(cocos2d::CCObject *sender)
{
    // TO DO : check form
    
    // NOTE : this is enabled when response comes
    this->m_CreateButton->setEnabled(false);
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

    this->m_LoginButton->setEnabled(false);
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
    this->m_LoginButton = CCMenuItemImage::create("ui/account_scene/login_button_normal.png", "ui/account_scene/login_button_active.png", this, menu_selector(AccountLayer::OnLoginButtonTouch));
    this->m_LoginButton->retain();
    
    CCMenu* menu = CCMenu::create(newButton, this->m_LoginButton, NULL);
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
    this->m_CreateButton = CCMenuItemImage::create("ui/account_scene/create_button_normal.png", "ui/account_scene/create_button_active.png", this, menu_selector(AccountLayer::OnCreateButtonTouch));
    this->m_CreateButton->retain();
    
    CCMenu* menu = CCMenu::create(backButton, this->m_CreateButton, NULL);
    menu->setPosition(ccp(260, 40));
    menu->alignItemsHorizontallyWithPadding(20);
    this->m_BackgroundImage->addChild(menu);

}

void AccountLayer::Reset()
{
    if(this->m_BackgroundImage)
    {
        this->m_BackgroundImage->removeFromParentAndCleanup(true);
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
    if(this->m_CreateButton)
    {
        this->m_CreateButton->release();
        this->m_CreateButton = nullptr;
    }
    if(this->m_LoginButton)
    {
        this->m_LoginButton->release();
        this->m_LoginButton = nullptr;
    }
}

AccountScene::AccountScene(): m_Layer(nullptr) {}

AccountScene::~AccountScene()
{
    if(this->m_Layer)
    {
        this->m_Layer->release();
        this->m_Layer = nullptr;
    }
}

bool AccountScene::init()
{
    if(!BaseScene::init()) return false;
    
    this->m_Layer = AccountLayer::create();
    this->m_Layer->retain();
    
    this->addChild(this->m_Layer);
    
    scheduleUpdate();
    
    return true;
}

void AccountScene::update(float deltaTime)
{
    BaseScene::update(deltaTime);
}

void AccountScene::OnResponse() const
{
    BaseScene::OnResponse();
    
    this->m_Layer->OnResponse();
}