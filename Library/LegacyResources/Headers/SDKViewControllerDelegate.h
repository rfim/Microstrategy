//
//  SDKViewControllerDelegate.h
//  MicroStrategyMobile
//
//  Copyright 2012 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "CurrentObjectChangedDelegate.h"

@class MSIObjectInfo;

/**
 * This is the protocol for the SDK user to communicate with IPadFolderController and IPadDataController.
 */
@protocol SDKViewControllerDelegate <NSObject>
@optional
/**
 * For MSTRObjectViewController, setViewFrame will cause the view frame to be revised to the new CGRect. The rect must be a valid rect, although it can be empty. If the rect is not valid, no action is taken.
 * It is not an error for the view to be partially or completely off the screen.
 * @param newViewFrame gives the new view frame size with the type of CGRect.
 */
- (void) setViewFrame: (CGRect)newViewFrame;

/**
 * For MSTRObjectViewController, setFitWidth will resize the document so that the width fits to the view width or to the actual document width.
 * @param isFitWidth gives fitting to view width or keeping the actual content width.
 */
- (void) setFitWidth: (BOOL)isFitWidth;

/**
 * For MSTRObjectViewController, refresh view.
 */
- (void) refresh;

/**
 * For MSTRObjectViewController, setOrientation sets the view orientation.
 * @param isViewOrientationPortrait is YES if the view is set to portrait.
 */
- (void) setViewOrientation: (BOOL)isViewOrientationPortrait;

/**
 * Enum for SDK action types
 * This is a list of SDK actions
 * Please refer to respondsToAction
 */
typedef enum {
	SDKUnknownAction,
	SDKFilterAction,
	SDKEmailAction,
	SDKModeSwitchAction,
	SDKMaximizeAction,
	SDKTransactionListAction,
	SDKBackAction,
	SDKLogoutAction,
	SDKNavToolbarSuppressedAction,
	SDKAnnotateShareAction
} SDKAction;

/**
 * For MSTRObjectViewController, respondsToAction checks an action with the given action ID and object ID can be performed or not.
 * @param actionType is set to any type defined in SDKAction.
 * @param objectID is set to a reserved parameter.
 * @return YES if the action can be performed, otherwise NO.
 */
- (BOOL) respondsToAction: (SDKAction)actionType withObject: (id)objectID;

/**
 * For MSTRObjectViewController, performAction:withObject: executes an action with the given action ID and object ID.
 * @param actionType is set to any type defined in SDKAction.
 * @param objectID is set to a reserved parameter.
 */
- (void) performAction: (SDKAction)actionType withObject: (id)objectID;

/**
 * For user who wants to set an object to be the delegate of current object changed by folder controller or data controller.
 * @param delegate is the object who wants to receive callback methods when current object changed.
 */
- (void) setCurrentObjectChangedDelegate: (id<CurrentObjectChangedDelegate>)delegate;

/**
 * For user who wants to receive current object info when current object changed. This method will call a callback function currentObjectChanged.
 */
- (void) notifyDelegateCurrentObjectChanged;

/**
 * For user who wants to load current selected report / document. This method will call a callback function loadSelectedReport.
 * @param delegate is the class who wants to load current selected report / document.
 */
- (void) notifyDelegateLoadSelectedReport;

@end
