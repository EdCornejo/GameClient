//
//  MenuBarNode.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 4/20/13.
//
//

#include "Headers.pch"


MenuBarNode::MenuBarNode(): m_IsOpen(false), m_SlideButton(nullptr), m_Body(nullptr), m_HomeButton(nullptr), m_SettingButton(nullptr), m_LogoutButton(nullptr) {}
MenuBarNode::~MenuBarNode()
{
    if(this->m_SlideButton)
    {
        this->m_SlideButton->release();
        this->m_SlideButton = nullptr;
    }
    if(this->m_Body)
    {
        this->m_Body->release();
        this->m_Body = nullptr;
    }
    if(this->m_HomeButton)
    {
        this->m_HomeButton->release();
        this->m_HomeButton = nullptr;
    }
    if(this->m_SettingButton)
    {
        this->m_SettingButton->release();
        this->m_SettingButton = nullptr;
    }
    if(this->m_LogoutButton)
    {
        this->m_LogoutButton->release();
        this->m_LogoutButton = nullptr;
    }
}

bool MenuBarNode::init()
{
    // TO DO : change scroll button to menuitem
    this->m_SlideButton = CCSprite::create("ui/system_menu/scroll_button.png");
    this->m_SlideButton->retain();
    this->m_SlideButton->setAnchorPoint(CCPointLowerRight);
    this->m_SlideButton->setPosition(ccp(14, -10));
    this->m_Body = CCSprite::create("ui/system_menu/background.png");
    this->m_Body->retain();
    this->m_Body->setAnchorPoint(CCPointLowerLeft);
    
    this->m_HomeButton = CCMenuItemImage::create("ui/system_menu/home.png", "ui/system_menu/home.png", this, menu_selector(MenuBarNode::OnHomeButtonClicked));
    this->m_HomeButton->retain();
    this->m_SettingButton = CCMenuItemImage::create("ui/system_menu/settings.png", "ui/system_menu/settings.png", this, menu_selector(MenuBarNode::OnSettingButtonClicked));
    this->m_SettingButton->retain();
    this->m_LogoutButton = CCMenuItemImage::create("ui/system_menu/settings.png", "ui/system_menu/settings.png", this, menu_selector(MenuBarNode::OnLogoutButtonClicked));
    this->m_LogoutButton->retain();

    CCMenu* menu = CCMenu::create(this->m_HomeButton, this->m_SettingButton, this->m_LogoutButton, NULL);
    CCRect bodyRect = this->m_Body->getTextureRect();
    menu->setPosition(ccp(bodyRect.size.width / 2, bodyRect.size.height / 2)); // change it's position
    menu->alignItemsHorizontallyWithPadding(10);
    this->m_Body->addChild(menu);

    this->addChild(this->m_Body);
    this->addChild(this->m_SlideButton);
    
    return true;
}

MenuBarNode* MenuBarNode::create()
{
    MenuBarNode* newMenuBarNode = new MenuBarNode();
    if(newMenuBarNode && newMenuBarNode->init())
    {
        newMenuBarNode->retain();
        return newMenuBarNode;
    }
    else
    {
        delete newMenuBarNode;
        newMenuBarNode = nullptr;
        return nullptr;
    }
}

bool MenuBarNode::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    CCPoint touchLocation = touch->getLocation();
    
    CCRect slideButtonRect = this->m_SlideButton->getTextureRect();
    
    CCPoint localPoint = this->m_SlideButton->convertToNodeSpace(touchLocation);
    
    if(slideButtonRect.containsPoint(localPoint))
    {
        this->Slide();
        
        return true;
    }
    CCRect bodyRect = GetRectForAnchorLowerLeft(this->m_Body);
    bodyRect.origin = this->convertToWorldSpace(bodyRect.origin);
    if(bodyRect.containsPoint(touchLocation))
    {
        return true;
    }
    
    return false;
}

void MenuBarNode::OnResponse() const
{
    if(this->m_HomeButton)
    {
        this->m_HomeButton->setEnabled(true);
    }
    if(this->m_SettingButton)
    {
        this->m_SettingButton->setEnabled(true);
    }
    if(this->m_LogoutButton)
    {
        this->m_LogoutButton->setEnabled(true);
    }
}

void MenuBarNode::Slide()
{
    if(this->m_IsOpen)
    {
        CCLOG("slide in"); 
        CCPoint slidePosition = CCPoint(PositionX, PositionY);
        CCMoveTo* moveIn = CCMoveTo::create(0.3, slidePosition);
        moveIn->setTag(ActionType_UI);
        this->stopActionByTag(ActionType_UI);
        this->runAction(moveIn);
    }
    else{
        CCLOG("slide out");
        CCPoint slidePosition = CCPoint(PositionX - this->m_Body->getTextureRect().size.width + 10, PositionY);
        CCMoveTo* moveOut = CCMoveTo::create(0.3, slidePosition); 
        moveOut->setTag(ActionType_UI);
        this->stopActionByTag(ActionType_UI);
        this->runAction(moveOut);
        // TO DO : slide out
    }
    
    this->m_IsOpen = !this->m_IsOpen;
}

void MenuBarNode::OnHomeButtonClicked(CCObject* sender)
{
    if(GameClient::Instance().GetClientStage()->GetStageType() == StageType_ClanCommunity)
    {
        return;
    }
    GameClient::Instance().GetClientObject().SendCSRequestExitStage(GameClient::Instance().GetClientStage()->GetStageID());
    this->m_HomeButton->setEnabled(false);
    this->m_SettingButton->setEnabled(false);
    this->m_LogoutButton->setEnabled(false);
}

void MenuBarNode::OnSettingButtonClicked(CCObject* sender)
{
    GameClient::Instance().GetClientObject().SendCSRequestExitStage(GameClient::Instance().GetClientStage()->GetStageID());
    this->m_HomeButton->setEnabled(false);
    this->m_SettingButton->setEnabled(false);
    this->m_LogoutButton->setEnabled(false);
}

void MenuBarNode::OnLogoutButtonClicked(CCObject* sender)
{
    GameClient::Instance().GetClientObject().SendCSRequestLogOutUserAccount(GameClient::Instance().GetDeviceID(), GameClient::Instance().GetUserID());
    this->m_HomeButton->setEnabled(false);
    this->m_SettingButton->setEnabled(false);
    this->m_LogoutButton->setEnabled(false);
}