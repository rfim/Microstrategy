//
//  SelectionHelper.h
//  IPadMain
//
//  Created by Liang Chen on 6/1/10.
//  Copyright 2010 MicroStrategy Inc. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <vector>

class DSSRWSectionNode;
class DSSRWGridIterator;
class DSSBaseElementProxy;
class DSSBaseElementsProxy;
struct _GUID;
typedef _GUID GUID;

@class WidgetPropertiesStore;
@class Commander;
@class Command;
@class BrushingHighlightData;
@class PopoverContext;

@interface MSISelectionCoordinate : NSObject
{
}
@property (nonatomic)	NSInteger selectionType;	//0 for title, 1 for header
@property (nonatomic)	NSInteger axisType;  //0 for row, 1 for column
@property (nonatomic)	NSInteger ordinal;
@property (nonatomic)	NSInteger depth;

@end


@interface MSISelectionHelper : NSObject {
}

@property (nonatomic, readonly) DSSRWGridIterator* mpGridIterator;
@property (nonatomic, strong, readonly) Commander* mpCommander;
@property (nonatomic, readonly) PopoverContext * mpPopoverContext;
@property (nonatomic, readonly) int colDepth;
@property (nonatomic, readonly) int rowDepth;
@property (nonatomic) bool isIgnoreShowAll;





- (id)initWithCommander:(Commander*)_commander andIterator:(DSSRWGridIterator*)_gridIterator;
- (void) loadDefaultSelections;
- (void) loadDefaultSelectionsContainSubtotal:(BOOL) isContainSubtotal;
- (void) reloadCurrentSelections;
- (std::vector<DSSBaseElementProxy*>) getSelectionInfo:(NSString*)objectID; // the caller should release this array
- (std::vector<DSSBaseElementsProxy*>) getAllSelectionInfo;
- (NSArray*) getSelectedElementIds:(NSString*)attributeID;
- (void) setupPopoverContext:(UIView*)targetView withFrame:(CGRect)targetFrame withInfoWindowLayoutKey:(NSString*)iwLayoutKey;


- (BOOL) isTitleSelectable:(int)iAxis withDepth:(int)iDepth;
- (void) handleTitleSelections:(int)iAxis withDepth:(int)iDepth;
- (void) handleTitleSelectionsWithDepthArray:(int)iAxis withDepthArray:(NSArray*)iDepthArray;
- (BOOL) hasSelectorTargetDefined;


- (BOOL) isHeaderSelectable:(int)iAxis withOrdinal:(int)iOrdinal withDepth:(int)iDepth;
- (BOOL) isHeaderSelectableWithGridIterator:(DSSRWGridIterator*)lGridIterator andAxis:(int)iAxis withOrdinal:(int)iOrdinal withDepth:(int)iDepth;
- (BOOL) isHeaderHasSelectable:(int)iAxis withDepth:(int)iDepth;
- (void) handleHeaderSelections:(int)iAxis withOrdinal:(int)iOrdinal withDepth:(int)iDepth;
// hozhang: for selection of map multiple layer with multiple templates
- (void) handleHeaderSelectionsWithGridIterator:(DSSRWGridIterator*)gridIterator withAxis:(int)iAxis withOrdinal:(int)iOrdinal withDepth:(int)iDepth;

//Clear warning by Nan Jiang, 08/03/2012
- (BOOL) handleHeaderSelectionsForEntireRow:(int)iAxis withOrdinal:(int)iOrdinal;
- (BOOL) handleHeaderSelectionsForCell:(int)ordinal withDepth:(int)depth;
- (BOOL) handleUnSelectAll;
- (BOOL)handleSelectionsForCoordinateArray:(NSArray*)array;//the object in the array is MSISelectionCoordinate


