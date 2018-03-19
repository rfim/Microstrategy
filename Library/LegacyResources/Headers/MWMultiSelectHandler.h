//
//  MSIMWBaseView.h
//  MicroStrategyMobile
//
//  Created by xi zhang on 7/19/11.
//  Copyright 2011 MicroStrategy Inc. All rights reserved.
//  
//

#import <UIKit/UIKit.h>
#import <MapKit/MapKit.h>
#import "MSIMWAnnotation.h"
#import "MSIMapEnums.h"
#import "MSIMWAnnotationView.h"

@class MSIMWBaseView, MultiComponentSelector;

@interface MWMultiSelectHandler : NSObject {
@private
	MSIMWBaseView * __weak parent;
	// a UIView for multiple select.
	MultiComponentSelector* quartzSelector;
	// a view allowing editing selector shapes.
	UIView* multiSelectEditView;
	// allow editing the selector shapes or not.
	BOOL multiSelectEditEnabled;
	// Select by rectangle, circle, freeform, auto nearby search (heatmap) or marker tap (non heatmap).
	MultiSelectTypes multiSelectType;
	// used as the key for the dictionary used in multi select.
	NSInteger currentMultiSelectShapeKey;
	// Multi selected selector info shown in each region in bucket. 
    // Key: an NSNumber object associated with a selector (shape or select by marker tap), value: an array of selector info.
	NSMutableDictionary* multiSelectedSelectorInfo;
	// Multi selected annotations. 
    // Key: an NSNumber object associated with a selector (shape or select by marker tap), value: an array of selected annotations.
	NSMutableDictionary* multiSelectedAnnotations;
    
	// Multi selected annotations.
    // Key: an NSNumber object associated with a selector (shape or select by marker tap), value: an array of selected clusters.
	NSMutableDictionary* multiSelectedClusters;
    
	// Multi selector shapes. 
	// Key: an NSNumber object which is the selector shape key, value: the overlay selector shape.
	NSMutableDictionary* multiSelectedOverlayShapes;
    
    // dictionary of multi selection colors keyed by Title. 
	NSMutableDictionary* overlayColors;

	// region keys.
    NSNumber* selectedRegionKey;
	NSMutableArray* multiSelectedRegionKeys;
    BOOL keepSelectorShapes;
}

// add initial selected view.
-(void)addInitialSelectedView:(MSIMWAnnotationView *)annoView;
// called when outside selection changes for example, prompt.
-(void)outsideSelectionsChanged;
//called when one of the multiple select types button is tapped.
-(void)toggleMultiSelectType:(NSInteger)type;
// called when multi select link drill is tapped.
-(void)toggleMultiSelectApply;
// called when 'clear all' button is tapped.
-(void)toggleMultiSelectClear;
// called when 'clear' button for a region is tapped.
-(void)toggleMultiSelectClearRegion:(NSInteger)regoionIndex;
// called when multi select edit icon is tapped.
-(void)toggleMultiSelectEdit;
// called when a new selection shape is drawn.
-(void)handleNewSelectorShapeDrawn;
// called when the cancel button on each selector shape is tapped.
-(void)handleTapForCancelSingleSelectorShape:(UIButton*)button;
// called when a marker tap happens and create new region.
-(void)handleSelectChangeByMarker:(MSIMWAnnotation *)annotation createNewRegion:(BOOL)newRegion;
// called when a marker selections are don
-(void)handleSelectChangeByMarkerDone;
// called when a tap happens in heat map.
-(void)handleSelectChangeByNearbySearch:(NSArray*)nearbyAnnotations stage:(NearbySearchStages)stage;
// get all of the currently selected annotation objects.
-(NSArray*)allCurrentlyMultiSelectedAnnotations;
// get all of the currently selected annotation objects by region order.
-(NSArray*)allCurrentlyMultiSelectedAnnotationsInOrder;
// get all of the currently selected annotation regions.
-(NSDictionary*)allCurrentlyMultiSelectedRegions;
// get all of the currently selector shapes.
-(NSDictionary*)allCurrentlyMultiSelectedShapes;
// get all of the currently selector info.
-(NSDictionary*)allCurrentlyMultiSelectedSelectorInfo;

-(NSArray*)annotationsForRegion:(NSNumber*)region;
-(NSInteger)numberOfRegions;

-(void)clearSelections;

- (UIColor *) colorForOverlayWithTitle:(NSString *) title;

-(void)handleTouchesBegan:(NSSet *)touches withEvent:(UIEvent *)event;
-(void)handleTouchesMoved:(NSSet *)touches withEvent:(UIEvent *)event;
-(void)handleTouchesEnded:(NSSet *)touches withEvent:(UIEvent *)event;


// resets the multi select type. when user clicks on a shape type and immediately clicks it back to get out of shape drawing mode. 
- (void) resetMultiSelectType;
- (NSInteger) selectedRegionIndex;

// highlight/dehighlight.
- (void) deHighlight;
- (void) highlightShapeWithIndex:(NSInteger)shapeIndex;


@property(nonatomic, weak) MSIMWBaseView * parent;
@property(nonatomic) MultiSelectTypes multiSelectType;
@property(nonatomic, readonly, strong) NSMutableArray* multiSelectedRegionKeys;
@property(nonatomic) BOOL keepSelectorShapes;
@property(nonatomic, strong) NSNumber* selectedRegionKey;
@property(nonatomic, readonly, strong) NSDictionary *multiSelectedOverlayShapes;

@end
