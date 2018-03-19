//
//  ElementPromptAnswer.h
//  MicrostrategyMobile
//
//  Copyright 2012 MicroStrategy, Inc.. All rights reserved.
//

#import "MSIPromptAnswer.h"

/**
 An ElementPromptAnswer contains 0 or more Element objects. Each might represent an element of Category, such as "Books", "Electronics", etc. They can also include dates.
 
 While you can use setElements: with an array of Elements directly, it is convenient to provide an array of strings. In that case, they will be looked up on the server as needed in order to get Element objects. Or use setStringValue: where the string is a comma-space-delimited string. E.g.: "Books, Electronics, Movies" or "02/14/2012, 03/17/2012, 07/04/2012".
 */
@interface MSIElementPromptAnswer : MSIPromptAnswer
{
	/**
	 array of Element or NSString. Can be used for Dates. All items in the arrya must be the same type.
	 */
	NSMutableArray*		mElements;
	
	@private
	BOOL				mHaveElementIDs;
}

@property (nonatomic, strong) NSArray*	elements;
@property (readonly)		  BOOL		haveElementIDs;


- (NSUInteger) count;

/**
	 Provides the requested Element or NSString.
	 @param	index	0-based index of the requested element. If it is out of range, an exception is thrown.
	 @return	the Element or string at the given index.
	 @throw		NSRangeException when index is out of range.
 */
- (MSIElement*) elementAtIndex: (NSUInteger) index;

// This used to be done by PromptHelper.
- (BOOL) didElementsChange: (NSArray*) elements;

// This method is to expose the mutable array of mElements so that others can directly add / remove the mElements
- (NSMutableArray*) getMutableElements;

// Sort elements by elementID
- (void) sortElements;

@end
