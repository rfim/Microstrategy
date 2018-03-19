//
//  RWBinaryService.h
//  MicroStrategyMobile
//
//  Created by Sun, Han on 1/19/13.
//  Copyright (c) 2013 MicroStrategy Inc. All rights reserved.
//

#import "MSIServiceImpl.h"

#define PORTRAIT_ORIENTATION 1
#define LANDSCAPE_ORIENTATION 2

typedef enum _RWDSeparatedBinaryCacheStatus{
	RWDSeparatedBinaryCacheReserved = 0,
	RWDSeparatedBinaryCacheAllValid,
	RWDSeparatedBinaryCacheDefinitionValid,
	RWDSeparatedBinaryCacheInvalid,
} RWDSeparatedBinaryCacheStatus;


@protocol SeparatedBinarySetting <NSObject>
@optional
- (void) setSeparatedBinary:(NSArray* ) iSeparatedBinary;
@end


@class RWSeparableBinaryReader;

@interface RWBinaryService : MSIServiceImpl <ServiceHandler, SeparatedBinarySetting>{
@protected
    MSIReportDisplayInfo* document;
    //Definition caching
    NSArray* separatedBinary;                               //Binary slices
    RWDSeparatedBinaryCacheStatus separatedBinryStatus;     //Binary cache status
    EnumBinaryDownloadMode downloadMode;                    //Binary download modes
    RWSeparableBinaryReader* rwBinaryReader;                //Binary parser for combined definition and data request\
    //Incremental downloading
    NSInteger resultFlags;
	NSInteger detailFlags;
    int orientation;
}

@property (nonatomic, strong) MSIReportDisplayInfo* document;
@property EnumBinaryDownloadMode downloadMode;
@property (nonatomic, strong) NSArray* separatedBinary;
@property RWDSeparatedBinaryCacheStatus separatedBinryStatus;
@property NSInteger resultFlags;
@property NSInteger detailFlags;
@property int orientation;

-(void) createBinaryReader;

-(BOOL) binaryCompressed;
/**
 * isCached:(MSIReportDisplayInfo*)
 * check cache for subscription and all binary parts mode on Polaris. Don't call for other modes.
 */
-(BOOL) isCached:(MSIReportDisplayInfo*) report;
-(NSArray*)getDataResultArray;
-(void) decompressBinary;
-(void) populateBinaryCacheIDs;
-(NSData*)decompressBinaryData:(NSData*)data;
@end
