//
//  DeviceInfo.h
//  GameClientMobile
//
//  Created by SungJinYoo on 5/31/13.
//
//

#ifndef __GameClientMobile__DeviceInfo__
#define __GameClientMobile__DeviceInfo__

// TODO : study jni
namespace flownet {

class DeviceInfo
{
private:
    DeviceInfo();
    ~DeviceInfo();
    
public:
    static const chat * GetDeviceName();
    static const char * GetCurrentAddress();
};

}

#endif /* defined(__GameClientMobile__DeviceInfo__) */
