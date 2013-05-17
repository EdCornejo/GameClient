//
//  CharacterCreateScene.h
//  GameClientMobile
//
//  Created by SungJinYoo on 5/15/13.
//
//

#ifndef __GameClientMobile__CharacterCreateScene__
#define __GameClientMobile__CharacterCreateScene__

class CharacterCreateSceneTextFieldDelegate : public CCObject, public CCTextFieldDelegate
{
public :
    virtual bool init();
    CREATE_FUNC(CharacterCreateSceneTextFieldDelegate);
};


class CharacterCreateScene : public BaseScene, public CCTouchDelegate
{
private:
    flownet::Gender m_SelectedGender;
    CCSprite* m_BackgroundImage;
    PlayerNode* m_PlayerNode;
    CCTextFieldTTF* m_NameField;
    CharacterCreateSceneTextFieldDelegate* m_TextFieldDelegate;
    CCMenuItemToggle* m_MaleButton;
    CCMenuItemToggle* m_FemaleButton;

public :
    CharacterCreateScene();
    virtual ~CharacterCreateScene();
    
    virtual bool init();
    
    virtual void update(float deltaTime);
    
    CREATE_FUNC(CharacterCreateScene);
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event) override;
    
private:
    void OnMaleButtonTouch(CCObject* sender);
    void OnFemaleButtonTouch(CCObject* sender);

    void OnBackButtonTouch(CCObject* sender);
    void OnCreateButtonTouch(CCObject* sender);
};

#endif /* defined(__GameClientMobile__CharacterCreateScene__) */
