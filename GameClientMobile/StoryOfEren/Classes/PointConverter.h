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
    static const float RateOfChangeCCPointToFlownetPointX;
    static const float RateOfChangeCCPointToFlownetPointY;
    static const float RateOfChangeFlownetPointToCCPointX;
    static const float RateOfChangeFlownetPointToCCPointY;
    static const float CCPointOffsetX;
    
public:
    static CCPoint Convert(flownet::POINT point);
    static flownet::POINT Convert(CCPoint point);
    static const float ModiXToViewPointX(const float modiX);
    static const float ModiYToViewPointY(const float modiY);
};

#endif /* defined(__GameClientMobile__PointConverter__) */
