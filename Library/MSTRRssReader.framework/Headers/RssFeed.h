//
//  RssFeed.h
//  MicroStrategyMobile
//
//  Created by Peng, Yan on 9/18/14.
//  Copyright (c) 2014 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreData/CoreData.h>

@class RssItem;

@interface RssFeed : NSManagedObject

@property (nonatomic, retain) NSNumber * index;
@property (nonatomic, retain) NSNumber * viewIndex;
@property (nonatomic, retain) NSString * metaLoadProcess;
@property (nonatomic, retain) NSString * downloadProcess;
@property (nonatomic, retain) NSString * pubDate;
@property (nonatomic, retain) NSString * reportID;
@property (nonatomic, retain) NSString * thumbnailUrl;
@property (nonatomic, retain) NSString * desc;
@property (nonatomic, retain) NSString * title;
@property (nonatomic, retain) NSString * url;
@property (nonatomic, retain) NSNumber * visible;
@property (nonatomic, retain) NSString * parserProcess;
@property (nonatomic, retain) NSString * thumbnailProcess;
@property (nonatomic, retain) NSNumber * reportType;
@property (nonatomic, retain) NSString * downLoadDate;
@property (nonatomic, retain) NSSet *items;
@end

@interface RssFeed (CoreDataGeneratedAccessors)

- (void)addItemsObject:(RssItem *)value;
- (void)removeItemsObject:(RssItem *)value;
- (void)addItems:(NSSet *)values;
- (void)removeItems:(NSSet *)values;

@end
