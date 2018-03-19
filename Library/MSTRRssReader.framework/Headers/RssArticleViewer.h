//
//  RssArticleViewer.h
//  MSTRRssReader
//
//  Created by Tian Baoming on 6/8/11.
//  Copyright 2011 EastBanc Technologies. All rights reserved.
//

/*
 this class is created to handle the case when RssWidget can only use
 part of the screen. most of the code is similar with RssArticleViewControllor
 however it seems hard to remove the duplicate code now
 */

#import <UIKit/UIKit.h>
#import <MessageUI/MFMailComposeViewController.h>
#import <WebKit/WebKit.h>

@class MSMRssButtonView;
@class MSMRssDateUtils;
@class RssItem;
@class MSMRssStatusBarView;
@class MSMRssTableViewModel;

@protocol MSMArticleDelegate;
@protocol MSMRssMainController;

@interface RssArticleViewer : UIView<MFMailComposeViewControllerDelegate, UIActionSheetDelegate, WKNavigationDelegate> {

}

@property (nonatomic, strong) WKWebView	*bodyView;
@property (nonatomic, assign) NSInteger     itemIndex;
@property (nonatomic, strong) RssItem		*item;
@property (nonatomic, strong) MSMRssTableViewModel * model;
@property (nonatomic, strong) NSString * feedURL;
@property (nonatomic, unsafe_unretained) id<MSMArticleDelegate,MSMRssMainController> delegate;

-(id) initWithFrame:(CGRect) rect Model:(MSMRssTableViewModel *) rssModel delegate:(id<MSMArticleDelegate,MSMRssMainController>) articleDelegate 
			feedURL:(NSString *) url isPortrait: (bool)portrait;
-(void) displayItem:(RssItem *) item itemIndex:(NSInteger) itemIndex;
//
-(void) share:(id)sender;
-(void) back:(id)sender;
@end
