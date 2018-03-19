//
//  SlidingControl.h
//  MSTRMobileSDK
//
//  Created by Srinivas Singanamalla on 10/20/10.
//  Copyright 2010 MicroStrategy Inc. All rights reserved.
//

#import <UIKit/UIKit.h>
@class MSISlidingControl;
@protocol MSISlidingControlDelegate

- (void) slidingControlDidSlideOut:(MSISlidingControl*)lpSlidingControl;

- (void) slidingControlDidSlideIn:(MSISlidingControl*)lpSlidingControl;

@end


@interface MSISlidingControl : UIView {
	
	@private
	CGRect boundaryRect;
	
	//Enums.h TOP, BOTTOM, RIGHT, LEFT. Currently only TOP and BOTTOM are supported
	NSInteger slideDirection;
	
	id<MSISlidingControlDelegate> __weak delegate;
	
	UIView* contentView;
	
	UIView* buttonView;
}

@property (nonatomic) CGRect boundaryRect;
@property (nonatomic) NSInteger slideDirection;
@property (nonatomic, weak) id<MSISlidingControlDelegate>delegate;

- (void) addToContentView:(UIView*)view;

- (void) slideIn;

- (void) slideOut;

@end
