//
//  VisualizationProtocol.h
//  MicroStrategyMobile
//
//  Created by Yingpei Zeng on 3/21/12.
//  Copyright (c) 2012 MicroStrategy Inc. All rights reserved.
//
//  Provide the common behaviors LayoutManagerViewer . Implemented by GridGraphViewer and WidgetViewer.

#import <Foundation/Foundation.h>
#import "AnnotationContext.h"

@class Border;

@protocol VisualizationViewerProtocol <NSObject>

@required
// Used to change the visualization to the given frame and update content if needed.
- (void)updateVisualizationFrame:(CGRect)iFrame;
// Get the title of the visualization. LayoutManagerViewer needs the string to show in TitleBarViewer.
- (NSString*)getVisualizationTitle;
// Return the visualizationViewer.
- (UIView*)getVisualizationViewer;

- (NSString *)getVisualizationNodeKey;

- (BOOL)isSupportLSMode;

@optional
- (void*)getVisualizationGridIterator;
- (void)isMultiSelectionMode:(BOOL)iIsMultiple withLMode:(BOOL)isLSMode;
- (BOOL)needToHandleTargetSelection; //tell it after done is pressed, we need to execute commands to filter target
- (void)handleMultiSelectionOnTarget;// filter targets after done is pressed
- (CGRect)getFixedFrame; // get the fixd size for text field
- (void)clearLassoSelection;
- (void)showActionMenu;
- (void)setCanvasViewAnnotationShape:(AnnotationShape)shapeType;
- (BOOL)shouldShowVisualizationTitleBar;
- (UIEdgeInsets)getEdgeInserts;
- (Border*)getBorderDrawer;
- (void*)getTitleBarFormat;

@end
