//
//  MSIAlertView.h
//  MicroStrategyMobile
//
//  Created by Zeng Jinliang on 1/19/16.
//  Copyright © 2016 MicroStrategy Inc. All rights reserved.
//

#import <UIKit/UIKit.h>

@class MSIAlertView;
@protocol MSIAlertViewDelegate <NSObject>
@optional

// Called when a button is clicked. The view will be automatically dismissed after this call returns
- (void)alertView:(nonnull MSIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex;

- (void)alertView:(nonnull MSIAlertView *)alertView willDismissWithButtonIndex:(NSInteger)buttonIndex; // equal to didDismissWithButtonIndex. just to keep code compatible
- (void)alertView:(nonnull MSIAlertView  *)alertView didDismissWithButtonIndex:(NSInteger)buttonIndex;  // equal to willDismissWithButtonIndex. just to keep code compatible
@end

@interface MSIAlertView : NSObject

- (nullable instancetype)initWithTitle:(nullable NSString *)title message:(nullable NSString *)message delegate:(nullable id /*<MSIAlertViewDelegate>*/)delegate cancelButtonTitle:(nullable NSString *)cancelButtonTitle otherButtonTitles:(nullable NSString *)otherButtonTitles, ... NS_REQUIRES_NIL_TERMINATION;

@property (nullable, nonatomic, strong) UIAlertController *alertController;
@property (nullable,nonatomic,weak) id /*<UIAlertViewDelegate>*/ delegate;

@property (nullable, nonatomic, copy) NSString *title;
@property (nullable, nonatomic, copy) NSString *message;

@property(nonatomic) NSInteger tag; // default is 0

@property(nonatomic) NSInteger cancelButtonIndex;

// returns index of button. 0 based.
- (NSInteger)addButtonWithTitle:(nullable NSString *)title;
- (nullable NSString *)buttonTitleAtIndex:(NSInteger)buttonIndex;
// shows popup alert animated.
- (void)show;

// hides alert sheet or popup. use this method when you need to explicitly dismiss the alert.
// it does not need to be called if the user presses on a button
- (void)dismissWithClickedButtonIndex:(NSInteger)buttonIndex animated:(BOOL)animated;

@property(nonatomic,readonly,getter=isVisible) BOOL visible;

@end

// API from UIAlertController
@interface MSIAlertView (UIAlertControllerAPI)

- (void)addAction:(nonnull UIAlertAction *)action;

@property (nonatomic, readonly, nullable) NSArray<UIAlertAction *> *actions;

@property (nonatomic, strong, nullable) UIAlertAction *preferredAction NS_AVAILABLE_IOS(9_0);
@end

@interface MSIAlertView (publicAPI)

// dismiss all alert controllers recursively
+ (void)dismissAlertsAnimated:(BOOL)animated completion:(void (^ __nullable)(void))completion;
// dismiss the top alert controller
+ (void)dismissTopAlertAnimated:(BOOL)animated completion:(void (^ __nullable)(void))completion;
// dismiss the top alert controller and return it
+ (nullable UIAlertController *)popTopAlertControl:(BOOL)animated;

// check the top view controller whether it's a alert and need dismiss
+ (BOOL)isAlertOnTop;

// used to show the swallowed alert controller
+ (nullable MSIAlertView *)showAlert:(nullable UIAlertController *)alertController animated:(BOOL)animated;

@end
