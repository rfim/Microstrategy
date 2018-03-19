//
//  RssUIViewController.h
//  RssFeedLib
//
//  Created by Roman Zolin on 4/30/10.
//  Copyright 2010 EastBanc Technologies. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "RssHeaderInSection.h"


@protocol MSMRssMainController;
@protocol MSMSelectedItems;

@class RssItem;
@class MSMRssTableViewModel;

@interface MSMRssSectionView: UITableView {
 
}
@property(nonatomic,unsafe_unretained) id<MSMRssMainController> zoomDelegate;
@end

@interface MSMRssUISectionViewController : UITableViewController <UITableViewDelegate, UITableViewDataSource, MSMRssHeaderInSectionDelegate> {
	
}

@property (nonatomic, strong) NSArray *feeds;
@property (nonatomic, unsafe_unretained) id<MSMSelectedItems,MSMRssMainController> delegate;
@property (nonatomic, unsafe_unretained) MSMRssTableViewModel * model;

- (id) initWithDelegate: (id<MSMSelectedItems,MSMRssMainController>) selectedItemsDelegate frame: (CGRect) rect title: (NSString *) title;
/**
 Refresh model with new value and then redraws view
 */
- (void) displayViewForModel: (MSMRssTableViewModel *) feedsModel;
/**
 Invokes from RssViewController. Makes UITableView instance to redisplay visible rows
 */
- (void) reloadView;
- (void) deselectReadArticle;
- (void) reloadAndSelectArticle:(RssItem *) article feedURL:(NSString *) feedURL;
- (void) setTitleForFeed:(NSString *) url;


@end
