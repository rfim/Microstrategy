//
//  AnswerPromptsTaskRequest.h
//  MSTRMobile
//
//  Created by Glenn Boysko on 4/6/10.
//  Copyright 2010 MicroStrategy, Incorporated. All rights reserved.
//

#import "MessageBasedTaskRequest.h"

@interface AnswerPromptsTaskRequest : MessageBasedTaskRequest {
@private
	NSInteger objectType;
	NSString* promptAnswerXML;
	BOOL duplicateMessage;
	BOOL validateMessage;
}

// Properties...
@property (nonatomic, copy) NSString* promptAnswerXML;
@property NSInteger objectType;
@property BOOL duplicateMessage;
@property BOOL validateMessage;

@end
