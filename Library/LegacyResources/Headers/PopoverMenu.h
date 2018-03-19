//
//  PopoverMenu.h
//  MicroStrategyMobile
//
//  Created by Yongli Jiang on 1/5/12.
//  Copyright (c) 2012 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

extern NSString* const MSIPopoverMenuDidDismissNotification;

/**
 * Popover Menu Style
 * iPad will use the Popover style
 * iPhone will use the action sheet style
 */
typedef enum EnumPopoverMenuStyle {
	MenuStylePopover,
	MenuStyleActionSheet,
} PopoverMenuStyle;

@class PopoverMenu;

@protocol PopoverMenuDelegate <NSObject>

- (void)popoverMenuDidDismissPopover:(PopoverMenu *)popoverMenu;
- (void)menuItemClicked:(PopoverMenu *)popoverMenu index:(NSInteger)index;

@end

@class PopoverMenuItem;
@class PopoverMenuController;

@interface PopoverMenu : NSObject {
@protected
	NSMutableArray *menuItems;
	PopoverMenuStyle style;
@private
	UIPopoverController *popOver;
    UIViewController* contentsViewController;
	UIActionSheet *actionSheet;
	BOOL isPopoverVisible;
	id<PopoverMenuDelegate> __weak delegate;
	Class popoverMenuControllerClass;
}

@property (nonatomic, strong) NSMutableArray *menuItems;
@property (nonatomic, weak) id<PopoverMenuDelegate> delegate;
@property (nonatomic, readonly) PopoverMenuController *popoverMenuController;
@property (readonly) BOOL isPopoverVisible;

- (id)initWithPopoverMenuControllerClass:(Class)controllerClass;
- (void)addMenuItem:(PopoverMenuItem *)menuItem atIndex:(NSInteger)index;
- (void)appendMenuItem:(PopoverMenuItem *)menuItem;
- (void)showFromBarButtonItem:(UIBarButtonItem *)item permittedArrowDirections:(UIPopoverArrowDirection)direction animated:(BOOL)animated;
- (void)showFromRect:(CGRect)rect inView:(UIView *)view permittedArrowDirections:(UIPopoverArrowDirection)direction animated:(BOOL)animated;
- (void)repromptFromBarButtonItem:(UIBarButtonItem *)item permittedArrowDirections:(UIPopoverArrowDirection)direction animated:(BOOL)animated;
- (void)repromptFromRect:(CGRect)rect inView:(UIView *)view permittedArrowDirections:(UIPopoverArrowDirection)direction animated:(BOOL)animated;
- (void)dismissPopoverMenu;
- (void)adjustStartingBarButtonItem:(UIBarButtonItem *)item;
@end
