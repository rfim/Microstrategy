//
//  ModelData.h
//  MicroStrategyMobile
//
//  Created by changlei pan on 7/30/12.
//  Copyright (c) 2012 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "DataProviderProtocol.h"
#import "Enums.h"
#import "MSTRHandler.h"
#import "header.h"


typedef enum _DataProviderType{
	TabularDataType = 0, CrosstabDataType,RWControlDataType
}DPType;

@class MSIHeaderValue;
@class MSIMetricValue;
@class ThresholdSliderData;
@class MSIPropertyGroup;
@class MSIMetricValue;
@class MSIDataElement;
@class MSIDisplayInfo;

@interface TemplateMetricsPlaceHolder : MSIHeader
{
}
@property(nonatomic, assign) NSInteger depth;
@end


@protocol ModelDataProtocol<MSIDataProviderProtocol>
@optional
- (NSString*) getRowDisplayHTMLStringAtRow:(NSInteger)rowIndex columnTitleIndex:(NSInteger)titleIndex withDelimiter:(NSString*)delimeter;
- (NSString*) getRowDisplayStringAtRow:(NSInteger)rowIndex columnTitleIndex:(NSInteger)titleIndex withDelimiter:(NSString*)delimeter;
- (NSString*) getColDisplayStringAtColumn:(NSInteger)colIndex rowTitleIndex:(NSInteger)titleIndex withDelimiter:(NSString*)delimeter;
- (NSString*) getEntireColDisplayStringAtColumn:(NSInteger)colIndex withDelimiter:(NSString *)delimeter;
- (NSString*) getRowTitleDisplayString:(NSInteger)index;
- (NSString*) getColTitleDisplayString:(NSInteger)index;
- (MSIHeader*) getHeader:(MSIDataElement*)iDataElement;
@end


@interface MSIModelData : NSObject<ModelDataProtocol,MSTRHandler>
{
}

@property (nonatomic, assign) BOOL isFormatInfoShared;

@property (nonatomic, readonly) NSMutableArray* rowHeaderArray;
@property (nonatomic, readonly) NSMutableArray* rowTemplateUnitArray;
@property (nonatomic, readonly) NSMutableArray* colHeaderArray;
@property (nonatomic, readonly) NSMutableArray* colTemplateUnitArray;
@property (nonatomic, readonly) NSMutableArray* metricHeaderArray;
@property (nonatomic, readonly) NSMutableArray* arrayWithRowOffset;
@property (nonatomic, readonly) NSMutableArray* arrayWithColOffset;
@property (nonatomic, readonly) NSMutableArray* arrayWithRowOffsetForUnit;
@property (nonatomic, readonly) NSMutableArray* arrayWithColOffsetForUnit;



/**
 * @return : an array with headValue and metricValue of whole row
 */
- (id)initWithIterator:(void*)ipIterator;
- (id)initWithIterator:(void *)ipIterator withSliceCountOnRow:(int)sliceCountOnRow andSliceCountOnCol:(int)sliceCountOnCol;

/**
 * @return : the widget property dictionary
 */
-(NSDictionary *) getVisualizationProperties;
- (MSIMetricValue*) allocMetricValueAtRow:(NSInteger)rowIndex atColumn:(NSInteger)colIndex;

/**
 *  Return the headerValue at given location.
 *  MSIMetricValue is subclass of headerValue
 *  @parameter1	: AxisType has two types: row_axis and col_axis
 *  @parameter2: row is 0-based
 *  @parameter3: 0		 1    2      3  : col (0-based)
 RegionID DESC Profit Revenue
 *  @return    : headerValue should be released by users
 *  @description: if AxisType is row_axis, loacation is based on row_axis range, row from top to bottom, col from left to right
 if AxisType is col_axis, loacation is based on col_axis range, row from left to right, col from top to bottom
 */
- (MSIHeaderValue*) allocHeaderValueByAxisType:(AxisType)type andRowIndex:(NSInteger)row andColumnIndex:(NSInteger)col;

