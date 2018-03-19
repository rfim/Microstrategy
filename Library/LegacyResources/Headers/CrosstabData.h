//
//  CrosstabData.h
//  iPadMain Library
//
//  Created by Liang Chen on 10/15/10.
//  Copyright 2010 MicroStrategy Inc. All rights reserved.
//  Deprecated. Will use MSIModelData instead


#import "CrosstabDataProtocol.h"
#import "MSTRHandler.h"


@class MSIHeaderValue;
@class MSIMetricValue;
@class ThresholdSliderData;
class DSSObjectInfo;
//@class MSIDisplayInfo;

@interface MSICrosstabData : NSObject <MSICrosstabDataProtocol,MSTRHandler> {
}

@property (nonatomic, assign) BOOL isFormatInfoShared;
@property (nonatomic, readonly) NSArray* mpRowHeaderArray;
@property (nonatomic, readonly) NSArray* mpColHeaderArray;
@property (nonatomic, readonly) NSArray* mpMetricHeaderArray;
@property (nonatomic, assign) void* mpDataGridIterator;

- (id)initWithIterator:(void*)ipIterator;

- (void) reinitData;

- (MSIHeaderValue*) createHeaderValuesAtRow:(NSInteger)rowIndex atColumn:(NSInteger)colIndex;
- (MSIMetricValue*) createMetricValuesAtRow:(NSInteger)rowIndex atColumn:(NSInteger)colIndex;
//Clear warning by Nan Jiang, 08/06/2012
- (MSIHeaderValue*) createColAxisHeaderValueAtRow:(NSInteger)rowIndex atColumn:(NSInteger)colIndex;

- (void *)formatManager;
- (NSArray*)getThresholdsByMetricIndex:(int)index thresholdsSemanticType:(_BaseFormSemanticType)SemanticType;
- (ThresholdSliderData*) getThresholdInfo:(NSString*)iMetricID defaultIndex:(int)index threshold:(BOOL&)oHasThresholdInfo;
- (NSString*) formatMetricValue:(double)iValue withIndex:(int)iIndex;
- (BOOL)isElementEqualAtRow:(NSInteger)rowIndex andColumn:(NSInteger)colIndex withElement:(void*)element withAxis:(int) tAxis;

- (NSMutableArray*) metricArray;
- (NSMutableArray*) attributeArray;
- (NSMutableArray*) attributeFormArray;
- (NSMutableArray*) colHeaderArray;
- (NSMutableArray*) rowHeaderArray;

- (NSArray*) headersInGroups;
- (NSArray*) headers;
- (DSSObjectInfo*) getDSSObjectInfobyDisplayInfo:(NSString*) info;
@end