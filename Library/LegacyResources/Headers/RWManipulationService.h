//
//  RWManipulationService.h
//  IPadMain
//
//  Created by Ji Jin on 4/25/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSIServiceImpl.h"
#import "MSTRHandler.h"

@class MSIReportDisplayInfo;


@interface RWManipulationService : MSIServiceImpl <ServiceHandler, MSTRHandler> {
@private
	MSIReportDisplayInfo* document;
	NSData* newData;
	NSString* events;
}

@property (nonatomic, strong) MSIReportDisplayInfo* document;
@property (nonatomic, copy) NSString* events;

@end
