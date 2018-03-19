//
//  TabularDataModel.h
//  MSTRMobileSDK
//
//  Created by Xiaochen Du on 11/17/10.
//  Copyright 2010 MicroStrategy Inc. All rights reserved.
//
#import "DataProviderProtocol.h"

@class MSIReportDataRow, MSIHeaderValue, MSIHeader, MSIDisplayInfo, MSIReportDataCell;
@protocol MSITabularDataProtocol<MSIDataProviderProtocol>

/**
 Total number of rows in row axis
 */
- (NSInteger)rowCount;

/**
 Total number of cells in a row of row axis
 */
- (NSInteger)headerCount;  

/**
 Row at specified index
 */
- (MSIReportDataRow *) rowByIndex: (NSInteger) index;

/**
 Data for a cell at specified row and column index
 */
//TODO: We will remove this soon.
- (MSIHeaderValue *) dataForRow: (NSInteger) rowIndex column:(NSInteger) columnIndex;


/**
 Return the MSIReportDataCell object at given location.
 */
- (MSIReportDataCell*) cellAt:(NSUInteger)row column:(NSUInteger)column; 

/**
 Return the header object at given column. 
 */
- (MSIHeader*) headerObjectByIndex:(NSInteger)index;

/**
 Return the header object matches the display info object.
 Note: The input MSIDisplayInfo* object is of type MSIMetric* or MSIAttributeForm*.
 If the input is of type MSIAttribute*, then this method returns a MSIHeader* corresponding to the
 first encountered MSIAttributeForm*, so it does not make much sense.
 */
- (MSIHeader*)headerObject:(MSIDisplayInfo*)object;


/**
 Return the display info by object id. 
 */
- (MSIDisplayInfo*) objectByID:(NSString*) objID;

@optional

/**
 Return the drill XML for cell. 
 */
- (NSString*) drillXML:(MSIReportDataCell*) cell;

/**
	Returns YES if the underlying grid can be marked for transactions, otherwise NO.	
 **/
- (BOOL) isTransactionEnabled;

/**
	Returns YES if the row can be marked for transactions, otherwise NO.
	Change the name please, rowMarkedForTransactions
 **/
- (BOOL) isMarkedForInsertOrDelete;

/**
	Returns YES if the cell at rowIndex and columnIndex is editable for transactions, otherwise NO.
 **/	
- (BOOL) isEditableAtRowIndex:(NSInteger)rowIndex columnIndex:(NSInteger)columnIndex;


/**
	Returns the formatted metric value for the given index info without applying the threshold defined on it. 
 **/
- (NSString*) formatRawMetricValueAtRow:(NSInteger)rowIndex atColumn:(NSInteger)colIndex;


@end

