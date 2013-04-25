//
//  AccountScene.h
//  GameClientMobile
//
//  Created by SungJinYoo on 4/10/13.
//
//

#ifndef __GameClientMobile__AccountScene__
#define __GameClientMobile__AccountScene__

class AccountLayer : public BaseLayer, public CCTextFieldDelegate
{
private:
    CCTextFieldTTF* m_EmailTextField;
    CCTextFieldTTF* m_PasswordTextField;
    CCTextFieldTTF* m_NameTextField;
    CCMenuItemToggle* m_GenderMaleButton;
    CCMenuItemToggle* m_GenderFemaleButton;
    
public:
    AccountLayer();
    virtual ~AccountLayer();
    
    virtual bool init();
    CREATE_FUNC(AccountLayer);

    virtual void registerWithTouchDispatcher();

    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    
private:
    void OnGenderMaleButtonPressed(CCObject* sender);
    void OnGenderFemaleButtonPressed(CCObject* sender);
    void OnLoginButtonPressed(CCObject* sender);
    void OnRegisterButtonPressed(CCObject* sender);
    
private:
    bool IsLoggedIn();
    bool IsFirstBoot();
    void SendRejoinRequest();
    
public:
    CCTextFieldTTF* GetEmailTextField();
    CCTextFieldTTF* GetPasswordTextField();
};

class AccountScene : public BaseScene
{
private:
    AccountLayer* m_AccountLayer;
    
public:
    AccountScene();
    virtual ~AccountScene();
    
    virtual bool init();
    CREATE_FUNC(AccountScene);
    
    virtual void update(float deltaTime) override;
    
    AccountLayer* GetAccountLayer();
};

#endif /* defined(__GameClientMobile__AccountScene__) */
