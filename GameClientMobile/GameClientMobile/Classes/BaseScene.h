//
//  BaseScene.h
//  GameClientMobile
//
//  Created by SungJinYoo on 3/5/13.
//
//

#ifndef __GameClientMobile__BaseScene__
#define __GameClientMobile__BaseScene__

using namespace cocos2d;

class BaseScene : public cocos2d::CCScene
{
protected:
    BackgroundLayer* m_BackgroundLayer;
    EffectLayer* m_EffectLayer;
    ObjectLayer* m_ObjectLayer;
    ActorLayer* m_ActorLayer;
//    PlayerLayer* m_PlayerLayer;
//    MonsterLayer* m_MonsterLayer;
    UILayer* m_UILayer;
    HeartbeatLayer* m_HeartbeatLayer;
    
public:
    BaseScene();
    virtual ~BaseScene();
    virtual bool init();
    CREATE_FUNC(BaseScene);
    
    virtual void update(float deltaTime) override;
    
public:
    BackgroundLayer* GetBackgroundLayer() const;
    EffectLayer* GetEffectLayer() const;
    ObjectLayer* GetObjectLayer() const;
    ActorLayer* GetActorLayer() const;
//    PlayerLayer* GetPlayerLayer() const;
//    MonsterLayer* GetMonsterLayer() const;
    UILayer* GetUILayer() const;
    HeartbeatLayer* GetHeartbeatLayer() const;
};

#endif /* defined(__GameClientMobile__BaseScene__) */
