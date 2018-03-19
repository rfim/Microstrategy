//
//  MSIMWBaseView.h
//  MicroStrategyMobile
//
//  Created by xi zhang on 7/19/11.
//  Copyright 2011 MicroStrategy Inc. All rights reserved.
//
// This class supports multi selections, default info window, user location, map types and bubble view with fixed size.
//

#import <UIKit/UIKit.h>
#import <MapKit/MapKit.h>
#import <CoreLocation/CoreLocation.h>
#import "MWMultiSelectHandler.h"
#import "MSIMapWidgetInfo.h"
#import "MSIMapWidgetViewerDelegate.h"
#import "MSIMapEnums.h"
#import "LaserPointerDelegate.h"
#import "MSIMWSimpleBaseView.h"
#import "PopoverLayoutMenu.h"

#define kMarkerAnimationDuration 0.5
#define IPAD_MAPMULTISELECTBAR_HORIZONTAL_SPACING 6
#define IPAD_MAPMULTISELECTBAR_VERTICAL_SPACING 10
#define IPHONE_MAPMULTISELECTBAR_VERTICAL_SPACING 10
#define IPAD_MAPMULTISELECTBAR_IMAGEVIEW_TAG 50
#define IPAD_MAPMULTISELECTBAR_BUTTON_WIDTH 46
#define IPAD_MAPMULTISELECTBAR_BUTTON_HEIGHT 46
#define IPAD_MAPMULTISELECTBAR_BUTTON_BASE_TAG 100
#define IPAD_MAPMULTISELECTBAR_ARROW_BAR 1000

#define DM_MARKER_TRANSITION_CONTAINER_TAG 200
#define DM_MARKER_TRANSITION_CONTAINER_SPINNER_TAG 201
#define DM_MARKER_TRANSITION_TO_MARKERS_TAG 202
#define DM_MARKER_TRANSITION_TO_DM_TAG 203

#define CLUSTER_VIEW_SIZE 40

@class MSIMWAnnotationView, MWCalloutView, MSIMWAnnotationView, MWTapGestureRecognizer, MWAggregationInfoWindowView;
@class MWIPhoneOptionsView;
@class MWCustomControl;
@class MSMMBProgressHUD;

@protocol SupportBaseToolbar <NSObject>

-(void)hideIPadMultiSelectionBar;
-(void)flipIPadMultiSelectBar;
-(void)closeIPadMultiSelectBar;
-(void)hideIPadMultiSelectionToolbar;
-(UIButton*)initializeIPadMultiSelectToolbarButtonWithFrame:(CGRect)buttonFrame title:(NSString*)title tag:(NSInteger)buttonTag;
-(void)setIPadMultiSelectToolbarButtonHiddenStatusWtihTag:(NSInteger)buttonTag hiddenStatus:(BOOL)status;
-(BOOL)locationServiceAvailable;

@end

@protocol SupportMultipleSelector <NSObject>

- (void) clearUIForNewSelection;
- (void) circleTapped:(id)circleItem;
- (void) rectangleTapped:(id)rectangleItem;
- (void) freeformTapped:(id)freeformItem;
- (void) clearTapped:(id)clearItem;
- (void) drillTapped:(id)drillItem;
- (void) selectModeChanged: (UISegmentedControl *) sender;
- (void) autoTapped:(id)autoItem;

@end

/*!
 *  process most of the interactions with the map
 */
@interface MSIMWBaseView : MSIMWSimpleBaseView <MKMapViewDelegate, MSILaserPointerDelegate, PopoverMenuDelegate, SupportMultipleSelector, SupportBaseToolbar>{
	
@protected
	// store all of the annotation view object.
	NSMutableDictionary* annotationViews;
	MWCalloutView* infoWinView;
    MWAggregationInfoWindowView* aggregationInfoWindow;
	MSIMWAnnotationView * selectedAnnoView;
	MSIMWAnnotation * __weak selectedAnnotation;
    MSIMWAnnotation * __weak selectedAnnotationForAggregation;
    
	NSDictionary* widgetPropList;
	
    // map UI bar.
    UIView* mapUIBarContainer;
    UIView* mapUIBar;

	//For iPhone UI.
 	BOOL mapUIBarShowed;
    
    // is the iPad MS UI Bar open or closed ? 
    BOOL mapMSBarVisible;

