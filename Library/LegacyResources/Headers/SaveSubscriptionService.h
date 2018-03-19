//
//  SaveSubscriptionService.h
//  MSTRMobile
//
//  Created by Kevin Maurer on 2/11/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSIServiceImpl.h"

@class MSIReportDisplayInfo;

@interface SaveSubscriptionService : MSIServiceImpl {
	@private
	MSIReportDisplayInfo* report;
	NSString* subscriptionName;
	
}

@property (nonatomic, strong) MSIReportDisplayInfo* report;
@property (nonatomic, copy) NSString* subscriptionName;

+ (SaveSubscriptionService*) newSubscribeService:(MSIReportDisplayInfo*)_report withSubsriptionName:(NSString*)name handler:serviceHandler;

@end
