//
//  MSTRWidgetHelper.h
//  iPadMain Library
//
//  Created by kyang on 2/18/11.
//  Copyright 2011 MicroStrategy Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Enums.h"
#import "RWDDashboardDelegate.h"
#import "MSIReportDisplayInfo.h"
#import "DSSIPhoneEnums.h"
#import "MSIURLHelper.h"
#include <string>
#include <vector>
#import "MSIModelData.h"
#import "MSISelectionHelper.h"
#import "MSIImageCache.h"
typedef enum
{
    MSIViewInPanelStackViewer_NormalViewer,
    MSIViewInPanelStackViewer_TextViewer,
    MSIViewInPanelStackViewer_ImageViewer,
    MSIViewInPanelStackViewer_RectangleViewer
}EnumMSIViewTypeInPanelStackViewer;

typedef enum EnumDataStatus {
	EnumDataFailed = 0,
	EnumDataPending,
	EnumDataSucceeded
}EnumDataStatus;

typedef enum _targetType
{
    EnumSelectorTargetTypeWrongIndex = -1,
    EnumSelectorTargetTypeNoTarget = 0,
    EnumSelectorTargetTypeInfoWindowOnly,
    EnumSelectorTargetTypeSelectorOnly,
    EnumSelectorTargetTypeInfoWindowAndSelector,
    EnumSelectorTargetTypeMultiInfoWindow,
    EnumSelectorTargetTypeMultiInfoWindowAndSelector
}EnumSelectorTargetType;

class DSSTemplate;
class DSSTemplateUnit;
//struct PopoverContext;
@protocol MSIDataProviderProtocol, IViewer;
@class ViewerDataModel, Commander, MSIHeaderValue;
@class PanelStackViewer;
@class RWControlData;
class DSSRWIterator;
class DSSRWSectionNode;
@class MSIWidgetViewer;
@class WebServerCredentials;
@class MSIWidgetHelper;
//deprecate
@class MSITabularData,MSICrosstabData;

@interface MSIWidgetHelperSelectionOperation :  NSObject{
}
@property ( nonatomic, assign) MSIWidgetHelper* widgetHelper;
@property ( nonatomic, assign) AxisType  axisType;   //0 for row, 1 for column
@property ( nonatomic, assign) NSInteger row;//@dlu: in our current API, we use ordinal and columnIndex in WidgetHelper classâs method. So, I suggest to use them also to keep consistent.
@property ( nonatomic, assign) NSInteger column;
-(instancetype)initWithAxisType:(AxisType)type rowIndex:(NSInteger) rowIndex colIndex:(NSInteger) columnIndex widgetHelper:(MSIWidgetHelper*) widgetHelper;
@end

@interface MSIWidgetHelper : NSObject<ImageCacheCallback> {
}

/*!
 @property   mpIterator
 @abstract   return the DSSRWIterator object, readonly.
 */
@property (nonatomic, readonly) DSSRWIterator* mpIterator;
/*!
 @property   commander
 @abstract   return the Commander object, readonly.
 */
@property (nonatomic, strong, readonly) Commander* commander;
/*!
 @property   widgetProps
 @abstract   return the widget Properties, readonly.
 */
@property (nonatomic, readonly) NSString* widgetProps; //TODO: change it to some object format instrad of string

@property (nonatomic, readonly) void* formatManager;
@property (nonatomic, readonly) MSISelectionHelper* selectionHelper;
@property (nonatomic, readonly) ViewerDataModel* viewerDataModel;

/**
 * @deprecated
 */
//init methods
/*!
 @method     initHelper:withCommander:withProps:
 @abstract   init method
 @param      _viewerDataModel the ViewerDataModel obj
 @param      _commander the Commander obj
 @param      _widgetProps the  widget properties
 @result     MSIWidgetHelper obj
 */
- (id)initHelper:(ViewerDataModel*)_viewerDataModel withCommander:(Commander*)_commander withProps:(NSString*)_widgetProps;

/**
 * @deprecated
 */
/*!
 @method     isCrosstabDataModel
 @abstract   check whether the data model is crosstab model
 @result     yes if it's crosstab data model
 */
- (BOOL)isCrosstabDataModel;