    // initial map type to show.
	MKMapType mapType;
	// normal marker or bubble.
//	graphicTypes graphicType;
//    NSInteger originalgraphicType;  // for density map marker style can change from density map to image and vice-versa.
	// enabled when link drill or selector action types are defined.
	BOOL multiSelectUIEnabled;
	// apply button enabled/disabled.
//	BOOL multiSelectApplyEnabled;
	// in multi select mode or not.
	BOOL multiSelectMode;
	// select or deselect.
	BOOL deselectEnabled;
	// multi select view.
	MWMultiSelectHandler* multiSelectHandler;
    
	// variables for clustering.
//    NSMutableDictionary* closeByViews;
	NSMutableArray* nearbyAnnotations;
    
	TouchSourceTypes touchSourceType;
	
	BOOL needHide;	
	CGRect selectedAnnoViewFrame;
	BOOL inProcess;
	// highlighting marker and no info window shown.
	BOOL onlyMarkerHighlighting;
	
	// for correctly positioning info window location.
	CGPoint curClickedPoint;
	NSTimeInterval hitTimeStamp;
	
	// a variable for checking if error happens in custom info window data retrieval.
	BOOL mErrorInSelectionHanlding;
	
	NSTimer * mpTimer;//tqms 456511, workaround to not invoke the selection command within 0.05 sec
	
	// interceptor for touches in map view since touch events cannot be propagated to this view directly and 
	// however we need some way to get the touch events and handle them.
	MWTapGestureRecognizer* mapTapRecognizer;
	
	BOOL tapInsideMap;

	BOOL regionHighlighted;
    
    UILongPressGestureRecognizer *longPressRecognizer;

    // set by single tap to clear all in multiple selection.
    BOOL clearSelectionsEnabled;
    TapObjectTypes objectTapped;
    // used in multi select density map
    MSIMWAnnotation * currentCenterAnnotationByAutoSelect;
//    NSMutableDictionary* markerByPath;
    
    UIProgressView *progressView;
    UIView* coverView;
    
    NSDictionary* initialAnnotationData;
    
    BOOL infoTapped;
    CGPoint iwPositionForRenderRegionInfo;
    
    // for cluster solutions.
    ClusterTypes clusterType;
    ClusterStatuses clusterStatus;
    NSMutableArray* annotationViewsByHitTest;
    MSIMWAnnotationView * annotationViewByTouchbegan;
    
    // delayed info window show due to map region change.
    BOOL delayedInfoWindowShow;
    
    
    //For iPhone BUtton
    UIButton* optionsButton;
    
    // For iPad &iPhone Button.
    UIButton* multiSelectionButton;
    
    // For iPhone options view
    MWIPhoneOptionsView* optionsView;
    
    UIButton* multipleMarkersDoneButton;
    
    BOOL ignoreTouch;
//    BOOL refresh;
    
//    NSMutableDictionary *bubbleImagesDict;
    
    // for now, plist properties for wisdom.
    BOOL preserveOrdering;
    BOOL showInfoWindowOnLoad;
    MWCustomControl* mapTypeCustomControl;
    MWCustomControl* userLocationCustomControl;
    UIButton *clusteringButton;
    BOOL allRegionTapped;
//    MSMMBProgressHUD *clusterMarkerLoadingIndicator;
    
	// the cluster caches in low zoom levels.
//    NSMutableDictionary* annotationCaches;
	// the clusters in current zoom level.
//    NSMutableDictionary* zoomLevelClusters;
    
    // used for new clustering solution. from map regions to zoom levels.
    NSInteger currentZoomLevel;
    NSInteger previousZoomLevel;
//    CLLocationDegrees currentLongitudeDelta;
    NSInteger highestMergeZoomLevel;
    NSInteger highestSplitZoomLevel;
    
    // sorted all annotations by grid density for later clustering.
//    NSMutableDictionary* allAnnotationsInSomeOrder;
    
    // annotations for animation.
    NSMutableDictionary* animatingClusters;
    NSMutableDictionary* animatingClusterViews;
    NSMutableArray* clusterImages;
    
    NSInteger currentZoomStatus;
//    NSDictionary* needClusterAnimation;
//    NSMutableDictionary* annotationsToRemove;
//    NSMutableDictionary* annotationsToAnimate;
    
    BOOL debugFlag;
    UILabel* debugLabel;
    UIButton* debugButton;
    NSMutableDictionary* debugClusterInfo; //TODOLAYERS-BETTER: make this for each layer
}

- (void)clearBeforeRefreshWidget;

- (void) reinit;

/*!
 *  init the map view
 */
- (void) renderMap;

/*!
 *  render map tool bar
 */
