//
//  CalendarTile.h
//  RingWidget
//
//  Created by Firat Alpergin on 1/27/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <UIKit/UIKit.h>

#define CURRENT_MONTH 1
#define PREVIOUS_MONTH 2
#define NEXT_MONTH 3

@class MSICalendarElement;

@interface MSICalendarTile : UILabel {
	NSInteger day;
	NSInteger month;
	NSInteger year;
	NSInteger index; //index within the grid tile list
	UIColor *selectedTextColor;
	UIColor *unselectedTextColor;
	UIColor *selectedBackgroundColor;
	UIColor *unselectedBackgroundColor;
	
	BOOL isSelected;
	BOOL isHighlighted;
	BOOL isAvailable;
	NSInteger monthBelonged;
}

@property NSInteger day;
@property NSInteger month;
@property NSInteger year;
@property(nonatomic, strong) UIColor *selectedTextColor;
@property(nonatomic, strong) UIColor *unselectedTextColor;
@property(nonatomic, strong) UIColor *selectedBackgroundColor;
@property(nonatomic, strong) UIColor *unselectedBackgroundColor;
@property BOOL isSelected;
@property BOOL isHighlighted;
@property BOOL isAvailable;
@property NSInteger monthBelonged;
@property NSInteger index;

-(void)setSelected:(BOOL)selected;
-(void)setHighlighted:(BOOL)highlighted;
-(id)initWithFrame:(CGRect)frame withBackgroundColor:(UIColor *)color;

@end