/**
 * @return : an array with headValue and metricValue of whole row
 */
- (NSMutableArray*)arrayWithHeaderValueOfWholeRowByAxisType:(AxisType)iAxis andRowIndex:(NSInteger)row;

/**
 * @return YES if headerValue is subtotal;
 */
- (BOOL) isSubtotalCell:(MSIHeaderValue*)headerValue;

/**
 * @return YES if this row represents a subtotal row
 */
- (BOOL) isSubtotalRowWithAxisType:(AxisType)type andRowIndex:(NSInteger)row;

/**
 * @return:  header object at given AxisType and col index.
 */
- (MSIHeader*) headerObjectByAxisType:(AxisType)type andColumnIndex:(NSInteger)col;
//- (Header*) headerObjectByIndex:(NSInteger)colIndex;
//- (Header*) headerObjectByColAxisIndex:(NSInteger)colIndex;

/**
 * @ return:  index of the given header object in row axis , NSNotFound if not found
 */
- (NSInteger)indexOfHeaderObject:(MSIHeader *)headerObject;

/**
 * @ return:  index of the given header object in column axis , NSNotFound if not found
 */
- (NSInteger)colAxisIndexOfHeaderObject:(MSIHeader *)headerObject;


/**
 *  Total number of rows in row axis
 */
- (NSInteger)rowCount;

/**
 *  Total number of cells in a row of row axis
 */
- (NSInteger)columnCount;

/**
 *  Total number of metric cell in a row of row axis if no attribute in column axis
 *  Total number of metric cell in a row of row axis if has attribute in column axis and metric in column axis
 *  Total number of cell in a column of column axis if metric in row axis.
 */
- (NSInteger)metricColumnCount;

/**
 * metric count
 */
- (NSInteger)metricCount;

/**
 *  the header num in row axis , do not include metric header
 */
- (NSInteger) rowAxisHeaderCount;

/**
 *  TemplateUnits count in row axis, won't be affected by attribute form
 */
- (NSInteger) rowAxisTemplateUnitCount;

/**
 *  the header num in column axis
 */
- (NSInteger) colAxisHeaderCount;

/**
 *  TemplateUnits count in column axis, won't be affected by attribute form
 */
- (NSInteger) colAxisTemplateUnitCount;

/**
 *  Is there metric in row axis
 @result   YES means there is
 */
- (BOOL)isMetricInRowAxis;

/**
 *  @return: YES if the underlying grid can be marked for transactions, otherwise NO.
 */
- (BOOL) isTransactionEnabled;

/**
 *  @return: YES if the row can be marked for transactions, otherwise NO.
 */
- (BOOL) rowMarkedForTransactions;


/**
 *  @return: YES if the cell at rowIndex and columnIndex is editable for transactions, otherwise NO.
 */
- (BOOL) isEditableWithAxisType:(AxisType)AxisType RowIndex:(NSInteger)rowIndex columnIndex:(NSInteger)columnIndex;




/**
 *  @parameter1: a double value you want to format
 *  @parameter2: the metric index fromat according to, 0-based, if two metric exits on the dataset, the value can be 0 or 1
 *  @return:     he formatted metric value for the given double value with the given metric index without applying the threshold defined on it
 */
- (NSString*) formatMetricValue:(double)iValue withMetricIndex:(int)iIndex;

/**
 *  @parameter1:  cell's row index
 *  @parameter2:  cell's col index, 0-based
 *  @return:     the formatted metric value for the given index and column info without applying the threshold defined on it,colIndex count from the first metricColumn,
 return nil if the position is not metric value
 */
// Returns the formatted metric value for the given index and column info without applying the threshold defined on it,colIndex count from the first metricColumn
//- (NSString*) formatRawMetricValueAtRowIndex:(NSInteger)row atColIndex:(NSInteger)col;
- (NSArray*)getThresholdsByMetricIndex:(int)index thresholdsSemanticType:(BaseFormSemanticType)SemanticType;
- (ThresholdSliderData*) getThresholdInfo:(NSString*)iMetricID defaultIndex:(int)index threshold:(BOOL*)oHasThresholdInfo;

