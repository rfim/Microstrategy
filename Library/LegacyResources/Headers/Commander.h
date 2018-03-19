//
//  Commander.h
//  IPadDashboardViewer
//
//  Created by bhuang on 3/23/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "RWDDashboardDelegate.h"
#import "DocumentManipulationDelegate.h"
#import "DSSIPhoneEnums.h"
#import "Enums.h"
#import "MSICommanderDelegate.h"

@class GridGraphViewer;
@class LinkDrillParameters;
@class LayoutManagerViewer;
@class LayoutManagerController;
@class BrushingHighlightData;
@class DocumentThemeHelper;
@class DocumentRenderer;
@class InfoWindowContainerViewer;

class DSSRWControl;
class DSSRWSectionNode;

class DSSRWControl;
class DSSRWSectionNode;
class DSSRWIterator;
class DSSRWSectionIterator;

#import <map>
#import <vector>
#import <string>



typedef enum EnumReplayManipulationsFlags
{
	CMDReplayManipulationsAllowLiveRequest	= 0x01,
	CMDReplayManipulationsKeepUCAO          = 0x02,
	CMDReplayManipulationsForceUseCache     = 0x04,
    CMDReplayManipulationsKeepClientAO      = 0x08,
} EnumReplayManipulationsFlags;

typedef enum EnumInfoWindowEventType
{
    InfoWindowEventTypePresent = 0,                 // default, as present info window
    InfoWindowEventTypeDisablePopping = 1,          // if source node target info window, but we doesn't want to present it out
    InfoWindowEventTypeDismiss = 2,                 // support dismiss info window event
    InfoWindowEventTypeRefreshContent = 3           // support refresh content without repop it out
} EnumInfoWindowEventType;

typedef enum EnumCommandContextType
{
    CommandContextTypeNone = 0,
    CommandContextTypePopover = 1,
    CommandContextTypeGroupBy = 2
} EnumCommandContextType;

class DSSCManipulationSetCurrentPanel;

@interface Command : NSObject
@property (nonatomic) void* mCommand;
@property (nonatomic) int mCommandType;
@property (nonatomic, strong) id mContext; // PopoverContext or GroupByContext
@property (nonatomic) int mContextType;
@property (nonatomic) bool mIsAutoManipulation;	// "Auto manipulation" is not trigger by user directly.
- (instancetype)init;
//- (void)dealloc;
@end

@interface PopoverContext : NSObject
@property (nonatomic, weak) UIView* mView;
@property (nonatomic) CGRect mRect;
@property (nonatomic) BOOL mAddToPass;
@property (nonatomic) int mCommandType; // command type, add to get more information in info window's handleEvent:withContext: yzeng
@property (nonatomic, strong) NSString* mInfoWinLayoutKey;
@property (nonatomic) NSInteger mPreferredPlacement;    // for info window enhancement
@property (nonatomic) int mEventType;
- (instancetype)init;
@end

@interface GroupByContext : NSObject
@property (nonatomic) BOOL mNeedRedraw;
- (instancetype)init;
@end


// Manipulation Context Types: End

struct NGBSelections
{
    int rowIndex;                                           // current page first row's index inside all NGB leaf node
    int rowCountPerPage;                                    // row count in one page
    DSSRWSectionIterator* topNGBIterator;                   // the top one of all the ngb node
    DSSRWSectionIterator* topNGBAllIter;                    // the top one (set as "all") of all the ngb node
public:
    NGBSelections(): rowIndex(0), rowCountPerPage(1), topNGBIterator(NULL), topNGBAllIter(NULL) {}
};

typedef enum _ManipulationType {
	ManipulationTypeMacro = -2, 
	ManipulationTypeReserved = -1, ManipulationTypeDrill = 0, ManipulationTypeLinkDrill = 1, ManipulationTypeLinkDrillURL = 2
} ManipulationType;

typedef enum _VizLevel {
	NoViz = 0, 
	VizOnLayout = 1, 
	VizOnTemplate = 2,
} VizLevel;

typedef enum{
    NGBAllBehaviorNone = 0,
    NGBAllBehaviorGoToNextPage,
    NGBAllBehaviorGoToPreviousPage,
    NGBAllBehaviorAfterRotation
} NGBAllBehaviorType;

