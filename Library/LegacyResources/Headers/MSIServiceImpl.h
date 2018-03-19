//
//  MSIServiceImpl.h
//  MSTR Foundation
//
//  Created by Amirali Charania on 2/20/09.
//  Copyright 2009 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSINetworkRequestImpl.h"
#import "Service.h"

#define ERROR_INFO_TAG @"errorInfo"
#define STATUS_CODE_TAG @"statusCode"
#define ERROR_CODE_TAG @"errorCode" 
#define ERROR_MSG_TAG @"errorMsg"
#define SRC_TAG @"src"

#define POLLING_FREQUENCY 300

#define CLIENT_VERSION_ERROR -2147204980
#define OBJECT_NOT_FOUND_ERROR -2147216373
#define SESSION_EXPIRED_ERROR -2147468986
#define BACKGROUND_THREAD_PRIORITY 0.1
#define FOREGROUND_THREAD_PRIORITY 1.0
#define DEFAULT_THREAD_PRIORITY 0.5

@class MSIProjectConfiguration, MSIProjectInfo, TaskGroup, TaskInfo, MSIReportDisplayInfo, MSTRException, MSIDevicePreferences, MSTRError;

@interface MSIServiceImpl : NSObject <Service, NetworkResponseHandler> {
@private
	BOOL async;
	NSThread* serviceThread;
@protected
	NSInteger state;
	id<MSINetworkRequest> request;
	id<ServiceHandler> __weak handler;
	TaskGroup *taskGroup;
	TaskInfo *taskInfo;
	id<Service> __weak innerService;
	BOOL suppressNetworkErrorAlerts;
	NSString* incrementalFileLocation;
    double priority;
    ClientRequestExecutionHandlerFlags requestExeFlag;
     BOOL makeIncrementalAvailable;
}

@property(nonatomic, weak) id<ServiceHandler> handler;
@property BOOL async;
@property NSInteger state;
@property (nonatomic) BOOL suppressNetworkErrorAlerts;
@property BOOL suppressLoginPrompt;
@property (nonatomic, strong) NSString* incrementalFileLocation;
@property (nonatomic, weak) id<Service> innerService;
@property double priority;
@property (nonatomic, assign) ClientRequestExecutionHandlerFlags requestExeFlag;
@property BOOL makeIncrementalAvailable;

-(void)run;
-(NSString*)getBaseURL:(MSIProjectInfo*)aProject task:(BOOL)task;
-(void)handleData:(NSData*)data request:(id<MSINetworkRequest>)request;
-(void)handleError:(MSTRError*)error request:(id<MSINetworkRequest>)request;
-(void)handleDataProgress:(long long)loadedSize Total:(long long)totalSize request:(id<MSINetworkRequest>)request;
-(void)setTaskGroup:(TaskGroup *)group;
-(NSString*)sessionState:(MSIReportDisplayInfo*) report;
-(MSIProjectConfiguration*)projectConfigForReport:(MSIReportDisplayInfo*)report;
-(void)reportException:(MSTRException*) ex report:(MSIReportDisplayInfo*) report;
-(void)reportError:(MSTRError*) e report:(MSIReportDisplayInfo*) report;
-(NSString*)getSessionState:(MSIProjectInfo*)projectInfo;
-(MSIProjectConfiguration*) getProjectConfig:(MSIProjectInfo*)projectInfo;
-(MSIDevicePreferences *) getDevicePrefs;
-(void)reportException:(MSTRException*) ex project:(MSIProjectInfo*) project;
-(void)handleServiceFailedError:(MSTRError*) error withProject:(MSIProjectInfo*)project;

/** Reset innerService
 */
-(void) resetInnerService;

@end


@interface MSIServiceImpl (Private)

- (void)threadRun;
- (void)handleClientVersionError:(MSTRError*)error;
- (void)handleObjectNotFoundError:(MSTRError*)error;
- (void)handleSessionExpiredError:(MSTRError*)error;

@end

