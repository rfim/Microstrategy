//
//  MSIMapWidgetViewerDelegate.h
//  IPadMain
//
//  Created by kongliang on 8/18/10.
//  Copyright 2010 MicroStrategy Inc. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "MSIMapEnums.h"
#import "MSIProjectInfo.h"

@class MSIMapWidgetInfo;
@class MSIWidgetHelper;
@class MWCalloutView;
@class MWAggregationInfoWindowView;
@class MSIMWLayer;

@protocol MapWidgetSimpleBaseViewerDelegate 
- (MSIMapWidgetInfo *) widgetInfo;
@end

@protocol MapWidgetBaseViewerDelegate <MapWidgetSimpleBaseViewerDelegate>

@property (nonatomic, strong) MSIMapWidgetInfo *widgetInfo;
@property (nonatomic) BOOL isInProcess;

- (int)checkMultiSelectActionType;
- (int)checkMultiSelectActionTypeForAllShownLayers;
- (BOOL)shouldShowLinkdrillButton;
- (BOOL)hasPanelStackInfoWindowForLayer:(NSInteger)layerIndex;
- (BOOL)hasPanelStackInfoWindow;
- (BOOL)hasLayoutInfoWindowForLayer:(NSInteger)layerIndex;
- (BOOL)hasLayoutInfoWindow;
- (MSIProjectInfo*) getProjectInfo;
- (void)handleMultiSelectEvent:(MultiSelectEventTypes)event selectionRegions:(NSDictionary*)annotationRegions;
- (void)handleMultiSelectedAnnotations:(NSArray*)annotations actionType:(MultiSelectActionTypes)actionType;
- (void)handleSelectionWithLayerIndex:(NSInteger)layerIndex withRowIndex:(NSInteger)rowIndex withRect:(CGRect)iRect;
- (void)handleMultiSelectedAnnotations:(NSArray*)annotations withRect:(CGRect)iRect;
- (void)handleSelectionWithLayerIndex:(NSInteger)iLayerIndex andRowIndex:(NSInteger)iRowIndex;
- (void)setupZoomScale:(UIView*)iView;
- (BOOL)isViewerInCustomInfoWindow;
- (BOOL)isTargetOnlyOnCustomInfoWindow;
@optional
- (void)disableBucketHandle:(BOOL)disable;
- (void)displayWarning:(NSString*)warningText;
- (void)regionDeletedAtIndex:(NSInteger)regionIndex;
- (BOOL)isDisplayedInAirplay;
- (void)hideLegend;
- (void)showLegend;

@end

@protocol MSIMapWidgetViewerDelegate <MapWidgetBaseViewerDelegate>

- (BOOL)isVIDocument;
- (BOOL)isShownInIPad;
- (void)addInfoWindowToBaseView:(MWCalloutView*)infoWindow;
- (void)addAggregationInfoWindowToBaseView:(MWAggregationInfoWindowView*)infoWindow;
- (void)dispatchTask;
- (BOOL)isTaskValid;
- (BOOL)isThreadDone;
- (void)loadAnnotationDataIncrementally;
- (void)notifyViewerWithProgress:(NSNumber*)progress;
//- (void)notifyViewerWithSecondaryDataProgress:(NSNumber*)progress;
//- (NSInteger)currentMetricIndex;
- (void)regionHighlightChangeWithKeyIndex:(NSInteger)keyIndex;
- (void)hidePanelStackInfoWindowForLayer:(NSInteger)layerIndex;
- (BOOL)shouldDisplayBucketView;
//- (void)infoWindowDismissed;
- (void)disableDashboardZoom:(BOOL)disable;
- (void)printTimeStamp:(BOOL)isForRenderBegin;

//- (void)getShapeGroup:(NSString *)tileKey
//              forType:(NSInteger)shapeType
//        withNorthEast:(CLLocationCoordinate2D)ne
//         andSouthWest:(CLLocationCoordinate2D)sw
//             callback:(void (^)(NSData *, NSError *))callback;

- (BOOL)areaSupportedInServer;
- (void)saveMapAreaConfig;
- (NSString *)getNoDataMessage;
/*
 for annotation update colorbyIndex and sizebyIndex
 */
//- (int)getColorByIndex;
//- (int)getSizeByIndex;
//- (void) setColorByEnable:(BOOL)enable;
//- (void) updateLegendView;
//- (void) setSizeByIndex:(int) sizeByIndex maxValue:(double)maxValueStr minValue:(double)minValue;
//- (NSString *) getFormatString:(double)value metricIndex:(int) mIndex;
//- (BOOL) hasLegend;

-(void) updateLegendWithLayerMetricChangedIfNeed:(MSIMWLayer *)layer metricIndex:(int) metricIndex;

- (void*)getGridIteratorByGridKey:(NSString*)gridKey;

- (MSIWidgetHelper *)getWidgetHelper;

@end
