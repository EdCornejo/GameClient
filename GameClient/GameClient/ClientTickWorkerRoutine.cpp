//
//  ClientTickWorkerRoutine.cpp
//  GameClient
//
//  Created by Sinhyub Kim on 6/10/13.
//  Copyright (c) 2013 RoughHands. All rights reserved.
//

#include "GameClientHeaders.pch"

namespace flownet
{
    

ClientTickWorkerRoutine::ClientTickWorkerRoutine():m_List(),m_LockList()
{
}

ClientTickWorkerRoutine::~ClientTickWorkerRoutine()
{
}

void ClientTickWorkerRoutine::Run(const INT64 timeslice)
{
    SystemTimer& clientTimer = GameClientTester::Instance().GetClientTimer();
    const ServerTime startTime = clientTimer.Check();
    const milliseconds timeSliceMil = milliseconds(timeslice);
    ServerTime availableTimeSlice = timeSliceMil - (clientTimer.Check()-startTime);
    
    const INT currentListIndex = this->GetRoutineCount()%ListMax;
    TesterRPCReceiverList& currentList = m_List[currentListIndex];
    TesterRPCReceiverList& nextList = m_List[(currentListIndex+1)%ListMax];
    BaseLock& currentLockList = m_LockList[currentListIndex];
    BaseLock& nextLockList = m_LockList[(currentListIndex+1)%ListMax];
    
    
    TesterRPCReceiver* testerRPCReceiver = nullptr;

    currentLockList.Lock();
    if( currentList.empty() )
        testerRPCReceiver = nullptr;
    else
    {
        testerRPCReceiver = currentList.front();
        currentList.pop_front();
    }
    currentLockList.UnLock();

    while( testerRPCReceiver )
    {
        testerRPCReceiver->Tick();
        nextLockList.Lock();
        nextList.push_back(testerRPCReceiver);
        nextLockList.UnLock();
        
    
        availableTimeSlice = timeSliceMil - (clientTimer.Check()-startTime);
        if( availableTimeSlice <= milliseconds(0) )
        {
            return;
        }
    
        currentLockList.Lock();
        if( currentList.empty() )
            testerRPCReceiver = nullptr;
        else
        {
            testerRPCReceiver = currentList.front();
            currentList.pop_front();
        }
        currentLockList.UnLock();
    }
}

void ClientTickWorkerRoutine::AddTesterRPCReceiver(flownet::TesterRPCReceiver *testerRPCReceiver)
{
    const INT nextListIndex = (this->GetRoutineCount()+1)%ListMax;
    TesterRPCReceiverList& nextList = m_List[nextListIndex];
    BaseLock& nextLockList = m_LockList[nextListIndex];

    nextLockList.Lock();
    nextList.push_back(testerRPCReceiver);
    nextLockList.UnLock();
}

void ClientTickWorkerRoutine::RemoveTesterRPCReceiver(flownet::TesterRPCReceiver *testerRPCReceiver)
{
    const INT currentListIndex = this->GetRoutineCount()%ListMax;
    TesterRPCReceiverList& currentList = m_List[currentListIndex];
    TesterRPCReceiverList& nextList = m_List[(currentListIndex+1)%ListMax];
    BaseLock& currentLockList = m_LockList[currentListIndex];
    BaseLock& nextLockList = m_LockList[(currentListIndex+1)%ListMax];


    {
        ScopedLock lockCurrentList(currentLockList);
        TesterRPCReceiverList::iterator iter = std::find(currentList.begin(), currentList.end(), testerRPCReceiver);
        if( iter != currentList.end() )
        {
            currentList.erase(iter);
            return;
        }
    }

    {
        ScopedLock lockNextList(nextLockList);
        TesterRPCReceiverList::iterator iter = std::find(nextList.begin(), nextList.end(), testerRPCReceiver);
        if( iter != nextList.end() )
        {
            nextList.erase(iter);
            return;
        }
    }
}
    
} // namespace flownet