/**
 * @deprecated:use (id<MSIDataProviderProtocol>)dataProvider instead, each widgetHelper handles only one dataSet;
 */
- (id<MSIDataProviderProtocol>)dataProviderByType:(DPType)nType;

/**
 * @deprecated
 */
- (NSInteger)numOfSecondaryDataProviders;

/**
 * @deprecated
 */
- (id<MSIDataProviderProtocol>)secondaryDataProviderByType:(DPType)nType andIndex:(NSInteger)nIndex;

/**
 * @deprecated: use (void)registerViewer:(id<IViewer>)iViewer instead
 */
- (void)registerMasterView:(id<IViewer>)iViewer forDataProviderAtIndex:(NSInteger)index;

/**
 * @deprecated: use (BOOL)hasRegularDrillByAxisType:(AxisType)type andRowIndex:(int)row andColIndex:(int)col instead
 */
- (BOOL)hasRegularDrillByAxisType:(AxisType)type andOrdinal:(int)iOrdinal andDepth:(int)iDepth;
- (BOOL)hasRegularDrillByGridIterator:(DSSRWGridIterator*)gridIterator andAxisType:(AxisType)type andOrdinal:(int)iOrdinal andDepth:(int)iDepth;

/**
 * @deprecated: use (BOOL)handleRegularDrillByAxisType:(AxisType)type andRowIndex:(int)row andColIndex:(int)col instead
 */
- (void)handleRegularDrillByAxisType:(AxisType)type andOrdinal:(int)iOrdinal andDepth:(int)iDepth;

/**
 * @deprecated: use (BOOL)hasLinkDrillByAxisType:(AxisType)type andRowIndex:(int)row andColIndex:(int)col instead
 */
- (BOOL)hasLinkDrillByAxisType:(AxisType)type andOrdinal:(int)iOrdinal andDepth:(int)iDepth;
- (BOOL) hasLinkDrillByGridIterator:(DSSRWGridIterator*)gridIterator andAxisType:(AxisType)type andOrdinal:(int)iOrdinal andDepth:(int)iDepth;

/**
 * @deprecated: use (void)handleLinkDrillByAxisType:(AxisType)type andRowIndex:(int)row andColIndex:(int)col instead
 */
- (void)handleLinkDrillByAxisType:(AxisType)type andOrdinal:(int)iOrdinal andDepth:(int)iDepth;

/**
 * @deprecated: use (BOOL)hasLinkDrillByAxisType:(AxisType)type andRowIndex:(int)row andColIndex:(int)col instead
 */
- (BOOL)hasLinkDrillOnMetricWithOrdinal:(int)iOrdinal andDepth:(int)iDepth;

/**
 * @deprecated: use (void)handleLinkDrillByAxisType:(AxisType)type andRowIndex:(int)row andColIndex:(int)col instead
 */
- (void)handleLinkDrillOnMetricWithOrdinal:(int)iOrdinal andDepth:(int)iDepth;

/**
 * @deprecated: use (void)handleLinkDrillByAxisType:(AxisType)type andRows:(NSArray*)rows andColIndex:(int)col instead
 */
- (void)handleLinkDrillByAxisType:(AxisType)type andOrdinals:(NSArray*)iOrdinals andDepth:(int)iDepth;
- (void)handleLinkDrillByGridIterator:(DSSRWGridIterator*)gridIterator andAxisType:(AxisType)type andOrdinals:(NSArray*)iOrdinals andDepth:(int)iDepth;

/**
 * @deprecated
 */
- (void)handleLinkDrillOnMetricWithOrdinals:(NSArray*)iOrdinals andDepth:(int)iDepth;

/**
 * @deprecated:use (BOOL) isCellSelectableByAxisType:(AxisType)type andRowIndex:(int)row andColIndex:(int)col instead
 */
- (BOOL)isCellSelectableByAxisType:(AxisType)type andOrdinal:(int)iOrdinal andDepth:(int)iDepth;
- (BOOL) isCellSelectableByGridIterator:(DSSRWGridIterator*)iGridIterator andAxisType:(AxisType)type andOrdinal:(int)iOrdinal andDepth:(int)iDepth;
/**
 * @deprecated:use (void) handleCellSelectionByAxisType:(AxisType)type andRowIndex:(int)row andColIndex:(int)col instead
 */
