//
//  main.cpp
//  GameClient
//
//  Created by Sinhyub Kim on 2/13/13.
//  Copyright (c) 2013 Sinhyub Kim. All rights reserved.
//

#include <iostream>
#include "GameClientHeaders.pch"

using namespace flownet;
using namespace std;

namespace flownet
{
class GameClientRPCReceiver : public GameClientRPCInterface
{
private:

public:
    GameClientRPCReceiver(){}
    virtual ~GameClientRPCReceiver(){}
    
    virtual void OnSCProtocolError() const override
    {
    }
    virtual void OnSCResponseConnect(INT64 connectionID) const override
    {
    }
    virtual void OnSCResponseSession(INT64 sessionID) const override
    {
    }
    virtual void OnSCResponseHeartbeat(INT64 heartbeatCountAck) const override
    {
    }
    virtual void OnSCResponseMyPlayerInfo(Player player) const override
    {
    }

};
} //namespace flownet

int main(int argc, const char * argv[])
{

    GameClientRPCReceiver gameClientRPCReceiver;
    GameClient::Instance().InitializeClient(&gameClientRPCReceiver);
    GameClient::Instance().StartClient();

    while(true)
    {
        LOG_STDOUT( std::cout << "GameClient is on. Worker Thread is Working.." << std::endl; );
        std::this_thread::sleep_for(seconds(9));
        
        GameClient::Instance().GetClientObject().SendCSRequestHeartbeat(1);
    }
    
    GameClient::Instance().TerminateClient();
    
    return 0;

}