- (void) initMapToolbar;
- (void) initIphoneMapToolbar;
- (void) initIPadMapToolbar;

/*!
 *  display the map with specified map type
 *
 *  @param maptype map type, such as Standard, Satellite, Hybrid etc.
 */
- (void) displayMap:(MKMapType)maptype;

// ARC forbids non-init method with "init" prefix. Renamed.
-(UIButton*)initializeIPadMultiSelectToolbarButtonWithFrame:(CGRect)buttonFrame
                                          normalImage:(NSString*)normalImage 
                                        selectedImage:(NSString*)selectedImage
                                                  tag:(NSInteger)buttonTag;
- (void) addAnnotationView:(MSIMWAnnotationView *)annoView forLayer:(NSInteger)layerIndex;
//- (void) updateAnnotationView:(MSIMWAnnotationView *)annoView;
- (void) setMultiSelectWithActionType:(int)actionType;
- (void) setMultiSelect:(BOOL)multiSelect;
// called when selections change inside map.
- (void) selectionsChanged:(NSArray*)selections;
- (void) selectionsChanged:(NSArray*)selections isClearOperation:(BOOL)clearOperation;
- (void) applyChangeWithSelections:(NSArray*)selections;
// called when selections change outside of map.
- (void) outsideSelectionsChanged;
- (void) resetAfterSelection;

- (void) resetMultiSelections;

- (NSArray *) allAnnotationsForLayer:(NSInteger)layerIndex;
//- (NSArray *) allZoomLevelClustersForLayer:(NSInteger)layerIndex;
//- (NSArray *) allAnnotations;
//- (NSArray *) allZoomLevelClusters;
- (NSArray*) allSelectedAnnotations;
- (NSArray*) allSelectorShapes;
- (NSMutableArray*) allSelectedRegonKeys;

- (BOOL)customInfoWindowCongfigured;
- (BOOL)customInfoWindowConfiguredForLayer:(NSInteger)layerIndex;
//@todo_cleanup : this method used to be a check for whether PS is shown in ipad. With popover based IW in ipad, the check is now limited to whether we are
// rendering in ipad or not. We should be able to phase out this API now with inline check. 
- (BOOL) isInfoWindowPopoverBasedForLayer:(NSInteger)layerIndex;
- (void) showInfoWindowWithDataViewer:(UIView*)viewer needInit:(BOOL)iNeedInit;
- (void) showAggregationInfoWindow;
- (void) showInfoWindow:(MSIMWAnnotationView *)annoView aggregation:(BOOL)aggregation;
// show info window when a region from bucket is selected.
- (void) showInfoWindowForRegion:(NSInteger)regionIndex position:(CGPoint)position;
- (void) hideInfoWindow: (id)object;
- (void) hideInfoWindow;
- (void)hideInfoWindowForIphone;
- (void)hideInfoWindowForIpad;
- (void) moveInfoWindow;

- (void)clearUIDueToPopoverDismissed;
- (void) bringBackAggregationInfoWindow;
- (void) triggerInfoWindowForAnnotation:(MSIMWAnnotation *)annoObj;

- (void) changeClosebyAnnoViewFromOldIndex:(NSInteger)oldIndex toNewIndex:(NSInteger)newIndex forLayer:(NSInteger)layerIndex;
- (void) clusterAnnotationViewsForLayer:(NSInteger)layerIndex;
- (void) adjustAnnotationClusterWithNeedInit:(BOOL)iNeedInit;

- (void) setupZoomScale:(UIView*)iView;

- (void)notifyRegionHighlightChangeWithKeyIndex:(NSInteger)keyIndex;
- (void)scaleAnnotationsWithTransform:(CGAffineTransform)transform;
- (void)handleDrillAction;
- (void)clearAllSelections;
- (void)addMarkerImage:(UIImage*)image forPath:(NSString*)path;
- (BOOL)markerImageAdded:(NSString *)path;
- (UIImage *)getMarkerImageForPath:(NSString *)path;

- (void) viewModeChanged; 
- (void) flipNavigationView:(id) sender;
- (void) flipNavigationViewComplete;

- (void)touchesBeganInternal:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesMovedInternal:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesEndedInternal:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)hidePopoverInfoWindow;
- (void)zoom:(NSInteger)selectorIndex;
- (void)clearRegionSelection:(NSInteger)regionIndex;
- (void)widgetDataPopulated:(NSDictionary*)data;
- (void)infoTapped:(CGPoint)displayPoint;
- (void)regionTapped:(NSInteger)regionIndex position:(CGPoint)position;

