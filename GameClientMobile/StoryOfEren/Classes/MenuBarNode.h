//
//  MenuBarNode.h
//  GameClientMobile
//
//  Created by SungJinYoo on 4/20/13.
//
//

#ifndef __GameClientMobile__MenuBarNode__
#define __GameClientMobile__MenuBarNode__

class MenuBarNode : public CCNode, public CCTouchDelegate
{
private:
    bool m_IsOpen;
    CCSprite* m_SlideButton;
    CCSprite* m_Body;
    CCMenuItem* m_HomeButton;
    CCMenuItem* m_SettingButton;
    CCMenuItem* m_LogoutButton;

public:
    enum {
        PositionX = 480,
        PositionY = 240,
    };
public:
    MenuBarNode();
    virtual ~MenuBarNode();

    virtual bool init() override;
    static MenuBarNode* create();
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event) override;
    
    void OnResponse() const;
    
private:
    void Slide();

    void OnHomeButtonClicked(CCObject* sender);
    void OnSettingButtonClicked(CCObject* sender);
    void OnLogoutButtonClicked(CCObject* sender);
};

#endif /* defined(__GameClientMobile__MenuBarNode__) */
