//
//  ReportRowDataAdapter.h
//  IPadMain
//
//  Created by Liang Chen on 8/13/10.
//  Copyright 2010 MicroStrategy Inc. All rights reserved.
//

#import "DataRowProtocol.h"

@class MSIHeaderValue, MSIHeader, MSIReportDataCell, MSIMetric;

@interface MSIReportDataRow : NSObject <MSIDataRowProtocol>{

}

- (MSIReportDataCell*)cellForHeader:(MSIHeader*)header formID:(NSString*)formID;

- (id)initWithRowData:(NSMutableArray*)_rowData;

- (MSIMetric*)metricObjectAtIndex:(NSInteger)index;

- (MSIHeaderValue*) headerValueForAttributeId:(NSString*)attributeID formId:(NSString*)formID;
@end
