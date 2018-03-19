//
//  MSICalendarArrowView.h
//  RingWidget
//
//  Created by Firat Alpergin on 1/26/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <UIKit/UIKit.h>

#define ARROW_DRAWING_WIDTH 12
#define ARROW_DRAWING_HEIGHT 16
#define ARROW_SIDE_PADDING 15

@class MSICalendarTopView;

@interface MSICalendarArrowView : UIView {
	MSICalendarTopView *__weak parent;
	BOOL isRightArrow;
}

@property BOOL isRightArrow;
@property(nonatomic, weak) MSICalendarTopView *parent;

@end