/*TQMS 993032, for the case of threshold type = rank_top and rank_bottom, whether to ignore Subtotal rows maters when determining the max value of threshold. In Heatmap, we should not consider the subtotal rows, so I add an additional method here.
 */
- (ThresholdSliderData*) getThresholdInfo:(NSString*)iMetricID defaultIndex:(int)index threshold:(BOOL*)oHasThresholdInfo withIgnoreTotal:(BOOL)ignoreTotal;

/**
 *  @parameter: any displayInfo like attributeForm, metric
 *  @return   : the header object matches the display info object.
 *  @description:  The input MSIDisplayInfo* object is of type MSIMetric* or MSIAttributeForm*.
 If the input is of type MSIAttribute*, then this method returns a MSIHeader* corresponding to the
 first encountered MSIAttributeForm*, so it does not make much sense.
 */
- (MSIHeader*)headerObject:(MSIDisplayInfo*)object;

/**
 *  @return   : the display info by object id. MSIHeaderValue and metric are all subclass of disPlayInfo
 *  @description: MSIHeaderValue and metric are all subclass of disPlayInfo
 */
- (MSIDisplayInfo*) objectByID:(NSString*) objID;

//- (Header*) getHeader:(MSIDataElement*)iDataElement;

/**
 *  @param3   : element is acturally an DSSBaseElementProxy pointer
 *  @return   : YES in case the given element is equal to the element in the dataset indicated by parameter 1,2,3
 */
- (BOOL)isElementEqualWithAxisType:(int)iAxisType AtRow:(NSInteger)rowIndex andColumn:(NSInteger)colIndex withElement:(void*)element;

/**
 *  @return   : first MSIHeader Element
 */
- (MSIHeaderValue *) getFirstHeaderElement:(MSIHeader*)iHeader;


/**
 *  @parameter 1   : (AxisType)type
 *  @parameter 2   : (NSInteger)rowIndex
 *  @parameter 3   : (NSInteger)depth, not colIndex, depth can be converted from colIndex using [widgetHelper getDepthByAxisType:(AxisType)type andColIndex:(int)col]
 *  @parameter 4   : (NSString*)delimeter, delimeter you want to insert into this string
 *  @return        : connection of headerValue, seperated by delimeter, loacated by given rowIndex and depth..
 */
- (NSString*) getDisplayStringWithAxisType:(AxisType)type AtRow:(NSInteger)rowIndex columnDepth:(NSInteger)depth withDelimiter:(NSString*)delimeter;

/**
 *  @return   : connection of headerValue in oneColumn, seperated by delimeter.
 */
- (NSString*) getEntireColDisplayStringAtColumn:(NSInteger)colIndex withDelimiter:(NSString *)delimeter;

/**
 * For Graph Matrix, updating slice iterator
 * @parameter 1   : Slice iterator
 **/
- (void) resetIterator:(void*) iterator;
/**
 * Quick API to detect subtotal cell.
 * @parameter 1: (AxisType)type
 * @parameter 2: cell ordinal
 * @parameter 3: cell depth
 **/
- (BOOL) isSubtotalCellWithAxisType:(AxisType) iAxisType withOrdinal:(int) iOrdinal withDepth:(int) iDepth;

- (MSIHeaderValue*) allocHeaderValueAtRow:(NSInteger)rowIndex atColumn:(NSInteger)colIndex;//lishan manipulation

/**
 * clear all cached data.
 **/
- (void)clearCachedData;

/**
 * update specified cache value.
 **/
- (void) updateDataElementInCacheAtAxisType:(AxisType)type andRowIndex:(NSInteger)row andColumnIndex:(NSInteger)col;

/*
 D3 Widget Color By Attribute support
 */
- (void*) getDropZoneSet;

@end
