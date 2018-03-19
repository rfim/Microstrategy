//
//  IViewer.h
//  IPadDashboardViewer
//
//  Created by bhuang on 3/23/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "Enums.h"

@class DataModel;
@class Commander;

@protocol IViewer

- (void)detachViewer;

@optional
- (void)handleEvent:(NSString*)ipEventName;
- (CGFloat)getMeasuredWidth;
- (CGFloat)getMeasuredHeight;
- (void)evaluateConditionalTransactionThreshold;
- (void)measure;

/**
 For viewer status restore, get the viewer's id.
 */
- (NSString*)getViewerID;

/**
 For viewer status restore, get the viewer status data (NSData*).
 */
- (NSData*)getViewerStatus;

@end


@class ViewerDataModel;
@protocol MSICommanderDelegate;
@protocol IWidgetProtocol <IViewer>


/**
 Init method of the widget.
 If the _props is not required, set it as nil
 this is the initializtation of the widget, do things that only needs to be initialized once, like initialization of parameters, preparing data
 */
- (id)initViewer:(ViewerDataModel*)_viewerDataModel withCommanderDelegate:(id<MSICommanderDelegate>)_commander withProps:(NSString*)_props;

/**
 call this method to get the supported orientations for widget in iPhone
 if no overridden, just use the default definition which is defined in WidgetViewer.mm
 */
+ (MSTROrientation)supportedOrientation;

/**
remove all subviews and use the specific view as the base view of the widget, use this method to customize the error view and pass it in, the error view is the one when the widget fails to render and we need to show something like error label to the user
*/
- (void) addViewToBaseView:(UIView*)view;

/**
display error message, override this method to customize the error label, note by doing this, you will use the default error label that is defined in WidgetViewer.mm
 */
- (void)displayError:(NSString*)errorMessage;

/**
Whethter the widget supports fit to screen on iPhone, could be overridden to define different behavior from default one
*/
- (BOOL)supportsFitToScreen;

@end

@protocol WidgetGridSwitchProtocol<NSObject, NSCoding> //this function only available when you use the widget to run a report
/**
 switch the widget to grid, could be overridden to define different behavior from default one
 */
- (void)quickSwitchToGrid;
/**
 switch the grid back to widget, override to provide different implemtation, else just follow the default behavior which is recreate the widget
 */
- (void)quickSwitchToWidget;

/**
  clean the views of the widget to save memory
  usually be called before recreateWidget or when the widget is deleted
 */
- (void)cleanViews;
/**
 could be called during initializing phase, or be called on layout/panel change, before switch grid back to widget, or be selected by the source, etc
 */
- (void)recreateWidget;

/*
 call rgus method when controller will reuse widget; if return YES, this widget could be reuse, otherwise not.
 */
- (BOOL) finishedFirstRendering;

//Used if the widget needs second dataset as the data source, else no need to override
- (void)requestSecondaryDataModel:(NSString*)nodeKey;

@end

