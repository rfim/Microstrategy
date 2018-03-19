//
//  MetricHeader.h
//  MSTRMobile
//
//  Created by Amirali Charania on 2/19/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Header.h"

@class MSIMetric, MSIMetricValue;

/**
 MSIHeader for a metric.
 */
@interface MSIMetricHeader : MSIHeader {

}

/**
 MSIMetric object
*/
@property (nonatomic, strong, readonly) MSIMetric* metric;

///**
// Returns a metric header value for a category index (row) and series index (column).
//*/
//- (MSIMetricValue*) getDataForCategoryIndex: (NSInteger) categoryIndex seriesIndex:(NSInteger) seriesIndex;

//- (MSIMetricValue*) dataForIndex:(NSUInteger)index;


///**
// Calculates the maximum value for this metric in the report data.
//*/
//- (MSIMetricValue*) maximumMetricValue;
//
///**
// Calculates the minimum value for this metric in the report data.
// */
//- (MSIMetricValue*) minimumMetricValue;

/**
 Calculates the maximum value for this metric in one series of the report data.
*/
//- (MSIMetricValue*) maximumMetricValue:(NSInteger) seriesIndex;

/**
 Calculates the minimum value for this metric in one series of the report data.
*/
//- (MSIMetricValue*) minimumMetricValue:(NSInteger) seriesIndex;

@end
