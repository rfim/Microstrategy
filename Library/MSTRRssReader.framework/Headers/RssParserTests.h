//
//  RssParserTests.h
//  RssFeedLib
//
//  Created by Nikolay Sazonov on 7/7/10.
//  Copyright 2010 EastBanc Technologies. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "AbstractTestCase.h"

@class FeedConfig;

@interface RssParserTests : AbstractTestCase {
	
	@private 
	FeedConfig * feedConfig;
	

}

- (void) testFailedAtomParsing;
- (void) testSuccessfullAtomParsing;
- (void) testUnsupportedVersionError;
- (void) testSuccessfullRssParsing;
- (void) testParserValidatorError;
- (void) testRssWithOptionalElements;

@end
