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
    
public:
    CFConnection(BoostIOService& ioService, PacketParser* packetParser);
    virtual ~CFConnection();
    virtual void            InitializeClient(const CHAR* connectAddress, const INT connectPort) override;
protected:
    virtual void        OnConnect(const BoostErrorCode& error, BoostTCPSocket* connectedSocket) override;
    virtual void        OnDisconnect() override ;
    
public:
    #include "GameCFProtocolSenderDeclaration.hpp"
};
typedef Vector<CFConnection*>::type CFConnectionList;

} // namespace flownet

#endif /* defined(__GameClient__CFConnection__) */
