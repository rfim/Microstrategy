//
//  MIPromptViewController.h
//  MicroStrategyMobile
//
//  Created by Jiao, Xiantao on 6/29/11.
//  Copyright 2011 MicroStrategy Inc. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Enums.h"
#import "MISplitViewController.h"

typedef void (^MIPromptViewControllerHideSummaryViewCompletionBlock)(void);

@class MIPromptDetailsViewController;
@class MIPromptSummaryScreen;
@class PromptHelper;
@class Prompts;

@interface MIPromptViewController : MISplitViewController {
@private
    UIViewController* __weak mpBackgroundViewController;
    MIPromptSummaryScreen* mpPromptSummaryController;
    MIPromptDetailsViewController* mpPromptDetailsViewController;
    BOOL mIsAutoDisplayDetailsView;
    BOOL mIsDetailsViewAsMasterPanel;
    BOOL mIsStandaloneNavigationBar;//793295
    UIView* mpContainerView;
    UIView* mpCoveringView;
    UIView* mpNavigationBarCoveringView;
    UIView* mpRightViewBackgroundCoveringView;
    UIView* mpSeparateLineView;
    UIImageView* mpNavigationBarRightToolbarImageView;
    CGFloat mNavigationBarRightToolbarRightOffset;
    bool mViewFirstAppear;
    NSMutableArray* mpChildNavigationBars;
    UINavigationBar* mpLeftNavigationBar;
    UINavigationBar* mpRightNavigationBar;
    UIImageView* mpLeftNavigationBarImageView;
    CGRect mpLeftNavigationBarFrameToAppear;
    CGRect mpRightNavigationBarFrameToAppear;
    ThemeType mThemeType;
}

- (id) initWithBackgroundViewController:(UIViewController*)backgroundViewController Prompts:(Prompts*)prompts
    promptHelper:(PromptHelper*)promptHelper;

- (void) hidePromptSummaryViewWithCompletionHandler:(MIPromptViewControllerHideSummaryViewCompletionBlock)handler;
- (void) showPromptDetailsView:(UIViewController*)rightViewController animated:(BOOL)animated;
- (void) hidePromptDetailsViewAnimated:(BOOL)animated;
- (void) removeChildNavigationBars;
- (BOOL) isPromptViewAppear;
- (BOOL) isPromptDetailsViewAppear;
- (MIPromptDetailsViewController*)detailsViewController;
- (MIPromptSummaryScreen*)getPromptSummaryController;
@property (nonatomic, assign, getter = isAutoDisplayDetailsView) BOOL autoDisplayDetailsView;
@property (nonatomic, assign, getter = isDetailsViewAsMasterPanel) BOOL detailsViewAsMasterPanel;
@property (nonatomic, assign, getter = isStandaloneNavigationBar) BOOL standaloneNavigationBar;//793295

@property (nonatomic) ThemeType mThemeType;
@end
