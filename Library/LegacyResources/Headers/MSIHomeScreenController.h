//
//  HomeScreenController.h
//  MSTRMobile
//
//  Created by Amirali Charania on 4/9/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <UIKit/UIKit.h>

@class MSIHomeScreenBackgroundView, MSICustomHomeScreenSettings, MSIHomeScreenController,
MSIHomeScreenButton, DisplayObject;

@protocol MSIHomeItemViewControllerDelegate

- (UIView*)loadingView:(CGRect)frame;
- (UIViewController*)viewController;
- (UIToolbar*)toolbar;
- (void)updateViewForCache;
- (void)setHomeButton:(UIBarButtonItem*)homeButton;
- (void)setDontRender:(BOOL)dontRender;

@optional
//this method is called when you click on the homeButton
- (void)homeButtonAction;

@end


@class PreloadProgressContainerView;

@interface MSIHomeScreenController : UIViewController {
	MSICustomHomeScreenSettings * customHomeScreenSettings;
	MSIHomeScreenBackgroundView* homeScreenBackgroundView;
	UIScrollView* homeScreenScrollView;
	UIView* homeScreenItemsContentView;
	id<MSIHomeItemViewControllerDelegate> homeItemViewControllerDelegate;
	UIImageView* homeScreenItemImageView;
	UIBarButtonItem* homeButtonItem;
	BOOL reload;
	NSInteger layoutMode;
	UIPageControl* homeScreenPageControl;
	BOOL pageControlUsed;
	NSMutableArray* homeScreenButtons;
	BOOL itemActionInProgress;
	BOOL shouldReconcileAfterAppear;
    PreloadProgressContainerView* mpPreloadProgressContainerView;
}

- (id)initWithHomeScreenSettings:(MSICustomHomeScreenSettings *)_customHomeScreenSettings;
- (void)displayHomeButton;
- (UIBarButtonItem*) newHomeBarButtonItem;

+ (NSString*)reportsListTitle;
+ (NSString*)sharedLibraryTitle;
+ (NSString*)helpTitle;
+ (UIImage*)defaultImageByButton:(MSIHomeScreenButton *)button;

@property (nonatomic, strong) id<MSIHomeItemViewControllerDelegate> homeItemViewControllerDelegate;
@property (nonatomic, strong) UIImageView* homeScreenItemImageView;
@property BOOL shouldReconcileAfterAppear;

@end
