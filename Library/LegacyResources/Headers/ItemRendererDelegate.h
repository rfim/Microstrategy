//
//  ItemRenderer.h
//  IPadDashboardViewer
//
//  Created by bhuang on 4/22/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@protocol ItemRendererDelegate

- (void)handleCEChartItemDoubleTouch:(UIView*)ipView;
- (void)handleCEChartItemSingleTouch:(UIView*)ipView;
- (void)handleCEChartAvgDataRecord:(double)iAvg withCount:(int)iCount withSeriesID:(int)iSeriesID;
- (void)handleTooltipRelease:(UIView*)ipView;
- (void)handleTextSingleTouch:(UIView*)ipView;
- (void)handleTextDoubleTouch:(UIView*)ipView;
- (void)handlePivotTextTouchesBegan:(UIView*)ipView;
- (void)handlePivotTextTouchesMoved:(UIView*)ipView withPoint:(CGPoint)iTouchPoint;
- (void)handlePivotTextTouchesEnded:(UIView*)ipView;
@end

