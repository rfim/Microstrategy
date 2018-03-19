//
//  LogicTests.h
//  RssFeedLib
//
//  Created by Nikolay Sazonov on 7/6/10.
//  Copyright 2010 EastBanc Technologies. All rights reserved.
//
//  See Also: http://developer.apple.com/iphone/library/documentation/Xcode/Conceptual/iphone_development/135-Unit_Testing_Applications/unit_testing_applications.html

//  Application unit tests contain unit test code that must be injected into an application to run correctly.
//  Define USE_APPLICATION_UNIT_TEST to 0 if the unit test code is designed to be linked into an independent test executable.

//#import <SenTestingKit/SenTestingKit.h>
//#import <UIKit/UIKit.h>
//#import "application_headers" as required

#import "AbstractTestCase.h"

@class RssCoreDataService;

@interface RssCoreDataServiceTests : AbstractTestCase {
	

}


- (void) testCreateItem;
- (void) testCreateFromFeedConfig;
- (void) testLoadAllFeeds;
- (void) testLoadAllFeedsByReport;
- (void) testLoadFeedWithURL;
- (void) testUpdateFeed;
- (void) testDeleteFeed;
- (void) testDeleteItem;
- (void) testSetDownloadStatus;
- (void) testSetParserStatus;

- (FeedConfig *) createFeedConfig:(int) feedIndex;
- (FeedConfig *) createFeedConfig:(int) feedIndex reportID:(NSString *) reportID;
- (RssItem *) createRSSItem:(int) itemIndex;


@end
