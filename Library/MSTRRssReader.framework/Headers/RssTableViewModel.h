//
//  RssTableViewModel.h
//  RssFeedLib
//
//  Created by Nikolay Sazonov on 5/27/10.
//  Copyright 2010 EastBanc Technologies. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@protocol MSMRssTableViewSourceDelegate;

@class RssItem;


/**
 Stores feeds loaded from MSTR library.
 Provide methods to manipulate with feeds data
 */
@interface MSMRssTableViewModel : NSObject {
	
}

-(id) initWithArray: (NSArray *) array andDelegate: (id<MSMRssTableViewSourceDelegate>) viewDelegate;

/**
 sortFeeds method sorts feeds array using feedSort comparator function
 */
-(void) sortFeeds;
/**
 Sorts rss items by date
 */
-(void) sortItemsForURL: (NSString *)feedURL;

/**
 isAllFeedsLoaded method checks if all feeds loaded already
 @return TRUE if all feeds loaded
 */
-(BOOL) isAllFeedsLoaded;

/**
 isFeedLoaded checks if feed loaded
 @param index - feed's index from feedsArray
 @return TRUE if feed loaded already
 */
-(BOOL) isFeedLoaded: (NSInteger) index;

-(NSString*) getLatestDownloadDate;

/**
 isAtLeastOneFeedFailed method return TRUE at least one rss feed failed during loading
 */
-(BOOL) isAtLeastOneFeedFailed;
-(NSInteger) getFeedsNumber;
-(void) setFeedVisible: (NSInteger) index visible: (BOOL) flag;
-(BOOL) isFeedVisible:(NSInteger) index;
-(NSString *) getTitleForFeedWithIndex:(NSInteger)index;


-(NSInteger) getVisibleFeedsNumber;
-(NSString *) getTitleForVisibleFeedWithIndex:(NSInteger) index;

-(BOOL) isVisibleFeedExpanded:(NSInteger) feedIndex;
-(void) setVisibleFeedExpanded:(NSInteger) feedIndex flag:(BOOL) expanded;

-(NSInteger) getNumberOfVisibleItemsWithFeed: (NSInteger) feedIndex;
-(NSInteger) getItemsNumberForVisibleFeed: (NSInteger) feedIndex;
-(NSInteger) getItemsNumberForVisibleFeedURL:(NSString *) feedURL;

-(RssItem *) getItemForVisibleFeedIndex: (NSInteger) feedIndex itemIndex: (NSInteger) index;
-(RssItem *) getItemForVisibleFeedURL: (NSString *) feedURL itemIndex: (NSInteger) index;
-(NSString *) getFeedUrlForVisibleFeedIndex: (NSInteger) index;
-(NSInteger) getIndexForItem:(RssItem *)item inFeed:(NSString *)feedURL;
-(NSInteger) getVisibleFeedIndexForURL:(NSString *) feedURL;

-(void) moveFeed: (NSInteger) fromIndex toIndex: (NSInteger) index;
-(void) didEditedFeedsSelection;

-(void) setFeedsConfig:(NSArray*)cfgs;

@end


