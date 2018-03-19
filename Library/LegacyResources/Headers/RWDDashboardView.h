//
//  RWDDashboardView.h
//  MSTRMobile
//
//  Created by Ji Jin on 4/17/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "DSSIPhoneEnums.h"
#import "RWDDashboardDelegate.h"
#import "Service.h"
#import "MSIDataViewer.h"
#import "MSIFiltersPanelStackDelegate.h"
#import "MSIAlertView.h"

@protocol RWDDashboardDelegate;
@protocol MSILaserPointerDelegate;

@class Commander;
@class MSIDocumentContext;
@class RootViewerContainer;
@class AnalysisPanelStackViewer;
@class ExpandablePanelView;
@class VIDocumentControlView;
@class HeatMapWidgetViewer;
@class MSIMapWidgetViewer;
@class GridViewer;
@class GraphViewer;
@class VIDocumentTabSwitchView;
@class GraphMatrixWidgetViewer2;
@class UILaserPointerGestureRecognizer;
@class MSILayoutInfo;
@class RWDataServiceResult;
@class FiltersPanelStackViewer;
@class DocumentRenderer;
@class LaserPointerController;

static const float navigationBarHeightPortrait = 44;
static const float navigationBarHeightLandScape = 32;
static const float widthInPotraitMode = 304;
static const float heightInLandscapeMode = 234;
static NSString* const TAP_ON_NAVIGATION_BAR = @"tab_on_navigation_bar";

//nli, docked sections. Wrapper for docked section, to deal with: 
//1. width wider than screen, 2. Grid and selector could call measure upward along the view hierarchy. 
@interface DockedSectionContainer : UIScrollView <UIScrollViewDelegate>{
    BOOL inUserInteraction;
}
@property (nonatomic, weak) UIView * layoutContainer;
@property (nonatomic, weak) Commander * mpCommander;
@property (nonatomic, strong) UIView * dockedSectionViewer;
- (void)scrolling:(CGPoint)offset;
@end

@interface RWDDashboardView : UIView 
    <MSIDataViewer,
    UIScrollViewDelegate, 
    MSIAlertViewDelegate,
    ServiceHandler, 
    UIActionSheetDelegate,
    MSIFiltersPanelStackDelegate>

{
	short mLogLevel;
	void* mpDoc;
	Commander* mpCommander;
	MSIDocumentContext* mpDocContext;
	UIView* layoutIndicator;
	bool bFullScreen;
    BOOL bShowingBarsTemporarily;//793295
	UILabel* mpErrorMessageView;
	RootViewerContainer* mpRootViewer;
	UIView* mpMasterPanel;
	bool isWaitingForUserRuntimeService;
	short mLayoutBarStyle;
	short mLayoutBarPosition;
    bool mEnableZoomByPinchOrDoubleTap;  // TQMS 513294 by zeng
	UIColor* mpBackgroundColor;
	ExpandablePanelView *expPanelView;
    
	// iPhone feature on full screen of report and document, by xliu
	UIView* __weak groupbyView;
	BOOL fullScreen;
	BOOL renderFail;

    NSString* mpDashboardName;
    
    // Airplay
    UIScreen *extScreen;
    UIWindow *extWindow;
    UIView *extWindowRootView;
    bool isVIDocument;
    AnalysisPanelStackViewer *mpDataView;
    UIView *displayInfoMaskView;
    UILabel *displayInfoView;
    bool displayInfoRemoved;
    UIView *blackMaskView;
    int tabNumber;
    UIView *filterView;
    bool isVGAMirroringDisabled;
    bool isVIDocumentRunning;
    bool firstVITabSelection;
    NSArray *tabNames;
    UIActionSheet* tabSelectionActionSheet;
    bool isTabSelectionActionSheetShown;
    UILabel* tabNameLabel;
    VIDocumentControlView *controlView;
    VIDocumentTabSwitchView* swipeAreaView;
    HeatMapWidgetViewer* heatMapView;
    MSIMapWidgetViewer * mapView;
    GraphMatrixWidgetViewer2* graphMatrixView;
    GridViewer* gridView;
    GraphViewer* graphView;
    int currentTab;
    bool isAirplay;
    bool isAirplayRendered;
    bool isBuiltInMirroring;
    
    LaserPointerController* mpLaserPointerController;
    
    //dock header and footer sections, by nli
    DockedSectionContainer * headerSection;
    DockedSectionContainer * footerSection;
    
    BOOL mIsGeneratePreviewImage;
    RWDataServiceResult* result;
    NSString* reportInfoName;
    NSInteger currentLayoutID;
    
    // TQMS 634075:Selector will be covered by the keyboard when the selector is located at the bottom part of the document. Modify scrollview contentOffset and dashboard view frame to make the selector visible. by njiang, 09/04/2012.
    BOOL mIsInRotation;
    UIView* activeView;
    //CGRect mDashboardViewOldFrame;
    // Add this property to judge whether dashboard view is moved up. By njiang, 09/24/2012.
    BOOL isRWDDashboardViewMovedUp;
    bool filtersPanelShown;
    //int mCannotStopRenderingProcessInstancesCount;//add by dlu
    NSMutableDictionary* layoutIDperState;
}

