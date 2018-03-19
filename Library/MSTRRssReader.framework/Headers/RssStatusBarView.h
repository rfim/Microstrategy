//
//  RSSStatusView.h
//  RssFeedLib
//
//  Created by Svetlana Kopylova on 6/1/10.
//  Copyright 2010 EastBanc Technologies. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface MSMRssStatusBarView : UIView {

}

-(void) setMessage:(NSString *) message;
-(void) setMessage:(NSString *)message withColor:(UIColor *)color;
+(CGFloat) defaultBarHeight;

@end
