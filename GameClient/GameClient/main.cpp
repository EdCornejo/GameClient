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
}

void CreateCFConnection(const INT numberOfConnection)
{
    for( INT i=0; i<numberOfConnection; ++i )
    {
        GameClientTester::Instance().GetCFConnectionManager().CreateCFConnection();
    }
}

int main(int argc, const char * argv[])
{
    LogTerminal::Initialize();
    LogSystem::Initialize("GameClientSystemLog");

    TesterRPCReceiver testerRPCReceiver;
    
    GameClientTester::Instance().InitializeClient(&testerRPCReceiver);
    GameClientTester::Instance().StartClient();

    INT serverPrintoutCount = 0;
    
    INT numberOfCFConnections = 0;
    
    while(true)
    {
        if( ++serverPrintoutCount >= 100 )
        {
            LogTerminal::Instance() << "GameClient is on. Worker Thread is Working..";
            LogTerminal::Instance().Commit();
            
            serverPrintoutCount = 0;
        }
        
        LogTerminal::GetLogReceiver()->FlushLog();
        LogSystem::GetLogReceiver()->FlushLog();
        
        static BOOL isAddConnectionTurn = true;
        static ServerTime lastTickTime = GameClientTester::Instance().GetClientTimer().Check();
        static ServerTime lastTurnOffTime = GameClientTester::Instance().GetClientTimer().Check();
        ServerTime currentTime = GameClientTester::Instance().GetClientTimer().Check();
        

        if( isAddConnectionTurn == false )
        {
            if( currentTime - lastTurnOffTime > milliseconds(20000) )
            {
                isAddConnectionTurn = true;
            }
        }
        else
        {
            if( (numberOfCFConnections % 390) == 0 )
            {
                isAddConnectionTurn = false;
                lastTurnOffTime = GameClientTester::Instance().GetClientTimer().Check();
            }
        }
        
        if( isAddConnectionTurn )
        {
            if( currentTime - lastTickTime >= ServerTime(300) )
            {
                if( numberOfCFConnections <= 5000 )
                {
                    const INT increment = 10;
                    new std::thread(CreateCFConnection, increment);
                    numberOfCFConnections += increment;
                    LogTerminal::Instance() << "Number Of Connections " << numberOfCFConnections;
                    LogTerminal::Instance().Commit();
                }
                lastTickTime = GameClientTester::Instance().GetClientTimer().Check();
            }
        }
        
        std::this_thread::sleep_for(milliseconds(100));
    }
    
    GameClientTester::Instance().TerminateClient();
    return 0;
}
