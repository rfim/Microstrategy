//
//  MIIDynamicTableDataItem.h
//  MicroStrategyMobile
//
//  Created by Victor Pena on 5/4/11.
//  Copyright 2011 MicroStrategy Inc. All rights reserved.
//
#import "MIDisplayStyle.h"

@class MetricExpression;
@class Commander;

@protocol MIIDynamicTableDataItemDelegate

- (void)handleSelectorDataFetched;
- (void)handleDisplayStylesChanged;

@end

@protocol MIIDynamicTableDataItem
- (MIDisplayStyle)getStyle;
/**
 getTitle - returns title property string.
 */
- (NSString*)getTitle;

/**
 getDetailedText - returns detailed text property string.
 */
- (NSString*)getDetailedText;

/**
 getImageName - returns image name property string.
 */
- (NSString*)getImageName;

@optional

- (void)setDynamicTableDataItemDelegate:(id<MIIDynamicTableDataItemDelegate>)iDelegate;

- (id<MIIDynamicTableDataItemDelegate>)getDynamicTableDataItemDelegate;

- (MetricExpression*)getMetricExpression;

- (double)getMetricValueMin;

- (double)getMetricValueMax;

- (double)getMetricValueCount;

- (BOOL)isMetricValueSupported;

- (BOOL)isSelectorModeInclude;

- (NSArray*)getDisplayStylesGroup;

- (void)setDisplayStylesGroup:(NSArray*)displayStyles;

- (void)setOrderWithIndex:(NSInteger)index;

// Manipulations

- (Commander*)getCommander;

- (BOOL)isExecuteImmediately;

- (void)execute;

- (void)prehandleMetricSelection:(MetricExpression*)metricExp;

- (void)handleMetricSelection:(MetricExpression*)metricExp;

- (void)prehandleSetPropertyWithName:(NSString*)propertyName propertySetName:(NSString*)propertySetName propertyValue:(void*)propertyValue;

- (void)handleSetPropertyWithName:(NSString*)propertyName propertySetName:(NSString*)propertySetName propertyValue:(void*)propertyValue;

- (void)prehandleToggleIncludeMetric:(BOOL)includeMode;

- (void)handleToggleIncludeMetric:(BOOL)includeMode;

- (void)prehandleDelete;

- (void)handleDelete;

@end
