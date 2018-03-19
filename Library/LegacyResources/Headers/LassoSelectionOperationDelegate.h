//
//  LassoSelectionOperationDelegate.h
//  MicroStrategyMobile
//
//  Created by Zhu, Wei on 4/25/12.
//  Copyright (c) 2012 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol LassoSelectionOperationDelegate <NSObject>

- (void) clearLassoSelection;
@optional
- (void) popupForKeepOnlyAndShow:(id)sender;
- (int) getHighlightObjectsCount;
- (int) readSelectTargetCount;
- (void) clearAllViewFilter;
- (void) drillForLassoSelection;
- (void) dismissTooltipWindow;
- (void) performKeepOrExclude:(BOOL)mode;
- (void) dismissActionMenu;
@end
