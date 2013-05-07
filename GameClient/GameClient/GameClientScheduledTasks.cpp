//
//  GameClientScheduledTasks.cpp
//  GameClient
//
//  Created by Sinhyub Kim on 2/19/13.
//  Copyright (c) 2013 RoughHands. All rights reserved.
//

#include "GameClientHeaders.pch"

namespace flownet
{

void TaskCheckHeartbeatTimeOver::Execute()
{
    m_GameClientObject->CheckHeartbeatTimeOver(m_HeartbeatCount);
}

void TaskSendHeartbeat::Execute()
{
    m_GameClientObject->SendCSRequestHeartbeat(m_GameClientObject->GetDeviceID(), m_HeartbeatCount, m_GameClientObject->GetConnectionID());
}

} // namespace flownet
