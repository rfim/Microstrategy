//
//  RSSParser.h
//  RssFeedLib
//
//  Created by Andrey Andreev on 4/27/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "RssParserAdapter.h"


@protocol MSMParserDelegate;
//@protocol RssMainController;

@class MSMRssCoreDataService;

@interface MSMRssParser : NSObject<MSMRssValidator, NSXMLParserDelegate> {

}


- (id) initWithDelegate:(id <MSMParserDelegate>)_delegate andDataService: (MSMRssCoreDataService *) _dataService ;
- (void) performParsing:(NSData *)_data forFeed:(RssFeed *) _feed;

@end


@protocol MSMParserDelegate

- (void) parseCompleted:(RssFeed *) feed;
- (void) parseFailed:(NSError *)error feed:(RssFeed *) feed;

- (NSMutableArray*)getFeedsConfigs;
@end