- (void)handleCellSelectionByAxisType:(AxisType)type andOrdinal:(int)iOrdinal andDepth:(int)iDepth;

- (void)handleCellSelectionWithGridIterator:(DSSRWGridIterator*)gridIterator andAxisType:(AxisType)type andOrdinal:(int)iOrdinal andDepth:(int)iDepth;

/**
 * @deprecated:use (void) handleSelectionsForCoordinateArray:(NSArray*)array instead
 */
- (void)handleCellSelectionByAxisType:(AxisType)type andOrdinals:(NSArray*)iOrdinals andDepth:(int)iDepth;
/**
 * @deprecated:use (void)handleCellSelectionsForEntireRowByAxisType:(AxisType)type andRowIndex:(int)row instead
 */

- (void)handleCellSelectionWithGridIterator:(DSSRWGridIterator*)gridIterator andAxisType:(AxisType)type andOrdinals:(NSArray*)iOrdinals andDepth:(int)iDepth;

- (void)handleCellSelectionWithMultiActionsParas:(NSArray *)multiActionsParas;

- (void)handleCellSelectionForEntireRowByAxisType:(AxisType)type andOrdinal:(int)iOrdinal;
/**
 * @deprecated:use (BOOL)isHeaderSelectableByAxisType:(AxisType)type andColIndex:(NSInteger)col instead
 */
- (BOOL)isHeaderSelectableByAxisType:(AxisType)type andIndex:(NSInteger)index;
/**
 * @deprecated:use (void)handleHeaderSelectionsByAxisType:(AxisType)type andColIndex:(int)col instead
 */
- (void)handleHeaderSelectionByAxisType:(AxisType)type andIndex:(NSInteger)index;
/**
 * @deprecated: two step
 first use (void) setupPopoverContext:(UIView*)targetView withFrame:(CGRect)targetFrame;
 then  use (void) handleCellSelectionByAxisType:(AxisType)type andRowIndex:(int)row andColIndex:(int)col;
 */
- (EnumDataStatus) handleCellSelectionAndInfoWindowOnView:(UIView*)view frame:(CGRect)frame withAxisType:(AxisType)type andOrdinal:(int)iOrdinal andDepth:(int)iDepth;
- (EnumDataStatus) handleCellSelectionAndInfoWindowOnView:(UIView*)view frame:(CGRect)frame withGridIterator:(DSSRWGridIterator*)gridIterator withAxisType:(AxisType)type andOrdinal:(int)iOrdinal andDepth:(int)iDepth;

/**
 * @deprecated: two step
 first use (void) setupPopoverContext:(UIView*)targetView withFrame:(CGRect)targetFrame;
 then  use (void) handleSelectionsForCoordinateArray:(NSArray*)array;
 */
- (EnumDataStatus) handleCellsSelectionAndInfoWindowOnView:(UIView*)view frame:(CGRect)frame withAxisType:(AxisType)type andOrdinals:(NSArray*)iOrdinals andDepth:(int)iDepth;

/**
 * @deprecated: two step
 first use (void) setupPopoverContext:(UIView*)targetView withFrame:(CGRect)targetFrame;
 then  use (void) handleCellSelectionsForEntireRowByAxisType:(AxisType)type andRowIndex:(int)row;
 */
- (void) handleCellSelectionForEntireRowAndInfoWindowOnView:(UIView*)view frame:(CGRect)frame withAxisType:(AxisType)type andOrdinal:(int)iOrdinal;

/**
 * @deprecated: we provide - (BOOL)isMetricInRowAxis in ModelData.h
 */
- (BOOL)isMetricInRowAxis;

/**
 * @deprecated: we provide  (NSInteger) rowAxisTemplateUnitCount in modelData.h
 */
- (NSInteger)numOfAttributesInRowAxis;
/**
 * @deprecated: we provide  (NSInteger) colAxisTemplateUnitCount in modelData.h
 */
- (NSInteger)numOfAttributesInColumnAxis;
/**
 * @deprecated: we provide  (NSInteger) metricCount in modelData.h
 */
- (NSInteger)numOfMetrics;
/**
 * @deprecated
 */
