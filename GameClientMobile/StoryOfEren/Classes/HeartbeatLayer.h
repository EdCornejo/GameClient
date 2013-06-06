//
//  HeartbeatLayer.h
//  GameClientMobile
//
//  Created by SungJinYoo on 3/6/13.
//
//

#ifndef __GameClientMobile__HeartbeatLayer__
#define __GameClientMobile__HeartbeatLayer__

using namespace cocos2d;

class HeartbeatLayer : public BaseLayer
{
private:
    CCLabelTTF* m_HeartbeatLabel;
    CCString m_HeartbeatText;
    CCLabelTTF* m_HeartbeatDelayLabel;
    CCString m_HeartbeatDelayText;
    CCLabelTTF* m_PacketSizeLabel;
    CCString m_PacketSizeText;
    
public:
    HeartbeatLayer();
    virtual ~HeartbeatLayer();
    virtual bool init();
    CREATE_FUNC(HeartbeatLayer);
    
    virtual void update(float deltaTime) override;
    
public:
    void changeHeartbeatText(flownet::INT64 heartbeatCount);
};

#endif /* defined(__GameClientMobile__HeartbeatLayer__) */
