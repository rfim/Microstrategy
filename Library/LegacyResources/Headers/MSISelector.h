//
//  Selector.h
//  IPadDashboardViewer
//
//  Created by Liu Xin on 3/26/10.
//  Copyright 2010 MSTR. All rights reserved.
//


#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "MSISelectorItemProtocol.h"

#include <vector>
#include <set>
#include <string>

@class MetricExpression;
@class Commander;
@class InfoWindowContainerViewer;
@class SelectorDataProxy;
@class MSIProjectInfo;
@class MSIElement;
@protocol MSISelectorDelegateInternal;

//#import "DSSFormat.h"
class DSSFormat;
class DSSRWControlIterator;

typedef enum 
{
	SELECTOR_STYLE_PULLDOWN = 0,
	SELECTOR_STYLE_SELECT_SLIDER,
	SELECTOR_STYLE_SELECT_LIST_BOX,
	SELECTOR_STYLE_RADIO_BUTTON_ARRAY,
	SELECTOR_STYLE_CHECKBOX_ARRAY,
	SELECTOR_STYLE_SELECT_STATE_BUTTON,
	SELECTOR_STYLE_HYPER_LINK,
	SELECTOR_STYLE_METRIC_SLIDER,
	SELECTOR_STYLE_METRIC_MENU,
	SELECTOR_STYLE_BPM_BUTTON,
	SELECTOR_STYLE_ACTION_SUBMIT,
	SELECTOR_STYLE_ACTION_DISCARD,
	SELECTOR_STYLE_ACTION_RECALCULATE,
    SELECTOR_STYLE_CALENDAR,
    SELECTOR_STYLE_RECURSIVE_ATTRIBUTE_FILTER, // Added by wenzhou, 05/05/2016. To support recursive attribute of financial report.
}EnumSelectorStyle;

typedef enum EnumSelectorOrientation
{
	SELECTOR_ORIENTATION_VERTICAL = 0,
	SELECTOR_ORIENTATION_HORIZONTAL,
}EnumSelectorOrientation;

typedef enum EnumItemWidthMode
{
	ITEM_WIDTH_MODE_SAME = 0,
	ITEM_WIDTH_MODE_AUTO,
}EnumItemWidthMode;

typedef enum EnumSelectorModeInclude
{
	SELECTOR_MODE_INCLUDE = 0,
	SELECTOR_MODE_EXCLUDE,
}EnumSelectorModeInclude;

typedef enum EnumSelectorSectionType
{
    SELECTOR_SECTION_TYPE_UC = 0,
    SELECTOR_SECTION_TYPE_CGB,
}EnumSelectorSectionType;


@protocol MSISelectorDelegate <NSObject>
@optional
/**
 * This API is used to fire selection with single item.
 * It is only suitable for attribute/metric list selector, if metric selector call this API, it will do nothing.
 * @param iItem: the item inside of selector, you could get specific item with [MSISelector getSelectorItemAtIndex:].
 */
- (void)handleSelectionWithSelectorItem:(id<MSISelectorItemProtocol>)iItem;
/**
 * This API is used to fire selection with multiple items, similar with the above one.
 * @param iItems: it is an array of id<MSISelectorItemProtocol>, you could get specific item with [MSISelector getSelectorItemAtIndex:].
 */
- (void)handleMultiSelectionWithSelectorItems:(NSArray*)iItems;
/**
 * This API is used to select specific panel for panel selector.
 * If attribute/metric selector call this API, it will do nothing.
 * @param iIndex: the panel index you want to select.
 */
- (void)handleSelectionWithPanelIndex:(NSInteger)iIndex;
/**
 * This API is used to pop out target info window on the passin anchor view.
 * If there is no target info window, do noting.
 * @param iView: from which view you want to present info window.
 */
- (void)showTargetInfoWindowAnchorView:(UIView*)iView;
@end

@interface MSISelector : NSObject {
	CGRect frame;
	EnumSelectorStyle mSelectorStyle;
	EnumSelectorOrientation mSelectorOrientation;
	BOOL mIsMultiSelect;
	int mItemWidthMode;
	float mItemSpacing;
	std::vector<std::string> mSelectorData;
	std::vector<int> mSelectedItems;
	std::string mReplaceTextForEmptySelectionOnDropDownUC;
	BOOL mHasAllElement;
	BOOL mShowAll;
	std::string mTitle;
	BOOL mIsTitleNeedShowInDropDown;       // TQMS 480033 - indicate whether we need to show mTitle in otherplace like dropdown titlebar when there is no controlTitleBar
    BOOL mUseSelectorDataProxy;            // indicate we are using proxy for incremental selector rendering
    SelectorDataProxy* __weak mSelectorDataProxy; // the proxy for fetching data
	
