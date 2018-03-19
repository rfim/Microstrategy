//
//  CalendarGridView.h
//  CalendarUIPrototype
//
//  Created by Firat Alpergin on 4/5/11.
//  Copyright 2011 MicroStrategy Incorporated. All rights reserved.
//

#import <UIKit/UIKit.h>

@class MSICalendarTileView, MSICalendarMonthMainView;

@interface MSICalendarMonthGridView : UIView {
	@private
	//BOOL firstMoveAfterTouchesBegan;
	//MSICalendarTileView *lastTouchedTile;
	NSMutableArray *tiles;
	NSMutableArray *sharedTiles; // for iPhone, tiles shared with next month, typically last row
	MSICalendarMonthMainView *__weak parentView;
	//NSMutableArray *selectedDays;
	NSInteger month;
	NSInteger year;
}

@property (nonatomic, strong) NSMutableArray *tiles;
@property (nonatomic, strong) NSMutableArray *sharedTiles;
@property (nonatomic, weak) MSICalendarMonthMainView *parentView;

-(id)initWithFrame:(CGRect)frame month:(NSInteger)curMonth year:(NSInteger)curYear parent:(MSICalendarMonthMainView *)parent;
-(void)setTilesTextColor:(UIColor *)textColor;

@end
