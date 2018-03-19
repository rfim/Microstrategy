//
//  GenericMSTRWebViewController.h
//  MSTRMobileSDK
//
//  Copyright 2011 MicroStrategy. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "MSTRWebView.h"


/**
 GenericMSTRWebViewController is View Controller that has a UITextField and a MSTRWebView.
 
 Whatever requests are typed in the UITextField by the user are rendered in the MSTRWebView.
 
 It gives the user the feel of Safari brower but can also support mstr and mstripad requests.
 */
@interface GenericMSTRWebViewController : UIViewController <MSTRWebViewDelegate,UITextFieldDelegate> {
	UIButton* buttonGo;
	UITextField* textFieldURL;
	MSTRWebView *mstrWebView;
}


/**
 Use this constructor to get a GenericMSTRWebViewController that loads a html file from the mainbundle initially
 
 @param filename The filename of the file in the main bundle to be loaded.
 @return a newly constructed GenericMSTRWebViewController object
*/ 
-(id) initWithHTMLFile:(NSString*) filename;


/**
 This method is used to load data from a file and use it to render the MSTRWebView.The file should be present in the main bundle.
 
 @param filename The filename of the file in the main bundle to be loaded.
 */
- (void)loadFromFile:(NSString*) filename;

/**
 Use this constructor to get a GenericMSTRWebViewController that loads a html file from the mainbundle initially
 
  @param filename The filename of the file in the main bundle to be loaded.
  @return a newly constructed GenericMSTRWebViewController object
 */ 
-(id) initWithHTMLFile:(NSString*) filename fromBundle:(NSBundle*) bundle;


/**
 This method is used to load data from a file and use it to render the MSTRWebView.The file should be present in the main bundle.
 
 @param filename The filename of the file in the main bundle to be loaded.
 */
- (void)loadFromFile:(NSString*) filename fromBundle:(NSBundle*) bundle;

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
 Sets the main page contents, MIME type, content encoding, and base URL.This method is to be invoked in the main thread only.
 
 @param data The content for the main page.
 @param MIMEType The MIME type of the content.
 @param encodingName The IANA encoding name as in utf-8 or utf-16.
 @param baseURL The base URL for the content.
 */
- (void)loadData:(NSData *)data MIMEType:(NSString *)MIMEType textEncodingName:(NSString *)textEncodingName baseURL:(NSURL *)baseURL;

/**
 Use this property to get the MSTRWebView object used by the GenericMSTRWebViewController. This property is readonly.
 The purpose of exposing this property is to enable customization of the MSTRWebView object.
 */
@property (nonatomic, strong, readonly) MSTRWebView* mstrWebView;


/**
 Use this property to get the UITextfield object used by the GenericMSTRWebViewController. This property is readonly.
 The purpose of exposing this property is to enable customization of the UITextField object.
 */
@property (nonatomic, strong, readonly) UITextField* textFieldURL;


/**
 Use this property to get the UIButton object used by the GenericMSTRWebViewController. This property is readonly.
 The purpose of exposing this property is to enable customization of the UIButton object.
 */
@property (nonatomic, strong, readonly) UIButton* buttonGo;


@end

