/*
 *  MSIDataViewer.h
 *
 *  Copyright 2010 MicroStrategy. All rights reserved.
 *
 */

#import "MSILayoutInfo.h"
#import "DSSIPhoneEnums.h"
#import "RWDDashboardDelegate.h"

@class RootViewerContainer;
@class Commander;
@class MSIDocumentContext;
@class CarryOverSelectionsSet;
@class DrillXMLCommandParameters;

/**
 * This protocol is the interface designed for any dashboard viewer for document/report rendering.
 * The document data controller will interact with the document viewer through this interface
 *
 */

@protocol MSIDataViewer<NSObject>


/**
 * Start to render document
 **/
- (void)startRenderDocument;

/**
 * Update Frame
 *
 * @param frame The frame rect
 **/
- (void)updateFrame:(CGRect) frame;

/**
 * Return dataview
 *
 * @return dataView
 **/
- (UIView*)dataView;

/**
 * Return isLandscape
 *
 * @return if is landscape
 **/
- (BOOL)isLandscape;

/**
 * Set landscape mode
 *
 * @param landscape is landscape mode
 **/
///- (void)setLandscape:(BOOL)landscape;

/**
 * Marks the receiver’s entire bounds rectangle as needing to be redrawn.
 *
 **/
- (void)setNeedsDisplay;

/**
 * Update the data model with new data
 *
 * @param data new binary data
 **/
- (void)updateWithData:(NSData*)data;

/**
 * Raised when viewer is to be closed
 *
 **/
- (void)viewerWillClose;

@optional
/**
 * Callback which is triggered when the data view is going to disappear on the screen
 **/
-(void) dataViewWillDisappear;

/** 
 * Update the associated RWD instance with the given data
 *
 * @param data New binary data
 */
//- (void)updateInstanceWithData:(NSData*) data;

// get value for 'execute in full screen mode' property
/** 
 * If executing in full screen mode
 *
 * @return If executing in full screen mode
 */
- (bool)executeInFullScreenMode;

- (BOOL)showingBarsTemporarily;//793295
- (void)restoreScreen;
/** 
 * Get grid display mode
 *
 * @return Grid display mode
 */
- (EnumDSSDisplayMode) getGridDisplayMode;

- (void) setGridDisplayMode:(EnumDSSDisplayMode)displayMode;

/** 
 * Return if gridGraphViewer is from widget
 *
 * @return If gridGraphViewer is from widget
 */
-(BOOL)gridGraphViewerIsFromWidget;

/** 
 * Notify dashboard viewer that manipulation is done
 *
 * @param iSuccess If the manipulation succeeded
 */
- (void) afterManipulation:(BOOL)iSuccess;

/** 
 * Get client manipulation
 *
 * @return Event string for not-commited client side manipulation
 */
- (NSString*)getClientManipulations;

/** 
 * Clear client manipulation that has not been committed
 *
 */
- (void)clearClientManipulations;

/**
 * Replay manipulations with delta binary data
 *
 * @param events Array of event string representing manipulations
 * @param newData Delta binary data needed for manipulation
 * @return If the replay async network request
 */
- (BOOL)replayManipulations:(NSArray*)events WithData:(NSData*)newData WithFlags:(unsigned int)flags;

/** 
 * Replay manipulation with delta binary data
 *
 * @param events Event string representing manipulations
 * @param newData Delta binary data needed for manipulation
 * @return If the replay async network request
 */
- (BOOL)replayManipulation:(NSString*)events WithData:(NSData*)newData WithFlags:(unsigned int)flags;

/** 
 * Replay drill manipulation with delta binary data
 *
 * @param parameters Drill parameters
 * @param newData Delta binary data needed for drill manipulation
 */
- (void)replayDrillManipulation:(DrillXMLCommandParameters*)parameters WithData:(NSData*)newData;

/** 
 * Begin replaying manipulation
 */
- (void)beginReplayManipulation;

/** 
 * End replaying manipulation
 *
 * @param needRefresh Do we need to refresh GUI after manipulation
 */
- (void)endReplayManipulation:(BOOL)needRefresh;

/** 
 * Return the root viewer container
 *
 * @return The root viewer container
 */
- (RootViewerContainer*)getRootViewerContainer;

/**
 * This method returns a boolean value indicating whether to display reprompt icon for current layout.  This method
 * is used by data controller to determine whether to show a "Filter" button on the right of navigation bar.
 *
 * @return true if it should display reprompt icon, false otherwise.
 */
- (bool) isDisplayRepromptIcon;

/** 
 * Update layout info
 *
 * @param layoutInfo Layout Info
 */
- (void)updateLayoutInfo:(MSILayoutInfo*)layoutInfo;

/** 
 * Set layout ID
 *
 * @param layoutID Layout ID
 */
