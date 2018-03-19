//
//  GlobalStateManager.h
//  MSTRMobileSDK
//
//  Created by Tejas Ranade on 5/4/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Service.h"


extern NSString *const MSTRClearOnCloseNotification;
extern NSString *const NetworkStatusDidChangeNotification;
@class MSIProjectInfo;
@class MSIProjectConfiguration;
@class ErrorDetails;
@class NewsstandDownloadManager;
@class TransPendingListModel;
@class DSSWCManager;
@class MSMBackgroundTransferManager;
@class WebServerConfiguration;
@class MSIAlertView;

@protocol PushNotificationListener

-(void)handleCleanup:(id)obj;

@end

/**
 * This class represents the state that we need to maintain globally 
 * during the execution of any MicroStrategy based app.
 **/
@interface MSIAppContext : NSObject

// For Reconcile Category
@property (nonatomic, readonly) NSArray* unreconciledProjects;

// Notification Info
@property (nonatomic, strong) NSDictionary* notificationInfoDictionary;
@property (nonatomic, weak) id<PushNotificationListener> PNListener;

// Compatibility Flag
@property BOOL compatibilityErrorHandled;

// Encryption
@property BOOL softwareEncryptionEnabled;

// DSS WC Manager
@property (nonatomic, strong) DSSWCManager* mDSSWCManager;

// Newsstand
@property (nonatomic, strong) NewsstandDownloadManager* mNewsstandDownloadMgr;
@property BOOL isLaunchedByNewsstandPush;
@property BOOL reconcileInitiatedBySilentNotification;
@property (nonatomic, strong) MSMBackgroundTransferManager* mMSTRBackgroundTransferMgr;
@property (atomic) UIBackgroundTaskIdentifier bgTask;
// is Running task
@property BOOL isRunningTasks; //DL;06/29/2012;ASYC EXEC;indicate app is running tasks now.

@property  (nonatomic) BOOL showMobileLoginPromptAutomatically;

@property (atomic, strong) UINavigationController* customizedNavigationController;

+ (MSIAppContext*) getInstance;

// Display Error in Alert
- (void)displayErrorInAlertViewWithErrorDetails:(ErrorDetails*)errorDetails;
- (void)displayErrorInAlertViewWithTitle:(NSString*)title message:(NSString*)message;
- (void)displayErrorInAlertViewWithTitle:(NSString*)title message:(NSString*)message delegate:(id)delegate;
- (void)displayErrorInAlertViewWithTitle:(NSString*)title message:(NSString*)message delegate:(id)delegate dismissPrevious:(BOOL)dismiss;
- (void)displayErrorInAlertViewWithTitle:(NSString*)title message:(NSString*)message delegate:(id)delegate dismissPrevious:(BOOL)dismiss withOtherButton:(BOOL)otherButton;
- (void)displayErrorInAlertViewWithTitle:(NSString*)title message:(NSString*)message delegate:(id)delegate dismissPrevious:(BOOL)dismiss withOtherButton:(BOOL)otherButton okButtonTitle:(NSString*)okButtonTitle otherButtonTitle:(NSString*)otherButtonTitle;
- (void)displayErrorInAlertViewWithTitle:(NSString*)title message:(NSString*)message delegate:(id)delegate dismissPrevious:(BOOL)dismiss withOtherButton:(BOOL)otherButton okButtonTitle:(NSString*)okButtonTitle otherButtonTitle:(NSString*)otherButtonTitle cancelButtonTitle:(NSString*)cancelButtonTitle;
- (void)displayErrorInAlertViewWithTitle:(NSString*)title message:(NSString*)message blocks:(void (^)(MSIAlertView * alertView,NSInteger buttonIndex))completionHandler dismissPrevious:(BOOL)dismiss withOtherButton:(BOOL)otherButton okButtonTitle:(NSString*)okButtonTitle otherButtonTitle:(NSString*)otherButtonTitle cancelButtonTitle:(NSString*)cancelButtonTitle;
- (void)dismissErrorAlertView;
//to avoid the crash that involved by AlertView is dismissed after the delegate is dealloc
- (void)dismissErrorAlertView:(id)delegate;
- (void)enableErrorAlertView:(BOOL)enable;
- (void)dismissErrorAlertViewWithErrorDetails:(ErrorDetails*)errorDetails delegate:(id)delegate;