/**
 * Indicate whether the current node has render subtree.
 */
@protocol SubtreeRenderer

// 765981 Added a return value for handle event to indicate whether subtree is recreated.
- (EventHandlingResult)handleEvent:(NSString *)ipEventName;

@end

@protocol IViewer;
@class MSIURLHelper, MSIDocumentContext, ViewerDataModel;
@class TransactionParameters;
@class CarryOverSelectionsSet;

class DSSRWNode;
class DSSDocumentInstance;
class DSSActionObject;
class DSSBaseElementProxy;
class PropertyInfo;
class NodeAndControls;
class DSSCMacroManipulation;
@class TransactionParameters;
@class DocumentManipulation;
@class ManipulationContext;


@class DrillXMLCommandParameters;
@class TransactionSubmission;
@class CommandVectorWrapper;

typedef std::map<std::string, std::vector<id<IViewer> __unsafe_unretained> > MapNodeKeyToViewers;
typedef std::map<std::string, std::string> MapNameToNodeKey;
typedef std::map<std::string, std::vector<id<IViewer> __unsafe_unretained> > MapNodeKeyToMasterViewers;

@interface Commander : NSObject <MSICommanderDelegate>{
	DSSDocumentInstance* docInstance;
	MSIDocumentContext* docContext;	// Strong reference 
	id<RWDDashboardDelegate> __weak delegate;
	//std::vector<std::string> actionObjects;
	//std::vector<bool> isAutoAction;	// Indicate whether each action object in actionObjects array is an auto manipuation.
//	MapNodeKeyToViewers mMapNodeKeyToViewers;
//	MapNameToNodeKey mMapNameToNodeKey;	
//	MapNodeKeyToMasterViewers mMapNodeKeyToMasterViewers;//record the master views of the specific node key,TQMS 446038
    NSString* baseURL;

	int webserverType;
	
	UIView* __weak mRootViewContainer;
    UIView* __weak mRWDDashBoardView;
	BOOL isInPortraitMode;
	BOOL enableGraphTooltips;
	
	Command* asyncCommand;
	DrillXMLCommandParameters* drillXMLCommandParams;
	int drillType;
	BOOL waitForPreviousManipulation;
	ManipulationType previousManipulationType;
	LinkDrillParameters* linkdrillParam;
	MSIURLHelper* urlHelper;
	PopoverContext* popoverContext;
	
	CGFloat groupbyHeight;
	CGRect frameForView;
	
	short layoutBarStyle;
	short layoutBarPosition;
    bool  enableZoomByPinchOrDoubleTap;
    
    UIImage* __weak mpDefaultFailImage;
	
	// suppor iPhone full screen feature in landscape mode, by xliu
	UIView* groupbyViewer;
	BOOL runAsReport;
	BOOL forceUpdateSubviews;	//To tell whether to call [super updateFrames] in the updateFrames method of LayoutContainderView.
	BOOL hasLayoutBarPortrait;
	BOOL isReport;
	BOOL isPostOrion;
	
	BOOL disableGraphAnimation; //471154 Need to disable graph animation when rendering preview.
	
	ViewerDataModel* layoutContainerViewerModel;
	
	//layout level properties, later may define a class named LayoutContext to hold all layout level properties, by xliu, 3/21/2011
	BOOL containWidget;
	short vizLevel;
	
	//Group by selected element carry over feature for iPhone, by xliu, 4/6/2011
	//NSMutableDictionary* groupbySelectedElements;	//Key is the attribute id, value is the selected element id.
	
	//For Group by ALL optimization, by xliu, 5/6/2011.
	NGBSelections mNGBSelections;	//Note down the current NGB section viewers status, which row is currently shown.
	BOOL skipAddNGBSection;
    
    BOOL isAnyCommandRunning;         // indicate whether there is any command is running
    BOOL cancelInfoWindowPopping;     // whether we need to cancel the popping of info window, for issue 508158, yzeng 
    GridGraphViewer* __weak mpGridGraphViewer;
    
