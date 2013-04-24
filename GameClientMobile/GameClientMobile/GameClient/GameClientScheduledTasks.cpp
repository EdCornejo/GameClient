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
    GameClientObject& gameClientObject = GameClient::Instance().GetClientObject();
    gameClientObject.CheckHeartbeatTimeOver(m_HeartbeatCount);
}

void TaskSendHeartbeat::Execute()
{
    GameClientObject& gameClientObject = GameClient::Instance().GetClientObject();
    gameClientObject.SendCSRequestHeartbeat(GameClient::Instance().GetDeviceID(), m_HeartbeatCount, gameClientObject.GetConnectionID());
    
}

} // namespace flownet
