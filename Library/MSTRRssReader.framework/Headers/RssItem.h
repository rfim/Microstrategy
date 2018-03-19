//
//  RssItem.h
//  RssFeedLib
//
//  Created by Andrey Andreev on 4/29/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <CoreData/CoreData.h>

@class RssFeed;

@interface RssItem :  NSManagedObject  
{
}

@property (nonatomic, strong) NSString * title;
@property (nonatomic, strong) NSString * pubDate;
@property (nonatomic, strong) NSString * desc;
@property (nonatomic, strong) NSNumber * index;
@property (nonatomic, strong) NSNumber * unread;
@property (nonatomic, strong) NSString * link;
@property (nonatomic, strong) RssFeed * feed;

@end



