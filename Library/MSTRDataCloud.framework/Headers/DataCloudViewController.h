//
//  DataCloudViewController.h
//  DataCloud
//
//  Created by Anton Alexeev on 5/18/10.
//  Copyright EastBanc Technologies 2010. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "IViewer.h"
#import "MSIWidgetViewer.h"

@class InfoWindowContainerViewer;
@class DataWrapperEx;
@class DataControllerImpl;
@class DataCloudView;
@class DataCloudItemView;

@protocol DataCloudViewDelegate

/** itemSelectedInt method handles touches on screen.
	This message comes from DataCloudView class
*/
- (void) itemSelectedInt:(int) index;
- (BOOL) isLinkDrill: (int) index;
- (BOOL) isSelectable: (int) index;
- (void) itemSelection:(DataCloudItemView*) iView;

-(void) setupZoomScale:(UIView*)iView;
-(BOOL)shouldHighLightItem:(DataCloudItemView*)checkItemView withSourceItemView:(DataCloudItemView*)sourceItemView;
-(void)handleUnSelectAll;
-(BOOL)allApplicable;
-(BOOL)quitInfoWindowMode;
-(BOOL)isInInfoWindowMode;
-(BOOL)skipEndTouch;
@end

/**
   DataCloudViewController interface
 */
@interface DataCloudViewController : MSIWidgetViewer<IWidgetProtocol, DataCloudViewDelegate> {
	// Stores data received from MSTR library
    DataWrapperEx *reportData;
	// Responsible for data visualization
	DataCloudView *dcView;
	id<MSICommanderDelegate> commander;
	int selectionDepth;
	BOOL isSelectable;
    int selectableDepths[100];
    int numOfSelectabelDepths;
    BOOL mSizeChanged;
    
    InfoWindowContainerViewer* mInfoWindow;
    BOOL mAllApplicable;
    BOOL mFirstTime;
    BOOL mInInfoMode;
    BOOL mOnlyInfoWindowDefined;
    
    BOOL mSkipEndTouch;
    int mActionType;
}

// IWidgetProtocol
- (id)initViewer:(ViewerDataModel*)ipDataModel withCommanderDelegate:(id<MSICommanderDelegate>)_commander withProps:(NSString*)ipProps;
+ (MSTROrientation)supportedOrientation;
- (void)clearBaseView;
- (void) addViewToBaseView:(UIView*)view;
- (void)displayError:(NSString*)errorMessage;
- (BOOL)supportsFitToScreen;
//
- (void) checkSelectable;
- (bool)reInitDataModels;
- (void) initializeSubviews;
- (void)loadReportData;
- (BOOL) isLinkDrill: (int) index;
- (BOOL) isSelectable: (int) index;
- (void) itemSelection:(DataCloudItemView*) iView;

-(BOOL)quitInfoWindowMode;

@property (nonatomic, strong) DataWrapperEx *reportData;
@property (nonatomic, strong) DataCloudView *dcView;
@property (nonatomic, readonly) int actionType;


@end