- (void)setLayout:(NSInteger)layoutID;

/** 
 * Left animation for switching layout
 */
- (void)animateLeft;

/** 
 * Right animation for switching layout
 */
- (void)animateRight;

/** 
 * Set cached layout ID
 *
 * @param cachedLayoutID Layout ID
 */
- (void)setCachedLayoutID:(NSInteger)cachedLayoutID;

- (void)enableInteraction:(BOOL)enabled;

/** 
 * Is document transaction enabled
 *
 * @return Is document transaction enabled
 */
- (BOOL)isTransactionEnabled;

/** 
 * If we should generate preview image for the document
 *
 * @return If we should generate preview image
 */
- (BOOL) isGeneratePreviewImage;

/** 
 * Set if we should generate preview image for the document
 *
 * @param generate If we should generate preview image
 */
- (void) setGeneratePreviewImage:(bool)generate;

/** 
 * Get layout information of document
 *
 * @param layoutKeys Array of layout keys in the document
 * @param key CurrentLayoutKey
 * @return YES, if the operation succeeded
 */
- (BOOL)getLayoutKeys:(NSArray**) layoutKeys currentLayoutKey: (NSString**) key;

/** 
 * Request missing binary data
 *
 * @param layoutKey Layout key to be requested
 */


/** 
 * Notify binary has been applied
 *
 * @param iSuccess If the binary applied successfully
 */
- (void)afterBinaryApplied:(BOOL)iSuccess;

/** 
 * Request missing binary data
 *
 * @param iLayoutKey Layout key
 * @param iHandler Handler for request
 * @param callbackFunction Callback function after request is done
 */
- (void)requestMissingBinary:(NSString*)iLayoutKey withHandler:(id<BinaryRetrievalHandler>)iHandler withSelector:(SEL)callbackFunction;

/** 
 * Has the first layout binary been loaded
 *
 * @return Has the first layout binary been loaded
 */
- (BOOL)isInitialLayoutLoaded;

/** 
 * Apply layout binary
 *
 * @param iLayoutKey Binary data's layout key
 * @param iNewData Binary data 
 * @param iDocDataPath Binary data file path
 */
- (void)applyLayout:(NSString*)iLayoutKey withData:(NSData*)iNewData andDataPath:(NSString*)iDocDataPath;

/** 
 * Restore transaction model for offline transaction
 *
 * @param iTransactionParameters Transaction model
 */
- (void)restoreDetailedTransactionModel:(TransactionParameters*)iTransactionParameters;

/** 
 * Restore transaction model for offline transaction
 *
 * @param iTransactionDetails Transaction model
 */
- (void)restoreDetailedTransactionArray:(NSArray*)iTransactionDetails;

/**
 * Restore data sets delta transaction model for offline transaction
 *
 * @param datasetIDs dataset ID array
 * @param deltas dataset delta model array
 */
- (BOOL)restoreDatasetTransactionModel:(NSArray*)datasetIDs andDataArray:(NSArray*)deltas;

/** 
 * Get transaction data
 *
 * @return Detailed transaction model array
 */
- (NSArray*)getDetailedTransactionData;

/** 
 * Get layout Indicator
 *
 * @return Layout indication view
 */
- (UIView*)getLayoutIndicator;

/** 
 * Restore zoom factor for back operation
 *
 */
- (void) restoreZoomForBackOperation;

/** 
 * Get gridgraphviewer's pageby status
 *
 * @return Pageby element ID selected by gridgraphviewer
 */
- (int)getGridGraphViewerPageByStatus;

/** 
 * Set gridgraphviewer's pageby status
 *
 * @param status Pageby element ID to select
 */
- (void)setGridGraphViewerPageByStatus:(int)status;

/** 
 * Check if default layout contains UC
 *
 * @return If default layout contains UC
 */
- (BOOL)isDefaultLayoutContainUC;

/**
 * For MSTRObjectViewController, setFrame will cause the view frame to be revised to the new CGRect. 
 * The rect must be a valid rect, although it can be empty. If the rect is not valid, no action is taken.
 * It is not an error for the view to be partially or completely off the screen.
 *
 * @param newViewFrame gives the new view frame size with the type of CGRect.
 */
- (void) setViewFrame: (CGRect)newViewFrame;

/**
 * For MSTRObjectViewController, setFitWidth will resize the document so that the width fits to the view width or to the actual document width.
 *
 * @param isFitWidth gives fitting to view width or keeping actuall width.
 */
- (void) setFitWidth: (BOOL)isFitWidth;

/**
 * For MSTRObjectViewController, refresh view.
 */
- (void) refresh;

/**
 * For MSTRObjectViewController, setViewOrientation sets the view orientation.
 *
 * @param isViewOrientationPortrait is YES if the view is set to portrait.
 */
- (void) setViewOrientation: (BOOL)isViewOrientationPortrait;

