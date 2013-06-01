//
//  GPS_ios.m
//  GameClientMobile
//
//  Created by SungJinYoo on 5/31/13.
//
//

#import "GPS_ios.h"

@implementation GPS_ios

static GPS_ios* sharedGPS = nil;

+ (GPS_ios*) sharedInstance
{
    @synchronized(self) {
        if(!sharedGPS)
            sharedGPS = [[GPS_ios alloc] init];
    }
    
    if(sharedGPS) {
        [sharedGPS startUpdatingLocation];
    }
    
    return sharedGPS;
}

+ (id) alloc
{
    @synchronized(self) {
        NSAssert(sharedGPS == nil, @"must be singleton");
        return [super alloc];
    }
    return nil;
}

- (id) init
{
    if(self = [super init]) {
        m_LocationManager = [[CLLocationManager alloc] init];
        m_LocationManager.delegate = self;
        m_LocationManager.desiredAccuracy = kCLLocationAccuracyHundredMeters;
    }
    
    return self;
}

- (double) getLatitude {
    return m_Latitude;
}

- (double) getLongitude {
    return m_Longitude;
}

- (NSString*) getAddress {
    return m_Address;
}

- (void) startUpdatingLocation {
    [m_LocationManager startUpdatingLocation];
}

- (void)locationManager:(CLLocationManager *)manager didFailWithError:(NSError *)error
{
    NSLog(@"gps info didFailWithError: %@", error);
}

- (void)locationManager:(CLLocationManager *)manager didUpdateToLocation:(CLLocation *)newLocation fromLocation:(CLLocation *)oldLocation
{
    NSLog(@"didUpdateToLocation: %@", newLocation);
    CLLocation *currentLocation = newLocation;
    
    if (currentLocation != nil) {
        m_Latitude = currentLocation.coordinate.latitude;
        m_Longitude = currentLocation.coordinate.longitude;
        
        CLGeocoder* geo = [[CLGeocoder alloc] init];
        [geo reverseGeocodeLocation:currentLocation completionHandler:^(NSArray *placemarks, NSError *error) {
            if(placemarks) {
                CLPlacemark* placemark = [placemarks objectAtIndex:0];
                NSMutableDictionary *addrDic = [[NSMutableDictionary alloc] initWithDictionary:placemark.addressDictionary];
                [addrDic removeObjectForKey:@"Street"];
                [addrDic removeObjectForKey:@"SubAdministrativeArea"];
                [addrDic removeObjectForKey:@"Thoroughfare"];
                [addrDic removeObjectForKey:@"ZIP"];
                [addrDic removeObjectForKey:@"Name"];
                [addrDic removeObjectForKey:@"PostCodeExtension"];
                [addrDic removeObjectForKey:@"Country"];
                [addrDic removeObjectForKey:@"SubLocality"];
                [addrDic removeObjectForKey:@"SubThoroughfare"];
                
                NSString* string = [addrDic objectForKey:@"City"];
                if(string == nil) string = [addrDic objectForKey:@"State"];
                
                NSLog(@"%@", string);

                if(m_Address) [m_Address release];
                
                m_Address = string;
                [m_Address retain];
            }
        }];
    }
    
    [manager stopUpdatingLocation];
}

@end
