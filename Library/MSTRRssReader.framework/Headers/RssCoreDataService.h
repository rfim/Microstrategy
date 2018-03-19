//
//  RssCoreDataService.h
//  RssFeedLib
//
//  Created by Nikolay Sazonov on 6/29/10.
//  Copyright 2010 EastBanc Technologies. All rights reserved.
//

#import <CoreData/CoreData.h>

@class RssItem;
@class RssFeed;
@class MSMFeedConfig;
@protocol MSMRssMainController;

#define MODEL_NAME @"RssFeedData"

@interface MSMRssCoreDataService : NSObject {
	
	@private
	NSPersistentStoreCoordinator * persistentStoreCoordinator;
	NSManagedObjectModel * managedObjectModel;
	NSManagedObjectContext * managedObjectContext;
    id<MSMRssMainController> __unsafe_unretained mainController;
}

@property (nonatomic, unsafe_unretained) id<MSMRssMainController> mainController;

- (id)initWithDelegate:(id<MSMRssMainController>)rssMainController;

+ (NSString *)applicationDocumentsDirectory;

// create and initialize new RssItem instance, insert it into ManagedObjectContext
- (RssItem *) createItem;

// create new RssFeed instance, initialize it from passedFeedConfig, insert it into ManagedObjectContext
// displayIndex is used to order feeds in table view
- (RssFeed *) createFeedFromConfig: (MSMFeedConfig *) feedConfig withDisplayIndex: (NSNumber *) index;

// loads array of RssFeeds with initialized collection of items from persistent store
- (NSArray *) loadAllFeeds:(NSString *) reportID reportType:(NSInteger) reportType;

- (RssFeed *) loadFeedWithURL:(NSString *)url reportID:(NSString *) reportID reportType:(NSInteger) reportType;

// update existent feed with collection of new items 
- (void) updateFeed: (RssFeed *) feed withTransientItems: (NSMutableDictionary *) items;

// remove feed with its items and propogate changes to data store immidiately
- (void) deleteFeed:(RssFeed *)feed;

// remove item and propogate changes to data store immidiately
- (void) deleteItem: (RssItem *) item;

// populate feed download status field with passed value and propogate changes to data store immidiately
- (void) setDownloadStatus: (NSString *) statusValue forFeed: (RssFeed *) feed;

- (void) setParserStatus: (NSString *) statusValue forFeed: (RssFeed *) feed;

- (void) saveContext;

@end
