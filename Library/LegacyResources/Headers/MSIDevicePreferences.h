//
//  MSIDevicePreferences.h
//  MSTR Foundation
//
//  Created by Amirali Charania on 2/20/09.
//  Copyright 2009 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>

// Define the ID for preferences
#define ID_NETWORK_TIMEOUT @"nt" //network timeout (in ms)
#define ID_FOLDER_CACHING @"fc" // Whether folder caching is enabled
#define ID_LOG_LEVEL @"ll"
#define ID_MAX_LOG_ENTRY_COUNT @"mls" //Maximum Log Size//number of max log entries
#define ID_START_WEEKDAY @"sw"   // The number of weekday to Start Week On. US31530, by wenzhou. 12/10/2015.
#define ID_ALLOW_ACCESS_SETTING @"uas"//Allow Users to Access Settings
#define ID_CONNECTIVITY @"cty" //Connectivity: web server list
#define ID_HOME_SCREEN @"hsc"
#define ID_MAX_GRID_COLUMNS @"mgc"
#define ID_PRELOAD_CACHES @"plc" //Automatically Pre-load Caches
#define ID_CLEAR_CACHE_ON_CLOSE @"cc" //Clear Caches When Application Closes
#define ID_MAX_STATUS_COUNT @"msc" //max number of status entries stored
#define ID_MAX_STATUS_INTERVAL @"msi" //number of days corresponding to the time period before a status entry is deleted from the status store
#define ID_MAX_PDF_SIZE @"ps" // Max PDF size in kilo pixels
#define ID_PASSWORD_EXPIRATION @"pe"
#define ID_SUBSCRIPTION_CHECK_INTERVAL @"sci" // Interval in seconds to check the new subscriptions
#define ID_CACHE_VALIDATION_INTERVAL @"cvi" // Interval in seconds to validate the device caches
#define ID_CACHE_REALTIME_DATA_TIME @"crd" //cache real-time data time
#define ID_DIAGNOSTICS_ENABLED @"de" // Whether diagnostics is enabled or not (disabled by default)
#define ID_DEVICE_TOKEN @"dt" //deviceToken sent back from APNS
#define	ID_PUSH_NOTIFICATION @"pn" //push notification enabled
#define ID_ACCEPTABLE_NETWORK_RESPONSE_TIME @"art" // acceptable network response time (in seconds)
#define ID_USE_CERTIFICATE_SERVER @"ucs" // use certificate server (disabled by default)
#define ID_CERTIFICATE_SERVER_URL @"cs" // certificate server (empty string by default)
#define ID_EMAIL_SCREEN @"es"  // Enable sharing including emailing, printing and annotation for old server
#define ID_EMAIL_PRINT_SAVE @"eps"  // Enable sharing including emailing, printing and saving to photos for new server
#define ID_ANNOTATION @"ea" // annotation enabled
#define ID_ENABLE_OPEN_PDF @"eopdf" // open pdf in other applications enabled
#define ID_AUTOUPDATE_CONFIGURATION @"uptc" //Auto update configuration every "t" time
#define ID_CONFIGURATION_LINK_INFO @"lnk" //Configuration Link Info, holds the info related to the configuration link and provide methods for fetching Configuration URL and Counter Fetching URL
#define ID_PENDING_CONFIGURATION_UPDATE_CHECK @"pcuc" //We failed with a configuration update check and needs retry later, for now, it will be triggered by Reconcile Service
#define ID_NEED_UPDATE_CONFIGURATION @"nuc" // whether need update configuration
#define ID_IGNORE_PRIVILEGE_ERROR @"ipe" //Ignore user privileges errors during reconcile
#define ID_ENABLE_FULL_SCREEN @"efs" //Enable full screen
#define ID_DEFINITION_SIZE_MEMORY_LIMIT @"defml" //Memory limit of definition size (in MB)
#define ID_DATA_SIZE_MEMORY_LIMIT @"datml" //Memory limit of data size (in MB)
#define ID_RENDERING_MEMORY_LIMIT @"renml" //Memory limit of rendering (in MB)
#define ID_MAX_INCREMENTAL_DATA_SLICE @"mids" //Maximum incremental data slice size (in MB)
#define ID_DISABLE_BUILTIN_PDF_VIEWER @"dbpv"	//disable build-in pdf viewer
#define ID_SHOW_DIAGNOSTICS @"usd"	//Enable user to access the diagnostics mode setting (default is true)
#define ID_SHOW_LOGGING @"usl"	//Enable user to access the logging setting (default is true)
#define ID_SHOW_CACHE @"usc"	//Enable user to access the cache setting (default is true)
#define ID_LOCALE_IDENTIFIER @"locid" //Device locale identifier when used
#define ID_DISABLE_UNAVAILABLE_LINKS @"dul" //disable unavailable links in offline mode
#define ID_DATA_WIPE_ON_CERTIFICATE_ERROR @"dw" //Data Wipe
#define ID_NEVER_PERSIST_CREDENTIALS @"npc"  //don't persist credentials to device local storage
#define ID_SHOW_LEARN_MORE_LINK @"slml"	// show learn more link setting
#define ID_LEARN_MORE_LINK @"lml"	// learn more link
#define ID_MONITOR_NETWORK_STATUS @"mns"	// monitor network status
#define ID_SHOW_TIMESTAMP_OPTIONS @"sto"	// monitor network status
#define ID_SHOW_CACHED_LOGIN @"scl"  // show cached login
#define ID_IS_USING_EMBEDDED_CERTIFICATE @"iuec" // is using embedded certificate now
#define ID_IS_CERTIFICATE_VALID @"icv" // is certificate valid
#define ID_USE_EXTERNAL_APP_TO_OPEN_KEYNOTE @"eaok" // use external application to open keynote files10.197.41.
#define ID_IS_OFFLINE_MODE @"iom"	// show offline mode status
#define ID_DEVICE_PASSWORD_TEST_MODE @"dtm"
#define ID_TOUCH_MODE @"daip"
#define ID_DEVICE_PASSWORD_REQUIRES_NUMERIC @"drn"
#define ID_DEVICE_PASSWORD_REQUIRES_SPECIAL_CHARACTER @"drsc"
#define ID_DEVICE_PASSWORD_REQUIRES_CAPITAL_LETTER @"drcl"
#define ID_DEVICE_PASSWORD_MINIMUM_LENGTH @"dcn"
#define ID_DEVICE_PASSWORD_MAXIMUM_ALLOWED_FAILED_ATTEMPTS @"dmatp"
#define ID_DEVICE_PASSWORD_LOCKOUT_DURATION @"dd"
#define ID_DEVICE_PASSWORD_REQUIRED_AFTER_RESUMING @"dbw"
#define ID_DEVICE_PASSWORD_REQUIRED_AFTER_RESUMING_DURATION @"dbwt"
#define ID_DEVICE_PASSWORD_REQUIRED_AFTER_LOCK @"drld"
#define ID_APP_THEME @"atc"
#define ID_THEME_TEST @"tct" //used for theme test, (0.Not enable test theme; 1.Dark; 2.light; 3.pre-defined; other:undefined)
#define ID_DISPLAY_LOG_OUT_OPTION @"aul"
#define ID_CLEAR_CACHE_ON_LOG_OUT @"ccl"
#define ID_ACCENT_COLOR @"ac"
#define ID_ENABLE_BACKGROUND_DOWNLOAD @"bgd"
#define ID_ENABLE_DEBUG_LOG_LEVEL @"edll"// for debugging logging
#define ID_ENABLE_IMAGE_CACHE_PERFORMANCE_LOG_LEVEL @"eicpll"
#define ID_GRID_PAGE_CELL_LIMITATION @"gcl"
#define ID_ALLOW_AUTHENTICATION_VIA_SYSTEMPASSCODE_FOR_TOUCHID @"apt"
#define ID_DISPLAY_MY_REPORTS @"dmr"
#define ID_SHOW_AUTHENTICATION_ERROR_DURING_RECONCILE @"sae"


