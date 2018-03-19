//
//  RssViewController.h
//  RssFeedLib
//
//  Created by Victor Shilo on 5/4/10.
//  Copyright 2010 EastBanc Technologies. All rights reserved.
// 
//
//  This is a main RSS class
//

#import <UIKit/UIKit.h>

#import "MSTRViewController.h"
#import "MSTRView.h"

#import "RssProtocols.h"
//#import "BaseWidget.h"


#define DEFAULT_REPORT_TITLE @"Default report title"


@class RssManager;
@class RssUISectionViewController;
@class RssEditViewController;
@class ReportDataService;
@class RssFeed;
@class RssTableViewModel;
@class RssStatusBarView;

@class MSITabularData;


@protocol RssDataDelegate;
@protocol SelectedItems;
@protocol RssTableViewSourceDelegate;
@protocol ArticleDelegate;

@interface RssViewController : MSTRViewController<RssDataDelegate, SelectedItems, RssTableViewSourceDelegate, 
	UINavigationControllerDelegate, ArticleDelegate> {

	@private
	// instance of RssManager class is responsible for feed parsing and saving in storage
	RssManager * rssManager;
	RssUISectionViewController * sectionViewController;
	RssEditViewController * editViewController;
    // mstrView instance is responsible for data visualization
	MSTRView * mstrView;
	// Stores data received from MSTR library
	MSITabularData *reportData;	
	// Stores base view frame's size (MSTRViewController)
	CGRect initialFrame;
	// Instance of RssTableViewModel stores loaded feeds
	RssTableViewModel * model;	
	// Button allows to switch in Edit mode
	UIBarButtonItem * editButton;
	// Button allows to save changes made in Edit mode
	UIBarButtonItem * doneButton;
	// Navigation element contains buttons and title to indicate actual state
	UINavigationItem * navigationItem;
	// Status bar located in the lower screen size and show time when item was updated
	// or error messages
	RssStatusBarView * statusBar;
	UILabel *errorMessage;
	// TRUE if Edit mode is active
	BOOL editTitleState;
	//string which holds all downloader and parser error messages to be shown in Alert box
	NSString * alertMessage;
	UIActivityIndicatorView *activityIndicator;
//	BaseWidget *baseWidget;
}

-(void) showErrorMessage: (NSString*) err_msg;


//@property (nonatomic, retain) BaseWidget *baseWidget;
@end

