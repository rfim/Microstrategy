//
//  MSIMapWidgetInfo.h
//  MSTRMobile
//
//  Created by xi zhang on 3/22/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <MapKit/MapKit.h>
#import "TabularDataProtocol.h"
#import "MSIMWAnnotation.h"
#import "MSIMapWidgetViewerDelegate.h"
#import "MWShapeConfig.h"
#import "MSIMapCommonConstants.h"
#import "MSIMapEnums.h"


@class MSIReportDisplayInfo, MSIReportDataRow, MWVizPropInfo, MSITabularData, MSIDataElement, MWNetworkLayer, MWVizPropInfoCollection, GeoMetaInfo, MSTRMKMapView, MSIMWView, MSIMapWidgetViewer;


/*!
 *  hold MWVizPropInfoCollection and MSIMWLayer instances and some helper method
 */
@interface MSIMapWidgetInfo : NSObject {
	
@private
	// TODOLAYERS-DONE: changed
//	id<MSITabularDataProtocol> reportData;
	//MWVizPropInfo* vizPropInfo;
    /*!
     *  hold configurations of all the template (layers)
     */
    MWVizPropInfoCollection *vizPropInfoCollection;
	BOOL hasPageBy;
	
	// support multi-attributes.
	//NSMutableArray* primaryGridAttributeInfos; //holds this info for all layers
	//NSInteger primaryGridMetricStartCellIndex;
    //NSMutableArray *primaryGridMetricStartCellIndexes;
	//NSInteger primaryGridMetricCount;
    //NSMutableArray *primaryGridMetricCounts;
   // NSMutableDictionary* fpDict;
    //NSMutableArray* fpDicts;
	//NSMutableDictionary* annotationData;
    //NSMutableArray *annotationDatas;
//	NSMutableDictionary* geoInfoLookupDict;
//	MSIMWAnnotation * lastAccessedAnnotation;
	//NSInteger validLeftmostSubtotalAttributeCellStartIndex;
	//BOOL subtotalOnTop;
   // BOOL validSubtotal;
	
	// store the maximal value for each metric.
	//NSMutableArray* maxMetricValues;
   // NSMutableDictionary *maxMetricValuesDict;
    
	// store the minimal value for each metric.
//    NSMutableArray* minMetricValues;
    //NSMutableDictionary *minMetricValuesDict;
    
    //NSMutableArray* metricNames;
	
	// affinity network info from another report.
//	MSIReportDisplayInfo* affinityReport;
//	NSMutableDictionary* __weak affinityLookup;
//	CGFloat maxAffinityLineThicknessMetricValue;
//	NSInteger affinityMetricCellStartIndex;
//	NSInteger affinityMetricCount;
	
//    NSMutableArray *affinityFormIDs;
    
//	NSNumberFormatter* formatter; // moved to MSIMWLayer
    id<MSIMapWidgetViewerDelegate> __weak widgetDelegate;
    MSIMapWidgetViewer* __weak mapWidgetViewer;
    
    
	// the expected structure is:
	// [1] the key is the element id.
	// [2] the value is a dictionary, this dictionary will contain all of the info about the particular element.
	// [2.1] key - "id", value - element id.
	// [2.2] key - "lat", value - latitude value.
	// [2.3] key - "lng", value - longitude value.
	// [2.4] key - "name", value - to be shown in the info window.
	// [2.5] key - "selected", value - initially selected or not.
	NSMutableDictionary* objectDictionary;
	
	//NSMutableArray* annotationBlockInfo;
//	NSInteger firstNonGeoFormIndex;
    
    // data loading variables.
    BOOL primaryDataDoneForAllLayers;
//    BOOL secondaryDataDone;
    //NSInteger primaryDataloadingProgress;
//    NSInteger secondaryDataloadingProgress;
    
    //void *iterator;
   // void *formatManager;
    
    // geo attribute position in the template.
//    NSInteger geoAttributeDepth;
    
    // initial region for first block data.
    MKCoordinateRegion initialRegion;
    
    // TODOLAYERS-DONE: commented out
    /*int latBaseFormID;
    int lngBaseFormID;
    int pointBaseFormID;
    const void *curLatData;
    const void *curLngData;
    const void *curPointData;
    NSString *curPointVal;
    NSString *latElID;
    NSString *lngElID;
    NSString *pointElID;
    NSString *jointElID;
    double curLatVal;
    double curLngVal;
    BOOL curLatValOK;
    BOOL curLngValOK;*/
    
    //BOOL currentMaxMinChanged;
    //NSString *geoAttrElID;
    
    // shape config xml, having geo role info.
    /*!
     *  shape config info which is used for all the layers
     */
    MWShapeConfig* shapeConfig;
//    NSString* __weak shapeConfigId;
//    NSString* shapeGeoRole;
//    NSString* __weak shapeType;
//    NSString* __weak parentShapeType;
//    NSMutableDictionary* searchTypes;
    
