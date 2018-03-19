//
//  ExecuteDocumentTaskRequest.h
//
//	This class is used to execute a document.
//
//  Copyright 2010 MicroStrategy, Incorporated. All rights reserved.
//

//	Comments
//	o Should this be moved to the Network group?
//	o Should it be accessible from the MSIServiceFactory? RequestFactory?

#import "MessageBasedTaskRequest.h"

@interface ExecuteDocumentTaskRequest : MessageBasedTaskRequest {
@private
	NSString* objectID;
	int execFlags;
	int gridResultFlags;
	NSString *styleName;
	BOOL minimalData;
	NSInteger executionMode;
    NSInteger selectorMode;
	
	NSString* originMessageID;
	NSString* promptsAnswerXML;
	NSString* objectsPromptAnswers;
	NSString* valuePromptAnswers;
	NSString* elementsPromptAnswers;
	NSString* linkAnswers;	
	NSString* srcAnswers;
	NSString* prevMsgID;
	NSInteger promptAnswerMode;
	
	int objType;
	int reportViewMode;
	int reportStateID;
	NSString* templateID;
	int creationFlags;
    BOOL backgroundExecution;
    NSString* deltaXML;
    
    //ASYNCEXC: The time to wait for the polling. Set this value to 0 (zero) if you don't need to poll for results
    int maxWait_;
}

@property (nonatomic, copy) NSString* objectID;
@property (nonatomic) int execFlags;
@property (nonatomic) int gridResultFlags;
@property (nonatomic, copy) NSString* styleName;
@property (nonatomic) BOOL minimalData;
@property (nonatomic) NSInteger executionMode;
@property (nonatomic) NSInteger selectorMode;

// properties specific to linked drilling
@property (nonatomic, copy) NSString* originMessageID;
@property (nonatomic, copy) NSString* promptsAnswerXML;
@property (nonatomic, copy) NSString* elementsPromptAnswers;
@property (nonatomic, copy) NSString* valuePromptAnswers;
@property (nonatomic, copy) NSString* objectsPromptAnswers;
@property (nonatomic, copy) NSString* linkAnswers;
@property (nonatomic, copy) NSString* srcAnswers;
@property (nonatomic, copy) NSString* prevMsgID;
@property (nonatomic) NSInteger promptAnswerMode;

@property (nonatomic) int objType;
@property (nonatomic) int reportViewMode;
@property (nonatomic) int reportStateID;
@property (nonatomic, strong) NSString* templateID;
@property (nonatomic) int creationFlags;
@property (nonatomic) BOOL backgroundExecution;
@property (nonatomic, copy) NSString* deltaXML;
@property (nonatomic) int maxWait;

@end
