//
//  DataProviderProtocol.h
//  MicroStrategyMobile
//
//  Created by kyang on 3/5/11.
//  Copyright 2011 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@class MSIPropertyGroup;
@class MSIHeader;
@class MSIMetricValue;
@class MSIDataElement;

@protocol MSIDataProviderProtocol<NSObject>
@optional
- (MSIPropertyGroup*) getCellPropertyGroupAtRow:(NSInteger)rowIndex atColumn:(NSInteger)colIndex;
- (MSIPropertyGroup*) getPropertyGroupForHeader:(MSIHeader*)header;
- (NSMutableArray*) getHeaderElements:(MSIHeader*)iHeader;
- (NSString*) getFormattedMetricValue:(MSIMetricValue*)iMetricValue;
- (NSString*) getDataElementID:(MSIDataElement*)iDataElement;
/**
 Return the widget property by name.
 */
- (NSString*)visualizationPropertyByName:(NSString*)propertyName;
/**
 Return the error message if there is any. If there is no error, this should return nil. 
 */
- (NSString*) error;
@end
