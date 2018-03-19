//
//  DataRowProtocol.h
//  MicroStrategyMobile
//
//  Created by kyang on 3/9/11.
//  Copyright 2011 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>


@class MSIHeaderValue, MSIReportDataCell, MSIHeader;

@protocol MSIDataRowProtocol<NSObject>
/**returns cell at specified index
 NOTE: each attribute form is represented by its own cell
 */
- (MSIReportDataCell*)cellAt:(NSInteger)index;

/**
 returns the MSIReportDataCell for specified header
 */
- (MSIReportDataCell*)cellForHeader:(MSIHeader*)header;
/**
 returns the attribute form cells num in the row
 */
@property (nonatomic, readonly) NSInteger formCount;
/**
 returns the metric cell num in the row
 */
@property (nonatomic, readonly) NSInteger metricCount;

/**
 returns total number of cells in the row
 */
- (NSUInteger)cellCount;

/**
 returns the MSIHeaderValue for specified index
 */
- (MSIHeaderValue*)headerValueForType:(NSInteger)type atIndex:(NSInteger)index;

/**
 returns the name of an attribute form in specified index
 */
- (NSString*)formNameAtIndex:(NSInteger)index;

/**
 returns the value of an attribute form in specified index
 */
- (NSString*)formValueAtIndex:(NSInteger)index;

/**
 returns the name of an metric in specified index, index starts from the first metrics in the row
 */
- (NSString*)metricNameAtIndex:(NSInteger)index;

/**
 returns the value of an metric in specified index, index starts from the first metrics in the row
 */
- (NSString*)metricValueAtIndex:(NSInteger)index;

/**
 returns the raw value of an metric in specified index, index starts from the first metrics in the row
 */
- (NSString*)metricRawValueAtIndex:(NSInteger)index;

/**
 returns the color of an metric in specified index, index starts from the first metrics in the row
 */
- (UIColor*)metricColorAtIndex:(NSInteger)index;

- (NSInteger)semanticsAtMetricIndex:(NSInteger)index;

/**
 Returns YES if this row represents a subtotal
 */
- (BOOL)isSubtotalRow;

/**
 Returns YES if the cell with the given index represents a subtotal
 */
- (BOOL)isSubtotalCellAtIndex:(NSInteger)index;

@end
