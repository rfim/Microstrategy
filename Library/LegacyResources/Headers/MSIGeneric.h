//
//  Generic.h
//  MSTR Foundation
//
//  Created by Amirali Charania on 2/25/09.
//  Copyright 2009 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <CoreLocation/CoreLocation.h>
#import "Enums.h"

@class MSTRError;

#define EMPTY_STRING @""
#define VISUALIZATION @"70"
#define VISUALIZATION_PDF @"-1"
#define VISUALIZATION_RW_MOJO @"-2"
#define VISUALIZATION_REPORT @"IPhoneReportStyle"
#define VISUALIZATION_INTERACTIVE_GRID @"IPhoneInteractiveGridStyle"
#define VISUALIZATION_TIME_SERIES @"IPhoneTimeSeriesStyle"
#define VISUALIZATION_MAP @"IPhoneMapStyle"
#define VISUALIZATION_DATA_CLOUD @"IPhoneDataCloudStyle"
#define MSTRSDK_BUNDLE @"FinalBundle.bundle"
#define GUID_NULL_STRING @"00000000000000000000000000000000"

@class WebServerConfiguration, MSIProjectInfo, MSIAppContext, DisplayObject, MSIReportDisplayInfo, MSICertificateManager;


@protocol DeprecatedUIApplicationMethods

	- (void)setStatusBarHidden:(BOOL)hidden animated:(BOOL)animated;
	- (BOOL) locationServicesEnabled;

@end

/**
 * SDKEnvSettings allows user to control some behavior of MicroStrategy SDK by expose settings to SDK user.
 * SDK user can set settings value, and SDK will use these settings inside.
 */
//dali:12/07/2011, add SDK envrionment settings for user >>>>>>>>
@interface SDKEnvSettings : NSObject{
@private

    //settings will be handled by sdk
    BOOL isUserDeviceUUID;
	BOOL isEnforceShowCloseForRetry;
	BOOL isNavBarItemsSuppressed;		// If YES, only suppress navigation bar items, not navigation bar itself
	BOOL isEnforceEnableDocumentZoom; // This setting will overwrite the document level setting of enabling zoom by pinch or double tap. It will directly set mEnableZoomByPinchOrDoubleTap in RWDDashboardView as true.
    
    //dictionary contains parameters for mobile server
    NSMutableDictionary* mServerParamsDict;
    
    //dictionary contains customized request header
    NSMutableDictionary* mCustomRequestHeaderDict;
    
    //FBConnect
    BOOL isUsingFBConnect;
    CGFloat zoomFactor;
  }

/**
 * Use this property to determine whether to send the device UUID to server in the login request.
 * Server side can do statistic analysis about operations from a device.
 * Default is NO.
 * By default, device UUID is sent only when it's connecting to MicroStrategy demo project.
 */
@property BOOL isUserDeviceUUID;

/**
 * Use this property to determine whether to show a "Close" button in an alert besides the "Retry" button when execution document fails.
 * Default is NO.
 */
@property BOOL isEnforceShowCloseForRetry;

/**
 * Use this property to determine whether the button items in the navigation bar of data controller is hidden.
 * Default is NO.
 */
@property BOOL isNavBarItemsSuppressed;

/**
 * Use this property to enable document zoom which will overwrite the document level setting of enabling zoom by pinch or double tap.
 */
@property BOOL isEnforceEnableDocumentZoom;

/**
 * Use this property to determine whether facebook connect is enabled.
 * If enabled, there will be a new "Facebook" authentication mode for project.
 * Default is NO.
 */
@property BOOL isUsingFBConnect;

/**
 * Use this property to set the zoom factor of the document view
 * Default is 1.
 */
@property CGFloat zoomFactor;

/**
 * Use this property to get or set the facebook app id.
 * If facebook connect is enabled, fbConnectAppID must to specified.
 */
//@property (nonatomic, copy) NSString* fbConnectAppID;

/**
 * set extra parameter used by server login task
 * @param aValue parameter value
 * @param aKey parameter key
 */
-(void)setServerParam:(NSString*)aValue forKey:(NSString*)aKey;

/**
 * Get all keys in the server parameters dictionary.
 * @return all keys
 */
-(NSArray *)getServerParamKeys;

