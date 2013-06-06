//
//  HeartbeatLayer.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 3/6/13.
//
//

#include "Headers.pch"

HeartbeatLayer::HeartbeatLayer() : m_HeartbeatLabel(nullptr), m_HeartbeatText(), m_PacketSizeLabel(nullptr),m_PacketSizeText()
{
    
}

HeartbeatLayer::~HeartbeatLayer()
{
    if(this->m_HeartbeatLabel)
    {
        this->m_HeartbeatLabel->release();
        this->m_HeartbeatLabel = nullptr;
    }
    if(this->m_PacketSizeLabel)
    {
        this->m_PacketSizeLabel->release();
        this->m_PacketSizeLabel = nullptr;
    }
}

bool HeartbeatLayer::init()
{
    if(!BaseLayer::init())
    {
        return false;
    }

#ifdef HEARTBEAT_DEBUG_SCREEN_ON
    this->m_HeartbeatText.initWithFormat("heartbeat count: %d, average delay time: %dms, latest delay time: %dms", 0, 0, 0);
    
    this->m_HeartbeatLabel = CCLabelTTF::create();
    this->m_HeartbeatLabel->retain();
    this->m_HeartbeatLabel->setPosition(ccp(200, 310));
    this->m_HeartbeatLabel->setHorizontalAlignment(kCCTextAlignmentCenter);
    this->m_HeartbeatLabel->setColor(ccc3(255, 0 ,0));
    this->m_HeartbeatLabel->setString(this->m_HeartbeatText.getCString());
    
    this->addChild(this->m_HeartbeatLabel);
#endif
#ifdef PACKET_SIZE_LOGGING
    this->m_PacketSizeLabel = CCLabelTTF::create();
    this->m_PacketSizeLabel->retain();
    this->m_PacketSizeLabel->setPosition(ccp(120, 10));
    this->m_PacketSizeLabel->setColor(ccc3(255, 0, 0));
    this->m_PacketSizeLabel->setString(this->m_PacketSizeText.getCString());

    this->addChild(this->m_PacketSizeLabel);
#endif

    scheduleUpdate();

    
    return true;
}

void HeartbeatLayer::update(float deltaTime)
{
#ifdef HEARTBEAT_DEBUG_SCREEN_ON
    this->m_HeartbeatLabel->setString(this->m_HeartbeatText.getCString());
#endif
#ifdef PACKET_SIZE_LOGGING
    this->m_PacketSizeText.initWithFormat("total recv/send packet size: %.2f Kbytes", static_cast<float>(PacketSizeLogger::Instance()->GetTotalPacketSize()) / 1024);
    this->m_PacketSizeLabel->setString(this->m_PacketSizeText.getCString());
#endif
}

void HeartbeatLayer::changeHeartbeatText(flownet::INT64 heartbeatCount)
{
#ifdef HEARTBEAT_DEBUG_SCREEN_ON
    DelayChecker* checker = DelayChecker::Instance();
    this->m_HeartbeatText.initWithFormat("heartbeat count: %d, average delay time: %d ms, latest delay time: %d ms", static_cast<int>(heartbeatCount), checker->GetAverageDelayTime(), checker->GetLatestDelayTime());
#endif
}
