//
//  MSINetworkRequestBase.h
//  MicroStrategyMobile
//
//  Created by Jiang, Yongli on 12/21/15.
//  Copyright © 2015 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSINetworkRequest.h"
#import "Enums.h"

@class ErrorDetails;

typedef NS_ENUM(NSInteger, EnumNetworkDownloadMode)  {
    EnumDownloadInMemory,
    EnumDownloadIncremental,
};

/**
 Delegate to handle network response
 */
@protocol NetworkResponseHandler <NSObject>

-(void)onSuccess:(NSData*)data request:(id<MSINetworkRequest>)request;
-(void)onFailure:(NSError*)error request:(id<MSINetworkRequest>)request;

@optional
-(void)onProgress:(long long)loadedSize Total:(long long)totalSize request:(id<MSINetworkRequest>)request;
-(void)onIncrementalData:(NSData*)data request:(id<MSINetworkRequest>)request;

@end

typedef NS_ENUM(NSInteger, MSIAuthenticationErrorReason)  {
    MSIAuthenticationErrorReasonNone,
    MSIAuthenticationErrorReasonPasswordError,
    MSIAuthenticationErrorReasonPasswordExpired
};

typedef void (^ChallengeCompletionHandler)(NSURLSessionAuthChallengeDisposition, NSURLCredential *);
typedef MSIAuthenticationErrorReason (^AuthenticationChallengeForNormalModeHandler)(NSURLAuthenticationChallenge *, ChallengeCompletionHandler);

@interface MSINetworkRequestBase : NSObject <MSINetworkRequest>{
@protected
    NSURL *baseURL;
    NSMutableDictionary* parameters;
    NSMutableData* dataCollection;
    long long readLength;
    BOOL isInAuthenticationError;
    BOOL isInAuthenticationErrorWithExpiration;
    int certificateError;
    BOOL isKeychainUnavailableForCertificate;
    BOOL isCertificatePinningError;
    AuthenticationChallengeForNormalModeHandler normalAuthenticationChallengeHandler;
}

/**
 * handler to handle network response
 */
@property (nonatomic, weak) id<NetworkResponseHandler> handler;

/**
 * NSMutableURLRequest sent for network request
 * The value can be get after start method
 */
@property (nonatomic, readonly) NSMutableURLRequest *request;

/**
 * Response of this network request. Can be retrieved after response is back.
 */
@property (nonatomic, strong, readonly) NSHTTPURLResponse* response;


/**
 * This property records the request start time.
 */
@property (nonatomic, strong, readonly) NSDate* requestStartTime;

/**
 * This property is used to control whether the network response progress information is sent to handler.
 * If NO, the onProgress:Total:request delegate method won't be called on handler.
 * Default value is NO.
 */
@property BOOL notifyProgress;

/**
 * This propert controls the download mode. Value can be EnumDownloadInMemory or EnumDownloadIncremental.
 * If EnumDownloadIncremental is used, when the response data size exceeds the incrementalDataMinSize,
 * handler method of onIncrementalData:request will be called.
 * Default value is EnumDownloadInMemory.
 */
@property EnumNetworkDownloadMode returnDataIncrementally;

/**
 * Minimal size of response data to be send to handler.
 * It's only used when the returnDataIncrementally is set to EnumDownloadIncremental.
 */
@property int incrementalDataMinSize;

/**
 * Property to set if the connection will use shared credentials storage
 * Default value is NO.
 */
@property (nonatomic) BOOL connectionShouldUseCredentialStorage;

/**
 * Network request timeout interval in seconds, please refer to the timeoutInterval in NSURLRequest
 */
@property (nonatomic) NSInteger networkTimeoutInterval;


/**
 * Whether network request start method will wait for the network request finishes.
 * start method will return immediately if isAsync is set to YES;
 * default value is NO.
 */
@property (nonatomic) BOOL isAsync;

/**
 * Whether cancel or perform default handling for client certificate authentication challenge.
 * Cancel authentication challenge if the value is set to YES;
 * default value is NO.
 */
@property (nonatomic) BOOL cancelAuthenticationChallengeForClientCertificateMode;

/**
 * Whether ignore the stored identity for client certificate authentication challenge.
 * Ignore the stored identity if the value is set to YES;
 * default value is NO.
 */
@property (nonatomic) BOOL ignoreIdentityForClientCertificateMode;

/**
 add paramater
 The parameter will be send with the request
 @method addParameter:key
 @param value
 @param key
 */