- (NSArray *)secondaryDataProviderNodeKeys;

/**
 * @deprecated
 */
- (void)invalidateSecondaryDataProviders;
/**
 * @deprecated:use validateLinkDrillCachedByAxisType:(AxisType)type andRowIndex:(int)row andColIndex:(int)col instead
 */
- (BOOL) validateLinkDrillCachedByAxisType:(AxisType)type andOrdinal:(int)iOrdinal andDepth:(int)iDepth;
/**
 * @deprecated:use validateLinkDrillCachedByAxisType:(AxisType)type andRowIndex:(int)row andColIndex:(int)col instead
 */
- (BOOL) validateLinkDrillCachedOnMetricAtOrdinal:(int)iOrdinal andDepth:(int)iDepth;


//init methods
/**
 @method     initHelper:withCommander:withProps:
 @abstract   init method, users do not need to call it
 @param      (DSSRWIterator*)_mpIterator
 @param      _commander the Commander obj
 @param      _widgetProps the  widget properties
 @result     MSIWidgetHelper obj
 */
- (id)setupHelper2:(DSSRWIterator*)_mpIterator withCommander:(Commander*)_commander withProps:(NSString*)_widgetProps;

/**
 @abstract   get data provider
 @result     object conforming to MSIDataProviderProtocol, please convert it to concrete object if there is any warning
 */
- (id<MSIDataProviderProtocol>)dataProvider;

/**
 @method     reInitDataModels
 @abstract   refresh iterator and data model
 @result     true if success.
 */
- (bool)reInitDataModels;
/*
 @abstract   refresh the iterator
 */
-(void) reInitDataModelIterator;
/*
 @abstract   refresh data model
  @result    true if success.
 */
-(bool)reInitDataModelStruct;

- (void)reinit:(ViewerDataModel*)_viewerDataModel;

/**
 * @return      :  widgetHelper for  all secondary data provider
 * @param       :  the ViewerDataModel obj
 * @description :  one widgetHelper corresponds to one dataSet. MSIWidgetViewer has one default widgetHelper. If a widget has only one dataSet, superClass widgetViewer will help             to create default widgetHelper corresponding to the dataSet. If a widget has 3 dataSet A,B,C,  A is primary dataSet,
 superClass widgetViewer will  help to create a default widgetHelper corresponding to  dataSet A, you can get widgetHelper
 for the dataSet A and B through this API.
 */
- (NSMutableArray*)arrayWithSecondaryDataProviderWidgetHelper:(ViewerDataModel *)viewerDataModel;


/**
 * @return         :  depth converted from colIndex
 * @param          :  the ViewerDataModel obj
 * @description    :  the demo dataSet has one attribute, while two attribute forms. all the metric share the same depth
 RegionID DESC Profit Revenue
 depth:  0       0     1       1
 colIndex :  0       1     2       3
 */
- (int)getDepthByAxisType:(AxisType)type andColIndex:(int)col;

/**
 * @return         :  colIndex converted from depth
 * @param          :  the ViewerDataModel obj
 * @description    :  if several colIndex has the same depth, return the min one
 */
- (int)getColIndexByAxisType:(AxisType)type andDepth:(int)depth;

- (int)getUnitIndexByAxisType:(AxisType)type andColIndex:(int)col;

- (int)getColIndexByAxisType:(AxisType)type andUnitIndex:(int)unitIndex;



/**
 @method     hasRegularDrillByAxisType:andOrdinal:andDepth:
 @abstract   has regular drill
 */
- (BOOL)hasRegularDrillByAxisType:(AxisType)type andRowIndex:(int)row andColIndex:(int)col;
/**
 @method     handleRegularDrillByAxisType:andOrdinal:andDepth:
 @abstract   handle regular drill
 */
- (void)handleRegularDrillByAxisType:(AxisType)type andRowIndex:(int)row andColIndex:(int)col;
/**
 @method     hasLinkDrillByAxisType:andOrdinal:andDepth:
 @abstract   Is there link drill at specific grid cell
 @result     Yes means there is link drill
 */
- (BOOL)hasLinkDrillByAxisType:(AxisType)type andRowIndex:(int)row andColIndex:(int)col;
/**
 @method     handleLinkDrillByAxisType:andOrdinal:andDepth:
 @abstract   hadle link drill
 */
