//
//  DataCloudItem.h
//  DataCloudIPhone
//
//  Created by Svetlana Kopylova on 4/28/10.
//  Copyright 2010 EastBanc Technologies. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>


/**
 Represents single data cloud item which should be shown on screen
 */
@interface DataCloudItem : NSObject {
    //array of AttributeElements
    NSArray *attributeElements;
    
    //array of MSIMetric
    NSArray *additionalMetrics;
    
    NSString *metricName;
	// Stores metric value
    CGFloat  metric;
	
	int rowIndex;
	
	// Item's coordinate values
	int viewX;
	int viewY;
	// Item's size values
	int viewWidth;
	int viewHeight;
	
	int viewDescender;
	NSString *viewTitle;
    
    CGSize labelSizeInMinFont;
	
	//MSIHeaderValue *headerValue;
}

@property (nonatomic, strong) NSArray *attributeElements;
@property (nonatomic, strong) NSArray *additionalMetrics;
@property (nonatomic) CGFloat metric;
@property (nonatomic, strong) NSString *metricName;
@property (nonatomic, strong) NSString *viewTitle;
@property (nonatomic) CGSize labelSizeInMinFont;
@property int viewX;
@property int viewY;
@property int viewWidth;
@property int viewHeight;
@property int viewDescender;
@property int rowIndex;
//@property (nonatomic, retain) MSIHeaderValue *headerValue;

//display title for the item
-(NSString *) getDisplayTitle;

@end
