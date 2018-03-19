//
//  RWDImageService.h
//  MSTRMobile
//
//  Created by Mingjun Zhu on 05/19/2010.
//  Copyright 2010 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSIServiceImpl.h"

#define PORTRAIT_ORIENTATION 1
#define LANDSCAPE_ORIENTATION 2


@class MSIReportDisplayInfo;

@interface RWDImageService : MSIServiceImpl <ServiceHandler> {
	@private
	MSIReportDisplayInfo* document;
	NSInteger layoutID;
	NSInteger orientation;
	NSArray* groupByElements;
	NSData* binData;
	NSArray* desiredElements;
	NSString* nodeKeyID;
	
	NSArray* imagesArray;
	NSArray* imagesTimeStampArray;
}

@property (nonatomic, strong) MSIReportDisplayInfo* document;
@property (nonatomic, strong) NSArray* groupByElements;
@property NSInteger layoutID;
@property NSInteger orientation;
@property (nonatomic, strong) NSArray* desiredElements;
@property (nonatomic, copy) NSString* nodeKeyID;
// @property (nonatomic, retain) NSArray* imagesArray;

- (void)setImagesArray:(NSArray*)iImagesArray forTimeStamp:(NSArray*) iImagesTimeStampArray;

@end