    NSMutableArray *layers; //will hold all the datasets
    
@public
	NSMutableDictionary* mapData;
}

/*!
 *  init method
 *
 *  @param iter          the first layer's grid iterator
 *  @param _Props        the xml string containing all the configurations
 *  @param delegate      delegate of id<MSIMapWidgetViewerDelegate>
 *  @param widgetViewer  reference to the MSIMapWidgetViewer instance
 *  @param isESRIAdapter whether the map is esri map or not
 *
 *  @return instance of MSIMapWidgetInfo
 */
- (id)initWithIterator: (void *)iter withProps: (NSString*)_Props withDelegate:(id<MSIMapWidgetViewerDelegate>)delegate withWidgetViewer:(MSIMapWidgetViewer*)widgetViewer asESRIAdapter : (BOOL) isESRIAdapter;
- (id)initWithObjectDictionary: (NSDictionary*) objectDict;
- (void)reinitWithObjectModel: (id<MSITabularDataProtocol>) objectModel
                     iterator: (void *)iter;

/*!
 *  populate map annotations when model is ready.
 *
 *  @param layerIndex index of the layer
 *  @param sequIndex  index of the annotation block
 *
 *  @return a message indication whether the population succeed or not
 */
- (NSString*)populateMapAnnotations:(NSInteger)layerIndex sequIndex:(NSInteger)sequIndex;

// populate map network layer when model is ready.
//- (void)populateNetworkLayer:(MWNetworkLayer*)networkLayer culSquence:(NSInteger)sequence;
//- (void)drawNetworklayerWithAnnotations:(NSArray*)annotations;

- (void)updateMetricNamesForLayer:(NSInteger)layerIndex;
- (NSArray*) getMetricNamesForLayer:(NSInteger)layerIndex;
//- (MSIReportDataRow*)rowForIndex:(NSInteger)rowIndex;
//- (NSString*)error;

/*!
 *  check the validation of all the templates and configurations
 *
 *  @return a message indication whether the templates and configurations are OK
 */
- (NSString*)buildAndValidateForAllLayers;

//TODOLAYERS-DONE: what to do with this? can we break it into for separate layers?
- (MKCoordinateRegion)regionForAllAnnotations;

// get the number formatted metric value.
- (NSString*) formattedMetricValueWithoutThresholdApplyAtLayerIndex:(NSInteger)layerIndex AtRow:(NSInteger)rowIndex atColumn:(NSInteger)colIndex;
- (void)setDelegate:(id<MSIMapWidgetViewerDelegate>)delegate;
//- (void)clearDelegate;

//- (NSInteger)annotationDataBlockNum;
- (NSInteger)accumulatedAnnotationDataBlockNumForLayer:(NSInteger)layerIndex;
- (NSArray*)annotationsForBlockIndex:(NSInteger)blockIndex forLayer:(NSInteger)layerIndex;
//- (NSInteger)affinityDataBlockNum;

//- (NSInteger)findFirstNonGeoFormIndex;
- (NSInteger)findDisplayFormIndexForLayer:(NSInteger)layerIndex;
- (BOOL)regionValid;
- (BOOL)regionChanged;
- (void)setInitialRegion:(MKCoordinateRegion)region;

- (void)updatePrimaryDataStatusForAllLayersWithMessage:(NSString*)message;
//- (void)updateSecondaryDataStatusWithMessage:(NSString*)message;
- (void)resetDataLoadingProgress;
//- (void)resetSecondaryDataLoadingProgress;

- (void)populateShapeComfigWithXml:(NSString*)configXml;
- (void)populateShapeComfigWithData:(NSData*)data;
//- (NSInteger)parentGeoRoleFormIndex;

- (BOOL)regionHasAllAnnotations:(MKCoordinateRegion)region;

/*!
 *  set grid iterator to seconary layer
 *
 *  @param iterator   grid iterator of one of the secondary layers
 *  @param layerIndex index of the layer
 */
- (void)addSecondaryLayerIterator:(void *)iterator withLayerIndex:(NSInteger)layerIndex;

/*!
 *  get the configuration of one layer
 *
 *  @param layerIndex index of layer
 *
 *  @return the configuration of specified layer
 */
- (MWVizPropInfo *)vizPropInfoForLayer:(NSInteger)layerIndex;

/*!
 *  to define if a form is geo info form
 *
 *  @param layerIndex           to define the layer
 *  @param startFormGlobalIndex the attribute's first form global index, to define one attribute
 *  @param formLocalIndex       the form's local index inside attribute, has different meaning with baseFormID which is the form index stored in iserver. formLocalIndex is used  to define the form which is to be checked
 *
 *  @return <#return value description#>
 */