@class WebServerList, IntegerPreference, MSIStringPreference, MSIHomeScreenSettings, BooleanPreference, ConnectivitySerializableContext, ConfigurationLinkInfo, WebServerConfiguration, MSIProjectConfiguration, MSIProjectInfo, WebServerInfo;


/**
 * MSIDevicePreferences holds all of the MicroStrategy preferences, including general settings, connectivity settings, home screen settings and so on.
 */
@interface MSIDevicePreferences : NSObject <NSCopying>{
@private
	NSMutableDictionary* preferenceDic;
	WebServerList* webServers;
	MSIHomeScreenSettings* homeScreen;
	ConfigurationLinkInfo* configLinkInfo;
	
	//NSInteger index;
	BOOL readOnly;
	ConnectivitySerializableContext* csCxt;//Used in loading XML
	NSString* configID;//The configuration ID comes from Web Server. This is useful when we want to apply multiple Config XMLs in future.
	NSInteger configCounter;//Configuration counter which indicates how may times administrator updated the Configuration XML
	
	NSInteger mode;//for parsing
	NSString* lastVersion; // the application version stored in preference.xml, it may not be current version
	BOOL isDemo;
	
	NSInteger clientVersion;	// new version used in the compatibility check for the features in preference
}

/**
 * Readonly property indicates whether the device preference can be modified.
 * DevicePreference is readonly by default when it's loaded from preferences file.
 * Copied DevicePreference is not readonly.
 */
