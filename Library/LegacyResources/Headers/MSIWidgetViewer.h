//
//  WidgetViewer.h
//  IPadDashboardViewer
//
//  Created by bhuang on 4/22/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "IViewer.h"
#import "ItemRendererDelegate.h"
#import "Enums.h"
#import "MSIDataViewer.h"
#import "VisualizationViewerProtocol.h"
#import "TemporaryCanvasViewForLassoSelectionDelegate.h"
#import "LassoSelectionOperationDelegate.h"
#import "AnnotationContext.h"
#import "WidgetPropManipulationProtocal.h"
#import "MSICommanderDelegate.h"
//#import "WidgetPropViewer.h"

//#import "TemporaryCanvasViewForLassoSelection.h"
//#import "ActionMenuForLassoSelection.h"
//#import "TooltipViewForLassoSelection.h"
//#import "MSTRPopover.h"
/**
 Base class for all MSIWidgetViewer classes.
 Put the commonly used functions here.
 Use Composite so that subclass could decide whether to use scrollview or not for themselves.
 */
@class DetachHelper, MSIWidgetHelper, PanelStackViewer, GridGraphViewer;
@class GradientViewer;
@class WaitView;
@class WidgetPropertiesStore;
@class MSIReportDisplayInfo;

#define ITEM_NOT_SELECTED -2
#define ITEM_NOT_SELECTABLE -3

@class ActionMenuForLassoSelection;
@class TemporaryCanvasViewForLassoSelection;
//yuazhang;823413;use separate file to define the protocol to fix warning
//@protocol  WidgetPropManipulationProtocal;

@interface MSIWidgetViewer : UIView <IWidgetProtocol, ItemRendererDelegate, WidgetGridSwitchProtocol,  VisualizationViewerProtocol, TemporaryCanvasViewForLassoSelectionDelegate, LassoSelectionOperationDelegate, WidgetPropManipulationProtocal, BinaryRetrievalHandler, UIActionSheetDelegate> {
	
	MSIWidgetHelper *widgetHelper;
	
	ThemeType mThemeType;
	
	//the pointer to the grid which will be used for quick switch
	GridGraphViewer* mpGridGraphViewer;
	
	NSMutableArray* mTaskQueue;
	
	bool mIsChartAvailable;
	
	BOOL mIsLayoutOrPanelChanged;
	
	BOOL mIsInValidTemplate;
    
    BOOL mbInWidgetMode; // boolean, YES: widget NO: widget is switched to grid
	
	NSString* mAccessibilityLabel;
    UILabel* mErrorLabel;
    WidgetPropertiesStore* mPropertiesStore;
    BOOL mStateSaved;
    BOOL mbDataModelChallenged;
    WaitView *mpWaitView;
    ViewerDataModel* viewerDataModel;
    
    BOOL mIsMtpMode;//indicating whether in multiSelection mode
    BOOL mIsLassoMode;
    UIView* mTemporyCanvasView;
    //TooltipViewForLassoSelection* mTooltipView;
    ActionMenuForLassoSelection* mActionMenu;
    CGSize mWidgetSizeForLastActionMenu;
    BOOL mIsVIDocument;
    UIView* mpDrillView;
    NSString* mDrillLabel;
    NSInteger mSelectedNumLasso;
    
    NSData* mRestoreData;
}
@property (nonatomic, strong) NSString* drillLabel;
@property (nonatomic, readonly) BOOL isVIDocument;
@property (nonatomic, strong) UIView* lassoCanvasView;
@property (nonatomic, assign) BOOL isMtpMode;
@property (nonatomic, assign) BOOL isLassoMode;
@property (nonatomic, strong) MSIWidgetHelper *widgetHelper;
@property (nonatomic, assign) bool mIsChartAvailable;

@property (nonatomic, strong) NSString* mAccessibilityLabel;
@property (nonatomic, strong) UILabel* errorLabel;
@property (nonatomic, strong) ViewerDataModel* viewerDataModel;
@property (nonatomic, strong) WidgetPropertiesStore* mPropertiesStore;
@property (nonatomic, assign) NSInteger mSelectedNumLasso;
@property (nonatomic, weak, readonly) MSIReportDisplayInfo* document;
@property (nonatomic, weak)  id<MSICommanderDelegate> commanderDelegate;
@property (nonatomic) BOOL ignoreReleaseDataModelMemory;
@property (nonatomic) BOOL delayReleaseDataModelMemory;

- (id)initViewer:(ViewerDataModel*)_viewerDataModel withCommanderDelegate:(id<MSICommanderDelegate>)_commander withProps:(NSString*)_props;

