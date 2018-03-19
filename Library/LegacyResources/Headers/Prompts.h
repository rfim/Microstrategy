//
//  Prompts.h
//  MSTRMobile
//
//  Created by Quality Engineering on 2/4/10.
//  Copyright 2010 MicroStrategy, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MIIDynamicTableDataIterator.h"



typedef enum _PromptTypes{
	EnumPromptTypeUnsupported = 0,
	EnumPromptTypeConstant,		// These are Value prompts and can be numeric, text, date&time, Big Decimal.
	EnumPromptTypeElements,
	EnumPromptTypeExpression,	// Not yet supported
	EnumPromptTypeObjects,		// Not yet supported
	EnumPromptTypeDimty			// Not yet supported
}PromptTypes;

@class MSIPrompt, ElementsPrompt, SAXSupport, MSIReportDisplayInfo, MSTRError;

/**
The Prompts class contains info about the prompts for a document, including supported prompts and all other prompts, local, status, error info, etc.
 
 @see Prompt and @see PromptHelper
 */
@interface Prompts : NSObject <NSCopying, MIIDynamicTableDataIterator> {
@private
	// Contains only supported prompts
	NSMutableArray* prompts;

	// Contains all prompts
	NSMutableArray* allPrompts;
	NSString* locale;
	MSIReportDisplayInfo* document;
	MSTRError* xmlError;
	BOOL readyForAnswering;
	BOOL preparationCanceled;
	BOOL hidesPromptsForAnsweringLocation;
    BOOL polarisVerver;
}

/** 
 An array of supported Prompt objects
 */
@property (nonatomic, strong) NSMutableArray* prompts;

/**
 An array of all Prompt objects, including unsupported ones
 */
@property (nonatomic, strong) NSMutableArray* allPrompts;

@property (nonatomic, copy) NSString* locale;
@property (nonatomic, strong) MSIReportDisplayInfo* document;
@property (nonatomic, readonly) MSTRError* xmlError;
@property BOOL readyForAnswering;
@property BOOL preparationCanceled;
@property BOOL hidesPromptsForAnsweringLocation;
@property BOOL polarisVerver;
-(id)initWithData:(NSData*)data withDocument:(MSIReportDisplayInfo*)doc;
-(NSString*)getXml;
-(NSString*)getAnswerXml;
//-(void)beforeBuildAnswerXml;
-(void)loadAnswers:(NSString*)xml;
-(BOOL)validateAnswers;
/**
 True if any of the prompts require preparation before submitting the answers. An element prompt with date answer requires preparation.
* /
- (BOOL) requiresPreparation;*/

-(void)prepareForAnswering;
-(void)cancelPreparation;
-(void)cancelRunningServices;

@end
