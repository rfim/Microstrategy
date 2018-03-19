//
//  RWDDashboardDelegate.h
//  MSTRMobile
//
//  Created by Ji Jin on 4/17/10.
//  Copyright 2010 MicroStrategy Inc. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "DSSIPhoneEnums.h"
#import "MSIURLHelper.h"
#import "Enums.h"

@class MSIReportDisplayInfo;
@class MSIURLHelper;
@class IServerCacheInfo;
@class TransactionParameters;
@class IPhoneInfoWindowViewController;
@class TransactionParameters;
@class LayoutManagerController;
@class SearchSelectionViewController;
@class NavigationTabBarController;
@class LinkDrillParameters;
@class DrillXMLCommandParameters;
@class LocationContext;
@class InfoWindowControllerImpl;
@class MSIManipulationParameter;
@class PopoverContext;
@class Command;

typedef enum _EnumDocumentState{
    DocumentStateBeforeRendering = 0,
    DocumentStateAfterRendering = 1,
    DocumentStateDuringRefreshing = 2,
    DocumentStateBeforeTransactionRefresh = 3
} EnumDocumentState;

@protocol BinaryRetrievalHandler<NSObject>
- (void)OnRetrievalFinished:(BOOL)iSuccessful withSelector:(SEL)callbackFunction;
@end

@protocol LayoutManagerControllerCommanderDelegate <NSObject>
// Called before LayoutManagerController is dealloced to clear reference.
-(void) clearMobileVIControllerViewRef;
@end

@protocol RWDDashboardDelegate<NSObject>

- (CGSize)availableSize;
- (BOOL)isLandscape;
- (BOOL)isClosedDocument;
- (MSIReportDisplayInfo*) getDocument;
- (void)applyManipulation:(MSIManipulationParameter*)manipulationParameters;
- (void)applyDrillManipulationWithParameters:(DrillXMLCommandParameters*) drillParameters;
- (void)updatePreviewImages:(NSDictionary*)images;
- (void)handleLinkDrill:(MSIURLHelper*)urlHelper;
- (void)handleLinkDrill:(MSIURLHelper*) urlHelper withLocationContext:(LocationContext *)locationContext;

//DL;10/16/2012;668506;support document refresh
- (void)handleDocumentRefresh:(RefreshDocumentEventOption)refreshDocumentEventOption withClientManipulations:(NSString*)clientManipulations;

- (BOOL)validateCacheHandleLinkDrill:(MSIURLHelper*)urlHelper;
- (void)applyLinkDrillWithParameters:(LinkDrillParameters*) linkDrillParams;
- (BOOL)validateCacheApplyLinkDrillWithParameters:(LinkDrillParameters*) linkDrillParameters;
- (void)submitTransactionDataManipulation:(TransactionParameters*) iParams;
- (void)submitTransactionDataManipulation:(TransactionParameters*) iParams showWaitView:(BOOL)iShowWaitView;
- (void)showPageByModalViewWithElements:(NSArray*)elements AndSelectedIndex:(int)index AndTitle:(NSString*)aTitle AndSelector:(id)selector;

// for info window, by njiang
- (void)setNavigationBarButtonEnabled:(BOOL)enabled;
- (void)setValue:(id)iValue forKey:(NSString *)key;
- (id)valueForKey:(NSString*)key;

//schatterjee 2011-03-30
- (void)handleActionSheet:(MSIURLHelper*)urlHelper;

// mzhu 2010-06-17
- (void)getImagesFromIServer:(NSArray*)iImagesArray forTimeStamp: (NSArray*) iImagesTimeStamp forAsync:(BOOL)ibAsync;

// 1:Portrait only 2:Landscape only 3:Portrait & Landscape
- (void)setSupportedOrientations:(NSInteger)iOrientations;
- (void)setOnlyLandscapeLayouts:(BOOL)iOnlyLandscapeLayouts;
- (BOOL)getOnlyLandscapeLayouts;//793295

- (int)getDisplayMode;
- (void)setDisplayMode:(int)iDisplayMode;
- (BOOL)isMultiSelectionMode;
- (void)setIsMultiSelectionMode:(BOOL)iMultiSelectionMode;
- (void)releaseData;

//lliu 2011-2-22 iPhone feature
- (void)nextLayout;
- (void)previousLayout;
- (void)toggleFullScreen;
- (BOOL)isFullScreen;
- (void)getEmptyGridDisplay:(NSString**) emptyMessage display:(BOOL*) displayEmptyMsg;

//jjin incremental download
- (void)requestMissingBinaryBlock:(NSString*)layoutKey;
- (void)requestMissingBinary:(NSString*)iLayoutKey withHandler:(id<BinaryRetrievalHandler>)iHandler withSelector:(SEL)callbackFunction;
- (void)requestMissingBinarySync:(NSString*)iLayoutKey forceUseDeviceCache:(BOOL)iForceUseDeviceCache;

//hasun 2011-4-15 transaction modification
- (void)handleTransactionModification;

- (BOOL)isDuringBackward;
- (BOOL)hasSetLayoutManipulationDuringBackward; // whether there is setLayout manipulation in binary collector
- (void)disableCacheCheck;

- (void) showWaitViewOnTop;
- (void) dismissWaitViewOnTop;
- (void)dataModelCreationFailed;

