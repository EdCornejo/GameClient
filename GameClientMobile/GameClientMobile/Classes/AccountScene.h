//
//  AccountScene.h
//  GameClientMobile
//
//  Created by SungJinYoo on 4/10/13.
//
//

#ifndef __GameClientMobile__AccountScene__
#define __GameClientMobile__AccountScene__

class AccountSceneTextFieldDelegate : public CCObject, public CCTextFieldDelegate
{
public:
    virtual bool init();
    CREATE_FUNC(AccountSceneTextFieldDelegate);
};

class AccountLayer : public CCLayerColor // TO DO : change parent class to BaseLayer when background image arrives
{
private:
    AccountSceneTextFieldDelegate* m_Delegate;
    CCSprite* m_BackgroundImage;
    CCTextFieldTTF* m_EmailField;
    CCTextFieldTTF* m_PasswordField;
    CCTextFieldTTF* m_PasswordConfirmField;
    CCTextFieldTTF* m_KeyboardAttachedTextField;
    CCMenuItemImage* m_CreateButton;
    CCMenuItemImage* m_LoginButton;
    
public:
    AccountLayer();
    virtual ~AccountLayer();
    
    virtual bool init() override;
    CREATE_FUNC(AccountLayer);
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event) override;
    
    virtual void OnResponse() const;
    
private:
    void OnBackButtonTouch(CCObject* sender);
    void OnCreateButtonTouch(CCObject* sender);
    void OnNewButtonTouch(CCObject* sender);
    void OnLoginButtonTouch(CCObject* sender);
    
    bool IsFirstBoot();
    
    void InitializeWithCreateAccount();
    void InitializeWithLoginWindow();
    
    void Reset();
};


class AccountScene : public BaseScene
{
private:
    AccountLayer* m_Layer;
    
public:
    AccountScene();
    virtual ~AccountScene();
    
    virtual bool init() override;
    CREATE_FUNC(AccountScene);
    
    virtual void update(float deltaTime) override;
    
    virtual void OnResponse() const override;
};

#endif /* defined(__GameClientMobile__AccountScene__) */
