//
//  Prompt.h
//  MSTRMobile
//
//  Created on 2/4/10.
//  Copyright 2010 MicroStrategy, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSIObjectInfo.h"
#import "MSTRHandler.h"
#import "MIIDynamicTableDataItem.h"

#import "Prompts.h"
#import "MSIPromptAnswer.h"


typedef enum _PromptValidationResult{
	PromptValidationSuccess = 0,
	PromptValidationMinError = 1,
	PromptValidationMaxError = 2
} PromptValidationResult;


@class SAXSupport, XMLBuilder, MSTRFormatter, MSIReportDisplayInfo, MSTRError, ErrorDetails;

/**
	A Prompt is used when a document needs to ask the user for answers. The two types currently supported are Element and Value (a.k.a. Constant). Object, Expression and Dimty are not yet supported.
 
	 Subclasses are responsible for the answers. Feb 2012: We will be migrating answer responsibility into the PromptAnswer class and its new subclasses.
 
	@see PromptHelper and @see PromptUtils for more info on using Prompts.
 */
@interface MSIPrompt : MSIObjectInfo <MSTRHandler, NSCopying, MIIDynamicTableDataItem> {
	PromptTypes promptType;

	NSString* title;	// E.g. Revenue Value
	NSString* min;
	NSString* max;
	NSString* meaning;	// E.g. Enter a numeric value for Revenue.
	BOOL required;
	NSMutableDictionary* properties;	// Keys like "DisplayStyle", "AttributeForms", etc.
	BOOL			handlingAnswer;
	MSTRFormatter*	formatter;
	BOOL			hasAnswer;			// This flag comes from XML and is not used for the hasAnswer property.
	MSIPromptAnswer*	mAnswer;			// Our subclasses will handle the specific type of answer
	BOOL			mIsHidden;			// This flag describes the prompt is hidden for rendering or not
	BOOL			mIsClosed;			// This flag describes the prompt is closed

@private
	NSString* locationId;
	NSInteger locationType;
	NSInteger locationPin;
	NSInteger dssPromptType;
	MSIReportDisplayInfo* __weak document;
	ErrorDetails* errorDetails;
}

/**
 The type of prompt will be one of the PromptTypes enum values in Prompts.h. E.g. EnumPromptTypeElements.
 @see PromptTypes and @see Prompts
 */
@property PromptTypes promptType;

@property (nonatomic, copy) NSString* title;
@property (nonatomic, copy) NSString* meaning;

@property NSInteger locationType;
@property NSInteger locationPin;
@property (nonatomic, copy) NSString* locationId;
@property (nonatomic, copy) NSString* min;
@property (nonatomic, copy) NSString* max;
@property (nonatomic, strong) NSMutableDictionary* properties;
@property NSInteger dssPromptType;
@property BOOL required;
@property (nonatomic, weak) MSIReportDisplayInfo* document;
@property (nonatomic, strong) ErrorDetails* errorDetails;
@property (nonatomic, strong) MSIPromptAnswer* answer;
@property (nonatomic, readonly) BOOL hasAnswer;
@property (nonatomic, getter = isHidden) BOOL hidden;
@property (nonatomic, getter = isClosed) BOOL closed;


/**
 Checks that the prompt has an answer, if one is required.
 
 @return	YES	if the prompt is not missing a required answer.
 */
- (BOOL) validateAnswer;
- (void) rebuildFormatter;
-(NSInteger) validateMinMax: (NSInteger) count;
-(MSTRFormatter*)formatter;

-(void)addLocationAttributes: (XMLBuilder *) builder;
-(void)buildAnswerXml:(XMLBuilder*)builder;
-(MSTRError*)beforeBuildAnswerXml;
-(void)buildTypeSpecificAnswerXml:(XMLBuilder*)builder;

/**
 getAnswerHandler is called just before a new answer is parsed from XML. Any old answer should be discarded before parsing begins. The discarding is being done within getAnswerHandler.
 
 @return the Prompt object that will handle the parsing of the answer.
 */
-(id<MSTRHandler>)getAnswerHandler;

-(void)setErrorWithMessage:(NSString*)msg;

@end

