//
//  RssIPadViewController.h
//  MSTRRssReader
//
//  Created by Alexandr Gordienko on 20.01.11.
//  Copyright 2011 EastBanc Technologies. All rights reserved.
//

#import <UIKit/UIKit.h>


#import <MicroStrategy iPad Main/MSTRViewController.h>
#import <MicroStrategy iPad Main/MSTRView.h>

#import "RssProtocols.h"

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

@interface RssIPadViewController : MSTRViewController<RssDataDelegate, SelectedItems, RssTableViewSourceDelegate, 
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
	
}

-(void) showErrorMessage: (NSString*) err_msg;

@end

