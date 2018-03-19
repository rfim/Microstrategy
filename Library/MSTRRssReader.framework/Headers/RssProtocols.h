//
//  RssProtocols.h
//  RssFeedLib
//
//  Created by Nikolay Sazonov on 6/30/10.
//  Copyright 2010 EastBanc Technologies. All rights reserved.
//

@class RssFeed;
@class RssItem;

typedef enum {
	MSMRssProcessingDownloadError,
	MSMRssProcessingNoDataError,
	MSMRssProcessingParserError
} MSMRssProcessingError;

@protocol MSMRssDataDelegate

-(void) onGetFeeds:(NSArray *)list;
-(void) onGetFeedDetails:(RssFeed *)feed;
/**
 failFeedProcessing method used to indicate error during rss feed processing(validate data and parse). Called from RSSManager class.
 */
-(void) failFeedProcessing:(RssFeed *) feed withError: (MSMRssProcessingError) error;

@end

@protocol MSMSelectedItems

-(void)didSelectItem:(NSInteger )itemIndex feedURL:(NSString *) feedURL;

@end

@protocol MSMRssTableViewSourceDelegate

- (void) didFeedsSorted;

@end;

@protocol MSMArticleDelegate 

-(void) willReadArticle:(RssItem *) article feedURL:(NSString *) feedURL;

-(void) didReadArticle;

@end




