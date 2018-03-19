//
//  MSIGridImageWaitView.h
//  MicroStrategyMobile
//
//  Created by Huang, Bing on 8/16/12.
//  Copyright (c) 2012 MicroStrategy Inc. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface MSIGridImageWaitView : UIView {
    UIActivityIndicatorView* mpIndicatorView;
}

- (void)start;
- (void)stop;

@end
