//
//  YearImageView.h
//  CalendarUIPrototype
//
//  Created by Firat Alpergin on 3/31/11.
//  Copyright 2011 MicroStrategy Incorporated. All rights reserved.
//

#import <UIKit/UIKit.h>

#define CIRCLE_RADIUS (([MSIGeneric getDeviceType] == DeviceTypeIPad) ? 7 : 6)
#define LINE_THICKNESS 2

@class SingleYearView;

@interface MSIYearImageView : UIView {
	@private 
	BOOL isSelected;
	BOOL isAvailable;
	SingleYearView *__weak parentView;
}

@property BOOL isSelected;
@property BOOL isAvailable;
@property (nonatomic, weak) SingleYearView *parentView;

@end
