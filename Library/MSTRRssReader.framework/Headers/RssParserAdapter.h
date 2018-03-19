//
//  ParserAdapter.h
//  RssFeedLib
//
//  Created by Andrey Andreev on 5/1/10.
//  Copyright 2010 EastBanc Technologies. All rights reserved.
//


@protocol MSMParserDelegate;

@class MSMRssCoreDataService;
@class RssFeed;
@class RssItem;

#define RSS_FEED_ROOT @"rss"
#define ATOM_FEED_ROOT @"feed"

@protocol MSMRssValidator

- (BOOL) validateItem: (RssItem *) item;

@end


@interface MSMParserAdapter : NSObject {
	
}

@property (nonatomic, strong) RssItem * currentItem;
@property (nonatomic, strong) NSMutableString * currentItemValue;
@property (nonatomic, strong) RssFeed * feed;
@property (nonatomic, strong) NSMutableArray * elements;
@property (nonatomic, strong) NSMutableDictionary * items;
@property (nonatomic, strong) NSString * lmt;
@property (nonatomic, unsafe_unretained) MSMRssCoreDataService * dataService;

-(id) initWithFeed:(RssFeed *)_feed dataSerivce:(MSMRssCoreDataService *) _dataService validator: (id<MSMRssValidator>) _validator parserDelegate:(id<MSMParserDelegate>)_parserDelegate;

- (void)parser:(NSXMLParser *)parser didStartElement:(NSString *)element attributes:(NSDictionary *)attributes;

- (void)parser:(NSXMLParser *)parser didEndElement:(NSString *)element;


- (void)parser:(NSXMLParser *)parser 
	foundCharacters:(NSString *)string;

@end

@interface MSMRssParserAdapter : MSMParserAdapter {	
}
@end

@interface MSMRss2ParserAdapter : MSMRssParserAdapter {
}
@end

@interface MSMAtomParserAdapter : MSMParserAdapter {	
}
@end