@property (readonly) BOOL readOnly;

/**
 * Configuration ID comes from mobile server when applying a new configuration URL.
 * Each configution has a unique configuration ID.
 */
@property (nonatomic, strong, readonly) NSString* configID;

/**
 * Use this method to get list of web servers connected by the application
 * @return list of webservers
 */
-(WebServerList*)getWebServers;

/**
 * Use this method to get the network timeout, in seconds, after which network request will be terminated.
 * Default value is 60.
 * @return network timeout
 */
-(IntegerPreference*)getNetworkTimeout;

/**
 * Use this method to get the max number of status entries stored on device
 * Default value is 50.
 * @return max status count
 */
-(IntegerPreference *)getMaxStatusCount;

/**
 * Use this method to get the number of days corresponding to the time period before a status entry is deleted from the status store
 * Default value is 7 days.
 * @return max status interval
 */
-(IntegerPreference *)getMaxStatusInterval;

/**
 * Use this method to get the number of max log entries stored on device.
 * Default value is 250.
 * @return maximum log count stored
 */
-(IntegerPreference *)getMaxLogEntryCount;

/**
 * Use this method to get the current log level. Default is Warning.
 * Logs will be displayed according to the log level. 
 * @return the current log level
 */
-(IntegerPreference *)getLogLevel;

/**
 * Use this method to get the weekday to start week on. Default is -1, means user hasn't set it and we should get the value by [NSCalendar autoupdatingCalendar].firstweekday.
 * Logs will be displayed according to the log level.
 * @return -1 or the weekday number use set (between 1 to 7)
 */
-(IntegerPreference *)getStartWeekday;

/**
 * Use this method to get the maximum pdf size supported by the application in kilo pixels.
 * Default value is 2048.
 * @return maximum pdf size suppported by the application
 */
-(IntegerPreference *)getMaxPDFSize;

/**
 * Use this method to get whether folder caching is enabled. It's enabled by default.
 * Application will connect to server every time of folder browsing if folder cache is disabled.
 * @return whether folders will be cached
 */
-(BooleanPreference *)getCacheFolders;

/**
 * Use this method to get whether settings access is allowed. It's allowed by default.
 * @return whether settings access is allowed
 */
-(BooleanPreference *)getAllowSettingsAccess;

/**
 * Use this method to get the maximum grid columns.
 * IPad default value is 50. IPhone default value is 10.
 * @return maximum grid columns supported
 */
-(IntegerPreference*)getMaxGridColumns;

/**
 * Use this method to get the cache preload mode
 * Cache preload mode can be CachePreLoadModeAutomatic or CachePreLoadModeOff. Default value is CachePreLoadModeOff.
 * @return cache preload mode
 */
-(IntegerPreference *)getCachePreLoadMode;

/**
 * Use this method to get the cache clear mode
 * Cache clear mode can be CacheClearModeOnClose or CacheClearModeAutomatic. Default value is CacheClearModeAutomatic.
 * @return the cache clear mode
 */
-(IntegerPreference *)getCacheClearMode;

/**
 * Use this method to get the password expiration limit in minutes.
 * Login and password will be cleared if expiration time is reached. Default value is -1 means password won't expire.
 * @return the password expiration limit in minutes.
 */
-(IntegerPreference *)getPasswordExpiration;

