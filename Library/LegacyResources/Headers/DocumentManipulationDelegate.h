//
//  DocumentManipulationDelegate.h
//  MicroStrategyMobile
//
//  Created by Xin Liu on 4/24/12.
//  Copyright (c) 2012 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
class DSSActionObject;
class DSSCManipulation;
@class LayoutManagerViewer;

@protocol DocumentManipulationDelegate <NSObject>

- (void)manipulationWillExecute;
- (void)manipulationDidExecute:(bool)needEnable missingBinary:(bool)isMissingBinary haveServerNodes:(bool)isHaveServerNodes;
- (void)afterManipulationDidExecute;
- (LayoutManagerViewer*)layoutManagerViewer;
- (void)handleRedrawParent:(DSSCManipulation*)manipulation;
- (void)manipulationsApplied:(BOOL)isNormalManipulation;
//- (void)setAsyncManipulation:(DocumentManipulation*)manipulation;
/*
- (void)requestMissingBinaryBlock:(NSString*)layoutKey;
- (void)requestMissingBinarySync:(NSString*)iLayoutKey forceUseDeviceCache:(BOOL)iForceUseDeviceCache;
- (void)applyManipulatonOnNodeKeys:(NSString*)nodeKeys withEvents:(NSString*)events needToApplyData:(BOOL)needToApplyData showWaitView:(BOOL)showWaitView;
- (void)addActionObject:(DSSActionObject*)ipActionObject isAuto:(bool)iIsAuto;
- (std::string) getAOEvents;

- (void)getMapNodeKeyToViewers:(MapNodeKeyToViewers**)oppMapNodeKeyToViewers andMapNameToNodeKey:(MapNameToNodeKey**)oppMapNameToNodeKey andMapNodeKeyToMasterViewers:(MapNodeKeyToMasterViewers**)oppMapNodeKeyToMasterViewers;
- (RootViewerContainer*)getRootViewerContainer;
*/
@end
