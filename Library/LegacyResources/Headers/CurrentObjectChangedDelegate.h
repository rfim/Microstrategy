//
//  CurrentObjectChangedDelegate.h
//  MicroStrategyMobile
//
//  Copyright 2012 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@class MSIObjectInfo;

/**
 * This is the protocol for the IPadFolderController and IPadDataController to communicate with its delegate who wants to get current object info when current object changed.
 * This protocol works with the protocol SDKViewControllerDelegate.
 @see	SDKViewControllerDelegate
 */
@protocol CurrentObjectChangedDelegate <NSObject>

/**
 * This method is a callback function implemented by user who want to get the current changed object info.
 * @param objectInfo is the object info after change.
 */
- (void) currentObjectChanged: (MSIObjectInfo*)objectInfo;

/**
 * This method is a callback function implemented by user who want to load the current selected report / document.
 * @param reportID is the object ID when the report / document is selected.
 * @param reportType is the object type when the report / document is selected.
 */
- (void) loadSelectedReport: (NSString*)reportID withType: (NSInteger)reportType;

@end
