//
//  GameClient.h
//  GameClient
//
//  Created by Sinhyub Kim on 2/13/13.
//  Copyright (c) 2013 RoughHands. All rights reserved.
//

#ifndef __GameClient__GameClient__
#define __GameClient__GameClient__

namespace flownet
{

// Implement as a Singleton Object
class GameClient
{
private:
    SystemTimer                     m_ClientTimer;
    ScheduledTaskWorkerRoutine      m_ScheduledTaskWorkerRoutine;
    TaskWorkerRoutine               m_GameTaskWorkerRoutine;
    TaskWorkerRoutine               m_RPCTaskWorkerRoutine;
    NetworkWorkerRoutine            m_NetworkWorkerRoutine;
    ThreadController                m_ThreadController;

    GameClientPacketHandler         m_ClientPacketHandler;
    GameClientPacketParser          m_ClientPacketParser;
    GameClientObject                m_ClientObject;         // NetworkObject

    static GameClient*              _SingletonInstance;

    // State for assert accurate operation calls
private:
    BOOL            m_IsInitialized;
    BOOL            m_IsStarted;


private://protected  // Singleton Instance. don't call GameClient() from public side
    GameClient();
public:
    ~GameClient();
    
    static GameClient&              Instance();
    static void                     DeleteInstance();
    
    void InitializeClient(GameClientRPCInterface* gameClientRPCReceiver);
    void StartClient();
    void TerminateClient();
    
public:
    inline SystemTimer&                 GetClientTimer()            {   return m_ClientTimer;   }
    inline ScheduledTaskWorkerRoutine&  GetScheduledTaskWorkerRoutine() { return m_ScheduledTaskWorkerRoutine;    }
    inline TaskWorkerRoutine&           GetGameTaskWorkerRoutine()  {   return m_GameTaskWorkerRoutine; }
    inline TaskWorkerRoutine&           GetRPCTaskWorkerRoutine()   {   return m_RPCTaskWorkerRoutine; }
    inline NetworkWorkerRoutine&        GetNeworkWorkerRoutine()    {   return m_NetworkWorkerRoutine; }
    inline ThreadController&            GetThreadController()       {   return m_ThreadController; }
    inline GameClientObject&            GetClientObject()           {   return m_ClientObject; }

};

} // namespace flownet

#endif /* defined(__GameClient__GameClient__) */
