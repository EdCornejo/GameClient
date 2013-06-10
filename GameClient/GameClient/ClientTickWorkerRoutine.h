//
//  ClientTickWorkerRoutine.h
//  GameClient
//
//  Created by Sinhyub Kim on 6/10/13.
//  Copyright (c) 2013 RoughHands. All rights reserved.
//

#ifndef __GameClient__ClientTickWorkerRoutine__
#define __GameClient__ClientTickWorkerRoutine__

namespace flownet
{

typedef List<TesterRPCReceiver*>::type TesterRPCReceiverList;

class ClientTickWorkerRoutine : public WorkerRoutine
{
private:
enum{
    ListA=0,
    ListB=1,
    ListMax,
};
    TesterRPCReceiverList   m_List[ListMax];
    BaseLock                m_LockList[ListMax];
    
public:
    ClientTickWorkerRoutine();
    virtual ~ClientTickWorkerRoutine();
    
    virtual void    Run(const INT64 timeslice) override;
    
    void            AddTesterRPCReceiver(TesterRPCReceiver* testerRPCReceiver);
    void            RemoveTesterRPCReceiver(TesterRPCReceiver* testerRPCReceiver);
    
};

} // namespace flownet


#endif /* defined(__GameClient__ClientTickWorkerRoutine__) */
