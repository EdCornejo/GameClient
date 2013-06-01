//
//  GPS_ios.h
//  GameClientMobile
//
//  Created by SungJinYoo on 5/31/13.
//
//

#import <CoreLocation/CoreLocation.h>

@interface GPS_ios : NSObject <CLLocationManagerDelegate> {
    CLLocationManager* m_LocationManager;
    double m_Latitude;
    double m_Longitude;
    NSString* m_Address;
}

+ (GPS_ios*) sharedInstance;

- (double) getLatitude;

- (double) getLongitude;

- (NSString*) getAddress;

- (void) startUpdatingLocation;

@end
