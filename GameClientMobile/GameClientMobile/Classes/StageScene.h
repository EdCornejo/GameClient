//
//  StageScene.h
//  GameClientMobile
//
//  Created by SungJinYoo on 3/11/13.
//
//

#ifndef __GameClientMobile__StageScene__
#define __GameClientMobile__StageScene__

class StageScene : public BaseScene
{
public:
    StageScene();
    virtual ~StageScene();
    virtual bool init() override;
    CREATE_FUNC(StageScene);
    
    virtual void update(float deltaTime) override;
};

#endif /* defined(__GameClientMobile__StageScene__) */
