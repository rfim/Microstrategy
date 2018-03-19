//
//  MapWidgetView.h
//  MSTRMobile
//
//  Created by xi zhang on 3/20/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <MapKit/MapKit.h>
#import <CoreLocation/CoreLocation.h>
#import "MSIMWAnnotation.h"
#import "MSIMapEnums.h"
#import "MSIMWBaseView.h"
#import "MSIMapCommonConstants.h"

#import "MWAreaRenderer.h"
#import "MWTileHierachy.h"
#import "MWLayerSelectionControlDelegate.h"

@class MSIMapWidgetInfo, MSIURLHelper;
@protocol IWidgetProtocol;
@protocol MSIMapWidgetViewerDelegate;
@class MWNavigationView;
@class MSMMBProgressHUD;
@class MWHeatMapTileOverlay;

@protocol MWViewDataSource

@property (nonatomic, readonly) NSDictionary* mapWidgetPropList;
@property (nonatomic, readonly, weak) id<MSIMapWidgetViewerDelegate, IWidgetProtocol> viewControllerDelegate;

- (MSIMapWidgetInfo *) widgetInfo;
//// get the maximal metric value given the metric index.
//- (NSNumber*)maxValueForMetricIndex:(NSInteger)metricIndex layerIndex:(NSInteger)layerIndex;
//// get the minimal metric value given the metric index.
//- (NSNumber*)minValueForMetricIndex:(NSInteger)metricIndex layerIndex:(NSInteger)layerIndex;

@end

@protocol MWViewDelegate < MKMapViewDelegate>

-(void)setMapWidgetCurrentRegion:(MKCoordinateRegion)region;
-(MKCoordinateRegion)getMapWidgetCurrentRegion;
-(BOOL)hasValidMapViewer;

-(void)handleTapForUserLocationButton;
-(void) setupZoomScale:(UIView*)iView;

@optional
- (void) drill:(MSIURLHelper*)_drillHelper;

@end


@class MSIDataElement, MWNetworkLayer, MultiComponentSelector;

@interface MSIMWView : MSIMWBaseView <UIPopoverControllerDelegate, MWOverlayRendererDelegate, MWShapeLoadingProgressDelegate, MWLayerSelectionControlDelegate> {
	
@private
	id<MWViewDataSource> __weak dataSource;
	id<MWViewDelegate> __weak delegate;
		
	
	BOOL panelInfoPopedUp;
	BOOL InfoWinAbonormalQuit;
    BOOL panelStackInfoWindowForceClose;
	
	// the layer for drawing affinity information.
//	MWNetworkLayer* affinityLayer;
//	UIView* affinityHiliteAnimationView;
    
//    NSMutableArray* metricNames;
//	// TQMS 431373 remember selection
//	NSUInteger mSelectedMetricIndex;
//	NSString* __weak mSelectedMetricName;
	
//	AnnotationDisplayMode mAnnotationDisplayMode;
	
//	NSInteger _displayMode;
//	NSInteger sequenceTotal;
	
	MKCoordinateRegion initialMapRegion;
    MKCoordinateRegion lastMapRegionForAllAnnotations;
    
    MWNavigationView* navMapView;
    BOOL navigationMapRegionVolatile;
    
    UIImage* minimizeImage;
    UIImage* maximizeImage;
    UIButton* minimizeMaximizeButton;
    MapOverViewState mapOverviewState;
    
//    BOOL needAdjustRegion;
    NSInteger zoomLevelForDMMarkerTransition;
    
    //NSMutableArray *allAnnots;
    MSMMBProgressHUD *progressHUD;// MSIMWBaseView's MSMMBProgressHUD is named clusterMarkerLoadingIndicator is for clustering
    
    // Variables used for density dictionary calculation - Moved from MWHeatMapView to MSIMWView so that we calculate the
    // density Map only once.
//    NSMutableArray* maxAggregatedValues;
//    NSMutableArray* minAggregatedValues;
//    NSMutableArray* densityDictionaries;
//    BOOL aggregateValuesCalculated;
//    BOOL aggregatedByLocation;
//    NSInteger currentMetricIndex;
//    BOOL showTransitionView;
    
    // area drawing variables.
//    ShapeLoadStyles shapeLoadStyle;
//    MWAreaRenderer* areaRenderer;
    CGPoint iwPositionInAreaMode;
    
