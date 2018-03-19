//
//  RssUIArticleViewController.h
//  RssFeedLib
//
//  Created by Roman Zolin on 4/30/10.
//  Copyright 2010 EastBanc Technologies. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <MessageUI/MFMailComposeViewController.h>
#import <WebKit/WebKit.h>

#define HEADER_ARTICLE_FONTSIZE 16

@class RssItem;
@class MSMRssDateUtils;
@class MSMRssStatusBarView;
@class MSMRssTableViewModel;

@protocol MSMArticleDelegate;
@protocol MSMRssMainController;

@interface MSMRssUIArticleViewController : UIViewController<MFMailComposeViewControllerDelegate, UIActionSheetDelegate, WKNavigationDelegate> {

}

@property (nonatomic, strong) WKWebView	*bodyView;
@property (nonatomic, assign) int			itemIndex;
@property (nonatomic, strong) RssItem		*item;
@property (nonatomic, strong) MSMRssTableViewModel * model;
@property (nonatomic, strong) NSString * feedURL;
@property (nonatomic, unsafe_unretained) id<MSMArticleDelegate,MSMRssMainController> delegate;
@property (nonatomic, assign) CGRect parentRect;

-(id) initWithModel:(MSMRssTableViewModel *) model delegate:(id<MSMArticleDelegate,MSMRssMainController>) articleDelegate feedURL:(NSString *) url frame: (CGRect) rect isInFullScreen: (bool) full isPortrait: (bool) portrait;
-(void) displayItem:(RssItem *) item itemIndex:(NSInteger) itemIndex;
-(IBAction) share:(id)sender;
-(IBAction) back:(id)sender;

@end