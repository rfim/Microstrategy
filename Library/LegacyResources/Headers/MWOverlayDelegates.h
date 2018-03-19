//
//  MWAreaDelegate.h
//  MicroStrategyMobile
//
//  Created by Zhang, Xi (Jason) on 11/15/13.
//  Copyright (c) 2013 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <MapKit/MapKit.h>
#import "MSIProjectInfo.h"
#import "MSIMapEnums.h"

@class MWShapeAnnotation;

// MSTR specific data.
@protocol MWOverlayMetaData <NSObject>

@property(nonatomic, retain) UIColor* color;
@property(nonatomic, assign) BOOL multiSelected;
@property(nonatomic, getter = getOverlayId, readonly) NSString* overlayId;

@end

// protocol for rendering different annotation styles: markers, bubbles, density map  and shapes.
@protocol MWOverlayRenderer <NSObject>

- (void)addData:(NSDictionary*)data;
- (void)render;
- (void)handleInteractionWithShapeAnnotation:(MWShapeAnnotation*)shapeAnnotation andTapPosition:(CGPoint)tapPosition;
- (void)hilightOverlay:(id<MWOverlayMetaData>)overlay;
- (void)dehilightOverlay:(id<MWOverlayMetaData>)overlay;
- (void)reinit;

@end

// delegate for handling interactivity etc.
@protocol MWOverlayRendererDelegate <NSObject>

- (MKMapView*)mapView;
- (void)setTouchSourceType:(TouchSourceTypes)touchType;
- (void)overlay:(id<MWOverlayMetaData>)overlay selected:(BOOL)selected atPosition:(CGPoint)position;

- (BOOL)onlyAnnotationsUsedInAffinityLinesShown;
- (BOOL)needDisplayAffinityLines;
- (void)drawAffinityLinesWithAnnotations:(NSArray*)annotations;
-(BOOL) getRefresh;
-(BOOL) getRedrawOnSelectionAction;

@end

