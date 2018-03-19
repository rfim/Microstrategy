//
//  DerivedReportInfo.h
//  MSTRMobile
//
//  Created by Kevin Maurer on 1/13/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSIReportDisplayInfo.h"

@class ReportInfo;

@interface DerivedReportInfo : MSIReportDisplayInfo {
	MSIReportDisplayInfo* baseReport;
}

@property (strong, readonly) MSIReportDisplayInfo* baseReport;

-(id)initWithMessageID:(NSString*)msgID stateID:(int)stID parentSubscription:(MSIReportDisplayInfo*)parentSub; 
-(DerivedReportInfo*)newDerivedReport;

-(NSInteger)propagateChangesToParent:(NSError**)error;

@end
