//
//  MapPromptView.h
//  MicroStrategyMobile
//
//  Created by xi zhang on 7/14/11.
//  Copyright 2011 MicroStrategy Inc. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "MSIMWBaseView.h"

#define TOGGLE_BUTTON_PORTRAIT_OFFSET_X 110
#define UI_BAR_PORTRAIT_OFFSET_X 110
#define UI_BAR_PORTRAIT_OFFSET_Y 212

@protocol MSIMapPromptViewDelegate <MapWidgetBaseViewerDelegate>
-(void)selectionsMade:(NSMutableArray *)annotationList;
@end

@interface MapPromptView : MSIMWBaseView {
    UISegmentedControl* modeToggleButton;
@private	
	id<MSIMapPromptViewDelegate> __weak promptDelegate;	
}

-(void)updateOnOrientationChange;

@property(nonatomic, weak) id<MSIMapPromptViewDelegate> promptDelegate;

@end
