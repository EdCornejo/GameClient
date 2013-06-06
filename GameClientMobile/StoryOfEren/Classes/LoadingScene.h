//
//  LoadingScene.h
//  GameClientMobile
//
//  Created by SungJinYoo on 3/13/13.
//
//

#ifndef __GameClientMobile__LoadingScene__
#define __GameClientMobile__LoadingScene__


class LoadingScene : public BaseScene
{
private:
    CCSprite* m_LoadingSprite;
    
public:
    LoadingScene();
    virtual ~LoadingScene();
    
    bool init();
    static LoadingScene* create();
    
    virtual void update(float deltaTime) override;
    
};

#endif /* defined(__GameClientMobile__LoadingScene__) */

