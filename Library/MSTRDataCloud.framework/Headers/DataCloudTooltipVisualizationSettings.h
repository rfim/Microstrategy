//
//  TooltipVisualizationSetting.h
//  DataCloudIPhone
//
//  Created by Anton Alexeyev on 4/30/10.
//  Copyright 2010 EastBanc Technologies. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>


@interface DataCloudTooltipVisualizationSettings : NSObject {
	UIFont  * font;
	UIFont  * fontBold;
	int fontSize;
	UIColor * fontColor;
	UIColor * fontColorOfMetrics;
	UIColor * backgroundColor;
	CGFloat backgroundAlpha;
	UIColor * strokeColor;
	CGFloat strokeThickness;
}

@property(nonatomic, strong) UIFont * font;
@property(nonatomic, strong) UIFont * fontBold;
@property int fontSize;
@property(nonatomic, strong) UIColor * fontColor;
@property(nonatomic, strong) UIColor * fontColorOfMetrics;
@property(nonatomic, strong) UIColor * backgroundColor;
@property CGFloat backgroundAlpha;
@property(nonatomic, strong) UIColor  * strokeColor;
@property CGFloat strokeThickness;

- (id) init;

@end
