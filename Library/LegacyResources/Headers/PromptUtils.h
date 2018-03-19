//
//  PromptUtils.h
//  MSTRMobileSDK
//
//  Copyright 2010 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>

/**
 PromptUtils provides class methods in support of Prompt's. 
 */
@interface PromptUtils : NSObject
{
	// All class methods, no instances or ivars.
}

/**
 getElementPromptAnswers: generates prompt answers from an array. Use this method if you have multiple element prompts. 
 Each item in the array represents the prompt answer for one element prompt, which can be generated using getElementPromptAnswersForUnit method.
 
 The primary delimiter is comma. Secondary is semicolon. Third is caret.
 
Note: No delimiters are escaped or disallowed in the attributed IDs, element IDs and display names. Avoid ,; and ^ when choosing display names.
 
 Example: AttrID;ElementID^DisplayName;ElementID^DisplayName;...,AttrID;ElementID^DisplayName;...

 @return	@"" if the array is nil or empty.
*/ 
+ (NSString*) getElementPromptAnswers: (NSArray*) array;


/**
 getObjectPromptAnswers: will generate prompt answers from an array. Use this method if you have more than one object prompt. 
 Each item in the array is either an array of ObjectInfo's, or a string that represents the prompt answers for the prompt. 
 
 The primary delimiter is caret.
 
 Example: ElementID^DisplayName
 
 @return	@"" if the array is nil or empty.
 
 */
+ (NSString*) getObjectPromptAnswers: (NSArray*) array;


/**
 getObjectPromptAnswersForOneUnit: generates prompt answers from an array of ObjectInfo's for only one object prompt. 
 @param		objectInfos		contains an array of ObjectInfo's. 
 
 The primary delimiter is ESC (0x1b). Secondary is tilde (~).
 
 Example: objStr<ESC>objStr<ESC>objStr
		  objID~objType~objName<ESC>objID~objType~objName
 
 @return	@"" if the array is nil or empty.
 
*/
+ (NSString*) getObjectPromptAnswersForOneUnit: (NSArray*) objectInfos;


/**
 getValuePromptAnswers: generates value prompt answers from an array of NSString values. 
 
 The primary delimiter is caret.
 
 Example: value1^value2^value3
 
 @return	@"" if the array is nil or empty.
 
 */
+ (NSString*) getValuePromptAnswers: (NSArray*) values;


/**
 getElementPromptAnswersForUnit:elements: generates elements prompt answers for one element prompt, (such as attribute, custom group, etc).
 The parameter elements contains an array of element IDs in NSString format. 

 The primary delimiter is semicolon. Second is caret.
 
 Example: unitIDStr;element1Str;element2Str;element3Str
 
 @return	@"" if the array is nil or empty.
 
 */
+ (NSString*) getElementPromptAnswersForUnit: (NSString*) unitID elements: (NSArray*) elements;

@end



