//
//  BucketElementCountImageView.h
//  MicroStrategyMobile
//
//  Created by Yonggang Xi on 5/19/12.
//  Copyright 2012 MicroStrategy Inc. All rights reserved.
//

#import <UIKit/UIKit.h>

#define MICOUNT_IMAGE_VIEW_DEFAULT_SIZE 20

@class RoundedRectView;

@interface BucketElementCountImageView : UIView {
	NSInteger countNumber;
	NSInteger countDigit;
	UILabel *countLabel;
	RoundedRectView *backgroundView;
}

- (void)updateCount:(NSInteger)count;

@end
