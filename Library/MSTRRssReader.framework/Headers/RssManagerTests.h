//
//  RssManagerTests.h
//  RssFeedLib
//
//  Created by Nikolay Sazonov on 7/9/10.
//  Copyright 2010 EastBanc Technologies. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "AbstractTestCase.h"

@class RssManager;
@class TestRssDataDelegate;



@interface RssManagerTests : AbstractTestCase {
	@private
	RssManager * rssManager;
	TestRssDataDelegate * dataDelegate;
	
}

- (void) testGetFeeds;
- (NSArray *) createConfigArray: (int) count;
- (NSArray *) createRandomConfigArray: (NSArray *) indexes;

@end
