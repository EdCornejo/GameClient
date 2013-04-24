//
//  GameClientRPCInterface.h
//  GameClientMobile
//
//  Created by Sinhyub Kim on 2/28/13.
//
//

#ifndef GameClientMobile_GameClientRPCInterface_h
#define GameClientMobile_GameClientRPCInterface_h

namespace flownet
{

// Pure virtual class, just define interface
class GameClientRPCInterface
{
protected:
    GameClientRPCInterface(){}
public:
    virtual ~GameClientRPCInterface(){}

public:
    virtual void OnSCProtocolError() const = 0;
    virtual void OnSCResponseConnect(INT64 connectionID) const = 0;
    virtual void OnSCResponseSession(INT64 sessionID) const = 0;
    virtual void OnSCResponseHeartbeat(INT64 heartbeatCountAck) const = 0;
    virtual void OnSCResponseMyPlayerInfo(Player player) const = 0;
};

} // namespace


#endif