- (void)handleLinkDrillByAxisType:(AxisType)type andRowIndex:(int)row andColIndex:(int)col;


/**
 @method     handleLinkDrillByAxisType:andRows:andColIndex
 @abstract   handle multi selected rows.
 */
- (void)handleLinkDrillByAxisType:(AxisType)type andRows:(NSArray*)rows andColIndex:(int)col;

/**
 @return    :YES if cache of linkDrill exits in the cache,
 */
-(BOOL) validateLinkDrillCachedByAxisType:(AxisType)type andRowIndex:(int)row andColIndex:(int)col;

/**
 * @param1    :  displayInfoID for a MSIDisplayInfo object
 * @return    :  vector of pointer for DSSBaseElementProxy object
 */
- (std::vector<DSSBaseElementProxy*>) getSelectedElementProxyItems:(NSString*)displayInfoID; // the caller should release this array

/**
 * @param1    :  displayInfoID for a MSIDisplayInfo object
 * @return    :  array of initial selected element ID for  given displayInfoID
 */
- (NSArray*) getSelectedElementIds:(NSString*)displayInfoID;

/**
 * @param1      :  the targetView which the popOverView show on
 * @param2      :  the frame popOver point to but not cover
 * @abstract    :  show  info window or change the posion of info window
 */
- (void) setupPopoverContext:(UIView*)targetView withFrame:(CGRect)targetFrame;

/**
 * @abstract   check header selectable
 * @param      axisType and colIndex
 */
- (BOOL)isHeaderSelectableByAxisType:(AxisType)type andColIndex:(NSInteger)col;

/**
 @abstract   handle header selection
 @param      axisType and colIndex
 */
- (void)handleHeaderSelectionsByAxisType:(AxisType)type andColIndex:(int)col;

/**
 @abstract   handle header selection for several columns
 @param      axisType and colArry containing several colIndex
 */
- (void) handleHeaderSelectionsWithColArrayByAxisType:(AxisType)iAxis andColArray:(NSArray*)iColArray;

/**
 @abstract   check if cell selectable
 */
- (BOOL) isCellSelectableByAxisType:(AxisType)type andRowIndex:(int)row andColIndex:(int)col;

/**
 @abstract   check if this column has selectable cell. (If subtotal is can't selectable, element is selectable, return YES.)
 */
- (BOOL) isColumnHasSelectableByAxisType:(AxisType)type andColIndex:(int)col;

/**
 @abstract   handle cell selection
 */
- (void)handleCellSelectionByAxisType:(AxisType)type andRowIndex:(int)row andColIndex:(int)col;

/**
 @abstract   handle cell selection for the whole row
 */
- (void)handleCellSelectionsForEntireRowByAxisType:(AxisType)type andRowIndex:(int)row ;

/**
 @abstract   handle cell selection for the whole row
 */
- (void)handleCellSelectionByAxisType:(AxisType)type andRows:(NSArray*)rows andColIndex:(int)col;

/**
 @abstract   handle cell selection for several cells, the array should contain several MSISelectionCoordinate object,
 it is defined as
 @interface MSISelectionCoordinate : NSObject
 {
}，
 @param   array should contain several MSISelectionCoordinate object, these objects should located in different attribute, 
         if several objects located in the same attribute, only the last object will take effect
 */
- (void)handleSelectionsForCoordinateArray:(NSArray*)array;//the object in the array is MSISelectionCoordinate

/**
 @abstract   handle cell selection for several widgetHelpers, the array should contain several MSIWidgetHelperSelectionOperation objects,
 it is defined as
 @interface MSIWidgetHelperSelectionOperation : NSObject
 {
 }
 @param   operations should contain several MSIWidgetHelperSelectionOperation objects
 @param   commander is used to executeCommands
 */
+ (void) handleSelectionsWithSelectionOperationArray:(NSArray*) operations withCommander:(Commander*)commander;

/**
 @method     handleURLLinkOnView:withURL:
 @abstract   handle URL link for attributes
 @param      link URL string
 @result     Is the url valid or not.
 */
- (BOOL)handleURLLinkOnView:(UIView*)iView withURL:(NSString*)link;