- (void)annotation:(MSIMWAnnotation *)annotation selected:(BOOL)selected;

- (void)handleLongPress:(UIGestureRecognizer*)gestureRecognizer;
- (void)optionsButtonAction;
- (void)metricIndexSelected:(NSInteger)metricIndex;
- (BOOL)allowSingleSelectTool;
//- (void)renderViewerInInfoWindow:(UIView*)viewer;
- (PopoverLayoutMenu*)newPopoverLayoutMenu;

- (void)animateAnnotationViewsAdded:(NSArray*)annoViews;
- (void)setClustering:(BOOL)enabled forLayer:(NSInteger)layerIndex beforeRendering:(BOOL)beforeRendering;
- (void)clearClusteringCaches;
- (NSMutableArray *)getAllAnnotations:(MWClusteredAnnotation *)annot;
//- (NSMutableArray *)getAnnotationsForLayer:(NSInteger)layerIndex;

- (void)highlightSingleSelectedAnnotation:(MSIMWAnnotation *)annotation selected:(BOOL)selected;
- (void)highlightMultiSelectedAnnotation:(MSIMWAnnotation *)annotation selected:(BOOL)selected;
- (CGRect)infoWindowPositionAtAnnotation:(MSIMWAnnotation *)annotation;

- (MKCoordinateRegion)getRegionForSelector:(NSArray *)selector withScaleFactor:(double)scaleFactor;
- (void)dismissPopoverMenus;

- (BOOL)isClusteringOnForLayer:(NSInteger)layerIndex;

//- (int)getZoomLevel;

@property (nonatomic, weak) id<MapWidgetBaseViewerDelegate> baseViewerDelegate;
@property (nonatomic) MKMapType mapType;
//@property (nonatomic) graphicTypes graphicType;
//@property (nonatomic) NSInteger originalgraphicType;
@property (nonatomic, readonly, strong) NSMutableDictionary* annotationViews;
@property (nonatomic) BOOL multiSelectUIEnabled;
//@property (nonatomic) BOOL multiSelectApplyEnabled;
@property (nonatomic) BOOL multiSelectMode;
@property (nonatomic) BOOL deselectEnabled;

@property(nonatomic) TouchSourceTypes touchSourceType;
@property(nonatomic) BOOL inProcess;
@property(nonatomic) BOOL onlyMarkerHighlighting;
@property(nonatomic) BOOL mErrorInSelectionHanlding;
@property(nonatomic, readonly) MWCalloutView* infoWinView;
@property(nonatomic, readonly) MWAggregationInfoWindowView* aggregationInfoWindow;
@property(nonatomic, weak) MSIMWAnnotation * selectedAnnotation;
@property(nonatomic, weak) MSIMWAnnotation * selectedAnnotationForAggregation;

@property(nonatomic) BOOL tapInsideMap;
@property(nonatomic) TapObjectTypes objectTapped;
@property(nonatomic) BOOL clearSelectionsEnabled;

@property (nonatomic, readonly) MWMultiSelectHandler* multiSelectHandler;
@property BOOL ignoreTouch;
@property BOOL refresh;

//@property (nonatomic, strong) NSMutableDictionary *bubbleImagesDict;

//@property (nonatomic, readonly, strong) NSMutableDictionary *zoomLevelClusters;

@property (nonatomic, strong) NSMutableDictionary *markerByPath;

@property (nonatomic, strong) MSMMBProgressHUD *clusterMarkerLoadingIndicator;

@property (nonatomic) BOOL clusteringEnabledSetting;

@end


// a custom gesture recognizer for intercepting touches in map view since touch events cannot be propagated to the MWBseView.
// however we need some way to get the touch events and handle them. Extending the UIGestureRecognizer class is one of the good 
// approaches since every UIGestureRecognizer object can intercept the touch events directly.
@interface MWTapGestureRecognizer : UIGestureRecognizer {
	MSIMWBaseView * __weak mainView;
    BOOL doubleTapped;
    NSTimeInterval touchTimeStamp;
}

@property (nonatomic, weak) MSIMWBaseView * mainView;

@end

@interface AnnotationGridInfo : NSObject {
    
    NSInteger gridIndex;
    NSMutableArray* annotations;
    
}

- (id)initWithGridIndex:(NSInteger)index;
- (NSComparisonResult)compare:(AnnotationGridInfo *)other;

@property (nonatomic) NSInteger gridIndex;
@property (nonatomic, readonly) NSMutableArray* annotations;

@end

