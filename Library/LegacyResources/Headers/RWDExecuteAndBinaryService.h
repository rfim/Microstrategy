//
//  RWDExecuteAndBinaryService.h
//  MicroStrategyMobile
//
//  Created by Ji Jin on 8/29/12.
//  Copyright (c) 2012 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "RWBinaryService.h"
#import "MSTRHandler.h"
#import "DeferExecCommon.h"

/*
 * max wait time before mobile server stop waiting for iserver and return results
 */
#define MAX_WAIT_TIME 300
#define MAX_WAIT_TIME_NEWSSTAND 1800000 //30 minutes
/*
 * HTTP header tag for passing message ID back
 */
#define MSGID_HEADER @"X-MSTR-msgID"

#define INBOX_MESSAGE_ERROR_CODE -99
#define MESSAGE_REQUEST_TIMEOUT_ERROR -1

@class MSIProjectConfiguration;
@class MSIDevicePreferences;

@interface RWDExecuteAndBinaryService : RWBinaryService <MSTRHandler, DeferExecCandidateReportStatusListener> {
    
@protected
	NSInteger timeout;
	NSInteger pollingFrequency;
	
	NSString *messageId;
	NSInteger messageStatus;
	NSInteger stateID;
	NSTimeInterval inboxMessageTime;
	
	NSInteger timeElapsed;
	NSTimeInterval timeStarted;
	MSIProjectConfiguration *projectConfig;
	NSString *sessionState;
	BOOL isExplicit;
	BOOL didRefresh;
	BOOL isManual;
	
	// Additional field used when executing a report or document...
	BOOL isPrompted;
	BOOL forPreload;
    
    // is the document/report background job?
    BOOL isBackground;
    
    //ASYNCEXC
    bool pollMessageStatusOnly;
    //ASYNCEXC~

	int binaryLimit;
    BOOL isSubscription;
    NSString* nodeKeys;
	NSString* definitionNodeKeys;
    BOOL useNewsstand;
    BOOL useBackgroundTransfer;
@private
	MSIDevicePreferences * devicePrefs;
}

@property NSInteger timeout;
@property NSInteger pollingFrequency;
@property(nonatomic, copy) NSString *messageId;
@property NSInteger messageStatus;
@property BOOL isExplicit;
@property BOOL isManual;
@property BOOL isPrompted;
@property BOOL forPreload;
@property BOOL isBackground;
@property bool pollMessageStatusOnly;

@property BOOL isSubscription;
@property (nonatomic, copy) NSString* nodeKeys;
@property (nonatomic, copy) NSString* definitionNodeKeys;
@property BOOL useNewsstand;
@property BOOL useBackgroundTransfer;
@property (nonatomic, copy) NSArray* dataSetIDs;

-(void)startInboxMessageRequest;
-(void)startPollStatusRequest;

@end
