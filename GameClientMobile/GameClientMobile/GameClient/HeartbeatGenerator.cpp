//
//  HeartbeatGenerator.cpp
//  GameClient
//
//  Created by Sinhyub Kim on 2/21/13.
//  Copyright (c) 2013 RoughHands. All rights reserved.
//

#include "GameClientHeaders.pch"

namespace flownet
{

HeartbeatGenerator::HeartbeatGenerator(GameClientObject* gameClientObject)
        :m_GameClientObject(gameClientObject),
        m_Period(HEARTBEAT_PERIOD_TIME),
        m_AllowanceTime(HEARTBEAT_ALLOWANCE_TIME),
        m_HeartbeatCount(0),
        m_LastHeartbeatTime(0)
{
}


HeartbeatGenerator::~HeartbeatGenerator()
{
}

void HeartbeatGenerator::StartHeartbeat(const ServerTime& startTime)
{
    m_LastHeartbeatTime = startTime;
    m_HeartbeatCount = 0;
    GameClient::Instance().GetScheduledTaskWorkerRoutine().AddTaskAfter(m_AllowanceTime, new TaskCheckHeartbeatTimeOver(m_HeartbeatCount));
    m_GameClientObject->SendCSRequestHeartbeat(GameClient::Instance().GetDeviceID(), m_HeartbeatCount, m_GameClientObject->GetConnectionID());
}

BOOL HeartbeatGenerator::CheckHeartbeatTimeOver(const INT64 heartbeatCount, const ServerTime& checkTime)
{
    if( m_HeartbeatCount < heartbeatCount )
    {
#ifndef HEARTBEAT_ALLOW_DEBUG
        ASSERT_RELEASE(false);
#endif // HEARTBEAT_ALLOW_DEBUG
        std::cout << "CheckHeartbeatTimeOver : heartbeatCount :" << m_HeartbeatCount << std::endl;
        return true;
    }
    if( checkTime-m_LastHeartbeatTime > m_AllowanceTime )
    {
#ifndef HEARTBEAT_ALLOW_DEBUG
        ASSERT_RELEASE(false);
#endif // HEARTBEAT_ALLOW_DEBUG
        std::cout << "CheckHeaertbeatTimeOver : " << (checkTime-m_LastHeartbeatTime).count() << std::endl;
        return true;
    }

    return false;
}

void HeartbeatGenerator::KeepHeartbeat(const INT64 heartbeatCount, const ServerTime& heartbeatTime)
{
    ASSERT_DEBUG(heartbeatCount == m_HeartbeatCount);
    
    if( heartbeatCount != m_HeartbeatCount )
    {
        std::cout << "KeepHeartbeat Time Error :" << (heartbeatTime-m_LastHeartbeatTime).count() << "LastHeartbeat : " << m_LastHeartbeatTime.count() <<  std::endl;
        return;
    }
    
    m_HeartbeatCount++;
    m_LastHeartbeatTime = heartbeatTime;
    
    GameClient::Instance().GetScheduledTaskWorkerRoutine().AddTaskAfter(m_Period, new TaskSendHeartbeat(m_HeartbeatCount));
}

} // namespace
