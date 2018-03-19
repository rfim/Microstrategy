//
//  CalendarTile.h
//  CalendarUIPrototype
//
//  Created by Firat Alpergin on 4/5/11.
//  Copyright 2011 MicroStrategy Incorporated. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "UIHelper.h"

#define TILE_MODE_SELECTED 0
#define TILE_MODE_HIGHLIGHTED 1
#define TILE_MODE_UNSELECTED 2
#define TILE_MODE_UNAVAILABLE 3

@class MSICalendarMonthGridView;
@class MSICalendarTileViewUIHelper;

@interface MSICalendarTileView : UILabel {
	@private
	NSInteger day;
	NSInteger month;
	NSInteger year;
	NSInteger mode;
	MSICalendarMonthGridView *__weak parent;
    MSICalendarTileViewUIHelper *calendarTileViewUIHelper;
}

@property NSInteger day;
@property NSInteger month;
@property NSInteger year;
@property NSInteger mode;
@property (nonatomic, weak) MSICalendarMonthGridView *parent;

-(void)setMode:(NSInteger)newMode;
-(id)initWithFrame:(CGRect)frame day:(NSInteger)curDay month:(NSInteger)curMonth year:(NSInteger)curYear uihelper:(UIHelper*)uiHelper;
@end
