//
//  MIPromptDetailsViewController.h
//  MicroStrategyMobile
//
//  Created by Jiao, Xiantao on 7/7/11.
//  Copyright 2011 MicroStrategy Inc. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "PromptSelectorController.h"
#import "Enums.h"
#import "MIPromptDetailsViewControllerUIHelper.h"

typedef enum _EnumPromptDetailsStatus {
	PromptDetailsStatusSaveAnswer = 0,
	PromptDetailsStatusApplyAnswer,
	PromptDetailsStatusCancel
} EnumPromptDetailsStatus;

@protocol MIIPromptDetailsViewControllerDelegate <NSObject>

- (void)PromptDetailsViewWillAppear;
- (void)PromptDetailsViewDidAppear;
- (void)PromptDetailsViewDidFinishWithStatus:(EnumPromptDetailsStatus)status fromSender:(id)sender;
- (void)PromptDetailsViewDidPopViews;
- (void)PromptDetailsViewDismissPopViews;
@end

@class MIBucketViewController;
@class BucketElementCountImageView;
@class PromptHelper;
@interface MIPromptDetailsViewController : UIViewController <PromptSelectorObserver,UITableViewDataSource, UITableViewDelegate> {
	NSMutableArray *selectedElements; // allocated or retained in subclass, released in this class
	MIBucketViewController *bucketViewController;
	PromptSelectorController *__weak currentSelectorController;
	BucketElementCountImageView *countImageView;
	UIViewController *__weak topViewController;
	id<MIIPromptDetailsViewControllerDelegate> __weak delegate;
	BOOL isAutoDisplayDetails;
    ThemeType themetype;
    BOOL multiSelect;
	MIPromptDetailsViewControllerUIHelper* detailsViewControllerUIHelper;
    PromptHelper* helper;
@private
    UITableView* selectedTableView;
    UISegmentedControl *mSearchControl;
    UIView* baseView;
    
    
    BOOL showClearAll;
}

- (void)display; // subclass needs to call this method in superclass if overrided
- (void)done;
- (NSMutableArray *)getSelectedElements;
- (void)displayInternal; /// internal details
- (id)initWithUIHelper:(UIHelper*)uiHelper;
@property (nonatomic, assign) BOOL multiSelect;
@property (nonatomic, weak) PromptSelectorController *currentSelectorController;
@property (nonatomic, weak) UIViewController *topViewController;
@property (nonatomic, weak) id<MIIPromptDetailsViewControllerDelegate> delegate;
@property (nonatomic, assign) BOOL isAutoDisplayDetails;
@property (nonatomic) ThemeType themetype;
@property (nonatomic, strong) MIPromptDetailsViewControllerUIHelper* detailsViewControllerUIHelper;
@property (nonatomic, strong) UITableView* selectedTableView;
@property (nonatomic, strong) UISegmentedControl *mSearchControl;
@property (nonatomic, strong) UIView* baseView;
@property (nonatomic, assign) BOOL showClearAll;
@end

@interface MIPromptDetailsViewController (Protected)

- (void)createElementListFromBucketElements:(NSArray *)elements; // needs to be overrided
- (void)initializeBucketViewController; // subclass can override this method to initialize
- (void)addElementsToBucket; // subclass need to call this method in superclass if overrided
- (void)bucketCountUpdated:(NSInteger)count;
//- (void)toggleClear; // subclass can override this method to handle clear all
- (void)toggleDone; // subclass need to call this method in superclass if overrided
- (void)toggleDone:(id)sender; // the method is defined for bar button item with functionality of "Done", subclass may want to call this with "nil" when there is no real button
- (void)toggleApply:(id)sender; // the method is defined for bar button item with functionality of "Apply", subclass may want to call this with "nil" when there is no real button
- (void)toggleCancel:(id)sender; // the method is defined for bar button item with functionality of "Cancel", subclass may want to call this with "nil" when there is no real button
//- (void)didToggleBucket; // subclass can ovverride this method
//- (void)didToggleDetailsView; // subclass can ovverride this method
- (void)deviceOrientationDidChange:(NSNotification *)notification; // subclass need to override this class to handle things after device changes orientation
- (void)enableButtons:(BOOL)enable;
- (void)initSelectedTableView;
- (void)initSegmentedControl;
- (NSString *)getSelectedNumberString;
@end