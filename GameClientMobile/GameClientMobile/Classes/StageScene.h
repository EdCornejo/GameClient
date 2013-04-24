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
private:
    StageInfoList m_StageInfoList;
    
public:
    StageScene();
    virtual ~StageScene();
    
    virtual bool init();
    CREATE_FUNC(StageScene);
    
    virtual void update(float deltaTime) override;
    
    void AddStageInfo(StageInfo* stageInfo);
    
    void OnCreateButtonClick(CCObject* sender);
    void OnJoinButtonClick(CCObject* sender);
};

#endif /* defined(__GameClientMobile__StageScene__) */
