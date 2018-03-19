//
//  ExecuteReportTaskRequest.h
//
//	This class is used to execute a report.
//
//  Copyright 2010 MicroStrategy, Incorporated. All rights reserved.
//

#import "MessageBasedTaskRequest.h"

@interface ExecuteReportTaskRequest : MessageBasedTaskRequest {
@private
	NSString* reportID;
	int execFlags;
	int reportViewMode;
	int resultFlags;
	NSString* styleName;

	NSString* originMessageID;
	NSString* promptsAnswerXML;
	NSString* objectsPromptAnswers;
	NSString* valuePromptAnswers;
	NSString* elementsPromptAnswers;
	NSString* linkAnswers;
	NSString* srcAnswers;

}

@property (nonatomic, copy) NSString* reportID;
@property (nonatomic) int execFlags;
@property (nonatomic) int reportViewMode;
@property (nonatomic) int resultFlags;
@property (nonatomic, copy) NSString* styleName;

// properties specific to linked drilling
@property (nonatomic, copy) NSString* originMessageID;
@property (nonatomic, copy) NSString* promptsAnswerXML;
@property (nonatomic, copy) NSString* elementsPromptAnswers;
@property (nonatomic, copy) NSString* valuePromptAnswers;
@property (nonatomic, copy) NSString* objectsPromptAnswers;
@property (nonatomic, copy) NSString* linkAnswers;
@property (nonatomic, copy) NSString* srcAnswers;

@end
