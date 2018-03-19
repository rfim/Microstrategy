//
//  SearchMetaDataTaskRequest.h
//  MicroStrategyMobile
//
//  Created by Amirali Charania on 7/14/11.
//  Copyright 2011 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSITaskRequest.h"

@interface SearchMetaDataTaskRequest : MSITaskRequest {
	NSString* searchXML;
	NSString* styleName;
	NSInteger blockBegin;
	NSInteger blockCount;
	NSString* searchPattern;
}

@property (nonatomic, copy) NSString* searchXML;
@property (nonatomic, copy) NSString* styleName;
@property NSInteger blockBegin;
@property NSInteger blockCount;
@property (nonatomic, copy) NSString* searchPattern;

@end
