//
//  DateElementPromptAnswer.h
//  MicroStrategyMobile
//
//  Copyright 2012 MicroStrategy Inc. All rights reserved.
//

#import "MSIElementPromptAnswer.h"

/**
 A DateElementPromptAnswer contains 0 or more Element objects. Each represents a date element, such as "02/14/2012", "03/17/2012", "07/04/2012"
 */
@interface MSIDateElementPromptAnswer : MSIElementPromptAnswer

/**
 Provides the requested Element or NSString.
 @param	index	0-based index of the requested element. If it is < 0 or >= count, an exception is thrown.
 @return	the Element or string at the given index.
 @throw		NSRangeException when index is out of range.
 */
- (NSString*) elementAtIndex: (NSUInteger) index;



/**
 You can use setElements: with an array of date Elements directly. However, it is often convenient to provide an array of strings. In that case, they will be looked up on the server as needed in order to get Element objects. Or use setStringValue: where the string is a comma-space-delimited string. E.g.: "02/14/2012, 03/17/2012, 07/04/2012".
 * /
- (void) setElements: (NSArray*) newElements;
*/
@end
