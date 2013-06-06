//
//  ClientDirector.h
//  GameClientMobile
//
//  Created by SungJinYoo on 3/25/13.
//
//

#ifndef __GameClientMobile__ClientDirector__
#define __GameClientMobile__ClientDirector__

class ClientDirector : public cocos2d::CCDisplayLinkDirector
{
public:
    ClientDirector(){}
    virtual ~ClientDirector(){}

    template<class CURRENT_SCENETYPE, class NEXT_SCENETYPE>
    bool ChangeScene()
    {
        CURRENT_SCENETYPE* scene = dynamic_cast<CURRENT_SCENETYPE*>(CCDirector::sharedDirector()->getRunningScene());
        if( scene == nullptr )
        {
            return false;
        }
        
        PauseRenderingTaskWorkerRoutine();
        
        NEXT_SCENETYPE* newScene = NEXT_SCENETYPE::create();

        this->replaceScene(newScene);
        
        //ResumeRenderingTaskWorkerRoutine();
        
        return true;
    }
    
    template<class NEXT_SCENETYPE>
    bool ChangeScene()
    {
        PauseRenderingTaskWorkerRoutine();
        
        NEXT_SCENETYPE* newScene = NEXT_SCENETYPE::create();

        this->replaceScene(newScene);
        
        //ResumeRenderingTaskWorkerRoutine();
        
        return true;
    }

public:
    bool IsRenderingWorkerRoutinePaused();
protected:
friend class BaseScene;
    // internal function for ChangeScene()
    void PauseRenderingTaskWorkerRoutine();
    void ResumeRenderingTaskWorkerRoutine();
};



#endif /* defined(__GameClientMobile__ClientDirector__) */
