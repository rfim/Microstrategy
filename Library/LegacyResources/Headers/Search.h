//
//  Search.h
//  MicroStrategyMobile
//
//  Created by Amirali Charania on 7/14/11.
//  Copyright 2011 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSIObjectInfo.h"

@class SAXStringHandler;

@interface MSMSearch : MSIObjectInfo {
	NSString* searchXML;
	SAXStringHandler* stringHandler;
}

@property (nonatomic, copy) NSString* searchXML;

@end
