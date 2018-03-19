//
//  MSIMWSimpleBaseView.h
//  MicroStrategyMobile
//
//  Created by xi zhang on 11/30/11.
//  Copyright 2011 MicroStrategy Inc. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <MapKit/MapKit.h>
#import "MSIMapWidgetViewerDelegate.h"
#import "CAScreenshotLayer.h"

@interface MSTRMKMapView : MKMapView <CAScreenshotLayerDelegate>{
    UIImage* screenshot;
    BOOL needsScreenshot;
}

-(UIImage*) getScreenshot;
MKMapRect MKMapRectForCoordinateRegion(MKCoordinateRegion region);

@property (nonatomic, strong) UIImage* screenshot;
@property BOOL needsScreenshot;

@end

@interface MSIMWSimpleBaseView : UIView {
    
@private	
    id<MapWidgetSimpleBaseViewerDelegate> __weak simpleDelegate;
    
@protected
	// MK map view.
	MSTRMKMapView* mkMapView;
//    NSMutableDictionary* densityDictionary;
    
}

@property(nonatomic, weak) id<MapWidgetSimpleBaseViewerDelegate> simpleDelegate;
@property (nonatomic, readonly, strong) MKMapView* mkMapView;
//@property(nonatomic, strong) NSMutableDictionary* densityDictionary;

- (int)getZoomLevel;
- (BOOL)isDisplayedInAirplay;

@end
