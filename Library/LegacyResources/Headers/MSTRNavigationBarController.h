//
//  MSTRNavigationBarController.h
//  MSTRMobile
//
//  Created by Yonggang Xi on 9/18/13.
//  Copyright 2013 MicroStrategy. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Enums.h"

@protocol MSTRNavigationBarButtonItemsFactory <NSObject>

/**
 * This method should return a UIBarButtonItem array for navigation bar left bar buttons.
 */
- (NSArray *)leftBarButtonItems;

/**
 * This method should return a UIBarButtonItem array for navigation bar right bar buttons.
 */
- (NSArray *)rightBarButtonItems;

/**
 * This method should return a MSTRSharedBarDataItem array for shared bar presented from share button.
 */
- (NSArray *)sharedBarDataItems;

/**
 * This method should return a UIBarButtonItem - share button for presenting shared bar.
 */
- (UIBarButtonItem *)shareButtonItem;

@end

typedef enum MSTRNavigationSharedBarStyle {
	MSTRNavigationSharedBarStyleActionSheet,
	MSTRNavigationSharedBarStylePopoverMenu,
	MSTRNavigationSharedBarStyleFloatingBar
} MSTRNavigationSharedBarStyle;

/**
 * This class is designed for all UIViewControllers which need to show a navigation bar at top.
 * The navigation bar is required (This class won't create a new navigation bar but manage the existing one).
 * This class will support multiple barButtonItems, and won't consider the device prior to iOS 5.
 */

@interface MSTRNavigationBarController : NSObject

@property (nonatomic, weak) UINavigationBar *navigationBar; // The navigation bar managed by the controller. The class will set and handle navigation items on the managed navigation bar.
@property (nonatomic, weak) UINavigationItem *navigationItem; // The navigation item managed by the controller. The class may customize the navigation appearance.
@property (nonatomic, weak) id<MSTRNavigationBarButtonItemsFactory> itemsFactory;
@property (nonatomic, weak) UIBarButtonItem *shareButtonItem; // The share button item is currently presenting shared bar. If this item is the same as last time presented, current presenting will be skipped. This item should be set to nil if outside already dismissed shared bar. // TQMS: 824316

@property (nonatomic) BOOL hidesBackButton; // If YES, the navigation item managed by the controller will hide the back button when it's on top of the stack.
- (void)setHidesBackButton:(BOOL)hidesBackButton;

@property (nonatomic) BOOL hidesLeftBarButtonItems; // If YES, the navigation item managed by the controller will hide the left bar button items when it's on top of the stack.
- (void)setHidesLeftBarButtonItems:(BOOL)hidesLeftBarButtonItems;

@property (nonatomic) BOOL hidesRightBarButtonItems; // If YES, the navigation item managed by the controller will hide the right bar button items when it's on top of the stack.
- (void)setHidesRightBarButtonItems:(BOOL)hidesRightBarButtonItems;

/**
 * This method is a suggested init method with a given navigation bar, navigation item and items factory.
 * @param navigationBar is the managed navigation bar.
 * @param navigationItem is the managed navigation item for view controller.
 * @param itemsFactory is the bar button items creator.
 */
- (id)initWithNavigationBar:(UINavigationBar *)navigationBar navigationItem:(UINavigationItem *)navigationItem itemsFactory:(id<MSTRNavigationBarButtonItemsFactory>)itemsFactory;

/**
 * This method will setup managed navigation bar with given theme and will add a dividing line at the bottom of navigation bar.
 * @param navigationBar is the managed navigation bar.
 * @param theme is the theme for navigation bar.
 */
- (void)setupNavigationBar:(UINavigationBar *)navigationBar withThemeType:(ThemeType)themeType;

/**
 * This method will setup managed navigation bar with given theme.
 * @param navigationBar is the managed navigation bar.
 * @param theme is the theme for navigation bar.
 */
- (void)setupNavigationBarNoDividingLine:(UINavigationBar *)navigationBar withThemeType:(ThemeType)themeType;

/**
 * This method will refresh managed navigation item left bar button items.
 */
- (void)refreshNavigationItemLeftBarButtonItems;

/**
 * This method will refresh managed navigation item right bar button items.
 */
- (void)refreshNavigationItemRightBarButtonItems;

/**
 * This method will present shared bar from share button with given style.
 * @param style is the presenting MSTRNavigationSharedBarStyle.
 * @param animated indicates the animation.
 */
- (void)presentSharedBarFromShareButtonWithStyle:(MSTRNavigationSharedBarStyle)style animated:(BOOL)animated;

/**
 * This method will dismiss shared bar.
 * @param animated indicates the animation.
 */
- (void)dismissSharedBarAnimated:(BOOL)animated;

/**
 * This method will return if shared bar is visible.
 * @return YES if shared bar is visible.
 */
- (BOOL)isSharedBarVisible;

/**
 * This method will reset the shared bar and share button to ensure the proper behavior.
 * The method should be called when controller start to handle share button action.
 */
- (void)resetSharedBarAndShareButton;

/**
 * This method will save the current StatusBarStyle locally. DE20108, add by wenzhou, 11/10/2015.
 * Attention! This method should be called before setupNavigationBar:withThemeType:.
 */
- (void) saveCurrentStatusBarStyle;
/**
 * This method will restore the previous StatusBarStyle. DE20108, add by wenzhou, 11/10/2015.
 * Attention! This method is paired with saveCurrentStatusBarStyle, and should be called after saveCurrentStatusBarStyle has been invoked and  before dismissController or hide controller.
 */
- (void) restorePreviousStatusBarStyle;

@end