/**
 * Replay saved carryover selections in the document
 *
 * @param selections Saved document selections for carryover
 * @param refresh Whether to refresh UI
 */
- (void) replayCarryOverSelections:(CarryOverSelectionsSet*)selections refreshUI:(BOOL)refresh;

/**
 * Return the groupby view
 *
 * @return The groupby view
 */
- (UIView*) getGroupbyView;

/**
 * Set the groupby view
 *
 * @param iGroupbyView The groupby view to be set
 */
- (void) setGroupbyView:(UIView*)iGroupbyView;

/**
 * Returnt the commander object
 * Commander is the manipulation controller in the viewer classes
 *
 * @return The commander object of the document
 */
- (Commander*) getCommander;

/**
 * Pause background thread after current task
 * Before data controller modifies the document model, we need to prevent viewer accessing it
 * this call is to stop the background thread so that document model can be modified
 *
 */
- (void) pauseAfterCurrentTask;

/**
 * Continue the next task in the background task queue
 * This is to be called after pauseAfterCurrentTask to continue the background queue
 *
 */
- (void) continueToDoNextTask;

/**
 * Test if graph mode is available for this document
 *
 * @return If chart is available
 */
- (BOOL) isChartAvailable;

/**
 * Return document context
 *
 * @return The document context
 */
- (MSIDocumentContext*) getDocContext;

/**
 * Return the current layout index
 *
 * @return The current layout index
 */
- (short) getCurrentLayoutIndex;

/**
 * Start to render VI document on external display - Airplay
 *
 */
- (void) renderVIDocumentOnExtDisplay;

/**
 * For airplay mode, display tab selection view
 *
 */
- (void)handleTabSelection;

/**
 * For airplay mode, display in control mode
 *
 */
- (void)changeToControlMode;

/**
 * For airplay mode, display in filter mode
 *
 */
- (void)changeToFilterMode;

/**
 * For airplay mode, notify stopping airplay mode
 *
 */
- (void)willBeClosed;

/**
 * Display wait indicator
 *
 */
- (void) showWaitViewOnExternalDisplay;

/**
 * Hide wait indicator
 *
 */
- (void) hideWaitViewOnExternalDisplay;

/**
 * Is VI document running
 *
 * @return Is VI document running
 */
- (BOOL)isVIDocumentRunning;

/**
 * Is in airplay mode
 *
 * @Return Is Document in airplay mode
 */
- (BOOL)isAirplay;

/**
 * Is the document a VI document
 *
 * @Return If the document is a VI document
 */
- (BOOL)isVIDocument;

/**
 * Return if Airplay finished rendering
 *
 * @return If Airplay finished rendering
 */
- (BOOL)isAirplayRendered;

/**
 * Reset delegate to avoid accessing released instance
 */
- (void)clearDelegate;

/**
 * Return dashboardName
 *
 * @return Dashboard's name
 */
- (NSString*) dashboardName;

- (BOOL)hasTransactionUpdate;


/**
 * Return all datasets' Ids
 *
 * @return all datasets' Ids
 */
-(NSArray*)getAllDataSetIDs;

/**
 * Return all datasets' prompt answers
 *
 * @return all datasets' prompt answers
 */
-(NSDictionary*)getDatasetPromptAnswers;

/**
 * Re-evaluate views after UC
 *
 * @return result code
 */
-(NSInteger)ReevaluateViewsForUC:(NSArray*)nodeKeys;

/**
 * Ask the viewer to stop rendering process, including background thread
 *
 * @return Whether the rendering has been stop instantly or not. If not, there will another call back later.
 */
- (BOOL)stopRenderingProcess;

/**
 *Wait back ground render process to stop.
 */

- (void)waitRenderingProcess;

//for new ui navigation bar
- (NSArray*) getAllLayoutNames;

- (NSArray*) getAllLayoutKeys;

- (NSString*) getCurrentLayoutName;

/**
 *Widgets views status record/restore
 */
- (NSData*)getWidgetViewsStatus;
- (int)replayWidgetViewsStatus:(NSData*) status;


//TQMS 946437: Delay the manipulation until view evaluation finishes for smart client document
- (void)viewEvaluationFinish;

@end


@protocol WidgetGCDWaitProtocol<NSObject>
- (BOOL)couldStopRenderingProcess;
/**
 * This API tell the view that the view data model will change, So if there is any background draw process, you need to stop them to wait new data is applied.
 */
- (void)viewerDataModelWillChange;

/**
 * This API tell the view that the view data model has finish update the mode, you could resume some process, e.g., background drawing.
 */
- (void)viewerDataModelDidChange;

/**
 * This API tell the viewer that the data model become invalidated, e.g., clear on close = yes, send App to background
 */
- (void)viewerDataModelBecomeInvalidated;
@end



