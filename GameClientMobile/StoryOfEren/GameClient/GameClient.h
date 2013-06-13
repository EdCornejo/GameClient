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

#ifndef GAMECLIENTTESTER


// Implement as a Singleton Object
class GameClient
{
private:
    static GameClient*              _SingletonInstance;

    GameClient();
    
public:
    ~GameClient();
    
    static GameClient& Instance();
    static void DeleteInstance();
    

// GameClient Network Part
private:
    // Managed by thread controller
    SystemTimer                     m_ClientTimer;
    ScheduledTaskWorkerRoutine      m_ScheduledTaskWorkerRoutine;
    TaskWorkerRoutine               m_GameTaskWorkerRoutine;
    NetworkWorkerRoutine            m_NetworkWorkerRoutine;
    ThreadController                m_ThreadController;

    // Managed by Cocos2D main therad
    RenderingTaskWorkerRoutine      m_RenderingTaskWorkerRoutine;
    
    GameClientPacketHandler         m_ClientPacketHandler;
    GameClientPacketParser          m_ClientPacketParser;
    GameClientObject                m_ClientObject;         // NetworkObject
    
    FCPacketHandler                 m_FCPacketHandler;
    FCPacketParser                  m_FCPacketParser;
    CFConnection                    m_CFConnection;
    
    GameClientRPCInterface*         m_GameClientRPCInterface;
   
    // State for assert accurate operation calls
private:
    BOOL            m_IsInitialized;
    BOOL            m_IsStarted;

public:
    void InitializeClient(GameClientRPCInterface* gameClientRPCReceiver);
    void StartClient();
    void TerminateClient();
    
public:
    inline SystemTimer&                 GetClientTimer()                {   return m_ClientTimer;   }
    inline ScheduledTaskWorkerRoutine&  GetScheduledTaskWorkerRoutine() {   return m_ScheduledTaskWorkerRoutine;    }
    inline TaskWorkerRoutine&           GetGameTaskWorkerRoutine()      {   return m_GameTaskWorkerRoutine; }
    inline RenderingTaskWorkerRoutine&  GetRenderingTaskWorkerRoutine() {   return m_RenderingTaskWorkerRoutine; }
    inline NetworkWorkerRoutine&        GetNeworkWorkerRoutine()        {   return m_NetworkWorkerRoutine; }
    inline ThreadController&            GetThreadController()           {   return m_ThreadController; }
    inline GameClientObject&            GetClientObject()               {   return m_ClientObject; }
    inline CFConnection&                GetCFConnection()               {   return m_CFConnection; }
    inline GameClientRPCInterface*      GetGameClientRPCInterface()     {   return m_GameClientRPCInterface; }

// End of GameClient Network Part
    
// GameClient Data Part
    
private:
    DeviceID        m_DeviceID;
    GameServerID    m_GameServerID;
    SessionID       m_SessionID;
    UserID          m_UserID;
    ActorID         m_MyActorID;
    ClientStage*    m_ClientStage;
    OTP             m_OTP;
    
    ChatMessageDeque m_ChatMessageLog;
    
public:
    DeviceID        GetDeviceID();
    GameServerID    GetGameServerID();
    SessionID       GetSessionID();
    UserID          GetUserID()         { return m_UserID; }
    ActorID         GetMyActorID();
    ClientStage*    GetClientStage();
    OTP             GetOTP()            { return m_OTP; }
    ChatMessageDeque& GetChatMessageLog() { return m_ChatMessageLog; }
    
    PlayerMap&      GetPlayerMap();
    MonsterMap&     GetMonsterMap();
    
    void SetDeviceID(DeviceID deviceID);
    void SetGameServerID(const GameServerID gameServerID)   { m_GameServerID = gameServerID; }
    void SetSessionID(SessionID sessionID);
    void SetUserID(const UserID userID)                     { m_UserID = userID;}
    void SetMyActorID(ActorID myActorID);
    void SetClientStage(ClientStage* clientStage);
    void SetOTP(const OTP otp)                              { m_OTP = otp; }

    void Finalize();
    
    void AddChatMessageLog(flownet::ActorID senderID, flownet::STRING senderName, flownet::STRING message);
    
// End of GameClient Data Part
    
// test data generation
    void InitializeTestData();
// end of test data generation

private:
    void InitializeDeviceID();
};

#endif // GAMECLIENTTESTER

} // namespace flownet

#endif /* defined(__GameClient__GameClient__) */