// Display Flosting View
/**
 Displays the floating view used to indicate some activity like Loading... Pivoting... or Sorting...
 This method is used in conjunction woth hideFloatingView to hide and remove the shown floating view
 @param view the view in which the floatingView is added
 @param position, the position of the floating view. The value could be TOP, CENTER or BOTTOM
 **/
- (void)displayFloatingViewOnView:(UIView*)view withText:(NSString*)text viewHastoobar:(BOOL)viewHastoobar hasActivityIndicator:(BOOL)hasActivityIndicator position:(NSInteger)pos;
- (void)displayFloatingView:(NSString*)text viewHastoobar:(BOOL)viewHastoobar autoHide:(BOOL)autoHide;
- (void)displayFloatingView:(NSString*)text viewHastoobar:(BOOL)viewHastoobar;
//Hides and removes the floating view which was shown by displayFloatingView
- (void)hideFloatingView;

// Network is Reachable
- (BOOL)networkIsReachable;	//Indicate whether network is reachable, which will be influenced by offline mode setting

//Cache Cleaning Methods
/**
 * Delete all caches of the app. This method won't clear certificate and encryption key
 **/
- (void)deleteAllCaches;
/**
 * Clear all caches of the project.
 * @param projectConfig: the project whose caches will be deleted
 **/
- (void)deleteProjectCaches:(MSIProjectConfiguration *)projectConfig;
/**
 * Delete caches for all projects in this server
 * @param wsConfig: the web server whose caches will be deleted
 **/
- (void)deleteServerCaches:(WebServerConfiguration *)wsConfig;
/**
 * Clear some caches can't be deleted at project level, like tmp directory, ...
 **/
- (void)deleteNotProjectLevelCaches;
/**
 * Update deferred tasks after project updated.
 **/
- (void)updateDeferredTaskAfterProjectUpdated;

// Display Loading Indicator
- (void) displayLoadingIndicatorOnView:(UIView*)view customView:(UIView*)customView text:(NSString*)text textLayoutHorizontal:(BOOL)textLayoutHorizontal;
- (void) displayLoadingIndicatorOnView:(UIView*)view customView:(UIView*)customView text:(NSString*)text;
- (void) hideLoadingIndicatorOnView:(UIView*)view;

// Preprocess MSIImageCache to get bundled images
- (void)prepareImageCache;
- (void)prepareImageCache:(bool)asynch;

// Configuration Autoupdate
- (void)setupPeriodicConfigurationAutoupdateCheck;

// Stand Alone Service
- (void)startStandAloneService:(id<Service>)service;

// this takes care of hiding after a certain time
- (void)displayAndHideMessage:(NSString*)text;

// get the pending transactions modal
- (TransPendingListModel*)getTransPendingListModel;

// Unconciled Projects
- (void)addToUnconciledProjects:(MSIProjectInfo*)project;

// Number and Date Locale
- (void)setNumberAndDateLocaleId:(NSString*) lId;
- (NSLocale*)numberAndDateLocale;
- (NSCalendar*)getCalendar;
- (NSCalendar*)getAutoupdatingCurrentCalendar;// Return the autoupdatingCalendar witch concurrent with iPad/iPhone -> Settings->General->Region/Language. By wenzhou, 12/15/2015. US31530.



// Mail and Http
- (void)handleMailto:(NSURL*)url;
- (void)handleHttp:(NSURL*)url;

// Notification
- (void)registerForNotificationWithAPN;
- (void)unRegisterForNotificationWithAPN;

// Device Token
- (void)removeDeviceTokenforProject:(MSIProjectConfiguration *)projectConfig;

// Newsstand
- (BOOL)isNewsstandEnabled;
-(void)endExtendedBackgroundTask;
-(void)startConfigurationUpdate;
-(void)dismissConfigurationUpdateAlert;
- (void)showOfflineBanner;
- (void)removeAllModalViewControllers;

- (void)logout;
-(DSSWCManager*)mDSSWCManager;
-(MSMBackgroundTransferManager*)mMSTRBackgroundTransferMgr;
@end
