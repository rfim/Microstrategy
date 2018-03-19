//
//  DeferExecCommon.h
//  MicroStrategyMobile
//
//  Created by dali on 1/17/12.
//  Copyright (c) 2012 MicroStrategy Inc. All rights reserved.
//


#include <CoreFoundation/CoreFoundation.h>
#import <Foundation/Foundation.h>
#import "MSIProjectInfo.h"
#import "MSTRError.h"
#import "MSIReportDisplayInfo.h"

#ifndef ASYNCEXC_ENABLED
#define ASYNCEXC_ENABLED true
#endif

#ifndef ASYNCEXC_DEBUG
//#define ASYNCEXC_DEBUG
#endif


#ifndef ASYNCEXC_ASSERT
//#define ASYNCEXC_ASSERT(condition, desc)  NSAssert(condition, desc)
#define ASYNCEXC_ASSERT(condition, desc)
#endif

//reconcile refresh timeinterval
#ifdef ASYNCEXC_DEBUG
#define ASYNCEXC_RECONCILE_MINTIMEINTERVAL 10
#else
#define ASYNCEXC_RECONCILE_MINTIMEINTERVAL 3
#endif

#ifndef ASYNCEXC_LOG_ENABLED

	// Enable or disable all that extra logging in the console.
	//#define ASYNCEXC_LOG_ENABLED
#endif



#ifdef ASYNCEXC_LOG_ENABLED
#define ASYNCEXC_LOG(message, ...) NSLog((@"ASYNCEXC: csfid %s [ln %d] [th %@] [self %p] " message), \
__PRETTY_FUNCTION__, __LINE__, [NSThread currentThread], self, ## __VA_ARGS__);
#else
#define ASYNCEXC_LOG(message, ...)
#endif


/*for error messages*/
#define DEFERRED_EXECUTION_ERROR_DOMAIN @"deferred execution"

//use one error code for deferred executions
#define DEFERRED_EXECUTION_ERROR_CODE 100100
#define DEFERRED_EXECUTION_ERROR_CODE_FAILED_TO_DEFER 100101
#define DEFERRED_EXECUTION_ERROR_CODE_DEFERRED_INBOX_IS_FULL 100102
#define DEFERRED_EXECUTION_ERROR_CODE_FAILE_TO_DOWNLOAD 100103
#define DEFERRED_EXECUTION_ERROR_CODE_CLIENT_CACHE_LOST 100104

#define DEFERRED_EXECUTION_COMP @"AsyncExecution"

#define DEFER_EXEC_BASEMESSAGEID_PREFIX	@"DEFEREXEC_BASEMESSAGEID_"



/*protocol for dataController*/
//DeferExecService need call [DeferExecCandidateReportStatusListener deferExecCandidateReport:deferExecCandidateReportStatusValue] when report status is changed
//dataController need cancel report's running job by calling [DeferExecService cancelForDefferedExecution] when user choose to defer/cacel a report
typedef enum _DeferExecCandidateReportStatus{
	DeferExecCandidateReportStatusTaskExecutionStarted = 0,
    DeferExecCandidateReportStatusTaskExecutionManipulationStarted = 4,
	DeferExecCandidateReportStatusReadyToDefer =1,
	DeferExecCandidateReportStatusPreviousReadyToDeferInvalid =2,
	DeferExecCandidateReportStatusExecutionFinished =3,
} DeferExecCandidateReportStatus;

@protocol DeferExecCandidateReportStatusListener
@required
-(void) deferExecCandidateReport:(MSIReportDisplayInfo*)report statusChanged:(NSNumber*)deferExecCandidateReportStatusValue ;
-(bool) isDeferredExecution;
@optional
-(void) setDeferredExecution:(bool)isDeferredExecution;
@end

@interface DeferExecCommon :NSObject

+(bool) isDeferExecSupported;

+(NSString*) deferExecStoreHome;

+(bool) isProjectSupported:(MSIProjectInfo*)project;

+(void) deferExecCandidateReport:(MSIReportDisplayInfo*)report statusChanged:(DeferExecCandidateReportStatus)deferExecCandidateReportStatusValue handler:(id) handler;

+(bool) isDeferredExecution:(id) handler;


+(void) deferExecCandidateReport:(MSIReportDisplayInfo*)report statusChanged2:(NSNumber*)deferExecCandidateReportStatusValue handler:(id) handler;

//Error handlings
+(MSTRError*) createMSTRErrorWithCode:(int)errorCode errMsg:(NSString*) errMsg;
+(MSTRError*) createMSTRErrorWithError:(id)error;
+(MSTRError*) createMSTRErrorWithErrorInfoAttDict:(NSDictionary *)attributeDict;
@end

//protocol for service supports deferred execution, not applied
@protocol DeferExecService
//cacel current running job for deferred execution
-(void) cancelForDefferedExecution:(MSTRError **)err;
@end


