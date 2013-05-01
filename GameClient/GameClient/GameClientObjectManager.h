//
//  GameClientObjectManager.h
//  GameClient
//
//  Created by Kim Sinhyub on 4/30/13.
//  Copyright (c) 2013 RoughHands. All rights reserved.
//

#ifndef __GameClient__GameClientObjectManager__
#define __GameClient__GameClientObjectManager__

namespace flownet
{

class GameClientObjectManager
{
private:
    GameClientObjectList        m_GameClientObjectList;
    BaseLock                    m_LockList;
    BoostIOService&             m_IOService;
    STRING                      m_ServerAddress;
    INT                         m_ServerPort;
    INT                         m_NumberOfGameClientObjects;
    volatile INT                m_NextGameClientObject;
    
public:
    GameClientObjectManager(BoostIOService& ioService, const STRING& serverAddress=SERVER_CONNECT_ADDRESS, const INT serverPort=SERVER_CONNECT_PORT);
    ~GameClientObjectManager();
    
    void  Initialize(const INT numberOfGameClientObject);
    
    // Round Robin Request ?
    GameClientObject*         GetNextGameClientObject();
    
private:
    // To guarantee multi-thread-safe behavior
    // DO NOT CreateDBServerConnection after initialize()
    GameClientObject*         CreateGameClientObject();
};

}


#endif /* defined(__GameClient__GameClientObjectManager__) */
