/*
 *  MSIMapEnums.h
 *  MicroStrategyMobile
 *
 *  Created by xi zhang on 3/15/11.
 *  Copyright 2011 MicroStrategy Inc. All rights reserved.
 *
 */

typedef enum _TouchSourceTypes{
	EnumTouchSourceMap = 0, EnumTouchSourceAnnotation, EnumTouchSourceArea,
    EnumTouchSourceIW,
    EnumTouchSourceBucketRegion, 
    EnumTouchSourceBucketAnnotation,
    EnumTouchSourceBucketBackButton,
    EnumTouchSourceBucketCloseButton,
    EnumTouchSourceMultiSelect,    // in multi selecting.
    EnumTouchSourceClearAll,       // 'clear all' button is tapped.
    EnumTouchSourceClear,          // 'delete a region' is tapped.
    EnumTouchSourceAggregationIW,   // tap from the aggregation info window.
    EnumTouchSourceRegionDetail
} TouchSourceTypes;

typedef enum _MultiSelectTypes{
	EnumMultiSelectTap = 0, EnumMultiSelectRect, EnumMultiSelectCircle, EnumMultiSelectFreeform 
} MultiSelectTypes;

typedef enum _BoundaryTypes{
	EnumBoundaryNormal = 0, EnumBoundarySimplified
} BoundaryTypes;

typedef NS_ENUM(NSInteger, GeoSourceTypes) {
    EnumGeoSourceAttribute = 0, EnumGeoSourceAttributeForm
};

typedef NS_ENUM(NSInteger, GeoFormatTypes) {
    EnumGeoFormatUnknown = -1, EnumGeoFormatLatLng = 0, EnumGeoFormatPoint = 1, EnumGeoFormatLatLngOld = 2
};

typedef enum _GeoDataTypes{
	EnumGeoDataLat = 0, EnumGeoDataLng, EnumGeoDataPoint 
} GeoDataTypes;

typedef enum _MultiSelectActionTypes {
	EnumMWActionTypeNone = 0,
	EnumMWActionTypeSelector,
	EnumMWActionTypeDrill,
	EnumMWActionTypeLinkDrill,
	EnumMWActionTypeUnknown
} MultiSelectActionTypes;


typedef NS_ENUM(NSInteger, MarkerStyles) {
    EnumMarkerImage = 1,
    EnumMarkerBubble,
    EnumMarkerArea,
    EnumMarkerHeatMap,
    EnumMarkerDynamic
};

// hozhang,20160621, with web map enhancement, GrapicType and MarkerType are introduced to replace original MarkerStles
typedef NS_ENUM(NSInteger, GraphicType) {
    EnumGraphicNone = 0,
    EnumGraphicMarker,
    EnumGraphicBubble,
    EnumGraphicArea,
    EnumGraphicDensity
};

typedef NS_ENUM(NSInteger, MarkerType) {
    EnumMarkerTypeNone = 0,
    EnumMarkerTypePin = 1,
    EnumMarkerTypeSquare,
    EnumMarkerTypeDiamond,
    EnumMarkerTypeImage,
    EnumMarkerTypeCircle
};

typedef NS_ENUM(NSInteger, BaseMapType) {
    EnumBaseMapNone = 0,
    EnumBaseMapEsri = 1,
    EnumBaseMapGoogle = 2,
    EnumBaseMapCustom = 3
};

typedef NS_ENUM(NSInteger, ShowLegendState){
    EnumShowLegendNo = 0,
    EnumShowLegendYes = 1,
    EnumShowLegendNone = 2
};

typedef enum _MapDataTypes{
	EnumMapDataGrid = 0, EnumMapDataList
} MapDataTypes;

typedef enum _ClusterTypes{
	EnumClusterFromMixSources = 0
} ClusterTypes;

typedef enum _ClusterDisplayTypes{
	EnumNavigateClusteredMarkers = 0, EnumNearbyCluster, EnumNoCluster, EnumNavigateAllMarkers
} ClusterDisplayTypes;

typedef enum _ClusterStatuses{
	EnumClusterInit = 0, EnumClusterShown, EnumClusterNotShownYet 
} ClusterStatuses;

typedef NS_ENUM(NSInteger, HeatMapThemes) {
    EnumHeatMapBlue = 1, EnumHeatMapRed, EnumHeatMapBlack
};

typedef enum _MultiSelectEventTypes{
	EnumMultiSelectEventEnter = 0, EnumMultiSelectEventCancel, EnumMultiSelectEventClearAll, EnumMultiSelectEventChange, EnumMultiSelectEventClear , EnumMultiSelectEventClose
} MultiSelectEventTypes;

typedef enum _NearbySearchStages{
	EnumNearbySearchTap = 0, EnumNearbySearchToleranceChanging, EnumNearbySearchToleranceUpdate
} NearbySearchStages;

typedef enum _TapObjectTypes{
	EnumTapMapOnly = 0, EnumTapToolbar, EnumTapMarker, EnumTapCallout, EnumTapSelectorShapes, EnumTapBucket
} TapObjectTypes;

typedef NS_ENUM(NSInteger, MapSizingStyle) {
    MapSizingStyleAbsoluteValue = 0, MapSizingStyleMinimumValue
};

typedef enum _MapOverViewState{
	MapOverViewStateMinimized = 0, MapOverViewStateMaximized, MapOverViewStateRemoved
} MapOverViewState;

typedef enum _DensityMapSwitchState {
    DensityMapSwitchNone = 0, DensityMapSwitchFromDensityToMarker, DensityMapSwitchFromMarkerToDensity
} DensityMapSwitchstate;

typedef enum _AnnotationDisplayMode {
	AnnotationDisplayModeImage = 0, AnnotationDisplayModeValue, AnnotationDisplayModeBoth
} AnnotationDisplayMode;

typedef enum _PositionTypes{
	EnumPositionInside = 0, EnumPositionClose, EnumPositionFaraway
} PositionTypes;

typedef enum _ShapeLoadStyles{
	EnumShapeLoadByTile, EnumShapeLoadByChunk
} ShapeLoadStyles;

typedef enum _ShapeRenderingStyles{
	EnumShapeRenderingTileBased = 0, EnumShapeRenderingOverlayBased
} ShapeRenderingStyles;

typedef enum _ShapeSearchTypes{
	EnumShapesearchByName = 1, EnumShapesearchByAlias
} ShapeSearchTypes;

typedef enum _PopoverPresentingStatus{
    EnumPopoverNotPresent = 0, EnumPopoverWillPresent, EnumPopoverDidPresent
} PopoverPresentingStatus;