    //for rotate
    NSMutableDictionary* mpWidgetState;
    NSInteger mCurrentDocumentView;
    
    // For mobile VI
    BOOL isVIDocument;                // indicate whether this is a mobile VI document
    BOOL showFilterPanel;
    
    // for VI dark/light theme
    ThemeType mThemeType;
    
    LayoutManagerViewer* __weak mLayoutManagerViewer;      // weak reference
    LayoutManagerController* __weak mLayoutManagerController;  // weak reference
    
    // For document manipulation refactory
    bool mUseNewWorkflow;
    DocumentManipulation* documentManipulation;
    ManipulationContext* manipulationContext;
    
    //nli, SDK support
    BOOL inSDKMode;
    BOOL isViewFitWidth;
    //CGFloat sdkScaleRatio;
    
    //nli, flags for preview generation in background thread
    BOOL previewCancelled;
    BOOL inPreviewGeneration;
   
    
    //nli, lock page header and footer
    UIView * dockedHeaderView;
    UIView * dockedFooterView;
    BOOL mRenderingHeader;
    BOOL mRenderingFooter;
    CGFloat layoutWidth;
    
    UIView* currentView;
    
    
    BOOL duringNormalManipulation;
    
    BOOL isTableObjectNode;
    TransactionSubmission* transactionSubmission;
    
    BOOL mIsNGBNeedRestored;
    BOOL mApplyGroupByToAllLayouts;
    
    // for viewer status restore
    NSMutableDictionary* mpViewerStatusDict;
    
    BOOL allowOfflineSubmit;
    BOOL submitInOrder;
}

// Currently RWDDashboardDataViewer keeps strong reference to RWDDashboardView, and RWDDashboardView keeps strong
// reference to Commander.  To avoid circular reference, we keeps weak reference here.
@property (nonatomic, weak) id<RWDDashboardDelegate> delegate;
@property (nonatomic, copy) NSString* baseURL;
@property (nonatomic) int webserverType;
//@property (nonatomic, assign) BOOL isInPortraitMode;
@property (nonatomic) BOOL enableGraphTooltips;
@property (nonatomic) CGFloat groupbyHeight;
@property (nonatomic) CGRect frameForView;
@property (nonatomic) short layoutBarStyle;
@property (nonatomic) short layoutBarPosition;
@property (nonatomic) bool  enableZoomByPinchOrDoubleTap;
@property (nonatomic, strong) UIView* groupbyViewer;
@property (nonatomic) BOOL runAsReport;
@property (nonatomic) BOOL forceUpdateSubviews;
@property (nonatomic) BOOL hasLayoutBarPortrait;
@property (nonatomic) BOOL isReport;
@property (nonatomic, weak) UIImage* mpDefaultFailImage;
@property (nonatomic) BOOL isPostOrion;
@property (nonatomic) BOOL disableGraphAnimation;
@property (nonatomic, strong) ViewerDataModel* layoutContainerViewerModel;
@property (nonatomic) BOOL containWidget;
//@property (nonatomic, assign) short vizLevel;
//@property (nonatomic, assign, readonly) BOOL widgetFitToScreen;
@property (nonatomic) BOOL skipAddNGBSection;
@property (nonatomic, readonly) BOOL firstNGBRow;
@property (nonatomic, readonly) BOOL lastNGBRow;
@property (nonatomic, readonly) BOOL hasNGBAll;
@property (nonatomic) NGBAllBehaviorType NGBAllBehavior;
@property (nonatomic) BOOL isAnyCommandRunning;
@property (nonatomic) BOOL cancelInfoWindowPopping;
@property (nonatomic) bool  isScrollingOrZooming;
@property (nonatomic) BOOL isVIDocument;
@property (nonatomic, assign) BOOL showFilterPanel;
@property (nonatomic) ThemeType mThemeType;
@property (nonatomic) BOOL isHTML5VIDocument;  // MoblieVI includes two types: 1. isVIDocument && isHTML5VIDocument == true : HTML5VI; 2. isVIDocument && !isHTML5VIDocument == true : FlashVI.
@property (nonatomic, weak) LayoutManagerViewer* layoutManagerViewer;
@property (nonatomic, weak) LayoutManagerController* layoutManagerController;
@property (nonatomic) BOOL inSDKMode;
@property (nonatomic) BOOL isViewFitWidth;
@property (nonatomic) CGFloat sdkScaleRatio;
@property (nonatomic) BOOL inPreviewGeneration;
@property (nonatomic) BOOL previewCancelled;
@property (nonatomic) BOOL updatePreviewInMain;
@property (nonatomic, strong) UIView * dockedHeaderView;
@property (nonatomic, strong) UIView * dockedFooterView;
@property (nonatomic) BOOL mRenderingHeader;
@property (nonatomic) BOOL mRenderingFooter;
@property (nonatomic) CGFloat layoutWidth;
@property (nonatomic) BOOL inGBA;
@property (nonatomic) bool mEnableCopyForText;
@property (atomic) BOOL duringRWDDashboardViewFadeInAnimation; //when rotation, the layoutContainViewer will fade in rwddashboardView in iPhone. it will conflict with the image viewer fade in animation. So i add this var to indicate whether RWDDashboardView is during animation.
//DL;SDK WhatIf;06/12/2012;submit transaction with targetKeys
@property (nonatomic, weak)UIView* responderSelectableTextViewer;
@property (nonatomic, strong)LinkDrillParameters* linkdrillParam;
@property (nonatomic)BOOL isTableObjectNode;
@property (nonatomic)BOOL isVIDocumentChanged;//for mobile vi shanli
@property (nonatomic, weak)DocumentRenderer* documentRenderer;
@property (nonatomic)CGFloat documentScaleRatio;
@property (nonatomic, strong)NSMutableSet * widgetSet;
@property (nonatomic)BOOL hasWidget;
@property (nonatomic, strong)CommandVectorWrapper* delayedCommands;
@property (nonatomic)BOOL needRescaleForiPhone6;   // TQMS 977109 - auto fit iPhone 5 view for iPhone 6
@property (nonatomic, readonly)BOOL applyGroupByToAllLayouts;
@property (nonatomic)BOOL allowOfflineSubmit;
@property (nonatomic) BOOL submitInOrder;
@property (nonatomic, strong) PopoverContext * savedPopoverContext;
/**
 * Generate a MacroManipulation with ResetToFirstPanel command added, if any.
 * @param irSelectorKey the node key of the selector whose target is a panel stack.
 * @return DSSCMacroManipulation* if there is any ResetToFirstPanel command, otherwise NULL.
 */
