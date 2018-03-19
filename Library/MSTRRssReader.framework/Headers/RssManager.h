//
//  RssManager.h
//  RssFeedLib
//
//  Created by Andrey Andreev on 4/29/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "RssParser.h"
#import "RssDownloader.h"

#define processRun @"run"
#define processComplete @"complete"
#define processFailed @"failed"

@protocol MSMRssDataDelegate;
@protocol MSMRssMainController;

/**
  RssManager interface is responsible for feed parsing and saving in storage
*/
@interface MSMRssManager : NSObject<MSMDownloaderDelegate, MSMParserDelegate, NSXMLParserDelegate> {
}
@property (nonatomic, unsafe_unretained) id<MSMRssDataDelegate> delegate;
@property (nonatomic, strong) NSMutableArray * feedsConfigs;
@property (nonatomic, strong) MSMRssCoreDataService * dataService;

-(id) initWithDelegate:(id<MSMRssDataDelegate,MSMRssMainController>)serviceDelegate ;

-(void) getFeeds;
-(void) destroyRSSItems;

-(void) loadFeedDetails:(RssFeed *)feed;

/**
   markAsRead method marks given item as read
 */
-(void) markAsRead: (RssItem *) item;
/**
   saveChanges method saves current context to storage
 */
-(void) saveChanges;

@end
