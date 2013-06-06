//
//  GPS.h
//  GameClientMobile
//
//  Created by SungJinYoo on 5/31/13.
//
//

#ifndef __GameClientMobile__GPS__
#define __GameClientMobile__GPS__

// TODO : study jni
namespace flownet {

class GPS
{
private:
    GPS();
    ~GPS();
    
public:
    static flownet::GPSPoint GetCurrentGPSPoint();
    static std::string GetCurrentAddress();
};

}

#endif /* defined(__GameClientMobile__GPS__) */
