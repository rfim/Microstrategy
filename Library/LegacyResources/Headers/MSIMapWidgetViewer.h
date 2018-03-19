//
//  MSIMapWidgetViewer.h
//  IPadMain
//
//  Created by kongliang on 8/14/10.
//  Copyright 2010 MicroStrategy Inc. All rights reserved.
//


#import "MSIWidgetViewer.h"
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <MapKit/MapKit.h>
#import "IViewer.h"
#import "MSIMapWidgetViewerDelegate.h"
#import "MSIMWAnnotation.h"
#import "RWDDashboardDelegate.h"
#import "MSIProjectInfo.h"
#import "GetMapShapesService.h"
#import "MSIMapLegendDelegate.h"
#import "MSIModelData.h"
#import "MSIMapLegendPropertiesStore.h"

class DSSRWGridIterator;
@class MSIDocumentContext, MSIMapWidgetInfo, MSIMWView, MSIMapWidgetViewerHelper, DetachHelper, MSIWidgetHelper, DataServiceController, MSIReportDisplayInfo, WaitView, VICommonLegend;
@class MWIPhoneHorizontalBucketView;


typedef enum _BucketSlideTypes{
	EnumBucketSlideToBottom = 0, EnumBucketSlideToTop, EnumBucketSlideToOpen,  EnumBucketSlideToClose
} BucketSlideTypes;


@interface MSIMapWidgetViewer : MSIWidgetViewer<MSIMapWidgetViewerDelegate, BinaryRetrievalHandler, ServiceHandler, UITableViewDelegate, UITableViewDataSource, MSIMapLegendPropertiesStoreDelegate, MSIMapLegendDelegateHelper> {
	
	BOOL baseViewReseted;
	DataServiceController *dataController;
	MSIReportDisplayInfo *objectInfo;
	WaitView *waitView;
	NSInteger waitViewType;
	NSInteger modelType;
	BOOL pageByScreenShown;
	NSMutableDictionary* viewMap;
	BOOL landscape;
	NSInteger fullScreenDuration;
	BOOL shouldReconcileOnDataLoad;
	// map widget property list used for map customization.
    NSDictionary* mapWidgetPropList;
	BOOL fullScreen;
	UIView* oldBaseView; //used in animations category
	BOOL animationInProgress; //used in animations category
	BOOL hasGroupBy; //used in documents
	MSIMapWidgetViewerHelper * mMapWidgetViewerHelper;
	PopoverContext* mpPopContext;
    // store previous regions.
	MKCoordinateRegion mkPreviousRegion;
	MKCoordinateRegion mkPreviousRegionInLandscape;
	MKCoordinateRegion mkPreviousRegionInPortrait;
    BOOL zoomLevelChanged;
    BOOL hasValidPreviousRegion;
	BOOL mIsSelectable;
	BOOL mIsPanelStackInfoWinDefined;
//	BOOL mIsLayoutInfoWinDefined;
	BOOL SecondaryDataModelRefreshNeeded;
//	BOOL hasPreviousMapType;    // a flag indicate whether we saved map type
//	NSInteger mPreviousMapType; // used to store previously selected map type before release current MSIMWView
//	NSInteger mPreviousMetricIndex;  // used to store previously selected metric
//    NSString* mPreviousMetricName;
//	AnnotationDisplayMode mPreviousAnnotationDisplayMode;
	// for binary data retrieval
	int rowIndex;
	CGRect rect;
	BOOL mIsTaskValid;
    // support for airplay.
    CGPoint pinOffset;
	CGFloat mfCurrentScale;
    BOOL inZooming;
    BOOL inPanning;
    MKCoordinateRegion currentRegion;
    CLLocationCoordinate2D currentCenter;
	// variables for multi select.
	UIView* selectionContainer;
    UIView* selectionHeaderView;
	UITableView* selectionBucket;
	NSMutableArray* selectedRegionKeys;
	NSDictionary* selectedElements;
	BOOL bucketShown;
    NSInteger bucketHandleWidth;
    NSArray* initialAnnotationData;
    NSMutableArray* sectionOpenInfo;
    NSInteger openSectionIndex;
    BOOL sectionOpenAnimation;
    // bucket open handle button.
    UIButton* openHandleButton;
    // close handle view.    
    UIImageView* closedHandleBackgroundImageView;
    // rootBucketContainer has subviews: handle for open/close and bucketContainer.
    UIView* rootBucketContainer;
    // used for slide effect. display either selectionContainer or regionSelectionContainer at one time.
    UIView* bucketContainer;
    // contentContainer has two subviews: selectionContainer and regionSelectionContainer.
    UIView* contentContainer;
    // views for show selections in a region.    
	UIView* regionSelectionContainer;
    UIView* regionSelectionHeaderView;
    UITableView* regionSelectionBucket;
	NSMutableArray* selectedRegionElements;
    UIView* highlightedRegionRow;
    MWIPhoneHorizontalBucketView* bucketView;
    BOOL annotationRenderedFromRegionInfoView;
    // used for showing message such as no data.
    UIImageView* messageContainerView;
    
    // map shape service.
    GetMapShapesService* mapShapesService;
    dispatch_queue_t serviceQueue;
    double pt1;
                                
    NSString* areaConfigStoreHomePath;
}

- (id)initViewer:(ViewerDataModel*)ipDataModel withCommander:(Commander*)ipCommander withProps:(NSString*)ipProps asESRIAdapter:(BOOL) isESRIAdapter;
- (void) invalidateTask;
- (bool) willExitThread;
- (MSIProjectInfo*) getProjectInfo;


-(void)setScrollViewContentOffset:(CGPoint)offset;
-(void)setScrollViewZoomScale:(float)zoomScale;
-(void)zoomEnded:(float)zoomScale;
-(void)scrollEnded;


@property (nonatomic, strong) NSDictionary* mapWidgetPropList;
//@property (nonatomic, strong) NSString* mPreviousMetricName;
@property (nonatomic, weak) MSIMWView* mstrMapView;

//sowang is rendering
/**
 *This property is used to check whether the rendering is finished
 */
@property (nonatomic, readonly) bool isWaitingViewShowing;
@property (nonatomic, strong) MSIMapWidgetViewerHelper *mMapWidgetViewerHelper;

@end