//TQMS 511934: Hide/show navigation bar right item when drill radar shows/disappears
-(void) hideRightNavigationBarButtons;
-(void) showRightNavigationBarButtons;

// yzeng 3/19/2012 mobile VI
-(LayoutManagerController*) layoutManagerController;


//VI document controller APIs
- (void) saveAsVIDocument:(NSString*)folderID andDocName:(NSString*)docName andDocDesc:(NSString*)docDescription andSFlag:(EnumDSSXMLDocSaveAsFlags)flag;

- (void) createNewFolder:(NSString*)parentFolderID andFolderName:(NSString*)folderName andFolderDesc:(NSString*)folderDescription;

-(CGRect) getSDKFrame;
-(BOOL) getInSDKMode;
-(BOOL) getSDKFitWidth;
-(BOOL) getSDKRefresh;
-(BOOL) getIsInPortrait:(BOOL)fallbackValue;

-(BOOL) getIsDeviceLocked;
- (void)showIVEPlayActionSheet;
- (void)hideActionSheet;
- (void)showIVEActionSheetForControlMode;
- (void)showIVEActionSheetForFilterMode;
- (void) updateFrame;

- (UIViewController*) getViewController;

//Carryover GB selections
- (void)updateGroupbySelectedElementsWithAttribute:(NSString*)attributeId AndSelectedElement:(NSString*)elementId;

- (BOOL)getShouldRotate;
- (void)setShouldRotate:(BOOL)shouldRotate;
- (void)displayPanelFilterButton:(BOOL)display; //hide/display the filter button when the filters panel is expanded/collapsed
- (void)displayFiltersPanelButton:(BOOL)display; /// hide/display normal doc which contains VI style panelview
// For search selector display search view in full screen.
- (void)restoreButtonActionBySearchView:(id)sender;

- (void)manipulationsApplied:(BOOL)isNormalManipulation;
//TQMS 675461, expose an API for viewer to disable the iPhone layout bar. By xliu, 2/20/2013.
- (void)disableControlsDuringAnimation:(BOOL)disable;

/** On iPhone in landscape mode, hide top bar and bottom bar if they are visible.
 * @param animated YES to animate. NO to go to full screen immediately.
 */
- (void)fullScreenWithAnimation:(BOOL)animated;

/// Return whether it is rendering navigation document.
- (BOOL)isRenderingNavigationDocument;

//by lishan stats log
-(void)handleRenderFinishTime:(NSDate *)date;

@optional

//yuazhang support custom event for link drill
- (void)handleCustomEvent:(NSString*)url;

//hasun 2010-12-8
- (void)handlePDF:(MSIURLHelper*)urlHelper;

- (void)handleEPUB:(MSIURLHelper*) urlHelper;
- (void)handleEPUB:(MSIURLHelper*) urlHelper withSender:(UIView *) iView;
//by lishan stats log
-(void)handleManipulationCount;//record mainp count when execute
-(void)handleManipRenderStartTime:(NSDate*)date;

-(void)handleManipStatsLog:(Command*)cmd WithTime:(NSDate*)date;
-(void)handleManipFinish:(NSDate*)date;
-(void)handleManipName:(NSString*)name;
-(void)handleManipValue:(NSString*)value;
-(void)handleManipDetails1:(NSString*)detail1;

-(void)handleManipStatsLogInfoWindowFromView:(UIView*)iView WithTime:(NSDate*)date;
-(void)handleManipStatsLogInfoWindowFromCmd:(void*)cmd context:(PopoverContext*)context WithTime:(NSDate*)date;

-(void)handleManipInfoWindowFinish:(NSDate*)date;//for infowindow pop success
-(void)handleManipInfoWindowDetails1:(NSString*)detail1;//for infowindow panel stack title

-(void)manipulationFinished;

/// Return navigation tab bar controller.
- (NavigationTabBarController *) navigationTabBarController;
/// Has navigation tab bar.
- (BOOL)hasNavigationTabBar;
/// Navigation tab bar is on screen.
- (BOOL)isNavigationTabBarOnScreen;
- (NSString*)getSelectedElementOfAttribute:(NSString*)attributeId;

/** 
 * Call back after all background rendering has been finished/stopped
 * This call back will only be triggered if the data controller first call stopRenderingProcess
 */
- (void)renderingProcessEnded;

- (BOOL)isInitialLayoutLoaded;

-(void)saveClientManipulation:(NSString*)events;

- (void)setDuringPersistOrCarryover:(BOOL)isDuringPersistOrCarryover;

- (BOOL)isHandlingShowingPrompt;

- (BOOL)shouldDelayManipulation;

// This is used for identify it is a smart client;
- (BOOL)isSmartClient;

// This is used to pass the frame of transaction submission button when submit offline for animation, US24911.
- (void)setSubmissoinButtonFrame:(CGRect)sender;
- (void)setAllowOfflineSubmission:(BOOL)isAllowed;

// Mobile Export
- (BOOL)isExporting;
- (void)handleWillQuitExport:(void (^)())handler;

// used to pass the documentState.
- (EnumDocumentState) getDocumentState;

// used for external URL API
- (void)setIsLinkDrill:(BOOL)isLinkDrill;

@end
