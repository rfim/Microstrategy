//
//  DrilledReportInfo.h
//  MSTRMobile
//
//  Created by Kevin Maurer on 11/12/09.
//  Copyright 2009 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "DerivedReportInfo.h"

@interface DrilledReportInfo : DerivedReportInfo {
	BOOL isLinkedDrill;
    
    //ASYNCEXC: support dirll path display
    NSMutableArray* drillPaths;
}

-(id)initWithMessageID:(NSString*)msgID stateID:(int)stID parentSubscription:(MSIReportDisplayInfo*)parentSub;
-(NSString*) drilledReportName;
-(void) drilledReportName:(NSString*) name;

@property BOOL isLinkedDrill;
@property (strong, readonly) NSMutableArray* drillPaths;

@end