/*!
 @method     handleCellSelectionForEntireRowAndInfoWindowOnView:frame:withAxisType:andOrdinal:andDepth:
 @abstract   handle cell seleciton and handle info window
 @discussion some widget may need the entire row for the info window
 */
//- (void) handleCellSelectionForEntireRowAndInfoWindowOnView:(UIView*)view frame:(CGRect)frame withAxisType:(AxisType)type andOrdinal:(int)iOrdinal;
/*!
 @method     handleHeaderSelectionsAndInfoWindowOnView:frame:withAxis:andOrdinal:andDepth:andInfoWindowLayoutKey;
 @abstract   handle header seleciton and handle info window
 @param      view the view where to attach the info window
 @param      frame the position of the info window
 @param      iAxis 0 means row axis, 1 means column axis
 @param      iOrdinal ordinal of the grid cell
 @param      iDepth depth of the grid cell
 @param      iwLayoutKey the info window layout key.
 */
- (EnumDataStatus) handleCellSelectionAndInfoWindowOnView:(UIView*)view
                                                    frame:(CGRect)frame
                                                 withAxis:(AxisType)iAxis
                                               andOrdinal:(int)iOrdinal
                                                 andDepth:(int)iDepth
                                   andInfoWindowLayoutKey:(NSString*)iwLayoutKey
                                                andCaller:(id <BinaryRetrievalHandler>)iCaller
                                              andSelector:(SEL)iCallbackFunction;

- (EnumDataStatus) handleCellSelectionAndInfoWindowOnView:(UIView*)view
                                                    frame:(CGRect)frame
                                         withGridIterator:(DSSRWGridIterator*)gridIterator
                                                 withAxis:(AxisType)iAxis
                                               andOrdinal:(int)iOrdinal
                                                 andDepth:(int)iDepth
                                   andInfoWindowLayoutKey:(NSString*)iwLayoutKey
                                                andCaller:(id <BinaryRetrievalHandler>)iCaller
                                              andSelector:(SEL)iCallbackFunction;

//Template validation
/**
 @method     getTemplate
 @abstract   get the widget template
 @result     DSSTemplate object
 */
- (DSSTemplate*)getTemplate;

/**
 @method     hasData
 @abstract   If there is data in widget's primary data provider
 @result     YES means there is data in widget's primary data provider
 */
- (BOOL)hasData;
/**
 @method     isMetricValid
 @abstract   If the metric valid
 @result     YES means valid
 */
- (BOOL)isMetricValid;


//Others
/**
 @method     getWidgetFrame
 @abstract   get widget frame
 @discussion if iPhone, the frame is the whole screen; if iPad, it's the actual size of the document
 @result     the frame of the widget
 */
- (CGRect)getWidgetFrame;

/**
 @method     isWidgetVisible
 @abstract   get whether the widget is visible
 @discussion this method may return different values depending on which document view is choosen
 @result     true if widget is visible, false otherwise
 */
- (bool) isWidgetVisible;

/**
 @method     registerViewer:
 @abstract   register viewer for main data provider
 @discussion users do not need to call this API
 @param      iViewer widget viewer
 */
- (void)registerViewer:(id<IViewer>)iViewer;

/**
 @abstract   register view if the dataSet is not main data provider
 @discussion this is for the handleEvent method of the view
 @param      iViewer widget view
 */
- (void)registerMasterViewer:(id<IViewer>)iViewer;

/**
 @abstract   unregister view
 @discussion if you have called resisterMasterView:iViewer, you should call this API when iViewer dealloc
 @param      iViewer widget view
 */
- (void)unregisterMasterViewer:(id<IViewer>)iViewer;

/**
 @method     getInfoWindowViewer
 @abstract   get the info window viewer for current view (if it exists)
 @discussion
 */
- (UIView*) getInfoWindowViewer;
- (UIView*)getInfoWindowViewerForIterator:(DSSRWGridIterator*)gridIterator;
- (NSArray*) getArrayWithInfoWindowViewersByAxisType:(AxisType)axisType andUnitIndex:(int)unitIndex;
- (NSArray*) getArrayWithAllInfoWindowViewers;