//Deprecated, customized widgets should not use this method anymore, instead, it should use the method above, this method will be removed in next release
- (id)initViewer:(ViewerDataModel*)_viewerDataModel withCommander:(Commander*)_commander withProps:(NSString*)_props;

- (void)setSelfAsDelegateForView:(UIView*)ipView;

// this method is used to clear highlight
//or something after info window is dismissed
//if you need, please overwrite it
-(void) handleInfoWindowDismissed:(NSObject*)lpObj;
- (void)reinit:(ViewerDataModel*)_viewerDataModel;
- (void*)getRWGridIterator;


-(NSString*)getPrefixForAccLabel:(UIView*)renderer;

-(void) formatGradientView:(GradientViewer*)gradientViewer;

// This method is used to tell rootviewercontainer not cancel
// the touch events sent to widget, usually because tooltip is
// displaying.
-(BOOL) canCancelTouches;
//Clear warning by Nan Jiang, 08/03/2012
- (void)updateFrame:(CGRect)frame;
- (void)dismissTooltips;
//add by cpan,3/25/2013 ,used in each widget's  "handleEvent" function.
- (BOOL) shouldHandleEvent:(NSString*)ipEventName;
+ (UINavigationController*) getNavigationController;
// Used to change the visualization to the given frame and update content if needed.
- (void) updateVisualizationFrame:(CGRect)iFrame;
// Get the title of the visualization. LayoutManagerViewer needs the string to show in TitleBarViewer.
- (NSString*) getVisualizationTitle;

#pragma mark lasso relasted method - overwriting required for them
//find selected elements by lasso, and save them. You will be asked highlight them in method highlightSelectedElementByLasso
-(int) getSelectedObjectsFromPoints:(NSArray*)iPointArray;
// highlight the elements selected by lasso
-(void) highlightSelectedElementByLasso;
//for single selection on target is originally support by each widget itself, here we need to support multi selection cases for VI
-(void) handleMultiSelectionOnTarget;
#pragma mark lasso releated methods
-(void) enterMtpMode;
-(void) exitMtpMode;
-(void) enterLassoMode;
-(void) exitLassoMode;
-(BOOL) supportDrill;
-(BOOL) targetDefineForVIWidget;
-(void) setupCanvasViewInLasso:(BOOL)isLasso;
-(void) setCanvasViewAnnotationShape:(AnnotationShape) shapeType;
-(void) showActionArrowButton:(BOOL)show;
-(void) updateMultipleDoneButtonIfNeeded;
-(void)countSelectedByLasso;
- (void)dismissTooltipsAndItsHighlighting;
- (void) drillForLassoSelection2;
- (BOOL) hasDropZoneInfo;
//return nil if no info
- (NSMutableArray*)getIndexArrayFromDropZoneByType:(int)dropZoneUnitType needsTemplateMetric:(BOOL)needsTemplateMetric on:(EnumDSSAxisName)axis;
-(UIView*)touchesHandlerView;
-(void)cancelSingleTapGestureOnVICanvasView;
-(BOOL)supportKeepOnlyAndShow;
- (UIView*)getVITitleBar;
#pragma mark drill for VI, GM and HM only now
- (UIView*)getContainerView;
- (NSArray*)getSelectedCoordinatesDrill;

- (void)releaseDataModelMemory;
@end




#import "Task.h"
@interface MSIWidgetViewer (Tasks)
- (void) setUpTaskQueue;
- (BOOL) dispatchTask;
- (BOOL) submitTask:(Task*)iTask;
- (void) appendToTaskQueue:(Task*)iTask;
- (void) cleanTaskQueue;
- (void) pauseTask;
- (void) resumeTask;
- (void) callbackTasks;
- (void) destroyTaskQueue;
- (void) callbackTasksAndReleasePara;
@end


@interface MSIWidgetViewer (Rotation)
- (void) showAfterRotation;
- (void) hideAfterRotation;
//keep state when rotation happened
- (void)saveStateBeforeRotate;
- (void)unsaveStateForRotate;
- (void)updateAfterRotate;
//keep widget viewer state
- (void)saveWidgetViewerState;
- (void)restoreWidgetViewerState;
@end

@interface MSIWidgetViewer (ProtectedMethod)
- (BOOL) emptyDataSetHandling;
- (BOOL) emptyDataSetHandlingForCalendar;
- (void)hideWaitView;
- (void)showWaitView;
- (void)decreaseWidgetCount;
//- (bool)reInitDataModels;

- (UIView*) getRootView;
- (UIView*) rootViewer;
- (void)createWidgetOnLayoutOrPanelChange;
- (void)cleanWidgetOnLayoutOrPanelChange;
- (void) adjustFrameSize;
- (void)onViewReload:(NSNotification*)sender;

@end


