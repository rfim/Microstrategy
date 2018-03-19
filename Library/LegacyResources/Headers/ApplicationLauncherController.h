//
//  ApplicationLauncherController.h
//  MSTRMobile
//
//  Created by Amirali Charania on 4/9/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "Service.h"

#define HOME_SCREEN_IMAGES_ID @"HOME_SCREEN_IMAGES"

@class DisplayObject, HelpController, MSIProjectInfo, MSIDevicePreferences;

/**
 * ApplicationLauncherController is a controller used when application is launched for the first time or when user applies a new configuration URL or receive a remote notification. 
 * When it's displayed, it creates the home screen view controller, after home screen controller is created successful, it will replace itself with the home screen view controller.
 */
@interface ApplicationLauncherController : UIViewController <ServiceHandler> {
@private
	NSURL* url;
	NSDictionary* notificationInfoDictionary;
	NSMutableData *xmlData;
	BOOL immediatelyLoad;
	BOOL postponedLoad;
	NSString* xmlURL;
	NSInteger authMode;
	NSInteger deviceType;
	BOOL isLoginPromptAlive;
	id<Service> service;
	BOOL isNewLauncher;
	bool mIsStartupFromURL;
	BOOL isCertificatePromptAlive;
	NSString *login;
	NSString *password;
	NSString *certificateServerURL;
	UIImageView *splashImageView;
	UIViewController *startupViewController;
	BOOL isLoginPromptDisplayed;
	BOOL applicationLauncherInitialized;
    UIViewController *launchScreenViewController;
    BOOL launchWithDemoConfigurationURL;
    BOOL displayErrorAlertWhenDisappeared;
    NSString *errorAlertTitle;
    NSString *errorAlertMessage;
    BOOL isOfflineModeDisabled;
}

/**
 * Initializer when launch application with a configuration URL. It calls the default initializer first.
 * @parameter _url configuration URL
 * @return a new instance of ApplicationLauncherController
 */
- (id)initWithURL:(NSURL*)_url;

/**
 * Initializer when launch application with a demo configuration URL. It would show the intro animation if enabled. It calls the default initializer first.
 * @parameter _url demo configuration URL
 * @return a new instance of ApplicationLauncherController
 */
- (id)initWithDemoConfigurationURL:(NSURL*)_url;

/**
 * Initializer when launch application by remote notification. It calls the default initializer first.
 * @parameter notificationInfoDictionary remote notification contains a subscription id in the notification info dictionary with key sid
 * @return a new instance of ApplicationLauncherController
 */
- (id)initWithNotificationInfoDictionary:(NSDictionary*)notificationInfoDictionary;

/**
 * Launch application with a designated view controller on top. Below this view controller is the home screen controller.
 * @param viewController the designated view controller to be displayed on top
 */
- (void)launchWithViewController:(UIViewController *)viewController;

/**
 * Return YES if intro animation is enabled, otherwise return NO.
 *
 */
- (BOOL)isIntroAnimEnabled;

/**
 * Create a view controller to display the object of the resultset home screen settings
 * @param displayObject display object of result set home screen settings
 * @return a new view controller
 */
+ (UIViewController*)createViewControllerForHomeScreenDisplayObject:(DisplayObject*)displayObject;

/**
 * Create a view controller to display the subscription report
 * @param subscriptionID ID of subscription to display
 * @return a new view controller
 */
+ (UIViewController*)createViewControllerForSubscriptionID:(NSString*)subscriptionID;

/**
 * Create a HelpController
 * @return a new HelpController
 */
+ (HelpController*)newHelpController;

/**
 * Load with Preferences data directly from Bundle - Used in Device Password mode.
 */
- (void)loadWithPreferencesData:(NSData*)preferencesData;

/**
 * Application URL
 */
- (NSURL*)url;

@property (nonatomic) BOOL postponedLoad;

@end