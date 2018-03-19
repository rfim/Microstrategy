//
//  RssMainController.h
//  MSTRRssReader
//
//  Created by Nikolay Sazonov on 3/11/11.
//  Copyright 2011 EastBanc Technologies. All rights reserved.
//

#import <UIKit/UIKit.h>

@class MSMRssConfig;

@protocol MSMRssMainController<NSObject>
- (void) openArticleView: (UIViewController *) viewController;
- (BOOL) isDocument;
- (void) zoomView:(UIView*)rootView;
- (float)zoomScaleFactor;
- (MSMRssConfig*)rssConfig;
- (bool) willDocumentQuit;
@end