/**
 * Get a server parameter value for a key
 * @param aKey key
 * @return value
 */
-(NSString*)getServerParamForKey:(NSString*)aKey;

/**
 * Remove a server parameter value for a key
 * @param aKey key
 */
-(void)removeServerParamForKey:(NSString*)aKey;

/**
 * Remove all server parameters
 */
-(void)clearServerParams;


/**
 * set customized request header for network requests
 * @param value customized request header's value
 * @param field customized request header's field
 */
- (void)setValue:(NSString *)value forHTTPHeaderField:(NSString *)field;

/**
 * Get all fields in the customized request header dictionary.
 * @return all fields
 */
-(NSArray *)getHTTPHeaderFields;

/**
 * Get a customized request header value for a field
 * @param field field
 * @return value
 */
-(NSString*)valueForHTTPHeaderField:(NSString*)field;

/**
 * Remove a customized request header value for a field
 * @param field field
 */
-(void)removeHTTPHeaderField:(NSString*)field;

/**
 * Remove all customized request headers
 */
-(void)clearHTTPHeaderFields;

@end
//dali:12/07/2011, add SDK envrionment settings for user <<<<<<<<<<

/**
 * It is a utility class which provides convienent methods to be used.
 */
@interface MSIGeneric : NSObject {
	
}

/**
 * Get SDK environment settigs
 * @return SDKEnvSettings
 */
+(SDKEnvSettings*)getSDKEnvSettings;

/**
 * Get string representation of the current date
 * @return date string without time
 */
+(NSString*)getCurrentDate;

/**
 * Get string representation of the current time
 * @return time string without date
 */
+(NSString*)getCurrentTime;

/**
 * Get all the URL escape characters
 * @return string containing all the URL escape characters
 */
+(NSString*)getURLEscapeCharacters;

/**
 * Get all the URL escape characters for Image Path
 * @return string containing all the URL escape characters
 */
+(NSString*)getURLEscapeCharactersForImagePaths;

/**
 * Convert GMT time to the time in current timezone
 * @param gmtTime GMT time
 * @return current timezone time
 */
+(NSTimeInterval)gmtToCurrentTimezone:(NSTimeInterval)gmtTime;


/**
 * Get the base url of a WebServerConfiguration
 * The URL is composed of request type, server name, port and path, example : http://123.43.43.43:8080/MicroStrategyMobile
 * @param webserverConfig WebServerConfiguration
 * @return base URL for the web server
 */
+(NSString*)getBaseURL:(WebServerConfiguration*)webserverConfig;

/**
 * Get the base url of a ProjectInfo and task
 * The URL is composed of request type, server name, port, path, servlet.
 * If it's a task URL, it contains the task name.
 * example, http://123.43.43.43:8080/MicroStrategyMobile/servlet/mstrWeb? if it't not a task
 * http://123.43.43.43:8080/MicroStrategyMobile/servlet/mobile?action=executeTask&taskParams= if it's a task
 * @param aProject project info used to get the WebServerConfiguration
 * @param task whether it's a task URL
 * @return base url for the project and task
 */
+(NSString*)getBaseURL:(MSIProjectInfo*)aProject task:(BOOL)task;

/**
 * Returns the int value of the object associated with the key. If not exists returns NSNotFound
 * @param dictionary dictionary created from connect info plist file
 * @param key key
 * @return int value for the key
 */
+ (NSInteger) intFromDict:(NSDictionary*) dictionary Key:(NSString*) key;

/**
 * Returns the float value of the object associated with the key. If not exists returns NSNotFound converted to float
 * @param dictionary dictionary created from connect info plist file
 * @param key key
 * @return float value for the key
 */
+ (CGFloat) floatFromDict:(NSDictionary*) dictionary Key:(NSString*) key;

/**
 * Returns the string value of the object associated with the key. If not exists returns empty string @""
 * @param dictionary dictionary created from connect info plist file
 * @param key key
 * @return string value for the key
 */
+ (NSString*) stringFromDict:(NSDictionary*) dictionary  Key:(NSString*) key;

/**
 * Get the base url of a WebServerConfiguration
 * The URL is composed of request type, server name, port and path, example : http://123.43.43.43:8080/MicroStrategyMobile
 * @param webserverConfig WebServerConfiguration
 * @return base URL string for the web server
 */