	UIFont* mItemFont;
	UIFont* mItemBoldFont;
	UIColor* mFontColor;
	UIColor* mBgColor;
	int mBgIntColor; //used for action selector, since it needs integer color instead of UIColor
	BOOL mIsActionLink; //used for action selector to distinguish the link and button
	NSString* mDisplayText;
	int mBgStyle;
	UIColor* mGradientColor;
	UIColor* mSelectionColor;
	UIColor* mSelectionGradientColor;
	UIColor* mSelectionFontColor;
	
	id<MSISelectorDelegate, MSISelectorDelegateInternal> __weak mSelectorDelegate;
	
	UIView* mParentView;
	
	NSTextAlignment mTextAlignment;
	
	BOOL mSelectorModeInclude;
	
	Commander* __weak mpCommander;
	BOOL mIsUnset;
	BOOL mIsCGorCons;
    
    //search selector info
    NSString* __weak attributeId;
	MSIProjectInfo* __weak project;
    BOOL searchOnServer;
    BOOL ucOnDataset;
    NSString* __weak cubeId;
    NSString* __weak cubeInstanceId;
    NSString* __weak formId;
    NSString* __weak controlKey;
    NSArray* __weak selectedElements;
    NSArray* __weak selectorElements;
    NSString* __weak attributeName;
    
    // TQMS 634076, add baseFormTypes as one property of selector, use this property to create search string for search selector. By njiang, 10/12/2012.
    //std::vector<int>  baseFormTypes;
    NSArray* baseFormTypes;
    
    // for search selector to get the info window, if the search selector is put in info window.
    DSSRWControlIterator* mpRWControlIterator;
    
    EnumSelectorSectionType mSectionType;
}

@property(nonatomic)    BOOL mIsActionLink;
@property(nonatomic)    int mBgIntColor;
@property(nonatomic)    BOOL mIsUnset;
@property(nonatomic)    BOOL mIsCGorCons;
@property(nonatomic)    BOOL mShowAll;
@property(nonatomic)	CGRect frame;
@property(nonatomic)	EnumSelectorStyle mSelectorStyle;
@property(nonatomic)	EnumSelectorOrientation mSelectorOrientation;
@property(nonatomic)	BOOL mIsMultiSelect;
@property(nonatomic)	int mItemWidthMode;
@property(nonatomic)	float mItemSpacing;
@property(nonatomic)	std::vector<std::string> mSelectorData;
@property(nonatomic)	std::vector<int> mSelectedItems;
@property(nonatomic, weak) id<MSISelectorDelegate, MSISelectorDelegateInternal> mSelectorDelegate;
@property(nonatomic)	BOOL mHasAllElement;
@property(nonatomic)	std::string mReplaceTextForEmptySelectionOnDropDownUC;
@property(nonatomic, strong)	UIFont* mItemFont;
@property(nonatomic, strong)	UIFont* mItemBoldFont;
@property(nonatomic, strong)	UIColor* mFontColor;
@property(nonatomic, strong)	UIColor* mBgColor;
@property(nonatomic)	int mBgStyle;
@property(nonatomic, strong)	UIColor* mGradientColor;
@property(nonatomic, strong)	UIColor* mSelectionColor;
@property(nonatomic, strong)	UIColor* mSelectionGradientColor;
@property(nonatomic, strong)	UIColor* mSelectionFontColor;
@property(nonatomic, weak) SelectorDataProxy* selectorDataProxy;
@property(nonatomic) BOOL useSelectorDataProxy;
@property(nonatomic)	std::string mTitle;
@property(nonatomic)	BOOL mIsTitleNeedShowInDropDown;
@property(nonatomic)	NSTextAlignment mTextAlignment;
@property(nonatomic)	BOOL mSelectorModeInclude;
@property(nonatomic, weak) Commander* mpCommander;
@property(nonatomic, strong) NSString* mDisplayText;
@property (nonatomic, weak) MSIProjectInfo *project;
@property (nonatomic, weak) NSString *attributeId;
@property (nonatomic, weak) NSString *cubeId;
@property (nonatomic, weak) NSString *cubeInstanceId;
@property (nonatomic, weak) NSString* formId;
@property (nonatomic, weak) NSString* controlKey;
@property (nonatomic) BOOL searchOnServer;
@property (nonatomic) BOOL ucOnDataset;
@property (nonatomic, weak) NSArray* selectedElements;
@property (nonatomic, weak) NSArray* selectorElements;
@property (nonatomic, weak) NSString *attributeName;
@property (nonatomic, strong) NSArray* baseFormTypes;
@property (nonatomic) DSSRWControlIterator* mpRWControlIterator;
@property (nonatomic) EnumSelectorSectionType mSectionType;
@property (nonatomic) CGFloat scaleRatio;
@property (nonatomic) BOOL enabled;

