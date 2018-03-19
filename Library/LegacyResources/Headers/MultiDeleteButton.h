//
//  MultiDeleteButton.h
//  MSTRMobile
//
//  Created by Firat Alpergin on 4/13/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <UIKit/UIKit.h>

#define DELETE_BUTTON_LEFT_PADDING 10
#define DELETE_BUTTON_MIDDLE_PADDING 5
#define DELETE_BUTTON_RIGHT_PADDING 5
#define DELETE_BUTTON_TOP_PADDING 3
#define TRASH_VIEW_WIDTH 24
#define TRASH_VIEW_HEIGHT 24

@class MultiSelectTableViewController;

@interface MultiDeleteButton : UIButton {
	UILabel *titleField;
	UIImageView *imageField;
	MultiSelectTableViewController *parent;
}

-(id)initWithFrame:(CGRect)frame title:(NSString *)title;
-(void)updateTitle:(NSString *)newTitle;
-(void)setEnabled:(BOOL)isEnabled;

@property (nonatomic, assign) MultiSelectTableViewController *parent;

@end
