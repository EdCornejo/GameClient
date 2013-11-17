//
//  RenderingTaskWorkerRoutine.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 3/14/13.
//
//

#include "GameClientHeaders.pch"

namespace flownet
{
    

RenderingTaskWorkerRoutine::RenderingTaskWorkerRoutine() : m_TaskQueue(), m_IsPaused(false)
{
}

RenderingTaskWorkerRoutine::~RenderingTaskWorkerRoutine()
{
    if( m_TaskQueue.IsEmpty() == false )
    {
        // ASSERT_RELEASE(false);
        EmptyTaskQueue();
    }
}

void RenderingTaskWorkerRoutine::Run(const INT64 timeslice)
{
    SystemTimer& clientTimer = GameClient::Instance().GetClientTimer();
    const ServerTime startTime = clientTimer.Check();
    const milliseconds timeSliceMil = milliseconds(timeslice);
    ServerTime availableTimeSlice = timeSliceMil - (clientTimer.Check()-startTime);
    
    if( m_IsPaused )
    {
        return;
    }
    Task* task = m_TaskQueue.Pop();
    while( task != nullptr )
    {
        task->Execute();
        delete task;
        task = nullptr;
        
        availableTimeSlice = timeSliceMil - (clientTimer.Check()-startTime);
        if( availableTimeSlice <= milliseconds(0) )
        {
            return;
        }
        
        if( m_IsPaused )
        {
            return;
        }
        task = m_TaskQueue.Pop();
    }
}

void RenderingTaskWorkerRoutine::AddTask(Task* task)
{
    ASSERT_RELEASE(task!=nullptr);
    m_TaskQueue.Push(task);
}

void RenderingTaskWorkerRoutine::EmptyTaskQueue()
{
    auto deleteFunction = [](Task* task){
        delete task;
    };
    m_TaskQueue.FlushWithLambda(deleteFunction);
}

void RenderingTaskWorkerRoutine::Pause()
{
    this->m_IsPaused = true;
}

void RenderingTaskWorkerRoutine::Resume()
{
    this->m_IsPaused = false;
}

    
} // namespace flownet
