//
//  CFConnectionManager.h
//  GameClient
//
//  Created by Kim Sinhyub on 4/30/13.
//  Copyright (c) 2013 RoughHands. All rights reserved.
//

#ifndef __GameClient__CFConnectionManager__
#define __GameClient__CFConnectionManager__

namespace flownet
{

class CFConnectionManager
{
private:
    CFConnectionList            m_CFConnectionList;
    BaseLock                    m_LockConnectionList;
    
    BoostIOService&             m_IOService;
    STRING                      m_FEServerAddress;
    INT                         m_FEServerPort;
    INT                         m_NumberOfCFConnections;
    volatile INT                m_NextCFConnection;
    
public:
    CFConnectionManager(BoostIOService& ioService, const STRING& serverAddress=FESERVER_CF_CONNECT_ADDRESS, const INT serverPort=FESERVER_CF_CONNECT_PORT);
    ~CFConnectionManager();
    
    void  Initialize(const INT numberOfCFConnection);
    
    // Round Robin Request ?
    CFConnection*         GetNextCFConnection();
    
private:
    // To guarantee multi-thread-safe behavior
    // DO NOT CreateDBServerConnection after initialize()
    CFConnection*         CreateCFConnection();
};

}


#endif /* defined(__GameClient__CFConnectionManager__) */
