//
//  BrowseElementsTaskRequest.h
//  MSTRMobile
//
//  Created by Firat Alpergin on 2/11/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSITaskRequest.h"

@interface BrowseElementsTaskRequest : MSITaskRequest {
	NSString* attributeId;
	NSString* filterXML;
	NSString* shortFilterXML;
	NSString* searchPattern;
	NSInteger blockBegin;
	NSInteger blockCount;
	NSString* searchForms;
	NSString* targetAttributeId;
	NSString* dataSourcesXML;
    NSString* datasetId;
    NSString* cubeinsId;
    NSString* ctlKey;
	NSString* messageID;
}

@property(nonatomic, copy) NSString* attributeId;
@property(nonatomic, copy) NSString* filterXML;
@property(nonatomic, copy) NSString* shortFilterXML;
@property(nonatomic, copy) NSString* searchPattern;
@property(nonatomic, copy) NSString* datasetId;
@property(nonatomic, copy) NSString* cubeinsId;
@property NSInteger blockBegin;
@property NSInteger blockCount;
@property(nonatomic, copy) NSString* searchForms;
@property(nonatomic, copy) NSString* targetAttributeId;
@property(nonatomic, copy) NSString* dataSourcesXML;
@property(nonatomic, copy) NSString* ctlKey;
@property(nonatomic, copy) NSString* messageID;

@end