- (BOOL)isGeoInfoFormForLayer:(NSInteger)layerIndex
andAttributeStartFormGlobalIndex:(NSInteger)startFormGlobalIndex
            andFormLocalIndex:(NSInteger)formLocalIndex;

//- (BOOL) isAttributeBeGeoInfoForLayer:(NSInteger)layerIndex andFormIndex:(NSInteger)formIndex
//              andAttrDisplayFormIndex:(NSInteger)attrDisplayFormIndex;

- (BOOL) isPrimaryDataPopulatedForLayer:(NSInteger)layerIndex;

- (BOOL)isDataDoneForAllLayers;
- (BOOL)isPrimaryDataDoneForAllLayers;

- (void)setMKMapViewForLayers:(MSTRMKMapView*)_mkMapView;

- (int)getZoomLevel;

- (BOOL)hasAreaLayerShown;
- (BOOL)hasClusterLayerShown;
- (BOOL)hasHeatmapLayer;
- (BOOL)hasHeatmapLayerShowingImage;
- (BOOL)hasHeatmapLayerShowingDensity;
- (BOOL)hasImageLayerShown;
- (BOOL)hasBubbleLayerShown;
- (BOOL)hasOnlyOneLayerShown;
- (MSIMWLayer*)getTheFirstShownLayer;

- (NSInteger)getAllShownAnnotationCount;

- (BOOL)isSingleLayerMode;
- (MSIMWLayer *)getFirstLayer;

- (BOOL)isAreaLayerWithTooManyRows;
- (BOOL)isMarkerLayerWithTooManyRows;
- (BOOL)willHaveTooManyMarkersAfterCluster:(BOOL)clustered ForLayer:(NSInteger)layerIndex;

- (BOOL)showCurrentLocationButton;

/*!
 *  get layer count
 *
 *  @return layer count
 */
- (NSInteger)getLayerCount;

- (void)createSecondaryLayerWithIterator:(void*)_iterator andConfigString:(NSString*)_xml andTemplateGridKey:(NSString *)templateGridKey;

/*!
 *  get specified layer
 *
 *  @param layerIndex index of the layer
 *
 *  @return the layer with type MSIMWLayer
 */
- (MSIMWLayer*)getLayerAtIndex:(NSInteger)layerIndex;

- (BaseMapType)getBaseMapType;
- (NSInteger)getDefaultMapType;
- (BOOL)getRedrawOnSelectorActionSetting;
- (BOOL)getIsVINewMap;

//@property (nonatomic, readonly) id<MSITabularDataProtocol> reportData;
/*!
 *  hole the MSIMWView for some operations
 */
@property (nonatomic, weak) MSIMWView* mwView;
//@property (nonatomic, readonly) MWVizPropInfo* vizPropInfo;
@property (readonly) BOOL hasPageBy;
// support multi attributes.
//@property (readonly) NSMutableArray* primaryGridAttributeInfos;
//@property (readonly) NSInteger primaryGridMetricStartCellIndex;
//@property (readonly) NSInteger primaryGridMetricCount;
//@property (nonatomic, readonly) NSMutableDictionary* annotationData;
//@property (nonatomic, readonly) NSMutableArray* annotationDatas;
@property (nonatomic, readonly) GeoMetaInfo* globalGeoMetaInfo;
@property (nonatomic, readonly) NSMutableArray* maxMetricValues;
@property (nonatomic, readonly) NSMutableArray* minMetricValues;
//@property (nonatomic, readonly) NSMutableArray* maxMetricValuesDict;
//@property (nonatomic, readonly) NSMutableArray* minMetricValuesDict;
//@property (nonatomic, readonly) NSInteger affinityMetricCount;
//@property (weak, nonatomic, readonly) NSMutableDictionary* affinityLookup;

//@property (nonatomic, readonly, getter = isPrimaryDataPopulated) BOOL primaryDataPopulated;

//@property (nonatomic, readonly) NSInteger geoAttributeDepth;
//@property BOOL currentMaxMinChanged;

/*!
 *  shape config which is used across layers
 */
@property (nonatomic, readonly) MWShapeConfig* shapeConfig;
//@property (weak, nonatomic, readonly) NSString* shapeConfigId;
//@property (nonatomic, readonly) NSString* shapeGeoRole;
//@property (weak, nonatomic, readonly) NSString* shapeType;
//@property (weak, nonatomic, readonly) NSString* parentShapeType;
//@property (nonatomic, readonly) NSMutableDictionary* searchTypes;
/*!
 *  an array of MSIMWLayer instances
 */
@property (nonatomic, readonly) NSMutableArray *layers;
@property (nonatomic) NSInteger rowCountForAllLayers;

//@property (nonatomic, strong) NSArray *vizPropInfoArray;


@end
