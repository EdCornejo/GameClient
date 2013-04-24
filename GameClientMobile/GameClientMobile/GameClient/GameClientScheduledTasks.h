//
//  GameClientScheduledTasks.h
//  GameClient
//
//  Created by Sinhyub Kim on 2/19/13.
//  Copyright (c) 2013 RoughHands. All rights reserved.
//

#ifndef __GameClient__GameClientScheduledTasks__
#define __GameClient__GameClientScheduledTasks__

namespace flownet
{

class TaskCheckHeartbeatTimeOver : public ScheduledTask
{
private:
    INT64       m_HeartbeatCount;
public:
    TaskCheckHeartbeatTimeOver(const INT64 heartbeatCount)
        :ScheduledTask("TaskCheckHeartbeatTimeOver"),m_HeartbeatCount(heartbeatCount){}
    virtual ~TaskCheckHeartbeatTimeOver(){}
    
    virtual void Execute() override;
};

class TaskSendHeartbeat : public ScheduledTask
{
private:
    INT64       m_HeartbeatCount;
public:
    TaskSendHeartbeat(const INT64 heartbeatCount):ScheduledTask("TaskSendHeartbeat"), m_HeartbeatCount(heartbeatCount){}
    virtual ~TaskSendHeartbeat(){}
    
    virtual void Execute() override;
};

} // namespace flownet

#endif /* defined(__GameClient__GameClientScheduledTasks__) */
