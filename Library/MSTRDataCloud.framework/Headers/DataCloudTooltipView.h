//
//  TooltipView.h
//  DataCloudIPhone
//
//  Created by Anton Alexeev on 5/5/10.
//  Copyright 2010 EastBanc Technologies. All rights reserved.
//

#import <UIKit/UIKit.h>

@class DataCloudTooltipVisualizationSettings;

#define MAX_WIDTH 350

@interface DataCloudTooltipView : UIView {
	NSArray	*text;
	NSArray	*additional;
	DataCloudTooltipVisualizationSettings *visualizationSettings;
	UIColor *customStrokeColor;
}

@property (nonatomic, strong) NSArray *text;
@property (nonatomic, strong) NSArray *additional;
@property (nonatomic, strong) DataCloudTooltipVisualizationSettings *visualizationSettings;
@property (nonatomic, strong) UIColor *customStrokeColor;

-(CGSize) getExpectedTextSize;
-(CGSize) getArrayTextSize:(NSArray *) array;
-(CGFloat) drawArrayText:(NSArray *) array fromY:(CGFloat) textY;

@end
