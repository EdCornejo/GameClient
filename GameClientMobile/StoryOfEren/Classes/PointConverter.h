//
//  PointConverter.h
//  GameClientMobile
//
//  Created by SungJinYoo on 3/13/13.
//
//

#ifndef __GameClientMobile__PointConverter__
#define __GameClientMobile__PointConverter__

USING_NS_CC;

class PointConverter
{
private:
    PointConverter();
    ~PointConverter();
    
public:
    static const float RateOfChangeCCPointToFlownetPointX = 0.05333333333333;
    static const float RateOfChangeCCPointToFlownetPointY = 0.10526315789474;
    static const float RateOfChangeFlownetPointToCCPointX = 18.75;
    static const float RateOfChangeFlownetPointToCCPointY = 9.5;
    static const float CCPointOffsetX = 15;
    
public:
    static CCPoint Convert(flownet::POINT point);
    static flownet::POINT Convert(CCPoint point);
    static const float ModiXToViewPointX(const float modiX);
    static const float ModiYToViewPointY(const float modiY);
};

#endif /* defined(__GameClientMobile__PointConverter__) */
