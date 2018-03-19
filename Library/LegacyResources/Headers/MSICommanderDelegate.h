//
//  MSMCommanderDelegate.h
//  MicroStrategyMobile
//
//  Created by Liu, Xin on 3/12/14.
//  Copyright (c) 2014 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
@class MSIURLHelper;
@class MSIReportDisplayInfo;

@protocol IViewer;

@protocol MSICommanderDelegate <NSObject>

@property (nonatomic) short vizLevel;
@property (nonatomic, readonly) BOOL widgetFitToScreen;
@property (nonatomic) BOOL isInPortraitMode;
@property (nonatomic) BOOL isReport;

- (void)setContentScaleFactorOnDemandToViews:(UIView*) iView;
- (void)unregisterViewer:(id<IViewer>)iViewer;
- (void)handleLinkDrill:(MSIURLHelper*) iUrlHelper;
- (float)getZoomScale;
- (BOOL)isFullScreen;
- (BOOL) hasNavigationTabBar;

@end