//will discard later
//yuazhang;823413;fix warning by remove duplicate declaration
- (BOOL) handleHeaderSelectionsWithPairArray:(int)iAxis withOrdinal:(NSArray*)iOrdinalArray withDepthArray:(NSArray*)iDepthArray;
- (void) handleHeaderSelections:(int)iAxis withOrdinals:(NSArray*)iOrdinals withDepth:(int)iDepth;
- (void) handleHeaderSelectionsWithGridIterator:(DSSRWGridIterator*)gridIterator withAxis:(int)iAxis withOrdinals:(NSArray*)iOrdinals withDepth:(int)iDepth;
//- (void)handleHeaderSelectionsWithGridIteratorOne:(DSSRWGridIterator*)gridIterator1 withGridIteratorTwo:(DSSRWGridIterator*)gridIterator2 withAxis:(int)iAxis withOrdinalsOne:(NSArray*)iOrdinals1 withOrdinalsTwo:(NSArray*)iOrdinals2 withDepth:(int)iDepth;
- (void) handleHeaderSelectionsWithMultiActionsParas:(NSArray*)multiActionParas;
- (void) handleHeaderSelectionsWithDepthArray:(int)iAxis withOrdinal:(int)iOrdinal withDepthArray:(NSArray*)iDepthArray;
//- (BOOL) handleHeaderSelectionsForCell:(int)ordinal withDepth:(int)depth;
//- (BOOL) handleUnSelectAll;
- (void) handleHeaderSelections:(int)iAxis withOrdinal:(int)iOrdinal withDepth:(int)iDepth withContext:(PopoverContext*)iContext;
- (void) handleHeaderSelections:(int)iAxis withOrdinal:(int)iOrdinal withDepth:(int)iDepth withInfoWindowLayoutGroubyNode:(DSSRWSectionNode*)iGroupbyNode;
- (void) handleHeaderSelectionsWithGridIterator:(DSSRWGridIterator*)gridIterator withAxis:(int)iAxis withOrdinal:(int)iOrdinal withDepth:(int)iDepth withInfoWindowLayoutGroubyNode:(DSSRWSectionNode*)iGroupbyNode;
- (void) handleHeaderSelectionsWithDepthArray:(int)iAxis withOrdinal:(int)iOrdinal withDepthArray:(NSArray*)iDepthArray withContext:(PopoverContext*)iContext;

//- (void) reloadCurrentSelections;
- (BOOL) isMetricSelectable:(int)iAxis withOrdinal:(int)iOrdinal withDepth:(int)iDepth;
- (void) handleMetricSelectable:(int)iAxis withOrdinal:(int)iOrdinal withDepth:(int)iDepth;
//- (BOOL) handleUnSelectAll;

- (void)resetPopoverContext;
- (Command*) getCommand:(int)iAxis withOrdinal:(int)iOrdinal withDepth:(int)iDepth;
- (Command*) getCommandWithIterator:(DSSRWGridIterator*)gridIterator withAxis:(int)iAxis withOrdinal:(int)iOrdinal withDepth:(int)iDepth;

#pragma mark Lasso Selection
// support lasso selections
- (void)hanldeKeepOrExclude:(BOOL)mode withIndice:(std::vector<std::pair<int, int> >&)lRowColumn forEntireRow:(bool)entireRow;
- (void)handleSelectionForSeveralRows:(NSSet*)iOrdinals;
-(void)handleMultipleDataPointSelection:(NSMutableArray*)iHeaderSelections andMetricSelections:(NSMutableArray*)iMetricSelection;
//check whether two attribute element ids represent the same attribute element.
-(BOOL)isAttrElementID:(NSString *)IDString1 EqualsToIDFromTemplate:(NSString *)IDString2;
- (int) getDisabledUnitsOnTemplate;
-(NSArray*) getNamesForDisabledUnits;
-(void)handleKeepOnlyAndShow:(int)unitIndex withIndice:(std::vector<std::pair<int, int> >&)lRowColumn forEntireRow:(bool)entireRow withPos:(int)pos withPropertiesStore: (WidgetPropertiesStore*)pStore isHorizontal:(BOOL)isHori withChartType:(int)chartType;
- (void)handleClearAllViewFilter;

-(BOOL) hasTemplateSelectorDefined;
-(BOOL) isTemplateSelectorEnableAll;
-(BOOL) handleUnSelectAllForTemplateSelector;
-(BOOL) hasHighlightSelectorDefined;
-(BOOL) isBrushingHighlightTargetOnly;
-(BOOL) handleBrushHighlight:(NSMutableArray*)selectedElements withMectricHighlight:(BOOL)isMetricType withUsedAttrs:(std::vector<GUID> )usedAttrIDs;
-(void)handleTemplateSelection:(NSMutableArray*)iHeaderSelections andIsDataPointSelection:(BOOL)isDataPointSelection;
-(BOOL) IsTemplateSelectorOnAxis:(int)iAxis withOrdinal:(int)iOrdinal withDepth:(int)iDepth;
-(BOOL) handleHeaderSelectionsAndClearOther:(int)iAxis withOrdinal:(int)iOrdinal withDepth:(int)iDepth;

@end
