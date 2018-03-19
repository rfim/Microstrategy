//
//  UIView+Viewer.h
//  IPadMain
//
//  Created by Liu Liang on 5/16/10.
//  Copyright 2010 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
typedef enum EnumPriorityID
{
	PID_ZERO = 0,
	PID_LOW,
	PID_MIDDLE,
	PID_HIGH
}EnumPriorityID;

typedef void (^FadeInAndOutOnCompletionBlock)();


@interface UIView (Viewer)
@property (nonatomic) CGFloat scaleRatio;

- (void)setOwnLayoutID:(short)iOwnLayoutID;
- (void)setSelectedLayoutID:(short)iSelectedLayoutID;
- (void)setOwnPanelID:(short)iOwnPanelID;
- (void)setSelectedPanelID:(short)iSelectedPanelID;

- (short)ownLayoutID;
- (short)selectedLayoutID;
- (short)ownPanelID;
- (short)selectedPanelID;

- (void) saveRemoveFromSuperview;

/**
	Finds the first ancestor view (including this view) that is a member of a particular class.
	Returns nil if not found
 **/
- (UIView*)ancestorOrSelfWithClass:(Class)cls;

/**
 For viewer which need to support on demannd rendering, this method needs to be
 implemented. When a viewer receives this message, it may need to reorganize the
 tasks in task queues.
 */
- (void)layoutOrPanelChanged;

/**
 * return the RootViewerContainer.
 * @depreciated You may failed to get the RootViewerContainer, if the current view is not under RootViewerContainer (for
 *              example, in info window). Use [Commander getRootViewContainer] instead.
 */
- (UIView*)getRootView;

/**
 * return the RootViewerContainer or the topView of info window.
 */
- (UIView*)getRealRootView;

/**
 Important note: Caller of this method should define the following properties.
 @property (nonatomic, assign) short ownLayoutID;
 @property (nonatomic, assign) short selectedLayoutID;
 @property (nonatomic, assign) short ownPanelID;
 @property (nonatomic, assign) short selectedPanelID;
 */
- (EnumPriorityID)getPriorityID;

/**
 For viewer which registers to commander, this method needs to be
 implemented. When a viewer receives this message, it need to unregister from
 commander can call [super detachViewer].
*/
- (void)detachViewer;

/**
 When rotation happens, pass this message to subviews recusively.
 */
- (void)updateFrame:(CGRect)frame;

- (void)updateSelector:(NSArray*)lpSubviews;

/**
Default implementation of handleSingleTap is to pass it to its superview.
*/
- (void)handleSingleTap;

/**
Default implementation, return 0. A viewer needs to override this method if it
has non-zero z-index.
*/
//- (int)getZIndex;

/**
* Return whether the view itself or the view's any superview is hidden. 
*/
-(BOOL)isActuallyHidden;

/**
 * Return whether the view itself or the view's any superview is hidding or hidden. 
 */
-(BOOL)isActuallyHiddingOrHidden;

/**
 * Called by rootviewContainer after zooming is ended. The new zoomScale can be got from Utility
 */
- (void)didEndPinchOrMoveZooming;


/**
 * Rotation messages sent to views.
 */
// Rename the methods when deprecated orientation methods. 12/10/15 haoli
- (void) repWillRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromOrientation
    toInterfaceOrientation:(UIInterfaceOrientation)toOrientation;
- (void) repWillAnimateRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromOrientation
    toInterfaceOrientation:(UIInterfaceOrientation)toOrientation;
- (void) repDidRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromOrientation
    toInterfaceOrientation:(UIInterfaceOrientation)toOrientation;

/**
 * Animations.
 */
- (void) fadeInAndShowOnCompletion:(FadeInAndOutOnCompletionBlock)onCompletion;
- (void) fadeOutAndHideOnCompletion:(FadeInAndOutOnCompletionBlock)onCompletion;

/**
 * Some methods for finding a view in view hierarchy.
 */
- (UIView*)getSuperViewWithClasses:(NSArray*)iClasses fromView:(UIView*)iView;
- (UIView*)getSuperViewWithClass:(Class)iClass fromView:(UIView*)iView;
- (UIView*)getSuperViewWithClassName:(NSString*)iClassName fromView:(UIView*)iView;
- (UIView*)getSubViewWithClass:(Class)iClass fromView:(UIView*)iView;
- (UIView*)getSubViewWithClassName:(NSString*)iClassName fromView:(UIView*)iView;

- (BOOL) isHidingOrHidden;

/**
 * Find the current firstResponder down from the view itself.
 */
- (UIView *)findFirstResponder;
/**
 * Add subview if needed (the view is not its subview yet).
 */
- (void) addSubviewIfNeeded:(UIView*) iView;

/**
 * Remove from given superview if needed.
 */
-(void) removeFromSuperviewIfNeeded:(UIView*) iView;

/** 
 *Check if a view overrides drawRect.
 */
- (BOOL) hasOverridenDrawRect;

/**
 * This is used to update all the subview's scale ratio of current view.
 */
- (void) updateScaleRatio:(CGFloat)iScaleRatio;
@end