- (DSSCMacroManipulation *)getMacroManipulationWithResetToFirstPanelCommand:(const std::string &)irSelectorKey;

/** notify the panel stack(s) targeted by a given selector to reset to first panel. Added by yzhi.
 * Usually use NSNotification (observer pattern) to notify the panel stack viewers. However, if the panel stack viewer does not exist, DSSCManipulationSetCurrentPanel in commander will be executed. 
 * @param selectorKey the node key of selector
 */
- (void) notifyPanelStackResetToFirstPanel:(const std::string &) selectorKey;

/** Find all the panel stack targets for a given control node. Added by yzhi
 * If there are nested panel stacks, all the panel stack nodes nested inside are returned.
 * @param ipRWControl a control node
 * @param vTargetNodes array of target panel stack nodes
 */
- (void) findAllPanelStackTargets:(DSSRWControl *)ipRWControl
					  withOutputs:(std::vector<DSSRWSectionNode *> &) vTargetNodes;

/** Find the first panel stack targets for a given control node. Added by njiang
 * If there are nested panel stacks, all the panel stack nodes nested inside are returned.
 * @param ipRWControl a control node
 * @param vTargetNodes array of target panel stack nodes
 */
- (void) findFirstPanelStackTargets:(DSSRWControl *)ipRWControl
                        withOutputs:(std::vector<DSSRWSectionNode *> &) vTargetNodes;

/** Synchronize selector(s) with panel stack by a given index. 
 * Imported from PanelStackViewer. Added by yzhi.
 * @param ipRWSectionNode panel stack node
 * @param iIdx the panel index to synchronize
 */
