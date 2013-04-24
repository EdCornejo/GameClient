//
//  PointConverter.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 3/13/13.
//
//

#include "Headers.pch"

#define BOUNDARY 15

// NOTE : default mapping from flownet::POINT to CCPoint
//        rate of change(CCPoint / flownet::POINT) for x is 18.75
//        rate of change(CCPoint / flownet::POINT) for y is 9.5
//        rate of change(flownet::POINT / CCPoint) for x is 0.05333333333333.....
//        rate of change(flownet::POINT / CCPoint) for y is 0.10526315789474
//        flownet::POINT(0, 0) -> CCPoint(0 + boundary, 0)        boundary is 10 pixel
//        flownet::POINT(24, 20) -> CCPoint(480 - boundary, 190)   480 - 10 is right boundary

flownet::POINT PointConverter::Convert(CCPoint point)
{
    return flownet::POINT((point.x - CCPointOffsetX) * RateOfChangeCCPointToFlownetPointX, point.y * RateOfChangeCCPointToFlownetPointY);
}

CCPoint PointConverter::Convert(flownet::POINT point)
{
    return CCPoint((point.x * RateOfChangeFlownetPointToCCPointX) + CCPointOffsetX, point.y * RateOfChangeFlownetPointToCCPointY);
}

const float PointConverter::ModiXToViewPointX(const float modiX)
{
    return (modiX * RateOfChangeFlownetPointToCCPointX) + CCPointOffsetX;
}

const float PointConverter::ModiYToViewPointY(const float modiY)
{
    return modiY * RateOfChangeFlownetPointToCCPointY;
}