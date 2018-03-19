//
//  WebViewController.h
//  MSTRRssReader
//
//  Created by Pavel Shatrov on 4/22/11.
//  Copyright 2011 EastBanc Technologies. All rights reserved.
//
#import <UIKit/UIKit.h>
#import "MSTRNavigationBarController.h"
#import <WebKit/WebKit.h>
@interface MSMWebViewController : UIViewController<WKNavigationDelegate, MSTRNavigationBarButtonItemsFactory> {

}

@property (strong) NSURL *url;
@property (assign,nonatomic) BOOL hideNavBarWhenDisappear;
-(id)initWithFrame:(CGRect) frame isInFullScreen: (bool) full;
-(IBAction) back:(id)sender;
@end