+(NSString*)getBaseURLByWebServerConfig:(WebServerConfiguration*)webserverConfig;

/**
 * Get the base url of a ProjectInfo
 * The URL is composed of request type, server name, port and path, example : http://123.43.43.43:8080/MicroStrategyMobile
 * @param projectInfo ProjectInfo to get the WebServerConfiguration
 * @return base URL string for the projectInfo
 */
+(NSString*)getBaseURLByProjectConfig:(MSIProjectInfo*)projectInfo;

/**
 * Gets the servlet path of a WebServerConfiguration
 * The URL is composed of request type, server name, port, path and server type.
 * example : http://123.43.43.43:8080/MicroStrategyMobile/servlet/ (J2EE)
 * http://123.43.43.43:8080/MicroStrategyMobile/asp/ (ASP)
 * @param webserverConfig WebServerConfiguration
 * @return servlet path of the web server
 */
+ (NSString*) getServletPath:(WebServerConfiguration*)webserverConfig;


/**
 * Gets the servlet path for a project info.
 * The URL is composed of request type, server name, port, path and server type.
 * example : http://123.43.43.43:8080/MicroStrategyMobile/servlet/ (J2EE)
 * http://123.43.43.43:8080/MicroStrategyMobile/asp/ (ASP)
 * @param projectInfo ProjectInfo to get the WebServerConfiguration
 * @return servlet path of the project
 */
+(NSString*)getServletPathByProjectConfig:(MSIProjectInfo*)projectInfo;

/**
 * Gets a url string for the task of specified name
 * The objectInfo is required to determine the web server base url, and the web server type (J2EE / ASP)
 * example : http://123.43.43.43:8080/MicroStrategyMobile/servlet/taskProc?taskId=taskName (J2EE)
 * http://123.43.43.43:8080/MicroStrategyMobile/asp/taskProc.aspx?taskId=taskName (ASP)
 * @param taskName task name
 * @param objectInfo objectInfo to get the project
 * @return task url
 */
+ (NSString*)taskURL:(NSString*)taskName withObjectInfo:(MSIReportDisplayInfo*) objectInfo;

/**
 * Determine whether a url string contains "HTTP" or "HTTPS"
 * @param url url string
 * @return YES for url contains "HTTP" or "HTTPS", otherwise NO
 */
+(BOOL)isAbsoluteURL:(NSString*)url;

/**
 * Determine whether a url string is a data url
 * @param url url string
 * @return YES for url starts with "data:", otherwise NO
 */
+(BOOL)isDataURL:(NSString*)url;

/**
 * Get the locale of this application, if it doesn't have locale, get the current locale
 * @return locale of this application or current locale
 */
+(NSLocale*)getNumberAndDateLocale;

/**
 * Set application locale. If application doesn't have locale, do nothing.
 * @param lId locale id
 */
+(void)setNumberAndDateLocaleId:(NSString*) lId;

/**
 * Get Gregorian Calendar.
 * Locale of this calendar is set to the application's locale if it exists.
 * @return calendar
 */
+(NSCalendar*)getCalendar;

/**
 * Get the autoupdatingCurrentCalendar witch concurrent with iPad/iPhone -> Settings->General->Region/Language. By wenzhou, 12/15/2015. US31530.
 * Locale of this calendar is set to the application's locale if it exists.
 * @return calendar
 */
+(NSCalendar*)getAutoupdatingCurrentCalendar;

/**
 * Returns the global application context object
 * @return MSIAppContext singleton object
 */
+ (MSIAppContext*)getMSTRAppContext;

/**
 * Updates state of the status bar. This API is meant to bridge the corresponding iOS4 API and its now deprecated OS3 counterpart. 
 * @param hidden hide status bar or not
 * @param animation whether do animation
 */
+ (void) setStatusBarHidden: (BOOL)hidden withAnimation:(BOOL)animation;

/**
 * Determine whether location service is enabled in this device
 * @param locationManager location manager
 */
+ (BOOL) locationServicesEnabled: (CLLocationManager *) locationManager;

/**
 * Check whether the device installs iOS4 and above
 * @return YES if iOS4 and above installed, otherwise NO.
 */
