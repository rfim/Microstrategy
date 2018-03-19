//
//  MapWidgetRowDisplayInfo.h
//  MSTRMobile
//
//  Created by xi zhang on 4/15/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <MapKit/MapKit.h>
#import "MSIMapEnums.h"

typedef enum _InfoWindowStyles{
    EnumIWDefault = 0, EnumIWPanelStack, EnumIWLayout
} InfoWindowStyles;

typedef enum _IAffinityLineTypes{
    EnumAffinityArcLine = 0, EnumAffinityStraightLine
} AffinityLineTypes;

typedef enum _AffinityUserSetTypes{
    EnumAffinityUserSetNone = 0, EnumAffinityUserSetLineThickness, EnumAffinityUserSetLineColor, EnumAffinityUserSetBoth
} AffinityUserSetTypes;

typedef NS_ENUM(NSInteger, MarkerClusterOptions) {
    MarkerClusterOptionsNone,
    MarkerClusterOptionsOn,
    MarkerClusterOptionsOff
};


@interface MWVizPropInfo : NSObject {
    
@private
    BOOL drillable;
    NSInteger reportType;
    
    GeoSourceTypes geoSourceType;
    GeoFormatTypes geoFormatType;
    BOOL aggregatedByLocation;
    // 'aggregatedByLocationAF'is only used internally and is the version of 'aggregatedByLocation' in attribute form case.
    BOOL aggregatedByLocationAF;
    NSString* geoAttrID;
    NSString* pointObjectID;
    NSString* latObjectID;
    NSString* lngObjectID;
    NSString* jointAttrID;
    
    GraphicType graphicType;
    GraphicType originalGraphicType;
    NSString* markerImageKey;
    BOOL needApplyThreshold;
    
    InfoWindowStyles infoWinStyle;
    BOOL useDocLayoutAsIW;
    NSString* targetLayoutKey;
    
    // bubble view config.
    NSInteger maxBubbleSize;
    
    // affinity configuration properties.
    BOOL displayAffinity;
    NSString* affinityGridNodeKey;
    NSString* affinityLineMetricID;
    NSString* affinityColorMetricID;
    AffinityLineTypes affinityLineType;
    AffinityUserSetTypes affinityUserSetType;
    CGFloat affinityLineThicknessSetByUser;
    NSString* affinityLineColorSetByUser;
    CGFloat maxAffinityLineThickness;
    UIColor* defaultAffinityLineColor;
    
    InfoWindowStyles affinityInfoWinStyle;
    BOOL affinityUseDocLayoutAsIW;
    NSString* affinityTargetLayoutKey;
    
    NSInteger defaultMapType;
    
    NSString* __weak errorMessage;
    
    BOOL redrawOnSelectorAction;
    
    // heat map config.
    HeatMapThemes heatMapTheme;
    
    MapSizingStyle mapSizingStyle;
    
    // for now, only apply for ipad.
    BOOL showMapToolbar;
    BOOL showMapViewOptions;
    BOOL showSelectionBucket;
    
    // esri
    BOOL isESRIAdapter;
    
    // cluster types.
    ClusterDisplayTypes clusterDisplayType;
    BOOL shouldShowBubbleAsDiamond;
    BOOL showArea;
    BOOL showMarker;
    NSString* esriShapeId;
    
    NSString* geoRoleLookupObjectId;
    
    // affinity line property.
    BOOL onlyAnnotationsUsedInAffinityLinesShown;
    
    NSString* dispayFormID;
    NSString *gridKey;
}

//- (id)initWithXML:(NSString*)xml asESRIAdapter:(BOOL) isESRI;
- (instancetype)initAsESRIAdapter:(BOOL)isESRI;
- (void)finalizeAfterParsing;

@property (nonatomic) BaseMapType baseMapType;
//@property (nonatomic) GraphicType graphicType;
@property BOOL drillable;
@property NSInteger reportType;
@property (nonatomic, assign) GeoSourceTypes geoSourceType;
@property (nonatomic, assign) GeoFormatTypes geoFormatType;
@property (nonatomic, assign) BOOL aggregatedByLocation;
@property (nonatomic, assign) NSInteger maxBubbleSize;
@property (nonatomic, copy) NSString* geoAttrID;
@property (nonatomic, copy) NSString* pointObjectID;
@property (nonatomic, copy) NSString* latObjectID;
@property (nonatomic, copy) NSString* lngObjectID;
@property (nonatomic, copy) NSString* jointAttrID;
@property (nonatomic, assign) MarkerType markerType;
@property (nonatomic, assign) GraphicType graphicType;
@property (nonatomic, assign) GraphicType originalGraphicType;
@property (nonatomic, copy) NSString* markerImageKey;
@property (nonatomic, assign) BOOL needApplyThreshold;
@property (nonatomic, assign) InfoWindowStyles infoWinStyle;
@property (nonatomic, copy) NSString* targetLayoutKey;
@property (nonatomic, assign) BOOL displayAffinity;
@property (nonatomic, copy) NSString* affinityGridNodeKey;
@property (nonatomic, copy) NSString* affinityLineMetricID;
@property (nonatomic, copy) NSString* affinityColorMetricID;
@property (nonatomic, assign) AffinityLineTypes affinityLineType;
@property (nonatomic, readonly) AffinityUserSetTypes affinityUserSetType;
@property (nonatomic, assign) CGFloat affinityLineThicknessSetByUser;
@property (nonatomic, strong) NSString* affinityLineColorSetByUser;
@property (nonatomic, assign) CGFloat maxAffinityLineThickness;
@property (nonatomic, assign) CGFloat defaultAffinityLineThickness;
@property (nonatomic, strong) UIColor* defaultAffinityLineColor;
@property (nonatomic, assign) InfoWindowStyles affinityInfoWinStyle;
@property (nonatomic, copy) NSString* affinityTargetLayoutKey;
@property (nonatomic, assign) NSInteger defaultMapType;
@property (weak, nonatomic, readonly) NSString* errorMessage;
@property (nonatomic, assign) BOOL redrawOnSelectorAction;
@property (nonatomic, assign) HeatMapThemes heatMapTheme;
@property (nonatomic, assign) MapSizingStyle mapSizingStyle;

@property (nonatomic) BOOL showMapToolbar;
@property (nonatomic) BOOL showMapViewOptions;
@property (nonatomic) BOOL showSelectionBucket;

@property (nonatomic) BOOL isESRIAdapter;
@property (nonatomic) ClusterDisplayTypes clusterDisplayType;
@property (nonatomic) BOOL shouldShowBubbleAsDiamond;
@property (nonatomic, strong) NSString* esriShapeId;

@property (nonatomic, strong) NSString* geoRoleLookupObjectId;
@property (nonatomic) BOOL onlyAnnotationsUsedInAffinityLinesShown;
@property (nonatomic, strong) NSString* dispayFormID;

@property (nonatomic, assign) BOOL showCurrentLocationButton;
//@property (nonatomic, assign) BOOL willClusterMarkers;
@property (nonatomic, assign) MarkerClusterOptions markerClusterOption;
@property BOOL useDocLayoutAsIW;
@property BOOL affinityUseDocLayoutAsIW;
@property BOOL aggregatedByLocationAF;
@property BOOL showArea;
@property BOOL showMarker;
@property (nonatomic, copy) NSString *gridKey;

@property (nonatomic) BOOL isRemoved;
@property (nonatomic) ShowLegendState showLegend;


@end
