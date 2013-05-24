//
//  BattleScene.h
//  GameClientMobile
//
//  Created by SungJinYoo on 3/11/13.
//
//

#ifndef __GameClientMobile__BattleScene__
#define __GameClientMobile__BattleScene__

class BattleScene : public BaseScene
{
public:
    BattleScene();
    virtual ~BattleScene();
    virtual bool init() override;
    CREATE_FUNC(BattleScene);
    
    virtual void update(float deltaTime) override;
};

#endif /* defined(__GameClientMobile__BattleScene__) */
