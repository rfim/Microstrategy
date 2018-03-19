//
//  SingleYearView.h
//  CalendarUIPrototype
//
//  Created by Firat Alpergin on 3/31/11.
//  Copyright 2011 MicroStrategy Incorporated. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "UIHelper.h"
#define TOP_OFFSET 5
#define YEAR_IMAGE_VIEW_HEIGHT 16

@class MSIYearStripView, MSIYearImageView;
@class SingleYearViewUIHelper;

@interface SingleYearView : UIView {
	@private
	NSInteger year;
	BOOL isSelected;
	BOOL isAvailable;
	MSIYearStripView *__weak parentView;
	MSIYearImageView *imageView;
	UILabel *yearLabel;
    SingleYearViewUIHelper* singleYearViewUIHelper;
}

@property NSInteger year;
@property (nonatomic, weak) MSIYearStripView *parentView;

- (id)initWithFrame:(CGRect)frame year:(NSInteger)yearValue selected:(BOOL)selected available:(BOOL)available;
- (void)setSelected:(BOOL)selected;
- (id)initWithFrame:(CGRect)frame year:(NSInteger)yearValue selected:(BOOL)selected available:(BOOL)available uihelper:(UIHelper*)uiHelper;
@end
