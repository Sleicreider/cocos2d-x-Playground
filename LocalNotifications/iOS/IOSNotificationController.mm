//
//  IOSNotificationController.m
//  MemoryProject
//
//  Created by Dominik Reisner on 14.01.20.
//

#include "IOSNotificationController.h"
#import <UserNotifications/UserNotifications.h>

static constexpr auto TRIGGER_SECONDS_INTERVAL = 60*60*9;

void IOSNotificationController::InitNotification()
{
    UNUserNotificationCenter* center = [UNUserNotificationCenter currentNotificationCenter];
    [center requestAuthorizationWithOptions:(UNAuthorizationOptionAlert + UNAuthorizationOptionSound)
       completionHandler:^(BOOL granted, NSError * _Nullable error) {
          // Enable or disable features based on authorization.
        if(granted)
        {
            IOSNotificationController::RescheduleNotification();
        }
    }];
}

void IOSNotificationController::RescheduleNotification()
{
    UNMutableNotificationContent* content = [[UNMutableNotificationContent alloc] init];
    content.title = [NSString localizedUserNotificationStringForKey:@"Collect free credits!" arguments:nil];
    content.body = [NSString localizedUserNotificationStringForKey:@"Find the pairs, challenge yourself"
            arguments:nil];
    
    UNTimeIntervalNotificationTrigger* trig = [UNTimeIntervalNotificationTrigger triggerWithTimeInterval:(TRIGGER_SECONDS_INTERVAL) repeats: YES];
     
    // Create the request object.
    UNNotificationRequest* request = [UNNotificationRequest
           requestWithIdentifier:@"MagicalPairsNotification" content:content trigger:trig];
    
    UNUserNotificationCenter* center = [UNUserNotificationCenter currentNotificationCenter];
    [center addNotificationRequest:request withCompletionHandler:^(NSError * _Nullable error) {
       if (error != nil) {
           NSLog(@"%@", error.localizedDescription);
       }
    }];
}