@property(nonatomic, strong) UIView* layoutIndicator;
@property(nonatomic, strong) MSIDocumentContext* mpDocContext;
@property(nonatomic, weak) UIView* groupbyView;
@property(nonatomic, strong) Commander* mpCommander;
@property(nonatomic, readonly) BOOL renderFail;
@property(nonatomic, copy) NSString* dashboardName;
@property(nonatomic, copy) NSString* reportInfoName;
@property(nonatomic, strong) DockedSectionContainer * headerSection;
@property(nonatomic, strong) DockedSectionContainer * footerSection;

// Airplay
@property(nonatomic, readonly) bool isVIDocument;    // also used in Mobile VI, and make its initializing time earlier
@property(nonatomic, readonly) bool isVIDocumentRunning;
@property(nonatomic, readonly) int tabNumber;
@property(nonatomic, readonly) bool isAirplay;
@property(nonatomic, readonly) bool isAirplayRendered;

// TQMS 634075:Selector will be covered by the keyboard when the selector is located at the bottom part of the document. Modify scrollview contentOffset and dashboard view frame to make the selector visible. by njiang, 09/04/2012.
// Use this activeView to modify dashboard frame when keyboard will show.
// When views becomes first responder, set it to activeView. When resign, set activeView nil.
@property(nonatomic, strong) UIView* activeView;
@property(nonatomic) BOOL isRWDDashboardViewMovedUp;
@property(atomic) int cannotStopRenderingProcessInstancesCount;
@property(nonatomic, strong) LayoutManagerController* layoutManagerController;
@property(nonatomic) bool filtersPanelShown;
@property (nonatomic, strong) FiltersPanelStackViewer* filterPanelView;

// Mobile export
@property(nonatomic, copy) NSString *templateKey;

// init the dashboard view.
- (void)adjustFilterPanelView:(BOOL)showFilterButton;
- (id) initWithData:(void*)iDocInstance 
     dataController:(id<RWDDashboardDelegate>)iDataController
            baseURL:(NSString*)iBaseURL
      webServerType:(int)iWebServerType
    currentLayoutID:(int)iCurrentLayoutID
    needRender:(BOOL)iNeedRender;       //TQMS 621038: need to support delay rendering case

- (void) startRenderDocument:(CGRect)frame 
                     preview:(NSDictionary*)images 
                     docName:(NSString*)docName 
                      layout:(NSInteger)layoutID;

// magnifier glass
- (void) magniyGrid:(bool)enabled;

// Set mpCommander.delegate to be nil
- (void)clearDelegate;
- (void)generatePreviewImage;

//get the RootViewerContainer
- (RootViewerContainer*)getRootViewerContainer;

// iPhone feature on full screen of report and document, by xliu
// Pass YES when initialized the RWDDashboardView, otherwise pass NO. When YES, the layout bar will be taken into consideration after the RWDDashboardView is created.
// So during initilization, the RWDDashboardView's frame is not correct, it includes layout bar size.
- (void)resetPositions:(BOOL)initial;
- (void)afterBinaryApplied:(BOOL)iSuccess;
- (void)hideOrShowSubviews;

- (void) setSDKMode;

// Airplay for IVE document
- (void)showWaitViewOnExternalDisplay;
- (void)hideWaitViewOnExternalDisplay;
- (void)tabSwipeLeft;
- (void)tabSwipeRight;
- (void)startAirplay;
- (void)airplayLayoutLoaded;

//- (void)showMasterPanel;
//- (void)hideMasterPanel:(BOOL)disable;

//nli, docked sections
- (void) refreshDockSections;

- (void)setCustomizedBackgroundColor:(UIColor *)backgroundColor;

/** Hide the tooltips after zooming.
 */
- (void)hideTooltipsAfterZooming:(UIView*)iView;

/// clean all views. By yzhi.
- (void)cleanAllViews;

- (void) finishRenderingProcess;
-(void) restoreScreen;//793295

//// add by jucheng for filter panel
- (void)clickFilterButton;

-(void)detachFiltersPanelViews;///  

- (void)updateFilterPanelView:(FiltersPanelStackViewer*)filter;

/**
 * These 2 APIs are used to broadcast view data mode will/did change;
 */
- (void)broadcastViewerDataModelWillChange;
- (void)broadcastViewerDataModelDidChange;
- (void)broadcastViewerDataModelBecomeInvalidated;

- (void)dismissMagnifyGlassInGridView;
@end

