
//
//  MSIMWLayer.h
//  MicroStrategyMobile
//
//  Created by Alpergin, Firat on 8/14/14.
//  Copyright (c) 2014 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MSIMapEnums.h"
#import "MSIMapCommonConstants.h"
#import "MSIMWSimpleBaseView.h"
#import "MWNetworkLayerDelegate.h"
#import "TabularDataProtocol.h"


@class MWVizPropInfo, MWHeatMapTileOverlay, MWAnnotationInfo, MSIMWAnnotation, MSIMWAnnotationView, MSIMWView, MWNetworkLayer, MWTileHierachy, MWAreaRenderer;
@protocol MWShapeCache;

@interface GeoMetaInfo : NSObject

@property (nonatomic, assign)CGFloat maxLat;
@property (nonatomic, assign)CGFloat minLat;
@property (nonatomic, assign)CGFloat maxLng;
@property (nonatomic, assign)CGFloat minLng;

@end

/*!
 *  MSIMWLayer is used for:
 *  - holding data of each layer and
 *  - rendering process of each layer
 */
@interface MSIMWLayer : NSObject <MWNetworkLayerDelegate>

@property BOOL isPrimaryLayer;
/*!
 *  the index of current layer
 */
@property (nonatomic) NSInteger layerIndex;
@property (nonatomic) NSInteger rowCount;

/*!
 *  DSSRWGridIterator hold the grid data of the corresponding template
 */
@property void *iterator;
@property (nonatomic, readonly, strong) id<MSITabularDataProtocol> reportData;
@property (nonatomic) BOOL isSelectable; // can be used as select source

/*!
 *  hold the configuration of the corresponding template
 */
@property (nonatomic, retain) MWVizPropInfo *vizPropInfo;

@property (nonatomic) BOOL showCurrentLayer; // property for layer selection popover

@property (nonatomic)NSInteger primaryDataLoadingProgress;
@property (nonatomic)BOOL primaryDataDone;

@property NSInteger primaryGridMetricStartCellIndex;
@property NSInteger primaryGridMetricCount;

/*!
 *  hold all the map annotations from the grid iterator
 */
@property (nonatomic, readonly) NSMutableDictionary* annotationData;
@property (nonatomic, retain) NSMutableArray *annotationBlockInfo;

@property (nonatomic, retain) NSMutableArray* maxAggregatedValues;
@property (nonatomic, retain) NSMutableArray* minAggregatedValues;
@property (nonatomic)BOOL aggregateValuesCalculated;

@property NSInteger currentZoomLevel;

/*!
 *  the template node name
 */
@property (nonatomic, strong) NSString *nodeName;

/*!
 *  hold all the metric names for displaying in the popover (for multiple layer) or dropdown list (for single layer).
 */
@property(nonatomic, strong) NSMutableArray* metricNames;
@property (nonatomic) NSInteger currentMetricIndex;
@property (nonatomic, strong) NSString *currentMetricName;

@property (nonatomic)BOOL currentMaxMinChanged;

@property (nonatomic) BOOL regionValid;

@property (nonatomic, strong) GeoMetaInfo* geoMetaInfo;

/*!
 *  the map we will put annotation views on
 */
@property (nonatomic, weak) MSTRMKMapView* mkMapView;
@property (nonatomic, weak) id<MSIMapWidgetViewerDelegate> mapWidgetViewerDelegate;
@property (nonatomic, weak) MSIMWView *mwView;

/*!
 *  display the annotation view as image or value text
 */
@property (nonatomic) AnnotationDisplayMode annotationDisplayMode;

- (void)updateAnnotationDisplayMode:(AnnotationDisplayMode)annotationDisplayMode;

- (id) initWithIterator:(void *)iterator withWidgetInfo:(MSIMapWidgetInfo*)widgetInfo withWidgetViewerDelegate:(id<MSIMapWidgetViewerDelegate>)_mapWidgetViewerDelegate;
- (void) setIterator:(void *)iterator;


/*
 for legend ,find the size metric max & min
 */
- (double) getMaximumMetricValueWithIndex: (int)metricIndex ;
- (double) getMinimumMetricValueWithIndex: (int)metricIndex ;


/*!
 *  check validation of current template
 *
 *  @return the checking result message
 */
- (NSString*) buildAndValidateForCurrentLayer;


- (BOOL)isGeoInfoFormWithAttributeStartFormGlobalIndex:(NSInteger)startFormGlobalIndex andFormLocalIndex:(NSInteger)formLocalIndex;

