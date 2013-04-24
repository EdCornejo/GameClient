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
    virtual bool init();
    CREATE_FUNC(BattleScene);
    
    virtual void update(float deltaTime);
};

#endif /* defined(__GameClientMobile__BattleScene__) */
