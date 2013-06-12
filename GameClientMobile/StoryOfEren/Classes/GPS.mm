//
//  GPS.cpp
//  GameClientMobile
//
//  Created by SungJinYoo on 5/31/13.
//
//

#include "GPS_ios.h"
#include "Headers.pch"

static double static_getLatitude() {
    return [[GPS_ios sharedInstance] getLatitude];
}

static double static_getLongitude() {
    return [[GPS_ios sharedInstance] getLongitude];
}

static std::string static_getAddress() {
    std::string address;
    if([[[GPS_ios sharedInstance] getAddress] length] != 0) address = [[[GPS_ios sharedInstance] getAddress] cStringUsingEncoding:NSUTF8StringEncoding];
    return address;
}

namespace flownet {

GPS::GPS() {}

GPS::~GPS() {}

GPSPoint GPS::GetCurrentGPSPoint()
{
    flownet::GPSPoint gpsPoint;
#ifdef FLOWNET_IPHONE
    gpsPoint.latitude = static_getLatitude();
    gpsPoint.longitude = static_getLongitude();
#elif defined FLOWNET_ANDROID
    gpsPoint.latitude = 0;
    gpsPoint.longitude = 0;
#else
    gpsPoint.latitude = 0;
    gpsPoint.longitude = 0;
#endif
    return gpsPoint;
}

}

std::string GPS::GetCurrentAddress()
{
#ifdef FLOWNET_IPHONE
    return static_getAddress();
#elif defined FLOWNET_ANDROID
    return "";
#else
    return "";
#endif
}