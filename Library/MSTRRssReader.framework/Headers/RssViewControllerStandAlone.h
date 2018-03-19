//
//  RssViewControllerStandAlone.h
//  RssFeedLib
//
//  Created by Nikolay Sazonov on 7/12/10.
//  Copyright 2010 EastBanc Technologies. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "RssProtocols.h";

@class RssManager;
@class RssUISectionViewController;
@class RssEditViewController;
@class ReportDataService;
@class RssFeed;
@class RssTableViewModel;
@class RssStatusBarView;

@protocol RssDataDelegate;
@protocol SelectedItems;
@protocol RssTableViewSourceDelegate;
@protocol ArticleDelegate;

@interface RssViewController : UIViewController<RssDataDelegate, SelectedItems, RssTableViewSourceDelegate, 
	UINavigationControllerDelegate, ArticleDelegate> {
	
	@private
	RssManager * rssManager;
	RssUISectionViewController * sectionViewController;
	RssEditViewController * editViewController;
	UIView * mstrView;
	CGRect initialFrame;
	RssTableViewModel * model;	
	UIBarButtonItem * editButton;
	UIBarButtonItem * doneButton;
	UINavigationItem * navigationItem;
	RssStatusBarView * statusBar;
	
	
}

- (id)	initWithFrame:(CGRect) frame;

@property (nonatomic, retain) RssTableViewModel * model;

@end