- (BOOL) SynchWithSelectorFromPanelStack:(DSSRWSectionNode *) ipRWSectionNode
					 withIndex:(const int) iIdx;

/** Check whether the panel stack selector is docked. 
 * Imported from PanelStackViewer. Added by yzhi.
 * @param lControl a given control node
 */
- (BOOL)isDockedPanelStackSelector:(NodeAndControls*)lControl;

- (void)executeCommitTransactionWithTargetKeys:(NSArray*)targetKeys;

- (void)setDocInstance:(DSSDocumentInstance *)instance;
- (DSSDocumentInstance*)getDocInstance;

- (void)setDocContext:(MSIDocumentContext *)iDocContext;

- (MSIDocumentContext *)getDocContext;

- (NSArray*)getPaletteColors;

- (void)registerViewer:(id<IViewer>)iViewer withNode:(DSSRWNode*)iNode;

- (void)registerViewer:(id<IViewer>)iViewer forNodeKey:(std::string)iNodeKey;

- (void)unregisterViewer:(id<IViewer>)iViewer;

- (void)unregisterViewersWithNodeKey:(std::string)iNodeKey;

- (void*)getCommandByType:(int)iType;

- (std::vector<void*>)getAvailableCommandTypes:(void*)iCellHandle;

//- (void)executeCommands:(std::vector<void*>)iCommands;
- (int)executeCommands:(const std::vector<Command*>&)iCommands;
- (int)executeCommands:(const std::vector<Command*>&)iCommands asyncExec:(BOOL)async;
- (int)executeCommands:(const std::vector<Command*>&)iCommands ShowWaitView:(BOOL)showWaitView;
- (int)executeCommands:(const std::vector<Command*>&)iCommands showWaitViewOnView:(UIView*)view;

//- (std::string) getEventsString:(void *)command;
- (DSSBaseElementProxy*) getReplacedElement:(std::string)iNodeKey;

- (MapNodeKeyToViewers*)getMapNodeKeyToViewers;
- (MapNodeKeyToMasterViewers*)getMapNodeKeyToMasterViewers;

- (void)updatePreviewImages:(NSDictionary*)images;

- (void)generatePreviewImage;

- (void)setRootViewContainer:(UIView*)iRootViewContainer;

- (UIView*)getRootViewContainer;

- (void)setRWDDashBoardView:(UIView*)RWDDashBoardView;

- (UIView*)getRWDDashBoardView;

- (void)registerMasterView:(id<IViewer>)iViewer forNodeKey:(std::string)iNodeKey;
- (void)unregisterMasterViewer:(id <IViewer>)iViewer;

- (void)hideAllInfoWindows:(BOOL)animated;

// mzhu 2010-06-17
- (void)getImagesFromIServer:(NSArray*) iImagesArray forTimeStamp: (NSArray*) iImagesTimeStamp forAsync:(BOOL)ibAsync;

- (void)setDisplayMode:(int)iDispalyMode;

- (int)getDisplayMode;

- (BOOL)isMultiSelectionMode;

- (void)afterManipulation:(NSValue*)iValue;

// for drill backward feature
- (void)beginReplayManipulation;
- (BOOL)replayManipulations:(NSArray*)events WithData:(NSData*)newData WithFlags:(unsigned int)flags;
- (BOOL)replayManipulation:(NSString*)events WithData:(NSData*)newData WithFlags:(unsigned int)flags;
- (void)replayDrillManipulation:(DrillXMLCommandParameters*)parameters WithData:(NSData*)newData;
- (void)endReplayManipulation:(BOOL)needRefresh;
- (void)executeLinkDrill:(LinkDrillParameters*) linkDrillParams;
- (BOOL)validateCacheExecuteLinkDrill:(LinkDrillParameters*) linkDrillParams;
- (BOOL)validateCacheHandleLinkDrill:(MSIURLHelper*) urlHelper;
- (void)handleLinkDrill:(MSIURLHelper*) urlHelper;
- (void)handleLinkDrill:(MSIURLHelper*) urlHelper withContext:(PopoverContext*)iContext;
- (void)handlePDF:(MSIURLHelper*) urlHelper;
- (void)handleEPUB:(MSIURLHelper*) _urlHelper;
- (void)handleEPUB:(MSIURLHelper*) _urlHelper withSender:(UIView *) iView;
- (void)handleActionSheet:(MSIURLHelper*)urlHelper;
- (void)handleURLSelection:(MSIURLHelper*)iUrlHelper;

