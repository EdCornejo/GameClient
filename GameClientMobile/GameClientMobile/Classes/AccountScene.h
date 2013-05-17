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

class AccountLayer : public CCLayerColor
{
private:
    AccountSceneTextFieldDelegate* m_Delegate;
    CCSprite* m_BackgroundImage;
    CCTextFieldTTF* m_EmailField;
    CCTextFieldTTF* m_PasswordField;
    CCTextFieldTTF* m_PasswordConfirmField;
    CCTextFieldTTF* m_KeyboardAttachedTextField;
    
public:
    AccountLayer();
    virtual ~AccountLayer();
    
    virtual bool init();
    CREATE_FUNC(AccountLayer);
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    
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
public:
    AccountScene();
    virtual ~AccountScene();
    
    virtual bool init();
    CREATE_FUNC(AccountScene);
    
    virtual void update(float deltaTime) override;
};

#endif /* defined(__GameClientMobile__AccountScene__) */