/**
 * Use this method to get the subscription check interval in seconds. Default value is 600.
 * Application will check subscription from server when the time interval is reached periodically.
 * @return the subscription check interval in seconds.
 */
-(IntegerPreference *)getSubscriptionCheckInterval;

/**
 * Use this method to get the cache validation interval in seconds. Default value is 600.
 * Application will check cache from server when the time interval is reached periodically.
 * @return the cache validation interval in seconds.
 */
-(IntegerPreference *)getCacheValidationInterval;

/**
 * Use this method to get the cache real-time data time. Default is 0.
 * @return cache real-time data time in seconds.
 */
-(IntegerPreference *)getCacheRealTimeDataTime;

/**
 * Use this method to get whether unavailable links is disabled. Default is disabled.
 * @return whether unavailable links is disabled
 */
-(BooleanPreference *)getDisableUnavailableLinks;

/**
 * Use this method to get whether diagnostics is enabled. Default is disabled.
 * @return whether diagnostics is enabled
 */
-(BooleanPreference *)getDiagnosticsEnabled;

/**
 * Use this method to get the push notification mode
 * The push notification mode can be on or off. Default is PushNotificationOff.
 * @return the push notification mode
 */
-(IntegerPreference *)getPushNotificationMode;

/**
 * Use this method to get the acceptable network response time in seconds.
 * Default is 2 seconds.
 * @return the acceptable network response time in seconds.
 */
-(IntegerPreference *)getAcceptableNetworkResponseTime;

/**
 * Use this method to get whether to use certificate server. Default is NO.
 * @return whether to use certificate server
 */
-(BooleanPreference *)getUseCertificateServer;

/**
 * Use this method to get the certificate server url. Default is empty string.
 * Application will connect to certificate server to download certificate.
 * @return the certificate server url
 */
-(MSIStringPreference *)getCertificateServerURL;

/**
 * Use this method to get whether email screen print annotation is enabled for old server. Default is YES.
 */
-(BooleanPreference *)getEnableEmailScreen;//823205

/**
 * Use this method to get whether email screen print save is enabled, for new server. Default is YES.
 A is Enable emailing, annotating and printing in old server
 B is Enable emailing, printing and saving to photos in new server
 C is Enable annotating in new server
 All properties is default YES in both client and server.
 
 #1,if A uncheck, its an old server with disable all features, client will disable both B and C.
 #2,if A check, it may be old or new, but we will always get right result.
 For old server , server will not send B and C, client will use default B and C, which is both check.
 For new server, we will get B and C from the server, which is current behavior for new server.
 */
-(BooleanPreference *)getEnableEmailPrintSave;//823205
/**
 * Use this method to get whether annotation is enabled. Default is YES.
 * If enabled, a annotation option will be displayed in the navigation bar of data controller.
 * @return whether annotation is enabled
 */
-(BooleanPreference *)getEnableAnnotation;

/**
 * Use this method to get whether pdf is enabled to open. Default is YES.
 * @return whether pdf is enabled to open
 */
-(BooleanPreference *)getEnableOpenPDF;

/**
 * Use this method to get the auto update configuration in minutes.
 * Application will check whether the configuration if updated periodically when the time is reached.
 * The default value is -1, means configuration is not checked.
 * @return auto update configuration
 */
-(IntegerPreference *)getAutoupdateConfiguration;

/**
 * Use this method to get whether privelege error is to be ignored. Default is NO.
 * @return whether priveledge error is to be ignored
 */
-(BooleanPreference *)getIgnorePrivilegeError;

/**
 * Use this method to get whether full screen is enabled. Default is YES.
 * Data controller will show a full screen button if it's enabled.
 * @return whether full screen is enabled
 */
-(BooleanPreference *)getEnableFullScreen;

/**
 * Use this method to get whether the built in PDF viewer is disabled. Default is NO.
 * @return whether built in PDF viewer is disabled
 */
-(BooleanPreference *)getDisableBuiltInPDFViewer;

/**
 * Use this method to get whether to show diagnostics section in the advanced settings page. Default is YES.
 * @return whether to show diagnostics
 */
-(BooleanPreference *) getShowDiagnostic;

/**
 * Use this method to whether to show logging section in the advanced settings page. Default is YES.
 * @return whether to show logging
 */
-(BooleanPreference *) getShowLogging;