+ (BOOL) iOS4Device;

/**
 * Check whether the device and iOS support muti tasking
 * @return YES if device and iOS support multi tasking, otherwise NO.
 */
+ (BOOL) isMultitaskingSupported;

/**
 * Get the incremental fetch size
 * It is used to determine the grid rows to be fetch one time.
 * @return incremential fetch size
 */
+ (NSUInteger)incrementalFetchSize;

/**
 * Check the device type, it can be either iPhone or iPad
 * @return device type, it's enum value of DeviceType in Enums.h
 */
+ (DeviceType)getDeviceType;
+ (UIInterfaceOrientation)applicationInterfaceOrientation;

/**
 * Get the root navigation controller.
 * The Application Delegate should extends from MSIAppDelegate and set a navigation controller to it when a navigation controller is created for this application.
 * @return root navigation controller
 */
+ (UINavigationController*)applicationNavigationController;

/**
 * Get the application window
 * The Application Delegate should extends from MSIAppDelegate to get the window.
 * @return application window
 */
+ (UIWindow*)applicationWindow;

+ (UIViewController*)applicationTopModalViewController;

/**
 * Creates an error label with the specified string.
 * The containingViewFrame should provide the size of the view which this label will be added to.
 * @param errorDesc error string displayed in the label
 * @param viewFrame label frame
 * @return UILabel displays the error string
 */
+ (UILabel*)errorLabelForDesc:(NSString*)errorDesc containingViewFrame:(CGRect)viewFrame;

/**
 Returns the certificate manager
 **/
+ (MSICertificateManager *)getCertificateManager;


/**
 * Convert the authentication mode text to enum value.
 * @param _authMode project authentication mode text
 * @return enum value of AuthModes for the authentication mode
 */
+ (NSInteger) authenticationModeForString: (NSString* )_authMode;

/**
 * Convert the authentication mode text to enum value.
 * @param _authMode web server authentication mode text
 * @return enum value of WebAuthModes for the authentication mode
 */
+ (NSInteger) webAuthenticationModeForString: (NSString* )_authMode;

/**
 * Convert the web server type text to enum value.
 * @param appType web server type text, J2EE or ASP
 * @return enum value for the web server type
 */
+ (NSInteger) webServerTypeForString:(NSString*) appType;

/**
 * Get the root folder display object for a project. This information is stored in device preferences.
 * The folder object will be opened when the project is opened.
 * @param projInfo project to be found
 * @return root folder display object
 */
+ (DisplayObject*)getRootFolderDisplayObject:(MSIProjectInfo*)projInfo;

/**
 * Utility method to set up user preferences from a plist file in SDK based custom applications.
 * The username and password for the project are provided statically in the plist itself.
 * If a duplicate project or a duplicate webserver exists it updates the existing settings from the plist file.
 * @param plistFileName the name of the resource file that specifies the user's connection information (for example: "ConnectionInfo.plist")
 */
+ (void)setupConnectInfoFromPlist:(NSString *)plistFileName; 

/**
 * Utility method to set up user preferences from a plist file in SDK based custom applications.
 * The username and password for the project are provided at runtime.
 * If a duplicate project or a duplicate webserver exists it updates the existing settings from the plist file.
 * @param plistFileName the name of the resource file that specifies the user's connection information (for example: "ConnectionInfo.plist")
 * @param projUID the username for the project
 * @param projPwd the password for the project
 */
+ (void)setupConnectInfoFromPlist:(NSString *)plistFileName withProjUsername:(NSString*)projUID projPassword:(NSString*)projPwd;

/**
 * Utility method to construct an error object from an error string
 * @param details error message for the error details
 * @return error object
*/
+ (MSTRError*) errorWithErrorDetails:(NSString*)details;

/**
 * Check the support orientiation for certain type of event.
 * @return supported orientiation
 * iPad device supports both portrait and landscape for all the events.
 * iPhone device supports only portrait for SharedLibrary, HomeScreen, Help, Settings and Status.
 * @param eventType enum value of EventTypes
 * @return MSTROrientation supported
 */
+(NSInteger)supportedOrientationForEvent:(NSInteger)eventType;