- (NSString*)getClientManipulations;
- (void)clearClientManipulations;

//- (void)updateGroupbySelectedElementsWithAttribute:(NSString*)attributeId AndSelectedElement:(NSString*)elementId;
- (NSString*)getSelectedElementOfAttribute:(NSString*)attributeId;
- (void)enableInteraction:(BOOL)enabled;

- (void)getEmptyGridDisplay:(NSString**) emptyMessage display:(BOOL*) displayEmptyMsg;
- (void)afterBinaryApplied:(BOOL)iSuccess;
- (BOOL)isDuringBackward;
- (BOOL)isDisabledforShowPrompt;
- (void)setSavedPopoverContext:(PopoverContext *)iContext;
/**
 * Used to judge whether have next page for NGB section.
 */
- (BOOL)hasNextPage;
/**
 * Used to judge whether have previous page for NGB section.
 */
- (BOOL)hasPreviousPage;
/**
 * Change NGB page to the next one. Called in [LayoutContainerViewer - (void)handleEvent].
 */
- (BOOL)goToNextPage;
/**
 * Change NGB page to the previous one. Called in [LayoutContainerViewer - (void)handleEvent].
 */
- (BOOL)goToPreviousPage;
/**
 * Used to judge whether NGB is stored in Commander.
 */
- (BOOL)isNGBSelectionsInited;
/**
 * Used to init NGB setting. It is called in DocumentIterator.
 */
- (void)initNGBSelections:(DSSRWSectionIterator*)ipIterator;
/**
 * Used to judge whether NGB need to reinit, e.g., change sub NGB, it need to reinit NGB setting.
 */
- (BOOL)needReInitNGBSelections;
/**
 * Reinit NGB setting inside Commander.
 */
- (void)reinitNGBSelections;
/**
 * Clear NGB setting inside Commander.
 */
- (void)clearNGBSelectionsAllFields;
/**
 * Calculate the current page index.
 */
- (void)getCurrentPageIndex:(int&)iCurrentPageIndex totalPageCount:(int&)iTotalPagesCount;
/**
 * Update the NGB setting inside commander. Called in [LayoutContainerViewer - (void)handleEvent].
 */
- (void)updateNGBSelectionAfterRotation;
/**
 * Judge whether a node is ngb leaf node.
 */
- (BOOL)isLeafNGBNode:(DSSRWNode*)ipRWNode;
/**
 * Get the first row indiex in the current page.
 */
- (int)getNGBFirstRowIndexInCurrentPage;

//For zooming, by yzeng, 9/26/2011.
//- (void) setContentScaleFactorOnDemandToViews:(UIView*) iView;
- (void) setContentScaleFactorOnDemandToViews:(UIView*) iView rootView:(UIView*) iRootView;
- (void) setContentScaleFactorOnDemandToViews:(UIView*) iView rootView:(UIView*) iRootView informView:(BOOL)inform;
//- (float)getZoomScale;
- (void) restoreZoomForBackOperation;
- (void) zoomDashboardToScale:(CGFloat)iScale;

//incremental download
- (void) requestMissingBinaryBlock:(NSString*) iLayoutKey;
- (BOOL)requestLayoutData:(DSSRWNode*)iRWNode withHandler:(id<BinaryRetrievalHandler>)iHandler withSelector:(SEL)callbackFunction;

- (void) showWaitViewOnTop;
- (void) dismissWaitViewOnTop;
- (void)dataModelCreationFailed;
- (void)hideMasterPanel:(BOOL)disable;
- (void)showMasterPanel;

//Offline transaction support
- (void)restoreDetailedTransactionModel:(TransactionParameters*)iTransactionParameters;
- (void)restoreDetailedTransactionArray:(NSArray*)iTransactionDetails;
- (NSArray*)getDetailedTransactionData;

- (GridGraphViewer*)getGridGraphViewerForReport;

