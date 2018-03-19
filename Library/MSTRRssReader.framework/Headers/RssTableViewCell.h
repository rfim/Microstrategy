//
//  RssTableViewCell.h
//  RssFeedLib
//
//  Created by Victor Shilo on 5/6/10.
//  Copyright 2010 EastBanc Technologies. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "RssItem.h"

@interface RssTableViewCell : UITableViewCell {

    UILabel * titleLabel;
    UILabel * dateLabel;
    UILabel * durationLabel;
	UIImageView * icon;
    
    RssItem * rssItem;
}

@property (nonatomic, retain) RssItem * rssItem;

+(CGFloat) calculateHeightForRssItem:(RssItem *) rssItem;

@end
