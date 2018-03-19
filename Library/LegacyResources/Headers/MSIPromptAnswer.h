//
//  PromptAnswer.h
//  MSTRMobile
//
//  Created by Victor Pena on 9/31/10.
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSIElement.h"
#import "Prompts.h"

typedef enum _AnswerModes{
	EnumAnswerModeSamePrompt = 1, EnumAnswerModeDoNotAnswer, EnumAnswerModeClose, EnumAnswerModeDynamic, EnumAnswerModeStatic, EnumAnswerModeCurrentUnit, EnumAnswerModeAllValidUnits, EnumAnswerModeDefaultAnswer
} AnswerModes;

/**
 This class was only used for linked drilling. It is starting to be used with Prompt, Prompts, PromptHelper or PromptUtils.
 
 Feb 2012: We are building on this class to make it useful for all Prompt answers.
 */
@interface MSIPromptAnswer : NSObject <NSCopying> {
@private
	PromptTypes promptType;
	NSString* promptGUID;
	NSInteger originType;
	NSString* originGUID;
	NSInteger answerMode;
	NSMutableArray* staticElements;
    NSString* baseFormID;
}

-(MSIElement*)newStaticElement;
-(void)clearAnswer;

@property PromptTypes promptType;
@property (nonatomic, copy) NSString* promptGUID;
@property NSInteger originType;
@property (nonatomic, copy) NSString* originGUID;
@property NSInteger answerMode;
@property (nonatomic, strong) NSMutableArray* staticElements;
@property (nonatomic, strong) NSString* stringValue;	// nonpreferred access when not a Text answer
//DL;07/09/2012;617393;support value prompt
@property (nonatomic, copy) NSString* baseFormID;

@end