//TQMS 511934: Hide/show navigation bar right item when drill radar shows/disappears
-(void) hideRightNavigationBarButtons;
-(void) showRightNavigationBarButtons;

//for widget rotating
- (void) saveWidgetStateWithObject:(id)obj forKey:(id)key;
- (id) loadWidgetStateForKey:(id)key;

// save or load widget state, for common use
- (void)saveWidgetState:(id)obj forKey:(id)key;
- (id)loadWidgetState:(id)key;

// carry over selections during link drill
- (CarryOverSelectionsSet*)carryOverSelections;
- (CarryOverSelectionsSet*)carryOverSelectionsWithMode:(CarryOverSelectedMode)mode withPromptMode:(BOOL)promptMode currentPrompt:(NSString *)currentPrompt;

/**
 replay manipulations in the carry over selection
 @param selections carry over selections set
 @param refresh Is it necessary to refresh the UI after replay action
 @return Whether the replay will need to send network request
 */
- (BOOL) replayCarryOverSelections:(CarryOverSelectionsSet*)selections refreshUI:(BOOL)refresh;
- (id) replayCarryOverPrompts:(CarryOverSelectionsSet*)selections withPromptQuestionXML:(NSData*)promptQestion withPrompts:(id)data;

// for transaction in photo uploader widget
- (void)photoUploadImageChange:(const std::vector<Command*>&)iCommands nodeKey:(NSString*)ipNodeKey imgPath:(NSString*)ipImgPath;

//for TQMS 833232
- (void)photoUploadImageChange:(const std::vector<Command*>&)iCommands nodeKey:(NSString*)ipNodeKey imgPath:(NSString*)ipImgPath shouldReplacePreImage:(BOOL) replace;

// for offline transaction in photo uploader widget
- (void)photoUploadImageChange:(NSString*)ipNodeKey imgPath:(NSString*)ipImgPath;

//new API for TQMS 833232
- (void)photoUploadImageChange:(NSString *)ipNodeKey imgPath:(NSString *)ipImgPath  shouldReplacePreImage:(BOOL) replace;

//DL;SDK;12/11/2012;SDK transaction support
/**
 * Returns <code>DSSRWIterator</code> mapped to transactionReport
 * @param transactionReportID transaction report's ID
 * @return DSSRWIterator*  DSSRWIterator mapped to transaction report
 */
-(DSSRWIterator*) getDSSRWIteratorForTransactionReport:(NSString*)transactionReportID;

// Add this function definition for clearing warning by Nan Jiang, 08/07/2012
- (void) refreshInstanceWithData:(NSData*)newData;

// Get the panel node from an panelSelection url.
- (std::vector<DSSRWSectionNode*>)getPanelNodeFromPanelSelectionURLHelper:(MSIURLHelper*) iUrlHelper;
- (BOOL)checkForTransactionUpdate;
#pragma mark -
#pragma mark Mobile rotation

- (NSInteger) chooseDocumentViewForInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation;
- (BOOL)shouldCancelRendering;
@property (nonatomic) NSInteger currentDocumentView;
/** Get the number of document views. 03/07, 2013 yzeng add.
 */
- (NSUInteger)getNumberOfDocumentViews;


//by lishan stats log
- (void)handleRenderFinishTime:(NSDate *)date;


- (CGRect) updateRootViewerContainerFrame: (CGRect)frame;
- (CGFloat) getHeaderSectionYOffset;
- (double) getDockedHeaderHeight;
- (double) getDockedFooterHeight;
- (CGFloat) getCalGroupbyHeight;
- (UIView *) checkIfInDockedSection:(UIView *)view;
- (BOOL)isViewerDocked:(UIView*)view;
// for info window enhancement
- (MapNameToNodeKey*) getMapNameToNodeKey;
//TQMS 675461, expose an API for viewer to disable the iPhone layout bar. By xliu, 2/20/2013.
- (void)disableIPhoneLayoutBar:(BOOL)disable;

-(NSArray*)getAllDataSetIDs;
-(NSDictionary*)getDatasetPromptAnswers;