/**
 * Use this method to get whether to show cache section in the advanced settings page. Default is YES.
 * @return whether to show cache
 */
-(BooleanPreference *) getShowCache;

/**
 * Use this method to get whether or not wipe data when client certificate contains error.
 * @return whether to wipe data
 */
-(BooleanPreference *)getDataWipeOnCertificateError;

/**
 * Use this method to get whether don't persist credentials. The default value is NO (the original behavior in pre-9.3 releases), which means that credentials are saved in Preferences.xml from one app session to the next.
 * @return whether don't persist credentials
 */
-(BooleanPreference *)getNeverPersistCredentials;

/**
 * Use this method to get whether show learn link.
 * @return whether show learn link
 */
-(BooleanPreference *)getShowLearnMoreLink;

/**
 * Use this method to get the learn more link.
 * @return the learn more link.
 */
-(MSIStringPreference *)getLearnMoreLink;

/**
 * Use this method to get the monitor network status setting
 * @return the monitor network status
 **/
-(BooleanPreference *)getMonitorNetworkStatus;

-(BooleanPreference *)getShouldShowTimestampOptions;

/**
 * Use this method to get whether we need to show cached login on authentication prompt
 * @return whether cached login should be shown
 */
-(BooleanPreference *)getShowCachedLoginOnAuthenticationPrompt;

/**
 * Use this method to get whether we need to use external application to open keynote files
 * @return whether we need to use external app or not
 */
-(BooleanPreference *)getUseExternalAppToOpenKeynote;

/**
 * Use this method to get offline mode status
 * @return the offline mode status
 **/
-(BooleanPreference *)getIsOfflineMode;

/**  Use this method to get the ProjectConfiguration from the ProjectInfo object.  If it is not found, returns nil.
 @param project project
 @returns ProjectConfiguration
 */
-(MSIProjectConfiguration*)getProjectConfiguration:(MSIProjectInfo*)project;

/**  Use this method to get the ProjectConfiguration from the ProjectID.  If it is not found, returns nil.
 @param projectID projectID
 @returns ProjectConfiguration
 */
-(MSIProjectConfiguration*)getProjectConfigurationByProjectID:(NSString*) projectID;

-(MSIProjectConfiguration*)getProjectConfigurationByProjectID:(NSString*) projectID serverUrl:(NSString*)serverUrl;

/**  Use this method to get the ProjectConfiguration from the ProjectName.  If it is not found, returns nil.
 @param projectName projectName
 @returns ProjectConfiguration
 */
-(MSIProjectConfiguration*)getProjectConfigurationByProjectName:(NSString*) projectName;

/**  Use this method to get the WebServerConfiguration from the WebServerInfo object.  If it is not found, returns nil.
 @param webserver webserver
 @returns WebServerConfiguration
 */
-(WebServerConfiguration*)getWebServerConfiguration:(WebServerInfo*)server;

/* Use this method to get the app theme from the mobile server
 @returns app theme. -1:not define; 1:dark; 2:light
 */
-(IntegerPreference *)getAPPTheme;

/*
 Use this method to test theme
 */
-(IntegerPreference *)getThemeTest;

/**
 * Use this method to identify whether to allow users to log out
 * @return whether allow users to log out
 **/
-(BooleanPreference *)getShouldDisplayLogOutOption;

/**
 * Use this method to identify whether should clear cache after users log out
 * @return whether should clear cache after users log out
 **/
-(BooleanPreference *)getShouldClearCacheOnLogOut;

/**
 * Use this method to get the accent color
 * @return accent color
 */
-(MSIStringPreference *)getAccentColor;

/**
 * Use this method to identify whether to enable background download
 * @return whether enable background download
 */
-(BooleanPreference *)getEnableBackgroundDownload;

/*
 Use this method to get grid page cell limitation
 */
-(IntegerPreference *)getGridPageCellLimitation;


/*
 use this method to identify whether to allow authentication via system passcode for Touch ID
 */
-(BooleanPreference *)getAllowAuthenticationViaSystemPasscodeForTouchID;

/*
 use this method to identify wheter to display my reports when folder browsering.
 */
-(BooleanPreference *)getDisplayMyReports;

/*
 use this method to identify whether error shouls show while doing reconcile.
 */
- (BooleanPreference *) getShowAuthenticationErrorDuringReconcile;
@end
