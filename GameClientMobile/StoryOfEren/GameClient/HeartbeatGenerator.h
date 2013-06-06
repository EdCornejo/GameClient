//
//  HeartbeatGenerator.h
//  GameClient
//
//  Created by Sinhyub Kim on 2/21/13.
//  Copyright (c) 2013 RoughHands. All rights reserved.
//

#ifndef __GameClient__HeartbeatGenerator__
#define __GameClient__HeartbeatGenerator__

namespace flownet
{

class GameClientObject;
class HeartbeatGenerator
{
private:
    GameClientObject*           m_GameClientObject;
    const milliseconds          m_Period;
    const milliseconds          m_AllowanceTime;

    INT64                       m_HeartbeatCount;
    ServerTime                  m_LastHeartbeatTime;
    
    
public:
    HeartbeatGenerator(GameClientObject* gameClientObject);
    ~HeartbeatGenerator();
    
    
    void StartHeartbeat(const ServerTime& startTime);
    BOOL CheckHeartbeatTimeOver(const INT64 heartbeatCount, const ServerTime& checkTime);
    void KeepHeartbeat(const INT64 heartbeatCount, const ServerTime& heartbeatTime);
    
    
};

}

#endif /* defined(__GameClient__HeartbeatGenerator__) */
