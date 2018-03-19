//
//  RssCustomTableViewCell.h
//  RssFeedLib
//
//  Created by Nikolay Sazonov on 6/16/10.
//  Copyright 2010 EastBanc Technologies. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#define CELL_ITEM_IDENTIFIER @"RssTableViewCell"
#define CELL_LOADING_IDENTIFIER @"LoadingCell"


@class MSMRssTableItemView;
@class RssItem;
@protocol MSMRssMainController;
@interface MSMRssCustomTableViewCell : UITableViewCell {

}

@property (nonatomic, strong) MSMRssTableItemView * itemView;

+ (CGFloat) calculateHeightForRssItem:(RssItem *) rssItem withMainController:(id<MSMRssMainController>)rssMainController;
- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier mainController:(id<MSMRssMainController>)rssMainController;

- (void) setItem: (RssItem *) item;


@end
