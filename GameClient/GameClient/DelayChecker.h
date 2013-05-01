//
//  DelayChecker.h
//  GameClientMobile
//
//  Created by SungJinYoo on 3/27/13.
//
//

#ifndef __GameClientMobile__DelayChecker__
#define __GameClientMobile__DelayChecker__

class DelayChecker{
private:
    DelayChecker();
    ~DelayChecker();
    
    static DelayChecker* instance_;

private:
    std::vector<flownet::ServerTime> m_DelayTimeList;
    flownet::ServerTime m_LatestDelayTime;
    flownet::ServerTime m_StartTime;
    
public:
    static DelayChecker* Instance();

    void CheckStartTime();
    void CheckEndTime();
    int GetAverageDelayTime();
    int GetLatestDelayTime();
    
private:
    void AddDelayTime(flownet::ServerTime time);
    void Reset();
};

#endif /* defined(__GameClientMobile__DelayChecker__) */