/**
 * Get the bundle of name "MicroStrategyMobileSDKResources" from main bundle
 * @return sdk resource bundle
 */
+ (NSBundle *) sdkResourcesBundle;


/**
 * Get the bundle of name "FinalBundle" in resource
 * @return final bundle
 */
+ (NSBundle *) finalBundle;

/**
 * Get the bundle from resouce by the bundle name
 * @param bundleName bundle name
 * @return bundle
 */
+ (NSBundle *) bundleNamed:(NSString *)bundleName;

/**
 * This is the version number of the MicroStrategy API.  This is independent of the app version (though for OOTB MSTR Mobile it will be the same), and is generally used for version comparison.
 * @return api version
 */
+ (NSString *) apiVersion;

/**
 * Get the relativate path of a file in a bundle
 * @param fileName file name
 * @param bundleName bundle name where the file locates
 * @return the relative file path
 */
+ (NSString *)pathOfFileNamed:(NSString *)fileName inBundleNamed:(NSString *)bundleName;

/**
 * Check whether the current device is a recent device such as iTouch 4, iPhone4, iPad2 and iPad3.
 * @return YES if it belongs to the above devices, otherwise NO.
 */
+ (BOOL)isRecentDevice;

/**
 * Get the hardware platform of this device, {@link http://stackoverflow.com/questions/448162/determine-device-iphone-ipod-touch-with-iphone-sdk}
 * @return hardware platform string
 */
+ (NSString *) platform;

/**
 * Get the platform information String(include network type) of the current device such as "iPad 2 wifi"
 * @return platform information String(include network type)
 */
+ (NSString*)platformString;

/**
 * Get the iOS Main version number
 * @return iOS main version
 */
+ (NSInteger) iOSMainVersion;

/**
 Set file attribute to skip backup attribute
 @param URL Path of the file
 **/
+ (BOOL)addSkipBackupAttributeToItemAtURL:(NSURL *)URL;


/**
 * Get the UID of this application. It's a string of length 40.
 * The UID is created when it's accessed for the first time and saved to key chain.
 * It will be retrieved from key chain when it's accessed again.
 * It's used to replace the iOS UDID since it's deprecated.
 * @return UID
 */
+ (NSString *)getApplicationUID;

/*
 * issue 589958, for retina display we should use new API UIGraphicsBeginImageContextByDeviceType instead of UIGraphicsBeginImageContext
 * @param iSize the size of the image context to be created
 */
+ (void)UIGraphicsBeginImageContextByDeviceType:(CGSize)iSize;

/**
 * Check whether this application is upgraded from an old version.
 * It's considered as upgrading from the old version if the lastVersion equals or less than the old version depending on the includePrevious parameter.
 * @param oldVersion old version
 * @param lastVersion the version before this launch
 * @includingPrevious whether version older than the oldVersion is consider, if YES, lastVersion can be lower than the oldVersion,
 * if NO, lastVersion should be the same as oldVersion
 * @return YES if the lastVersion equals or less than the oldVersion
 * NO if lastVersion is larger than the oldVersion
 */
+ (BOOL)isUpgradedFromVersion:(NSString *)oldVersion lastVersion:(NSString *)lastVersion includingPrevious:(BOOL)includingPrevious;


/**
 * If current device is using 4-inch screen, like iPhone5, iPhone 5s
 **/
+ (BOOL)is4inchScreen;

/**
 * return the navigation controller / bar frame
 **/
+ (CGRect)navigationControllerFrame:(BOOL)isPortrait;

+ (void)showCoverScreen;
+ (void)hideCoverScreen;
+ (void)displayErrorInAlertViewWithTitle:(NSString*)title message:(NSString*)message delegate:(id)delegate
						 dismissPrevious:(BOOL)dismiss withOtherButton:(BOOL)otherButton
						   okButtonTitle:(NSString*)okButtonTitle otherButtonTitle:(NSString*)otherButtonTitle;

/**
 * return deviceID
 * @return device ID
 */
+ (NSString*) getDeviceID;

/**
 * return completeURL after encoding
 * @param iURL resource URL
 * @param iBaseURL base URL of the mobile server
 * @return Complete URL
 */
+ (NSURL*) getCompleteURL:(NSString*)iURL withBaseURL:(NSString*)iBaseURL;

