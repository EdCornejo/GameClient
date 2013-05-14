//
//  SplashScene.h
//  GameClientMobile
//
//  Created by SungJinYoo on 4/15/13.
//
//

#ifndef __GameClientMobile__SplashScene__
#define __GameClientMobile__SplashScene__

class SplashScene : public BaseScene
{
private:
    bool m_HasRequestSession;
    bool m_HasRequestConnect;
public:
    SplashScene();
    virtual ~SplashScene();
    
    virtual bool init();
    CREATE_FUNC(SplashScene);
    
    virtual void update(float deltaTime) override;
};

#endif /* defined(__GameClientMobile__SplashScene__) */
