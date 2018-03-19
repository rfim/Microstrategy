//
//  MSINetworkRequestImpl.h
//  MSTR Foundation
//
//  Created by Amirali Charania on 2/20/09.
//  Copyright 2009 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSINetworkRequestBase.h"
#import "Enums.h"

@class MSIProjectConfiguration, MSIProjectInfo, WebServerConfiguration;

@interface MSINetworkRequestImpl : MSINetworkRequestBase {
@protected
    NSString* sessionState;
}

/**
 * Need to set sessionState when network request need to process in MicroStrategy I-Server.
 */
@property (nonatomic, copy) NSString* sessionState;

/**
 * Set project if the network request will process on a MicroStrategy project.
 * An alternative way is using method setProjectInfo
 */
@property (nonatomic, strong) MSIProjectConfiguration* project;

/**
 * Request server type. Can be mobile server or certificate server.
 * Default is mobile server.
 */
@property NetworkRequestServerType serverType;

/**
 * This property is used to control whether network error alert will be pop out.
 */
@property BOOL suppressNetworkErrorAlerts;

/**
 * This property is used to indicate that the network connection information, such as webserver configuration, credentials and other connectivity related information 
 * are retrieved from PreferenceStore.
 * Default value is YES. Set it to NO if PreferenceStore is not used.
 */
@property BOOL usePreferenceStoreForNetworkConnection;

/**
 * Request Flag.
 */
@property (nonatomic) ClientRequestExecutionHandlerFlags requestExeFlag;

/**
 * Wether the network request will be sent in offline mode.
 * Default value is YES.
 */
@property BOOL isInfluencedByOfflineMode;

/**
 @method getWebServer
 @result web server used in this network request. If request is not on a web server, return nil
 */
-(WebServerConfiguration*)getWebServer;

/**
 Set ProjectInfo if the network request will process on a MicroStrategy project.
 An alternative way is to set project property.
 @method setProjectInfo:
 @result
 */
-(void)setProjectInfo:(MSIProjectInfo*)_prInfo;

/**
 Deprecated. Use suppressNetworkErrorAlerts instead.
 */
-(BOOL)displayNetworkErrors;

/**
 Deprecated
 Don't use it any more.
 */
-(NSString*)networkTimeoutDetailMessage;

@end
