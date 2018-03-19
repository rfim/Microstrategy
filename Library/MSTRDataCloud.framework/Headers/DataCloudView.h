//
//  DataCloudView.h
//  DataCloudDataCloud
//
//  Created by Anton Alexeev on 6/9/10.
//  Copyright 2010 EastBanc Technologies. All rights reserved.
//

#import <UIKit/UIKit.h>
//#import <MSTRMobileSDK/MSTRView.h>
#import "DataCloudViewController.h"
#import "LaserPointerDelegate.h"

@class DataViewerImpl;
@class DataCloudItemVisualizationSettings;
@class DataCloudItemView;
@class DataCloudTooltipVisualizationSettings;
@class DataCloudTooltipViewController;

//#define PADDING_LEFT  5
//#define PADDING_RIGHT  5
//#define PADDING_TOP  5
//#define PADDING_BOTTOM  5
//#define LEADING  10
//#define INTERVAL 10

#define MAX_FONT_DEF 80
#define MIN_FONT 7
#define MAX_ALLOWED_LABELS_PAD 400
#define MAX_ALLOWED_LABELS_PHONE 120

enum equation {
    squareRootEquation, logarithmEquation, linearEquation
};

enum align {
    alignLeft, alignRight, alignCenter, alignJustify
};

@class ReportData;

/*
    DataCloudView extends MSTRView interface.
    This interface contains methods for calculating window size
    depending on metric value of attributes    
*/

@interface DataCloudView : UIView <MSILaserPointerDelegate>
{
	// visualizationSettings stores parameters for visualization purposes
	// such as colors and font
	DataCloudItemVisualizationSettings * visualizationSettings;
	
	// Current font and metric values depends on 'fitting' algorithm and
	// recalculates each time current layout doesn't fit device's screen
	// current maximum font size
	int maxFontSize;
    //user-configured maximum font size
    int maxFontSizeConf;
	// current minimum font size
	int minFontSize;
	// current maximum metric
	CGFloat maxMetric; 
	// current minimum metric
	CGFloat minMetric;
	
	// equationType - algorithm type to calculate font size depending on metric value
	enum equation equationType;
	// element align on screen type
	enum align alignMode;
	// Stores coordinates of a touch. Sets in controller when touches began
	CGPoint initialTouchPoint;
	// items array stores elements of DataCloudItem type
	NSArray *items;
	//
	NSArray *sortedItems;
	// itemsToShow array stores remaining elements that show on screen after 'fitting' algorithm
	NSMutableArray *itemsToShow;
	// TRUE if data has attributes an metrics. Sets in DataCloudViewController
	BOOL hasAttributesAndMetrics;
	// Delegate that handles DataCloudViewDelegate protocol message (DataCloudViewController type)
	NSObject<DataCloudViewDelegate> *__unsafe_unretained delegate;
	NSString *error_msg;
	
	@private
	// Stores visualization settings(color, font) for tooltip
	DataCloudTooltipVisualizationSettings * tooltipVisualizationSettings;
	DataCloudTooltipViewController * tooltip;
	// TRUE if touches began. Sets in controller
	BOOL tapHolded;
	// TRUE if item was selected on screen
	BOOL itemSelected;
	// dataCloudItemViews array stores elements that really shown on screen (added to MSTRView)
	NSMutableArray * dataCloudItemViews;
	// Estimated height layout (can change during 'fitting' algorithm) 
	CGFloat layoutHeight;
	UILabel *errorMessage;
	BOOL itemsFromCenter;
	DataCloudItemView * highlightedItem;
	DataCloudItemView *selectedItem;
	
    NSMutableArray* selectedItemSiblings;
	NSMutableString* mAccString;
	
	//
    int padding_left;
    int padding_right;
    int padding_top;
    int padding_bottom;
    int leading;
    int interval;
    BOOL actionDefined;
}
@property (nonatomic, assign) BOOL actionDefined;
@property (nonatomic, strong) DataCloudItemVisualizationSettings *visualizationSettings;
@property (nonatomic) CGFloat maxMetric;
@property (nonatomic) CGFloat minMetric;
@property int maxFontSize;
@property int maxFontSizeConf;
@property int minFontSize;
@property (nonatomic) enum equation equationType;
@property (nonatomic) enum align alignMode;
@property CGPoint initialTouchPoint;
@property (nonatomic, strong) NSArray *items;
@property (nonatomic, strong) NSMutableArray *itemsToShow;
@property BOOL hasAttributesAndMetrics;
@property (nonatomic, unsafe_unretained)NSObject<DataCloudViewDelegate> *delegate;
//@property (nonatomic, copy) NSString *error_msg;
@property (nonatomic, readonly) NSMutableArray * dataCloudItemViews;

- (void) setError_msg:(NSString *)str;
- (BOOL) refreshHighlightedItem:(NSSet *) touches;
- (void) tapAndHold:(NSSet *)touches;
- (void) tap:(NSSet *)touches;
- (void) endTouch:(NSSet *)touches;
- (void) viewSetup;
- (void) addItemViewToSelectedSiblings:(UIView*)view;
- (void) drawDataCloudItemsText;
- (void) deSelectTheItem;
- (void) updateInfoWindowWhenTouchesMove:(NSSet *)touches;
- (void) hideTooltip;

@end
