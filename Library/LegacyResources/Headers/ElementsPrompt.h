//
//  ElementsPrompt.h
//  MSTRMobile
//
//  Created by Quality Engineering on 2/4/10.
//  Copyright 2010 MicroStrategy, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "Enums.h"
#import "MSIPrompt.h"

@class MSIObjectInfo, MSIFilter, MSTRError, SAXStringHandler, BrowseElementsService;

@interface ElementsPrompt : MSIPrompt {
@private
//	NSMutableArray* answerElements;
	MSIObjectInfo* origin;
	NSString* dateFormID;
	MSIFilter* restriction;
//	NSArray* dateAnswer;
	MSTRError* taskError;
	NSMutableArray* suggestedAnswers;
	BOOL isParsingSuggestedAnswerList;
	NSString* dataSourcesXML;
	SAXStringHandler* dsrcsHandler;
	BrowseElementsService *currentService;
	BOOL mClassConfirmed;

}

//@property (nonatomic, retain) NSMutableArray* answerElements;
@property (nonatomic, strong) MSIObjectInfo* origin;
@property (nonatomic, readonly) NSString* dateFormID;
@property (nonatomic, strong) MSIFilter* restriction;
@property (nonatomic, strong) NSArray* dateAnswer;
@property (nonatomic, strong) NSArray* suggestedAnswers;
@property (nonatomic, copy) NSString* dataSourcesXML;
@property (readonly) BOOL haveElementIDs;

/**
 Creates a string from the answer dates or answer elements. The parts are delimited by @", ".
 @return the string of answers
*/
- (NSString*) getAnswerString;

- (MSIPromptAnswer*) newAnswerWithType: (DataTypes) dataType;

-(BOOL) isDateSelection;
-(void)cancelRunningService;

@end
