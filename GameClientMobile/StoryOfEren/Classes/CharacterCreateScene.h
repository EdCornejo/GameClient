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

// TO DO : extend BaseLayer
class CharacterCreateLayer : public CCLayerColor
{
private:
    flownet::Gender m_SelectedGender;
    CCSprite* m_BackgroundImage;
    PlayerNode* m_PlayerNode;
    CCTextFieldTTF* m_NameField;
    CharacterCreateSceneTextFieldDelegate* m_TextFieldDelegate;
    CCMenuItemToggle* m_MaleButton;
    CCMenuItemToggle* m_FemaleButton;
    CCMenuItemImage* m_CreateButton;

public :
    CharacterCreateLayer();
    virtual ~CharacterCreateLayer();
    
    virtual bool init();
    
    CREATE_FUNC(CharacterCreateLayer);
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event) override;
    
    // TO DO : when extending layer changes, this also need to be changed
    void OnResponse() const;
    
private :
    void OnMaleButtonTouch(CCObject* sender);
    void OnFemaleButtonTouch(CCObject* sender);
    void OnBackButtonTouch(CCObject* sender);
    void OnCreateButtonTouch(CCObject* sender);
};


class CharacterCreateScene : public BaseScene, public CCTouchDelegate
{
private:
    CharacterCreateLayer* m_Layer;

public :
    CharacterCreateScene();
    virtual ~CharacterCreateScene();
    
    virtual bool init();
    
    virtual void update(float deltaTime);
    
    CREATE_FUNC(CharacterCreateScene);
    
    virtual void OnResponse() const override;
};

#endif /* defined(__GameClientMobile__CharacterCreateScene__) */
