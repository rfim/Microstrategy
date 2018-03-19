//
//  MWAnnotationData.h
//  MSTRMobile
//
//  Created by Xi Zhang on 3/20/10.
//  Copyright 2009 MicroStrategy. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <MapKit/MapKit.h>
#import <CoreLocation/CoreLocation.h>
#import "Enums.h"
#import "MSIMapEnums.h"
#import "MWAnnotationInfo.h"
#import "MWOverlayDelegates.h"

@class MSIMetric;
@class MSIHeaderValue, MWNetworkElement;
@class MWClusteredAnnotation;
@class MSIMWLayer;

@interface MSIMWAnnotation :  NSObject <MKAnnotation, MWOverlayMetaData> {
	
@protected
    
	NSString *subtitle;
	NSString *title;
    
    BOOL coordinateIsAvailable;
    
	// map data from grid or form list.
	MapDataTypes dataType;
    
    BOOL singleSelected;
	BOOL multiSelected;
	NSString *annotId;
    
	NSMutableArray* gridAttributeInfo;
	NSInteger formCount;
	NSInteger metricCount;
	UIColor* color;
	NSString* imagePath;
	NSString* drillURLString;
	
	NSInteger selectedMetricIndex;
	
	// absolute row index
	NSInteger rowIndex;
	// internal row index.
	NSInteger currentRowIndexInAnnotation;
	BOOL subtotalOnTop;
	
	NSInteger displayMode;
	NSMutableDictionary* affinityElements;
    
    /* Holds the information related to the annotation and performs operations on the grid iterator on the annotation's behalf. */
    MWAnnotationInfo *annotationInfo;
    /* Which rows in the report data does this annotation correspond for? */
    NSMutableArray *rowIndexes;
    NSString* firstNonGeoFormValue;
    
    CLLocationCoordinate2D coordinate;
    MWClusteredAnnotation* __weak parent;
    BOOL cloned;
    
    NSInteger layerIndex;
}

-(id)initWithLat:(CLLocationDegrees)lat Lng:(CLLocationDegrees)lng;

-(NSString*) formNameForIndex:(NSInteger)index;
-(NSString*) formValueForIndex:(NSInteger)index;
-(NSString*) metricNameForIndex:(NSInteger)index;
-(NSString*) metricValueForIndex:(NSInteger)index;
-(NSString*) metricRawValueForIndex:(NSInteger)index forSubTotal:(BOOL)subtotal;
-(NSString*)metricNonThresholdValueForIndex:(NSInteger)index;
-(UIColor*) metricColorForIndex:(NSInteger)index defaultColor:(UIColor*)defColor;
-(BOOL) metricBoldForIndex:(NSInteger)index;
-(BOOL) metricItalicForIndex:(NSInteger)index;
-(NSInteger) metricFontSizeForIndex:(NSInteger)index;

-(NSInteger) semanticsForMetricIndex:(NSInteger)index;
// add an network element object directly.
-(void)addNetworkElement:(MWNetworkElement*)element;
-(void)setColorForAnnotationWithIndex:(NSInteger)metricIndex defaultColor:(UIColor *)defaultColor;

// how far a coordinate from this annotation.
- (PositionTypes)positionAnnotationWithCoordinate:(CLLocationCoordinate2D)coord gridRadius:(CLLocationDegrees)gridRadius;
- (CGFloat)distanceFromCoordinate:(CLLocationCoordinate2D)coord;

- (NSInteger)count;

-(BOOL)isUsedInAffinityLines;

@property(nonatomic,copy) NSString *subtitle;
@property(nonatomic,copy) NSString *title;
@property(nonatomic, assign) CLLocationCoordinate2D coordinate;

@property(nonatomic, assign) BOOL coordinateIsAvailable;

@property(nonatomic, assign) MapDataTypes dataType;
@property(nonatomic, assign) BOOL singleSelected;
@property(nonatomic, assign) BOOL multiSelected;

@property(nonatomic, strong) NSMutableArray* gridAttributeInfo;
@property(nonatomic, assign) NSInteger formCount;
@property(nonatomic, assign) NSInteger metricCount;
@property(nonatomic, assign) NSInteger displayMode;

@property(nonatomic, strong) UIColor* color;
@property(nonatomic, copy) NSString* imagePath;
@property(nonatomic, copy) NSString* drillURLString;

@property (nonatomic, assign) NSInteger selectedMetricIndex;

//used for selection in ipad
@property (nonatomic, assign) NSInteger rowIndex;

@property (nonatomic, assign) NSInteger currentRowIndexInAnnotation;
@property (nonatomic, assign) BOOL subtotalOnTop;

// associated affinity elements.
@property(nonatomic, readonly) NSMutableDictionary* affinityElements;

@property(nonatomic, copy) NSString *annotId;
@property(nonatomic, strong) MWAnnotationInfo *annotationInfo;
@property(nonatomic, readonly) NSMutableArray *rowIndexes;
@property(nonatomic, copy) NSString* firstNonGeoFormValue;

@property (nonatomic, weak) MWClusteredAnnotation* parent;
@property (nonatomic, assign) BOOL cloned;
@property NSInteger layerIndex;


@end