// for Mobile VI
/**
 * Get the visualziationContentPanelStackKey for current layout. This panelstack is the root of
 * visualization zone (and page-by zone, according to lcao). Use [NSString cStringUsingEncoding:] 
 * if you need c string or std::string.
 */
- (NSString*) getCurrentVisualizationContentPanelStackKey;
/**
 * Get the visualziationContentPanelStackKey for given layout.
 */
- (NSString*) getVisualizationContentPanelStackKeyForLayout:(NSString*)iLayoutKey;
/**
 * Set the visualziationContentPanelStackKey for given layout.
 */
- (void) setVisualizationContentPanelStackKey:(NSString*)iVIContentPanelStackKey forLayout:(NSString*)iLayoutKey;
/**
 * Remove the visualziationContentPanelStackKey for given layout.
 */
- (void) removeVisualizationContentPanelStackKeyForLayout:(NSString*)iLayoutKey;
/**
 * This method can get current layout even it is not a VI document, and an exception is pre-orion document.
 */
- (NSString*) getCurrentLayoutKey;


//for new ui navigation bar
- (NSArray *) getAllLayoutNames;

- (NSArray *) getAllLayoutKeys;

- (NSString*) getCurrentLayoutName;

- (BOOL)isFullScreen;

- (BOOL) hasNavigationTabBar;

-(DocumentThemeHelper*) documentThemeHelperWithComponentKey:(NSString*)componnetKey;
/**
 * Collect all the views status that response to [IViewer getViewerStatus].
 */
- (NSData*)getViewersStatus;
/**
 * Restore all the views data stored as before.
 * @return
 * 1, success
 * 0, iData is nil
 * -1, iData format is wrong,
 */
- (int)replayViewersStatus:(NSData*)iData;
/**
 * Get the specific Viewer's status data, whose ID = iViewerID.
 */
- (NSData*)getSavedViewerStatus:(NSString*)iViewerID;

- (void) increaseWidgetCount:(id)viewer;
- (void) decreaseWidgetCount:(id)viewer;

- (NSString*)documentName;

- (void)viewEvaluationFinish;

/*
 * Get document level color by item index;
 */
- (NSInteger)getColorByItemIndexByColorByString:(NSString*) colorByItemString;

/**
 * Get the transform for auto fitting iPhone 5 view for iPhone 6/6+ aspect-ratio devices, TQMS 977109
 * @param originHeight the oritianl height of content
 * @param isLandscape whether the device is landscape
 * @param isFullscreen if YES, we should consider that the navigation bar is not existing
 * @param extraOccupiedHeight we may need to consider the height occupied by other things like the groupby bar
 */
- (CATransform3D)getRescaleTransformForIPhone6:(CGFloat)originHeight
                                   isLandscape:(BOOL)isLandscape
                               isFullscreenDoc:(BOOL)isFullscreen
                           extraOccupiedHeight:(CGFloat)extraOccupiedHeight;

/**
 * This function will return the change list, the content is a list of UIView
 */
- (NSArray*)getViewerChangedListAfterManipulation;

// Mobile Export
- (BOOL)isExporting;
- (void)handleWillQuitExport:(void (^)())handler;

- (bool)shouldDismissIW:(InfoWindowContainerViewer *)infoWindow withURLHelper:(MSIURLHelper *)urlHelper;

@end

@interface Commander (MOBILE_VI)
//View side will call saveAsVIDocument: to send save as reqeust from controller
- (void) saveAsVIDocument:(NSString*)folderID andDocName:(NSString*)docName andDocDesc:(NSString*)docDescription andSFlag:(EnumDSSXMLDocSaveAsFlags)flag;
- (void) createNewFolder:(NSString*)parentFolderID andFolderName:(NSString*)folderName andFolderDesc:(NSString*)folderDescription;
/**
 * This method helps handling brushing highlight event in vi documents
 */
- (void) handleBrushingHighlightForVI:(BrushingHighlightData *)highlightData; // Moved to here by wenzhou, 05/18/2015. To avoid warning "Category is implementing a method which will also be implemented by its primary class".
@end



@interface CommandVectorWrapper : NSObject
@property (nonatomic) std::vector<Command*> mCommands;
@end
