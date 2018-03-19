//
//  LaserPointerDelegate.h
//  MicroStrategyMobile
//
//  Created by Hang Hang on 3/1/12.
//  Copyright (c) 2012 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol MSILaserPointerDelegate <NSObject>

- (void)startLaserPointerTapAndHold:(UIGestureRecognizer *)gestureRecognizer;
- (void)stopLaserPointerMode:(UIGestureRecognizer *)gestureRecognizer;
- (void)laserPointerTouchMoved:(UIGestureRecognizer *)gestureRecognizer;

@end
