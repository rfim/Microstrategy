//
//  Metric.h
//  MSTRMobile
//
//  Created by Amirali Charania on 2/18/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSIDisplayInfo.h"

@class MSTRFormatHelper;

@interface MSIMetric : MSIDisplayInfo {

}

- (void) setFormatForCategory: (NSInteger)cat cp:(NSInteger)cp ts:(NSInteger)ts nn:(NSInteger)nn dp:(NSInteger)dp cs:(NSString*)cs format:(NSString*)format;
- (NSString*) formattedValue:(NSNumber*)number;

@property (nonatomic) NSInteger category;
@property (nonatomic) NSInteger currencyPosition;
@property (nonatomic) NSInteger thousandSeparator;
@property (nonatomic) NSInteger negativeNumbers;
@property (nonatomic) NSInteger decimalPlaces;
@property (nonatomic, copy) NSString* currencySymbol;
@property (nonatomic, copy) NSString* metricFormat;
@property (nonatomic, copy) NSString* controlPropertyFormatString;
@property NSInteger metricIndex;

@end