#pragma mark -
#pragma mark custom selector support

//@mandatory

/**
 Get the text tint color for iOS 7 through DocumentThemeHelper.
 */
+ (UIColor*)textTintColor;

/**
 The method first clears all the sub views in iParentView, then generate the selector viewer's content, maybe several sub UIViews, and add them to iParentView as its subviews.
 @param iParentView Specifies the parent view of all the views generated in the method.
 */
- (void) updateSubViewsTo:(UIView*)iParentView;

/**
 The method is to switch the selection. It will highlight the newly selected element, dehighlight the old one, etc.
 @param iSelectedIndex Specifies the index of the element to be selected. 
 */
- (void) setSelection:(int)iSelectedIndex;

/**
 The method is to update the selector viewer display accrording to the input. The common scenario is when multiple selectors are have the same targets, if one of them switches, the others need to get updated.
 @param iSelectedItems Specifies the selected items.
 */
- (void) updateSelectionDisplay:(const std::set<int>&)iSelectedItems;

/**
 The method is called when rotation happens. It is to adjust all its subviews when ratation happens.
 */
- (void) layoutElements;

/**
 Add this API for table control
 */
- (void)changeAppearanceForTable;

#pragma mark -
#pragma mark interl use

- (id)initWithStyle:(EnumSelectorStyle)iSelectorStyle AndIsMultiSelect:(BOOL)iIsMultiSelect;

- (void) addChildren:(UIView*)iParentView;
//- (void) createSubViewsTo:(UIView*)iParentView;

- (void) loadProperties:(DSSFormat*)ipFormat;

- (void) calcAndSetSelectedElements:(int)iIndex selected:(std::set<int>&)ioSelected;

- (void) setSelected:(int)iIndex isSelected:(BOOL)iIsSelected;

- (float) calcHeightIfFitSize;

- (CGSize)calcPreferViewSize:(CGFloat)iFontSize fontName:(const char*)iFontName includeTitle:(BOOL)iIncludeTitle;

- (UIView*)getRootViewerContainer:(UIView*)iView;

- (CGFloat)getMinHeightLimit;

//- (void) applyPropertiesToSubViews;
- (void)setUserInteractionEnabled:(BOOL)userInteractionEnabled;

#pragma mark -
#pragma mark Mobile rotation support

- (void) cleanUpTemporaryUIElements;

// @deprecated
+ (MSISelector*) createSelectorWithStyle:(EnumSelectorStyle)iSelectorStyle
					AndIsMultiSelect:(BOOL)iIsMultiSelect 
					AndOrientation:(EnumSelectorOrientation) iSelectorOrientation
					AndFrame:(CGRect) iRect
					AndParent:(UIView*)iParentView;

+ (MSISelector*) createSelectorWithStyle:(EnumSelectorStyle)iSelectorStyle
					 AndIsMultiSelect:(BOOL)iIsMultiSelect
					   AndOrientation:(EnumSelectorOrientation) iSelectorOrientation
							AndParent:(UIView*)iParentView;

#pragma mark - Attribute Selector Related
/**
 * This API is used to get the selected indexes of selector items, the return object is an array of NSNumber, like [0, 1, 2 ...].
 * If the selector is Metric/Panel selector, it will return nil.
 * If the selector is search selector, it will return an empty array.
 */
- (NSArray*)getSelectedIndexes;
/**
 * This API is used to get the total count of items in side selector.
 * If there is item "(All)" inside selector, it will be also include in this count.
 * If the selector is Metirc/Panel selector, it will return -1.
 */
- (NSInteger)getSelectorItemCount;
/**
 * This API is used to get the item at special index.
 * If the selector is Metric/Panel selector, it will return nil.
 * If the selector is attribute search selector, it will return nil.
 * @param iIndex: the index of item
 */
- (id<MSISelectorItemProtocol>)getSelectorItemAtIndex:(NSInteger)iIndex;

#pragma mark - Panel Selector Related
/**
 * This API is used to get total count of panels in its list.
 * If the selector is attribute/metric, it will return -1.
 */
- (NSInteger)getPanelCount;
/**
 * This API is used to get the index of selected panel.
 * If there is no selected panel, it will return 0.
 * If the selector is attribute/metric, it will return -1.
 */
- (NSInteger)getSelectedPanelIndex;

@end

@protocol SelectorDropDownDelegate
- (void) growHeight:(int)iDeltaValue;

@end

