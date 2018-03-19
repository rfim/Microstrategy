//
//  TabularData.h
//  IPadMain
//
//  Created by Liang Chen on 8/11/10.
//  Copyright 2010 MicroStrategy Inc. All rights reserved.
//  Deprecated. Will use MSIModelData instead


#import "TabularDataProtocol.h"
#import "MSTRHandler.h"


@class MSIPropertyGroup;


@interface MSITabularData : NSObject<MSITabularDataProtocol, MSTRHandler> {
	void* mpDataGridIterator;
	void* mpFormatManager;
	NSMutableArray* mpRowHeaderArray;
	NSMutableArray* mpMetricHeaderArray;
	BOOL isFormatInfoShared;
	NSMutableDictionary * mpVisPropsDict;
	NSMutableArray* mpAttributeArray; // we only have attribute forms in the row header array;
									  // to avoid cross retain, we put all attributes in this array
    
    BOOL mpHasRowHeader; // If mpHasRowHeader == YES, we will resolveHeaders when mpRowHeaderArray is empty, otherwise we just go ahead
}

@property (nonatomic) BOOL isFormatInfoShared;


- (id)initWithIterator:(void*)ipIterator;

- (MSIPropertyGroup*) getCellPropertyGroupAtRow:(NSInteger)rowIndex atColumn:(NSInteger)colIndex;

/**
	0		 1    2      3  : reportDataCellColIndex
	RegionID DESC Profit Revenue
	reportDataCellRowIndex: 0-based index
	reportDataCellColIndex:0-based index
 **/	
- (MSIHeaderValue*) createHeaderValueAtRow:(NSInteger)reportDataCellRowIndex atColumn:(NSInteger)reportDataCellColIndex;

/**
 0		 1    2      3  : reportDataCellColIndex
 RegionID DESC Profit Revenue
 reportDataCellRowIndex: 0-based index
 reportDataCellColIndex:0-based index
 **/
- (MSIHeaderValue*) createMetricValueAtRow:(NSInteger)reportDataCellRowIndex atColumn:(NSInteger)reportDataCellColIndex;

//< For TQMS 474680
/**
 * Get first MSIHeader Element
 */
- (MSIHeaderValue *) getFirstHeaderElement:(MSIHeader*)iHeader;

/**
 * return NSNotFound if not found
 */
- (NSInteger)indexOfHeaderObject:(MSIHeader *)headerObject;

// the three function added by cpan.
- (int)attributeCount;
- (int)attributeFormCount;
- (int)metricCount;
- (NSString*) formatMetricValue:(double)iValue withIndex:(int)iIndex ;
- (BOOL)isElementEqualAtRow:(NSInteger)rowIndex andColumn:(NSInteger)colIndex withElement:(void*)element;
- (NSMutableArray*) metricArray;
- (NSMutableArray*) attributeArray;
- (NSMutableArray*) attributeFormArray;
@end
