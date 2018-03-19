//
//  SecurityManager.h
//  MicroStrategyMobile
//
//  Created by Amirali Charania on 11/24/11.
//  Copyright 2011 MicroStrategy Inc. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "SecurityHandlerDelegate.h"
#import "Enums.h"
#import <CoreData/CoreData.h>
#import "FileManager.h"
#import "SecurityPolicyManager.h"
#import <MessageUI/MFMailComposeViewController.h>

@class SecurityPolicy;

@protocol SecurityHandler

/**
 * Initialize the security framework with the paramters read from the property SecurityFramework in the Info.plist. The Info.plist has
 * SecurityFramework property with the value in format "SecurityFramework&Application ID&MSTR Aplication Version"
 * @param parameters the paramters used for framework initialization
 */
- (void)initialize:(NSArray* _Nullable)parameters;

/**
 * Returns the current UIWindow for the application based on the Security Framework of the application
 * @param UIWindow the UIWindow
 */
- (UIWindow* _Nullable )window;

/**
 * Authorize using the current Security Framework
 */
- (void)authorize;

/**
 * Set the delegate to recevie the SecurityHandler callback
 * @param delegate the delegate
 */
- (void)setDelegate:(id<SecurityHandlerDelegate> _Nullable)delegate;

/**
 * Returns the current fileManager
 */
- (id<FileManager> _Nullable)fileManager;

/**
 * Returns the current SecurityFramework enum which is defined in Enums.h
 * @return the securityFramework
 */
- (SecurityFramework)securityFramework;

/**
 * Returns whether the application has authorized with the current Security Framework
 * @return authorized
 */
- (BOOL)isAuthorized;

/**
 * Returns whether the URL is authenticated. This is only used for Facebook app switch authentication, where we have to handle the URL from Facebook app or Safari. For other types of authentication, this method returns NO
 * @return authorizedByFacebook
 */
- (BOOL) authorizedWithURL:(NSString * _Nullable) urlString;

/**
 * Returns the persistent store coordinator based on the current Security Frameowrk.
 * This API is the wrapper on the API initWithManagedObjectModel: defined in the class NSPersistentStoreCoordinator
 * @param objectModel the objectModel
 * @return persistent store coordinator
 */
- (NSPersistentStoreCoordinator* _Nullable)newPersistentStoreCoordinatorWithManagedObjectModel:(NSManagedObjectModel* _Nullable)objectModel;

/**
 * Returns the persistent store based on the current Security Framework. 
 * The API is a wrapper on the API addPersistentStoreWithType:configuration:URL:options:error: defined in the class NSPersistentStoreCoordinator
 * @param objectModel the objectModel
 * @param storeType the store type
 * @prarm configuration the configuration
 * @param storeURL the storeURL
 * @prarm options the options
 * @param error the error
 * @return persistent store
 */
- (NSPersistentStore* _Nullable)addPersistentStoreToCoordinator:(NSPersistentStoreCoordinator* _Nullable)persistentCoordinator
                                             WithType:(NSString* _Nullable)storeType configuration:(NSString* _Nullable) configuration
                                                  URL:(NSURL* _Nullable)storeURL options:(NSDictionary* _Nullable)options
                                                error:(NSError* _Nullable * _Nullable)error;

/**
 * Returns whether the feature is avialable with the current Security Framework. 
 * The enum for MSTRFeature are defined in Enums.h
 * @param feature feature
 * @return whether the feature is supported with the current security framework
 */
- (BOOL)featureAvailable:(MSTRFeature)feature;

/**
  * Implement the method in case the framework needs to unauthorize the application when tha application goes in the background
  */
- (void)unAuthorizeOnBackground;

/**
 * Returns the singleton secuirty handler for the current security framework
 * @return the security handle
 */
+ (id<SecurityHandler> _Nullable) securityHandler;

/** Return whether the contents of the application should be covered when the application goes in background so that the contents are not shown
  * when user launches the application again until the user authorizes
  * @return whether the contents needs to be covered
  */
- (BOOL)shouldHideScreenContentsOnBackground;

- (BOOL)canHandleURL:(NSURL* _Nullable)url;
- (BOOL)webViewShouldLoadDataUsingURL;

- (void)updateSecurityPolicy:(SecurityPolicy* _Nullable)securityPolicy;

- (id<SecurityPolicyManager> _Nullable)securityPolicyManager;

- (void) openWithExternalApp:(NSString* _Nullable)url;

- (void)applicationLaunchedWithURL;
- (void)applicationLaunchWithURLFailed;
- (BOOL)canPersistCertificate;
- (BOOL)isAuthorizationInProgress;
- (void)setAuthorizationInProgress:(BOOL)authorizationInProgress;
- (BOOL)shouldHideCoverScreen;
- (BOOL)shouldThrowExceptionIfAccessedBeforeAuhtorization;
- (BOOL)requiresAuthorizationOnApplicationOfNewURL;

- (BOOL)isSecurityFrameworkURL:(NSURL* _Nullable)url;
- (BOOL)handleSecurityFrameworkURL:(NSURL* _Nullable)url fromApplication:sourceApplication;


/*
 send the file
 fileData shouldn't be empty
 fileName can be empty
 */
- (void) sendFileWithData:(NSData* _Nonnull)fileData name:(NSString* _Nullable)fileName;

/**
 * Send mail to the url
 */
- (void) mailto:(NSURL* _Nullable)url;

/**
 * Send mail with attachment data
 * @param parameters are used to create the MFMailComposeViewController:
 * Key @"attachment" is a NSData instance containing the contents of the attachment, it must not be nil.
 * Key @"mimeType" is a NSString specifying the MIME type for the attachment, as specified by the IANA. It must not be nil.
 * Key @"fileName" is a NSString specifying the intended filename for the attachment which is displayed below the attachment's icon if the attachment is not decoded when displayed. Must not be nil.
 * Key @"subject" is a NSString specifying the message's Subject header.
 * Key @"body" is a NSString containing the body contents of the email message.
 * Key @"isHTML" is a NSString indicating if the body argument is to be interpreted as HTML content, @"true" for YES.
 * @param controller is the UIViewController where the MFMailComposeViewController would be displayed on.
 * @param delegate is the mailComposeDelegate for the MFMailComposeViewController.
 * @param shouldAutorotate is a Boolean that controls whether the MFMailComposeViewController can be autorotated.
 */
- (void)mailWithAttachmentByParameters:(NSDictionary* _Nonnull)parameters controller:(UIViewController* _Nonnull)controller delegate:(id<MFMailComposeViewControllerDelegate> _Nullable) delegate shouldAutorotate:(BOOL) shouldAutorotate;

/**
 * Convert the original title to the correct title.
 * @param originalString the original title
 * @return the correct title
 */
- (NSString * _Nullable)titleForEmail:(NSString * _Nullable) originalString;

- (BOOL)shouldLoadBakedInConfig;
- (void)setLoadBakedInConfig:(BOOL)loadBakedInConfig;

- (void)setSupportLandscapeOnly:(BOOL)value;
- (BOOL)canHandleNetworkAuthenticationChallenge:(NSURLAuthenticationChallenge * _Nullable)challenge;
- (BOOL)handleNetworkAuthenticationChallenge:(NSURLAuthenticationChallenge * _Nullable)challenge completionHandler:(void (^ _Nullable)(NSURLSessionAuthChallengeDisposition, NSURLCredential * _Nonnull))completionHandler;

@end
