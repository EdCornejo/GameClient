//
//  RenderingTaskWorkerRoutine.h
//  GameClientMobile
//
//  Created by SungJinYoo on 3/14/13.
//
//

#ifndef __GameClientMobile__RenderingTaskWorkerRoutine__
#define __GameClientMobile__RenderingTaskWorkerRoutine__

namespace flownet
{

class RenderingTaskWorkerRoutine : public WorkerRoutine
{
private:
    TaskQueue       m_TaskQueue;
    volatile BOOL   m_IsPaused;
    
public:
    RenderingTaskWorkerRoutine();
    virtual ~RenderingTaskWorkerRoutine();
    
    virtual void    Run(const INT64 timeslice) override;
    void            AddTask(Task* task);
    void            Pause();
    void            Resume();
    
    BOOL            IsPaused(){ return m_IsPaused;}
    
private:
    void            EmptyTaskQueue();
    
    
};

} // namespace flownet

#endif /* defined(__GameClientMobile__RenderingTaskWorkerRoutine__) */