/*!
 *  get annotation from grid iterator
 *
 *  @param sequIndex sequence index of the annotation block of the template
 *
 *  @return a message indicating whether the population is success or not
 */
- (NSString*)populateMapAnnotations:(NSInteger)sequIndex;

- (NSInteger)annotationDataBlockCount;
- (NSArray*)annotationsForBlocIndex:(NSInteger)blockIndex;

- (void)updateMetricNames;

- (BOOL)isDataDone;

- (void)resetDataLoadingProgress;

- (NSInteger)findDisplayFormIndex;

- (NSMutableArray*)getAnnotationsOnMap;

- (void)updateLayerWithMetricChangedTo:(NSInteger)metricIndex;

- (void)updateMetricIndexForAnnotation:(id<MKAnnotation>)annotation;

/*!
 *  add all annotations back when we check the layer selection radio in the popover
 */
- (void)addAllAnnotationsToMap;
/*!
 *  remove all annotations back when we uncheck the layer selection radio in the popover
 */
- (void)removeAllAnnotationsFromMap;

- (BOOL)clusteringAllowed;

- (void)refreshLayer;

/*
 for updating legend
 */
@property int colorByIndex;
@property int sizeByIndex;
@property BOOL hasColorBy;
@property BOOL hasSizeBy;
-(BOOL) isBubbleMapNeedDefault;
//-(void) getDropZoneInfo;

/*
 this tag is for imageType to distinguish the color-threshold or image-threshold type
 */
@property BOOL colorThresholdType;

#pragma mark - markers map
@property (nonatomic, strong) NSMutableDictionary *bubbleImagesDict;

#pragma mark - density map
@property (nonatomic, retain) NSMutableArray* densityDictionaries;
/*!
 *  the overlay we rendered for density map
 */
@property (nonatomic, retain) MWHeatMapTileOverlay *heatMapTileOverlay;
/*!
 *  the dictionary indication the density of each point
 */
@property (nonatomic, retain) NSMutableDictionary* densityDictionary;

/*!
 *  generate density map based on densityDictionary and zoom level etc.
 */
- (void)generateDensityMap;
- (void)addHeatMapTileOverlay;

#pragma mark - cluster
@property (nonatomic)NSInteger highestMergeZoomLevel;
@property (nonatomic)NSInteger highestSplitZoomLevel;
@property (nonatomic, assign) ClusterDisplayTypes clusterDisplayType;
/*!
 *  indicating whether the layer should cluster annotations or not
 */
@property (nonatomic) BOOL clusteringEnabled;
/*!
 *  cache the clustered result for each level for performance consideration
 */
@property (nonatomic, strong) NSMutableArray *zoomLevelClusters;
@property (nonatomic, strong) NSMutableArray *allAnnotationsInSomeOrder;
@property (nonatomic, strong) NSMutableDictionary *annotationsCacheForCluster;
@property (nonatomic, strong) NSMutableArray *annotationsToAnimate;

- (void)onlySetClusteringProperty:(BOOL)clusteringEnabled;

- (double)getGridSpanForZoomLevel:(NSInteger)zoomLevel scale:(double)scale;

- (void)updateAnnotationView:(MSIMWAnnotationView *)annoView;
- (void)animateAnnotationViewAdded:(MSIMWAnnotationView *)annotationView;

- (void)clusterNearbyAnnotationViews;

#pragma mark - affinity line
/*!
 *  DSSRWGridIterator for affinity line template
 */
@property (nonatomic) void* affinityLineIterator;
/*!
 *  responsible for rendering the affinity line
 */
@property (nonatomic, strong) MWNetworkLayer *affinityLayer;

- (void)drawAffinityLayer:(NSNumber*)sequence;
- (NSInteger)affinityDataBlockNum;
- (NSString*)updateWithAffinityLineData:(void *) affinityLineDataIterator;
- (void)drawNetworklayerWithAnnotations:(NSArray*)annotations;

#pragma mark - area map

@property (nonatomic, strong) MWTileHierachy *tileService;
@property (nonatomic, strong) id<MWShapeCache> shapeCache;
@property (nonatomic, strong) MWAreaRenderer *areaRenderer;

/*!
 *  prepare for area rendering
 */
- (void)prepareAreaRendering;

/*
 use for bubble map has both colorby and sizeby metric show "default" behavor as the web
 */



@end