/**
 * return completeURL after encoding
 * @param iURL resource URL
 * @param iBaseURL base URL of the mobile server
 * @param urlEncodedd
 * @return Complete URL
 */
+ (NSURL*) getCompleteURL:(NSString*)iURL withBaseURL:(NSString*)iBaseURL urlEncodeAbsoluteUrl:(BOOL)urlencode;

/**
 * return if current device's iOS version is 5.0 or higher
 **/
+ (BOOL) iOS5OrHigher;
/**
 * return if current device's iOS version is 7.0 or higher
 **/
+ (BOOL) iOS7OrHigher;

/**
 * return if current device's iOS version is 8.0 or higher
 **/
+ (BOOL) iOS8OrHigher;

/**
 * return if current device's iOS version is 9.0 or higher
 **/
+ (BOOL) iOS9OrHigher;

/**
 * return iOS standard barbutton-like button with stretchable background image
 * @param title is the title shown on button
 * @param image is the background image
 * @param target is the control target
 * @param action is the action when button is pressed
 * @return button (autoreleased)
 */
+ (UIButton *)createStdButtonWithTitle:(NSString *)title backgroundImage:(UIImage *)image target:(id)target action:(SEL)action;

/**
 * return the launch image name for the device
 * @param interfaceOrientation The device orientation
 * @return launch image name
 **/
+ (NSString*)getLaunchImageName:(UIInterfaceOrientation)interfaceOrientation;

/**
 * return the lock image name for touch id security
 * @param interfaceOrientation The device orientation
 * @return lock image name
 **/
+(NSString*)getLockImageName:(UIInterfaceOrientation) interfaceOrientation;

/**
 * return if the device is iphone 5
 * @return Whether device is iphone 5
 **/
+ (BOOL)isDeviceiPhone5;

/**
 * return if the device is iphone 6
 * @return Whether device is iphone 6
 **/
+ (BOOL)isDeviceiPhone6SizeFamily;

/**
 * return if the device is iphone 6 plus
 * @return Whether device is iphone 6 plus
 **/
+ (BOOL)isDeviceiPhone6PlusSizeFamily;

/**
 * return if the device is iphone 6S
 * @return Whether device is iphone 6S
 **/
+ (BOOL)isDeviceiPhone6S;

/**
 * return if the device is ipad mini 4
 * @return Whether device is ipad mini 4
 **/
+ (BOOL)isDeviceiPadMini4;

/**
 * return if the device is ipad air 2
 * @return Whether device is ipad air 2
 **/
+ (BOOL)isDeviceiPadAir2;

/**
 * return if the device is ipad pro
 * @return Whether device is ipad pro
 **/
+ (BOOL)isDeviceiPadPro;

/**
 * Check if network is connected
 * @return If network is reachable
 */
+(BOOL)networkIsReachable;
+(BOOL)isNewsstandEnabled;
+(BOOL)isPushNotificationEnabled;
+(BOOL)isBackgroundTransferEnabled;
+(BOOL)softwareEncryptionEnabled;
+(void)setEncryptionKey:(NSString*)key;
+(NSString*)getEncryptionKey;
+(BOOL)isIPhone5;

/**
 * Binary request flags detailed info
 */
+(NSString*)binaryFlag1Info:(int)flag1;
+(NSString*)binaryFlag2Info:(unsigned)flag2;

/**
 * return screen shot on iOS7 above system
 * @return UIImage generated for screen shot
 */
+(UIImage *)generateScreenShotAboveiOS7:(CGFloat)offset;
+(NSString*)longestStringInArray:(NSArray*)strings;
+(UIImage*)imageWithColor:(UIColor *)color;
+(UIImage*)imageWithView:(UIView *)view;
+ (BOOL)isRetina;

+ (BOOL)isPersistSelection;
+ (BOOL)enableTemplateMultiSelection;

+ (NSString*)appName;
+ (BOOL)isPreferenceCarryOverThroughServer;

+ (void)showDocAuthenticationError:(NSInteger)error isLocalAuth:(BOOL)localAuth delegate:(id)delegate;

/**
 * @return current wifi's ssid@bssid
 */
+ (NSString *)currentNetworkID;
@end
