//
//  DataWrapper.h
//  MSTRDataCloud
//
//  Created by Pavel Shatrov on 9/7/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class MSITabularData;
@class MSICrosstabData;
@class MSIReportDataRow;

@interface DataWrapperEx : NSObject {
	MSITabularData *reportData;
	MSICrosstabData* crosstabData;
	BOOL isCrossTab;
}

- (id)init: (BOOL) isCross withTab: (MSITabularData *) tab withCross: (MSICrosstabData*) cross;

- (NSString *) getError;
- (MSIReportDataRow *) rowByIndex: (NSInteger) index;
- (NSInteger)headerCount;
- (NSInteger)rowCount;
- (NSString*)visualizationPropertyByName:(NSString*)propertyName;

@end
