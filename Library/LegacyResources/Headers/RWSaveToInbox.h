//
//  RWSaveToInbox.h
//  iPadMain Library
//
//  Created by Ji Jin on 8/24/10.
//  Copyright 2010 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "XMLAPIService.h"

@class MSIReportDisplayInfo;

@interface RWSaveToInbox : XMLAPIService {
	MSIReportDisplayInfo* document;
}

@property (nonatomic, strong) MSIReportDisplayInfo* document;
@end