/**
 @method     hideControlledInfoWindow
 @abstract   hide popped info window that is controled by current view (if it exists)
 @discussion
 */
- (void) hideControlledInfoWindow;

/**
 @method     hideInfoWindowContainsSelf
 @abstract   hide the info window which contains the rwnode itself
 @discussion
 */
- (void)hideInfoWindowContainsSelf;

/**
 @method     getPanelStackViewer
 @abstract   get the panelstack viewer for current view (if it exists)
 @discussion
 */
- (PanelStackViewer*) getPanelStackViewer;

/**
 @method     isInfoWindowSupported
 @abstract   returns YES if info window is supported by current view
 @discussion
 */
- (BOOL) isInfoWindowSupported;

- (BOOL)isPanelStackInfoWinDefinedForGridIterator:(DSSRWGridIterator*)gridIterator;

/**
 @method     isLayoutInfoWindowSupported
 @abstract   returns YES if layout info window is supported by current view
 @discussion
 */
- (BOOL) isLayoutInfoWindowSupported;

/**
 @method     isInLayoutInfoWindow
 @abstract   returns YES if current view is in a info window layout
 @discussion
 */
- (BOOL) isInLayoutInfoWindow;

/**
 @method     isInPanelStack
 @abstract   returns YES if current view is in a panel stack viewer
 @discussion
 */
- (BOOL)isInPanelStack;

/**
 @method     isInInfoWindow
 @abstract   returns YES if current view is in a panelstack as info window
 @discussion
 */
- (BOOL)isInInfoWindow;

/**
 @method     isAnalysisDocument
 @abstract   check if the document is an IVE document, naming follows Flex
 @discussion
 */
- (BOOL) isAnalysisDocument; //check if the document is an IVE document, naming follows Flex

/**
 @method     getSupportedOrientationByWidgetStyle:
 @abstract	 returns supported orinetation by the widget in iPhone
 @discussion used in the subsection viewer
 @param      style of widget defined in Generic.h
 */
+ (MSTROrientation)getSupportedOrientationByWidgetStyle:(NSString*)widgetStyle;


/**
 @method     getSupportedOrientationByWidgetClass:
 @abstract   returns supported orinetation by the widget in iPhone
 @discussion used in the section viewer
 @param      class of widget
 */
+ (MSTROrientation)getSupportedOrientationByWidgetClass:(Class)widgetClass;

/**
 @method     onlyInfoWindowDefined:
 @abstract  check if the unit targets only infowindow
 @discussion used in widget
 @param      template unit
 */
-(BOOL) onlyInfoWindowDefined:(DSSTemplateUnit*)lpUnit;

/**
 @method     onlyInfoWindowDefinedOnAxis:
 @abstract  check if the axis of template targets only infowindow
 @discussion used in widget
 @param      integer indicating the axis. Pass 0 for row axis and 1 for column axis.
 */
-(BOOL) onlyInfoWindowDefinedOnAxis:(int)axis;

/**
 @method     getIsReport:
 @abstract  check if the we are running a report or a report service document
 */
-(BOOL) isReport;


/**
 @method getWebServerCredentials
 @abstract get web server credential for the document.
 */
- (WebServerCredentials*)getWebServerCredentials:(MSIReportDisplayInfo*)objectInfo;

/**
 @abstract :chcek if user has privilege on this document.
 @return   :YES if user has the privilege
 */
- (BOOL)checkPrivilege:(EnumDSSPrivilegeType)privilege;

/**
 @abstract :This API is workable When dataType is RWControlDataType,
 */
- (void)handleItemSelection:(int)itemIndex;

/**
 @return :return YES if the dataSet is RWControlDataType, or in another word ,the iterrator passed into the widgetHelper is RWControlGridIntertator
 */

- (BOOL)isRWControl;

/**
 @return :nodeKey string for the widget
 */
- (std::string)nodeKeyForWidget:(MSIWidgetViewer*)widget;



-(BOOL) hasLinkDrillDefined;
-(BOOL) hasURLLinkDefined;
-(void) setInfoWindowPopup:(BOOL) isInfoWindowPopup;
-(BOOL) hasInfoWindowDefinedOnUnit:(DSSTemplateUnit*)lpUnit;

