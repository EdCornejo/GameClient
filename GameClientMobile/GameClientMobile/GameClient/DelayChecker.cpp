//
//  DelayChecker.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 3/27/13.
//
//

#include "GameClientHeaders.pch"

DelayChecker* DelayChecker::instance_ = nullptr;

DelayChecker::DelayChecker(): m_DelayTimeList(), m_LatestDelayTime(0), m_StartTime(0)
{

}

DelayChecker::~DelayChecker()
{

}

DelayChecker* DelayChecker::Instance()
{
    if(!instance_)
    {
        instance_ = new DelayChecker();
    }
    return instance_;
}

void DelayChecker::CheckStartTime()
{
    flownet::SystemTimer& timer = flownet::GameClient::Instance().GetClientTimer();
    this->m_StartTime = timer.Check();
}

void DelayChecker::CheckEndTime()
{
    if(this->m_StartTime == flownet::ServerTime(0))
        return;
    
    flownet::SystemTimer& timer = flownet::GameClient::Instance().GetClientTimer();
    this->AddDelayTime(timer.Check() - this->m_StartTime);
    this->m_StartTime = flownet::ServerTime(0);
}

int DelayChecker::GetAverageDelayTime()
{
    flownet::ServerTime totalDelayTime = flownet::ServerTime(0);
    
    std::for_each(this->m_DelayTimeList.begin(), this->m_DelayTimeList.end(), [&totalDelayTime](flownet::ServerTime savedTime)
    {
        totalDelayTime += savedTime;
    });
    
    if(this->m_DelayTimeList.size() > 100)
    {
        this->Reset();
    }
    
    
    if(this->m_DelayTimeList.size() == 0)
    {
        return 0;
    }
    else
    {
        return totalDelayTime.count() / this->m_DelayTimeList.size();
    }
}

int DelayChecker::GetLatestDelayTime()
{
    return this->m_LatestDelayTime.count();
}

void DelayChecker::AddDelayTime(flownet::ServerTime time)
{
    if(this->m_LatestDelayTime < time)
    {
        this->m_LatestDelayTime = time;
    }
    
    this->m_DelayTimeList.push_back(time);
}

void DelayChecker::Reset()
{
    this->m_DelayTimeList.clear();
}