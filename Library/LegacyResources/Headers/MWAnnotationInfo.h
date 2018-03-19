//
//  MWAnnotationInfo.h
//  MicroStrategyMobile
//
//  Created by Alpergin, Firat on 2/26/13.
//  Copyright (c) 2013 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface MWAnnotationInfo : NSObject {
    void *iterator;
    void *formatManager;
    /* These offsets need to be calculated before making a getFormat call on the FormatManager. Done only once. */
    int rowOffset;
    int colOffset;
    NSLock *lock;
}

-(id)initWithIterator:(void *)iter formatManager:(void *)formatManager rowOffset:(NSInteger)_rowOffset colOffset:(NSInteger)_colOffset;
-(NSString *)formNameAtFormIndex:(NSInteger)formIndex;
-(NSString *)metricNameAtMetricIndex:(NSInteger)metricIndex;
-(NSString *)formValueAtFormIndex:(NSInteger)formIndex rowIndex:(NSInteger)rowIndex;
-(NSString *)metricValueAtMetricIndex:(NSInteger)metricIndex rowIndex:(NSInteger)rowIndex formatted:(BOOL)isFormatted applyThreshold:(BOOL)applyThreshold;
-(NSString *)fillStyleAtMetricIndex:(NSInteger)metricIndex rowIndex:(NSInteger)rowIndex;
-(NSString *)fillColorAtMetricIndex:(NSInteger)metricIndex rowIndex:(NSInteger)rowIndex;
-(NSString *)metricBoldAtMetricIndex:(NSInteger)metricIndex rowIndex:(NSInteger)rowIndex;
-(NSString *)metricItalicAtMetricIndex:(NSInteger)metricIndex rowIndex:(NSInteger)rowIndex;
-(NSString *)metricFontSizeAtMetricIndex:(NSInteger)metricIndex rowIndex:(NSInteger)rowIndex;
-(NSString *)semanticsAtMetricIndex:(NSInteger)metricIndex rowIndex:(NSInteger)rowIndex;

@end
