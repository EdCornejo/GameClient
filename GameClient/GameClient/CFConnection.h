//
//  CFConnection.h
//  GameClient
//
//  Created by Kim Sinhyub on 4/30/13.
//  Copyright (c) 2013 RoughHands. All rights reserved.
//

#ifndef __GameClient__CFConnection__
#define __GameClient__CFConnection__

namespace flownet
{

class CFConnection : public ClientObject
{
friend class GameFCPacketHandler;

private:
    ConnectionID        m_CFConnectionID;
    DeviceID            m_DeviceID;
    
public:
    CFConnection(BoostIOService& ioService, PacketParser* packetParser);
    virtual ~CFConnection();
    virtual void            InitializeClient(const CHAR* connectAddress, const INT connectPort) override;

    void                    SetCFConnectionID(const ConnectionID connectionID)  {   m_CFConnectionID = connectionID; }
    const ConnectionID      GetConnectionID()                                   {   return m_CFConnectionID; }
    
    void                    SetDeviceID(const DeviceID& deviceID)   { m_DeviceID = deviceID; }
    DeviceID                GetDeviceID()       {   return m_DeviceID; }
    
protected:
    virtual void        OnConnect(const BoostErrorCode& error, BoostTCPSocket* connectedSocket) override;
    
public:
    #include "GameCFProtocolSenderDeclaration.hpp"
};
typedef Vector<CFConnection*>::type CFConnectionList;

} // namespace flownet

#endif /* defined(__GameClient__CFConnection__) */