    // area rendering variables.
//    MWTileHierachy* tileService;
//    id<MWShapeCache> shapeCache;
    
    MWHeatMapTileOverlay *heatMapTileOverlay;
    BOOL removeProgressView;
    //float lastProgressValue;
}

@property(nonatomic, weak) id<MWViewDataSource> dataSource;
@property(nonatomic, weak) id<MWViewDelegate> delegate;

// for single layer metric selection button
//@property(nonatomic, readonly) NSMutableArray* metricNames;
//@property(nonatomic) NSUInteger mSelectedMetricIndex;
//@property(nonatomic, weak) NSString* mSelectedMetricName;

//@property(nonatomic, strong) MWNetworkLayer* affinityLayer;
//@property(nonatomic) AnnotationDisplayMode mAnnotationDisplayMode;

/*!
 *  show navigation density map view when density map is displayed as image marker
 */
@property(nonatomic, readonly) MWNavigationView* navMapView;
@property(nonatomic) BOOL navigationMapRegionVolatile;
@property (nonatomic, strong) MWHeatMapTileOverlay *heatMapTileOverlay;

//#pragma mark - area map
//TODO-LATER: move to layer, move area related things to MSIMWLayer
//@property (nonatomic, strong) MWTileHierachy *tileService;
//@property (nonatomic, strong) id<MWShapeCache> shapeCache;
//@property (nonatomic, strong) MWAreaRenderer *areaRenderer;

//@property(nonatomic, getter = getOverlayRenderer, readonly, strong) id<MWOverlayRenderer> overlayRenderer;

- (void)popoverControllerDidDismissPopoverHandler;

//- (void)updateByMetricFlipWithMetricIndex:(NSInteger) metricIndex andLayerIndex:(NSInteger)layerIndex;

/*!
 *  display progress indicator, for clustering and area requesting
 *
 *  @param isDisplayed whether to display the progress indicator
 */
- (void) displayProgressIndicator:(BOOL)isDisplayed;
//-(void)updateAnnotationView:(MSIMWAnnotationView *)annoView;
//called when current user location button is tapped.

-(void)toggleUserLocation;

/*!
 *  prepare before map region change
 *
 *  @param animated animate or not
 */
-(void)mapRegionWillChangeAnimated:(BOOL)animated;
/*!
 *  do something after map region did change
 *
 *  @param animated animate or not
 */
-(void)mapRegionDidChangeAnimated:(BOOL)animated;

- (MWAreaRenderer*)getAreaRendererByLayerIndex:(NSInteger)layerIndex;

-(UIView*)reOrganizePanelStackInfoWindow:(UIView*)infoWindow needInit:(BOOL)iNeedInit;
-(UIView*)reOrganizeLayoutInfoWindow:(UIView*)infoWindow needInit:(BOOL)iNeedInit;
// callback when failure happens in data retrieval.
-(void)handleinfoWinFailure;
// show affinity network info.
//-(void)updateMapForInitialAnnotationAsText;

- (BOOL)isDefaultInfoWindowShown;

/*!
 *  add the populated annotations into map
 *
 *  @param progressIndex the annotation block index which is 1-based
 *  @param layerIndex    the index of the layer
 *
 *  @return a message indicating whether the update succeeded or not
 */
-(NSString*)updateMapWithProgress:(NSInteger)progressIndex forLayer:(NSInteger)layerIndex;
// since MW View is not the direct delegate of MKMapView, this would serve as a callback from the actual delegate
// @todo_cleanup : why is MSIMWView not the delegate ? Is this the right approach ?
- (void) annotationViewsAdded;
- (void) overlayRendererAdded;
- (void) updateMetricFlipView;

- (void) clearCache;
- (BOOL) shouldSwitchBetweenDMAndMarkerForGraphicType:(NSInteger)graphicType;
- (void)dismissPopoverMenus;
- (void)dismissOptionsView;
//- (void)prepareAreaRendering;
- (void)addAllAnnotationsForLayer:(NSNumber *)layerIndex;
- (void)removeAllAnnotationsForLayer:(NSNumber *)layerIndex;

- (NSDictionary*)getHeatMapDensityDictionaryForLayer:(NSInteger)layerIndex;
//- (NSInteger)getLayerCount;



@end