+ (BOOL)hasImportantDrillPath:(DSSTemplateUnit*)ipDrillUnit;
/**
 @param      axisType and colIndex
 @param      the unit index
 @return :the selectorTargetType on a specific unit
 */
- (EnumSelectorTargetType) getSelectorTargetTypeFromAxis:(AxisType)type andUnitIndex:(int)index;


/**
 * get integer repsenting of UIColor in order 'RGB'
 * @param input UIColor
 * @return the color represented by integer in 'RGB' order
 */
-(int)ColorToInt:(UIColor*)color;

/**
 * get the background color for panel view containing the input widgetViewer,color represented by integer in 'RGB' order
 * @param input widgetView
 * @return the color represented by integer in 'RGB' order
 */
- (int) readPanelColor:(MSIWidgetViewer*)view;

/**
 * get the background color for subSection view containing the input widgetViewer,color represented by integer in 'RGB' order
 * @param input widgetView
 * @return the color represented by integer in 'RGB' order
 */
-(int) readSubSectionColor:(MSIWidgetViewer*)view;

/**
 * get the background color for document,color represented by integer in 'RGB' order
 * @param input widgetView
 * @return the color represented by integer in 'RGB' order
 */
-(int)readDocumentColor:(MSIWidgetViewer*)view;

/**
 * return the panelView from the given panelStack name and row index of the dataset
 * @param the target panelStack nodekey
 * @param the row index of the dataset
 * @return the first panelView in the panelstack
 */
- (UIView*)getPanelViewFromPanelStack:(NSString *)panelStackName andRowIndex:(int)rowIndex;

/**
 @method     getLinkDrillCountByAxisType:andOrdinal:andDepth
 @abstract   get link drill count at specific grid cell, return 0 if no linkdrill
 @result     an int means the number of linkdrill
 */
- (NSInteger) getLinkDrillCountByAxisType:(AxisType)type andOrdinal:(int)iOrdinal andDepth:(int)iDepth;

- (NSString*)getLinkDrillDisplayStringByAxisType:(AxisType)type andOrdinal:(int)iOrdinal andDepth:(int)iDepth andOrder:(int)order;

- (void)handleLinkDrillByAxisType:(AxisType)type andOrdinal:(int)iOrdinal andDepth:(int)iDepth andOrder:(int)order;

- (void) queueReusableView:(UIView*)view forPanelStack:(NSString*)panelStackName;
/**
 * return the view type for the given view
 * @param the viewer we want to get the type
 * @return the view type EnumMSIViewTypeInPanelStackViewer
 */
+ (EnumMSIViewTypeInPanelStackViewer) getViewerTypeInPanelStackViewer:(UIView*)viewer;

/**
 * return the text in the viewer, only valid for MSIViewInPanelStackViewer_TextViewer ,otherwise return nil
 * @param the viewer we want to get the text from
 * @return the text if viewer type is MSIViewInPanelStackViewer_TextViewer,sometimes the text is dynamic text,
 we need to analyse, then set the righ text using + (void) setTextViewer:(UIView *)viewer withText:(NSString *)text;
 */
+ (NSString*) getTextViewerText:(UIView*)viewer;

/**
 * set the right text for the viewer,only valid for MSIViewInPanelStackViewer_TextViewer ,otherwise do nothing
 * @param the viewer we want to set text for
 * @param the text we want to set
 */
+ (void) setTextViewer:(UIView *)viewer withText:(NSString *)text;

/**
 * return the imageSource in the viewer, only valid for MSIViewInPanelStackViewer_ImageViewer ,otherwise return nil
 * @param the viewer we want to get image source from
 * @return the text if viewer type is MSIViewInPanelStackViewer_ImageViewer ,sometimes the imagesource is dynamic image,
 we need to analyse and get the right image then set the righ image using
 + (void) setImageViewer:(UIView *)viewer withImage:(UIImage *)image;
 */
+ (NSString*) getImageViewerImageSource:(UIView*)viewer;

/**
 * set the right image for the viewer,only valid for MSIViewInPanelStackViewer_ImageViewer ,otherwise do nothing
 * @param the viewer we want to set image for
 * @param the image we want to set
 */
+ (void) setImageViewer:(UIView *)viewer withImage:(UIImage *)image;


@end
