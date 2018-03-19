//
//  TooltipViewController.h
//  DataCloudIPhone
//
//  Created by Anton Alexeev on 5/4/10.
//  Copyright 2010 EastBanc Technologies. All rights reserved.
//

#import <UIKit/UIKit.h>

@class DataCloudTooltipView;
@class DataCloudTooltipVisualizationSettings;

@interface DataCloudTooltipViewController : UIViewController {
	UIView *__unsafe_unretained superView;
	DataCloudTooltipVisualizationSettings *visualizationSettings;	
}

@property (nonatomic, strong) DataCloudTooltipVisualizationSettings *visualizationSettings;
@property (nonatomic, unsafe_unretained) UIView *superView;

- (void) show:(NSArray *) text withAdditional:(NSArray *) additional aroundRect:(CGRect) rect withStrokeColor:(UIColor *) strokeColor touchPoint:(CGPoint) point;
- (void) hide;


@end
