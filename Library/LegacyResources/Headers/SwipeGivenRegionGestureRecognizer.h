//
//  SwipeGivenRegionGestureRecognizer.h
//  MicroStrategyMobile
//
//  Created by Zeng Yingpei on 1/18/13.
//
//  Detect the swipe gesture that happens at given region of a view but not the whole view.
//  Make it can be configured as a continuous gestureRecognizer. 05/03/2013.
//
//  Usage:
//  1. When it is a discrete gesture recognizer, it simply changes to UIGestureRecognizerStateRecognized
//  and calls the action once when the swipe gesture is recognized.
//
//  2. When it is configured as continuous gesture recognizer, it begins (UIGestureRecognizerStateBegan)
//  when user moved enough to make the swipe recognized. It changes (UIGestureRecognizerStateChanged)
//  when the finger moves. It ends (UIGestureRecognizerStateEnded) when the finger is lifted.

#import <UIKit/UIKit.h>

@interface SwipeGivenRegionGestureRecognizer : UIGestureRecognizer

@property (nonatomic) CGRect givenRegion;
@property (nonatomic) UISwipeGestureRecognizerDirection allowedDirection;    // default is right
@property (nonatomic) CGFloat maxAllowedPauseTimeBeforeMove;                 // default is 0.5 s
@property (nonatomic, copy) NSArray* ignoreClassesArray;                             // these kind of classes will be ignore if user swipes upon them
@property (nonatomic) CGFloat maxAllowedAngleDeviation;     // the max allowed angle deviation from the right direction, default is M_PI/6
@property (nonatomic, readonly) UISwipeGestureRecognizerDirection recognizedDirection; // the actual recognized direction


// Below properties are used when it is configured as a continuous gesture recognizer.

@property (nonatomic) BOOL isContinuousGestureRecognizer;                    // default is NO
@property (nonatomic, readonly) CGFloat draggedOffset;                               // The total dragged offset but not delta value. Offset in recognizedDirection is positive.
@property (nonatomic, readonly) CGFloat draggedVelocity;                             // velocity of dragging, point per second

@end
