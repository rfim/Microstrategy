//
//  CrosstabDataProtocol.h
//  MicroStrategyMobile
//
//  Created by kyang on 3/3/11.
//  Copyright 2011 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "DataProviderProtocol.h"
#import "Enums.h"

@class MSIReportDataCell, MSIReportDataRow;

@protocol MSICrosstabDataProtocol<MSIDataProviderProtocol>
/**
 Total number of rows in row axis
 */
- (NSInteger)rowCount;
/**
 Total number of columns in a row
 */
- (NSInteger)columnCount;  
/**
 the number of metric columns
 */
- (NSInteger)metricColumnCount;  
/**
 the attriture form affects the result
 */
- (NSInteger) rowAxisHeaderCount;	
/**
 TemplateUnits count, won't be affected by attribute form
 */
- (NSInteger) rowAxisTemplateUnitCount;   
/**
 the header num in column axis
 */
- (NSInteger) colAxisHeaderCount; 
/**
 TemplateUnits count, won't be affected by attribute form
 */
- (NSInteger) colAxisTemplateUnitCount;  

/**
 Row at specified index in rowAxis, the object of the array is MSIHeaderValue(Adapter)
 */
- (MSIReportDataRow*) rowByIndex: (NSInteger) index;	

/**
 Row at specified index in columnAxis
 */
- (MSIReportDataRow*) colAxisRowByIndex:(NSInteger)index;

/**
 Return the MSIReportDataCell object at given location.
 */
- (MSIReportDataCell*)cellByAxisType:(AxisType)type andOrdinal:(NSInteger)iOrdinal andDepth:(NSInteger)iDepth; 

@optional
- (NSString*) getRowDisplayHTMLStringAtRow:(NSInteger)rowIndex columnTitleIndex:(NSInteger)titleIndex withDelimiter:(NSString*)delimeter;
- (NSString*) getRowDisplayStringAtRow:(NSInteger)rowIndex columnTitleIndex:(NSInteger)titleIndex withDelimiter:(NSString*)delimeter;
- (NSString*) getColDisplayStringAtColumn:(NSInteger)colIndex rowTitleIndex:(NSInteger)titleIndex withDelimiter:(NSString*)delimeter;
- (NSString*) getEntireColDisplayStringAtColumn:(NSInteger)colIndex withDelimiter:(NSString *)delimeter;
- (NSString*) getRowTitleDisplayString:(NSInteger)index;
- (NSString*) getColTitleDisplayString:(NSInteger)index;
- (MSIHeader*) getHeader:(MSIDataElement*)iDataElement;
@end

