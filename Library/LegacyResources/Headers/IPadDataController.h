/*
 *  IPadDataController.h
 *  MSTRMobile
 *
 *  Created by Amirali Charania on 1/27/10.
 *  Copyright 2010 MicroStrategy. All rights reserved.
 *
 */

#import "DSSIPhoneEnums.h"
#import "Enums.h"

#define RIGHT_DIRECTION 0
#define LEFT_DIRECTION 1
#define UP_DIRECTION 2
#define DOWN_DIRECTION 3

#define PAGE_BY_LIST_STYLE 0
#define PAGE_BY_PICKER_STYLE 1

#define ANIMATION_NONE 0
#define ANIMATION_LEFT 1
#define ANIMATION_RIGHT 2
#define ANIMATION_UP 3
#define ANIMATION_DOWN 4


#define POSITION_TOP 0
#define POSITION_BOTTOM 1

@class MSIURLHelper, MSIReportDisplayInfo, MSIObjectInfo;
@class DefaultProjectProperties;
@class TransactionParameters;
@class IPhoneInfoWindowViewController;
@class LayoutManagerController;
@class CarryOverSelectionsSet;
@class LocationContext;

@protocol IPadDataController

/**
 * Display error
 *
 * @param errorMessage Error message to be displayed
 */
- (void)displayError:(NSString*)errorMessage;

/**
 * Show or hide data viewer
 *
 * @param visibility Boolean for visibility
 */
- (void)setDataViewerVisibility:(BOOL)visibility;

/**
 * 	This should be triggered when the data view is loaded or is set to be visible.
 */
- (void)dataViewLoaded;


/**
 * Set the report or document
 *
 * @param report Document or report to be displayed
 */
- (void)setReport:(MSIReportDisplayInfo*)report;
- (MSIReportDisplayInfo*)report;

/**
 * Return base URL
 *
 * @return Base URL
 */
- (NSString*)baseURL;

/**
 * Show reprompt interface
 *
 * @param attribute Prompt info
 * @param searchRequired If require search
 * @param multiSelect Allow multiselect
 */
- (void)displayPromptWithAttribute:(MSIObjectInfo*)attribute searchRequired:(NSInteger)searchRequired multiSelect:(NSInteger)multiSelect;

/**
 * Handle URL event
 *
 * @param urlParameters URL dictionary
 */
- (void)handleEventWithParameters:(NSDictionary*)urlParameters;

/**
 * Handle URL event
 *
 * @param urlParameters URL dictionary
 * @param locationContext Context of the URL event
 */
- (void)handleEventWithParameters:(NSDictionary*)urlParameters withLocationContext:(LocationContext *)locationContext;

/**
 * Return layout manager controller for mobile VI
 *
 * @return The layout manager controller
 */
- (LayoutManagerController*) layoutManagerController;   

@optional

/** 
 * Get web server type (J2EE or ASP)
 *
 * @return Web server type
 * @see WebServerTypes
*/
- (int)getWebServerType;

/** 
 * Add layout indicator
 *
 * @param layoutIndicator Layout indicator view
 */
- (void)addLayoutIndicator:(UIView*)layoutIndicator;

/** 
 * Get current layout index
 *
 * @return Current layout index
 */
- (short) getCurrentLayoutIndex;

/** 
 * Restore data controller state
 *
 * @return If restore succeed
 */
- (BOOL)restoreContents;

/** 
 * Restore offline transaction content
 *
 * @param docID Report/Document ID
 * @param timeStamp Offline transaction timestamp
 */
- (void) restoreTransactionContents:(NSString*)docID andTime:(NSString*)timeStamp;

/** 
 * Get settings for display message in grid when grid is empty
 *
 * @param emptyMessage Message to be displayed when grid is empty
 * @param displayEmptyMsg If we will display the message
 */
- (void)getEmptyGridDisplay:(NSString**) emptyMessage display:(BOOL*) displayEmptyMsg;

/** 
 * Resubmit binary data service for refreshing
 *
 * @param showMessage Prompt user about the refreshing or not
 */
- (void)resubmitDataService:(BOOL) showMessage;

/** 
 * Resubmit binary data service for refreshing
 *
 * @param showMessage Prompt user about the refreshing or not
 * @param suppressNetworkErrorAlerts Suppressing error message
 * @param ignoreCache Should refreshing ignore device cache
 */
- (void)resubmitDataService:(BOOL) showMessage suppressNetworkErrorAlerts:(BOOL)suppressNetworkErrorAlerts ignoreDeviceCache:(BOOL)ignoreCache;

/**
 * This method will setup the left bar button of the navigation bar for this controller.
 *
 * @param homeButton ignore.
 */
- (void)setHomeButton:(UIBarButtonItem*)homeButton;

/** 
 * Disable viewers cache. This will sacrifice performance for memory
 */
-(void)disableViewerCache;

// by lishan stats log
-(void)handleRenderFinishTime:(NSDate *)date;
-(void)handleManipulationCount;
-(void)handleManipRenderStartTime:(NSDate*)date;
-(void)setReportQueueRecall:(BOOL)isRecall;
//VI document controller APIs  // Useless , commented by wenzhou, 05/19/2015.
//- (void) saveAsVIDocument:(NSString*)folderID andDocName:(NSString*)docName andDocDesc:(NSString*)docDescription andSFlag:(EnumDSSXMLDocSaveAsFlags)flag;
// sowang VI document previewImages
//-(void)setPreviewImages: (NSDictionary*) perviewImages;

/**
 * Apply a manipulation selection to the document
 * 
 * @param iSelections An archived carryover selection set
 */
- (void) executeManipulationSelections:(NSData*)iSelections;

/**
 * Display a list of conference servers for user to join
 *
 * @param servers A list of server names
 */
- (void)displayConferences:(NSArray*)servers;

/**
 * Return the current selections from the document
 *
 * @param mode Whether to carry over by name or ID
 * @return The set of selections in the current document
 */
- (CarryOverSelectionsSet*) getCurrentCarryOverSelections:(CarryOverSelectedMode)mode;

/**
 * Rerun the document
 *
 * @param ignoreDeviceCache Whether we want to reuse device document cache or ignore it
 */
- (void) rerunDocument: (NSNumber*) ignoreDeviceCache;

@end
