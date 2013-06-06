//
//  ExpBarNode.h
//  GameClientMobile
//
//  Created by SungJinYoo on 5/27/13.
//
//

#ifndef __GameClientMobile__ExpBarNode__
#define __GameClientMobile__ExpBarNode__

class ExpBarNode : public CCNode
{
private:
    bool m_Initialized;
    CCLabelTTF* m_LevelLabel;
    CCSprite* m_CurrentExpBar;
    

public:
    ExpBarNode();
    virtual ~ExpBarNode();
    
    virtual bool init() override;

    CREATE_FUNC(ExpBarNode);
    
    virtual void update(float deltaTime) override;

    void Update();
};

#endif /* defined(__GameClientMobile__ExpBarNode__) */
