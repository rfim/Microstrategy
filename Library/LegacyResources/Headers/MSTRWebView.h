//
//  MSTRWebView.h
//  MSTRMobileSDK
//
//  Created by Sumeru Chatterjee on 1/14/11.
//  Copyright 2011 MicroStrategy. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import "MSTRVCRetriever.h"
#import <WebKit/WebKit.h>
@class MSTRWebView;
/**
 This is the protocol for the MSTRWWebView to communicate with its delegate.The protocol extends the UIWebViewDelegate protocol.
 */
@protocol MSTRWebViewDelegate <WKNavigationDelegate>
@optional
/**
 This method is called on the delegate when the MSTRWebView is going to display a View Controller in response to a mstr:// or mstripad:// request.
 If the delegate returns false the loading of the viewController is aborted.
 
 The main purpose of this method is to provide the delegate functionality to show the View Controller in a customized manner.
 */           
- (BOOL)webView:(MSTRWebView*)webView willDisplayMSTRVC:(UIViewController *)viewController;
/**
 This method used to get the UINavigationController from the delegate tht should be used to push the view controller requested(document,report,folder etc.)
 
 If the delegate does not implement this method or if it returns nil,MSTRWebView simply replaces its own view with tht of the requested view.
 In that case drilling wont be supported.
*/
- (UINavigationController*)webViewNeedsNavigationController:(MSTRWebView*)webView;
@end


/**
	MSTRWebView is a wrapper around UIWebView to extend its functionality to support links with schemes mstr:// and mstripad://
 
	The basic purpose is support for html that contains html links together with links to MicroStrategy objects(reports,documents,folders etc.)
 */
@interface MSTRWebView : UIView <MSTRVCRetrieverDelegate,WKNavigationDelegate> {
	MSTRVCRetriever* vcRetriever;
	WKWebView* webView;
	id<MSTRWebViewDelegate> __weak delegate;
	UIViewController* vc;
	UIViewAnimationTransition transitionStyle;
}

/**
 This method is used to load data from a file and use it to render the WebView.The file should be present in the main bundle.
 
 @param filename The filename of the file in the main bundle to be loaded.
 */
- (void)loadFromFile:(NSString*) filename;

-(void) loadFromFile:(NSString *)filename fromBundle:(NSBundle*) bundle;
/**
 Connects to a given URL by initiating an asynchronous client request. 
 
 @param request A URL request identifying the location of the content to load.
*/ 
- (void)loadRequest:(NSURLRequest *)request;
/**
 Sets the main page content and base URL.This method is to be invoked in the main thread only

 @param string The content for the main page.
 @param baseURL The base URL for the content.
 */
- (void)loadHTMLString:(NSString *)string baseURL:(NSURL *)baseURL;

/**
Sets the main page contents, MIME type, content encoding, and base URL.This method is to be invoked in the main thread only
@param data The content for the main page.
@param MIMEType The MIME type of the content.
@param encodingName The IANA encoding name as in utf-8 or utf-16.
@param baseURL The base URL for the content.
*/
- (void)loadData:(NSData *)data MIMEType:(NSString *)MIMEType textEncodingName:(NSString *)textEncodingName baseURL:(NSURL *)baseURL;//to be invoked in the main thread only

/**
 Use this property to get or set the delegate of the MSTRWebView
 */
@property (nonatomic, weak) id<MSTRWebViewDelegate> delegate;
/**
 Use this property to get the view controller currently in use by the MSTRWebView.
 */
@property (nonatomic,strong, readonly) UIViewController* vc;
/**
 Use this property to get the webview used to render html content.This property has been exposed so that the webview can be customized.
 */
@property (nonatomic, strong, readonly)  WKWebView* webView;
/**
 Use this property to get or set the transition Style of the animation when a view controller is loaded in response to a MSTR request.
*/
@property (nonatomic)  UIViewAnimationTransition transitionStyle;

@end
