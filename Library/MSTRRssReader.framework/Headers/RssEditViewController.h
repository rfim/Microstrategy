//
//  RssEditViewController.h
//  RssFeedLib
//
//  Created by Nikolay Sazonov on 5/19/10.
//  Copyright 2010 EastBanc Technologies. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "RssTableViewModel.h"

@protocol RssTableViewSourceDelegate;

@interface RssEditViewController : UITableViewController <UITableViewDataSource, UITableViewDelegate> {
	
	UIView * headerView;
	@private
	RssTableViewModel * model;
	
}

-(id) initWithFrame: (CGRect) frame;
-(void) displayViewForModel: (RssTableViewModel *) feedsModel;

@end