- (void)addParameter:(NSString *)value forKey:(NSString *)key;

/**
 Helper method to encode special characters
 @method encode:
 @result encoded string with percentage espace
 */
-(NSString*)encode:(NSString*)value;

/**
 @method getBaseURL
 @result base url used in network request
 @discussion subclass can override this method to return a new baseURL and new url will be used in network request
 */
-(NSURL *)getBaseURL;

/**
 Set base url used in network request.
 If a project or projectInfo is provided, and the baseURL is not set, the web server url get from the project will be used as the baseURL.
 @method setBaseURL:
 @param baseURL
 */
- (void)setBaseURL:(NSURL *)baseURL;

/**
 Set HTTP method in network request
 @method setHTTPMethod:
 @param HTTPMethod
 @discussion if HTTPMethod is not set, MSINetworkRequestImpl will choose appropriate HTTPMethod
 */
- (void)setHTTPMethod:(NSString *)HTTPMethod;

/*
 Set HTTP header in network request if needed.
 @method setHTTPHeaderValue:forField:
 @param value
 @param field
 @discussion if multiple value are set for one field, only the last will be used
 */
- (void)setHTTPHeaderValue:(NSString *)value forField:(NSString *)field;

/**
 Add log entry for this url
 @method logURL:
 @param url to be logged
 @result
 */
-(void)logURL:(NSURL*)url;

/**
 Add file for upload
 @method addFile:forKey:
 @param filePath
 @param fileKey
 @result
 */
-(void)addFile:(NSString *)filePath forKey:(NSString *)fileKey;

/*
 Get the string representation of request body
 @method getRequestBody
 @result string as request body
 @discussion to get correct request body, parameters needed to be set first.
 Subclass can provide different version of getRequestBody to bypass the parameters. The request body will be used in the network request.
 */
-(NSString *)getRequestBody;

/**
 Set if the url can be logged
 @method setIsURLLoggable:
 @param isLoggable
 @discussion url default is loggable. If url needs to be kept confidential for some reason, pass NO to this method.
 */
- (void)setIsURLLoggable:(BOOL)isLoggable;

/**
 @method setAuthenticationChallengeForNormalModeHandler:
 @param handler
 @discussion This is a convenient way to inject a authentication challenge handler to normal mode (need login and password).
 If the authentication challenge handler logic is complex and need to work together with default handling logic,
 can use subclass and override the handleAuthenticationChallengeForNormalMode:completionHandler
 */
- (void)setAuthenticationChallengeForNormalModeHandler:(AuthenticationChallengeForNormalModeHandler)handler;

/**
 Subclass can implement this method to add extra parameters
 @method addParameters
 @discussion provide for subclass
 */
- (void)addParameters;

/**
 subclass can provide own handleResponse code to replace the default
 @method handleResponse:data:
 @discussion provide for subclass
 */
- (void)handleResponse:(NSHTTPURLResponse*)response data:(NSData*)data;

/**
 This method should only be used or overrided by subclass.
 @method reportError:
 @discussion provide for subclass
 */
- (void)reportError:(NSError*)error;

/**
 This method should only be used or overrided by subclass.
 @method reportSuccess
 @discussion provide for subclass
 */
- (void)reportSuccess:(NSData*)data;

/**
 subclass can provide own loggableRequestBody code to replace the default
 @method loggableRequestBody
 @discussion provide for subclass
 */
- (NSString*)loggableRequestBody;

/**
 Subclass can implement different authentication challenge handling logic
 @method handleAuthenticationChallengeForNormalMode:completionHandler
 @discussion provide for subclass
 If the handling is simple and don't need to subclass, can use setAuthenticationChallengeForNormalModeHandler instead
 */
- (void)handleAuthenticationChallengeForNormalMode:(NSURLAuthenticationChallenge *)challenge completionHandler:(void (^)(NSURLSessionAuthChallengeDisposition, NSURLCredential *))completionHandler;

/**
 subclass can provide own implementation with network error
 @method task:didFailWithError:
 @discussion provide for subclass
 */
- (void)task:(NSURLSessionTask *)task didFailWithError:(NSError *)error;

/**
 Get the actual class name for logging
 @method loggingClassName
 @result a string represent the class name
 */
- (NSString*)loggingClassName;

/**
 Server authentication mode for a request. Default value is WebAuthModeDefault.
 Override this method to provide web authentication mode for a request.
 @method serverAuthenticationMode;
 @result the web authentication mode for the request.
 */
- (WebAuthModes) serverAuthenticationMode;
@end

