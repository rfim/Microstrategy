/*
 *  PropertyPackage.h
 *  IPhoneChart
 *
 *  Created by dong shi on 1/11/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MSICHART_PROPERTYPACKAGE_H
#define	MSICHART_PROPERTYPACKAGE_H

#include "../Common.h"
#ifndef _VE_CE
#include "DSSIPhoneEnums.h"
#endif
#include "map"
#include "vector"
class CComVariant;

namespace MsiChart
{
	typedef enum
	{
		//for grid.
		_alignmentVertical,
		_alignmentTextDirection,
		_applyToGraphThreshold,
		_backgroundAlpha,
		_backgroundColor,
		_backgroundStyle,
		_highlightColor,
		_border3DStyle,
		_border3DWeight,
		_borderStyleBottom,
		_borderStyleLeft,
		_borderStyleRight,
		_borderStyleTop,
		_borderColorBottom,
		_borderColorLeft,
		_borderColorRight,
		_borderColorTop,
		_color,
		_currencySymbol,
		_currencyPosition,
		_decimalPlaces,
		_fontFamily,
		_fontSize,
		_fontWeight,
		_fontStyle,
		_fontScript,
		_fontStrikeout,
		_gradientAngle,
		_gradientColor,
		_gradientXOffset,
		_gradientYOffset,
		_hInsideColor,
		_hInsideStyle,
		_numberFormat,
		_negativeNumbers,
		_numberCategory,
		_paddingBottom,
		_paddingLeft,
		_paddingRight,
		_paddingTop,
		_patternColor,
		_patternStyle,
		_textAlign,
		_textDecoration,
		_thousandSeparator,
		_vInsideColor,
		_vInsideStyle,
		_wordWrap,
		_scaleFactor,
		_isDrillable,
		_inputControlStyle,
		_inputControlSetting,
		_gridEnd,
		
		//other: non-grid & renamed begin
		_unDefined = _gridEnd, //to save one slot.
		_heightMode,
		_columnScenario,
		_appearanceVisible,


		_addButtonBarPadding,
		_appearanceHideDuplicates,
		_appearanceName,
		_appearanceSpecialEffect,
		_appearanceTooltip,
		_appearanceTooltipText,
		_autoResizeInDesign,
        _applyGBToAllLayouts,
        _maxselectionNumber,
        _minselectionNumber,
		_autoSubmit,
        _searchOnServer,
        _noSelectionBehavior,
		_availableViewModes,
		_bandingEnabled,
		_bandingGroup1,
		_bandingGroup2,
		_bandingOptions,
		_bandingRowsColumns,
		_blackAndWhite,
		_bottomLeftCornerRadius,
		_bottomRightCornerRadius,
		_canDropLabelsCategory,
		_canDropLabelsLinear,
		_canGrow,
		_canGrowHorizontal,
		_canShrink,
		_canShrinkHorizontal,
		_canStagger,
		_colorGroup1,
		_colorGroup2,
		_defaultGridStyle,
		_defaultsVersion,
		_defaultViewMode,
		_dropShadowDepth,
		_dropShadowEffect,
		_editableViewMode,
		_embedFonts,
		_enableGridPadding,
		_expandToFit,
		_extraWidth,
		_fillStyle, 
		_forceNewPage,
		_graphFormat,
		_graphQualityDPI,
		_gridAreaPercent,
		_gridPosition,
		_gridTextAlign,
		_gridWidth,
		
		_hasRoundedCorners,
		_headerWidth,
		_height,
		_htmlText,
		_htmlType,
		_hyperlink,
		_includeBookmarks,
		_indented,
		_isHyperlink,
		_keepGroupTogether,
		_keepSectionTogether,
		_left,
		_hideIfEmpty,
		_lineColor2,
		_lineSpacing,
		_lockAspectRatio,
		_isChildrenHorizontal,
		_isDataHorizontal,
		_lineColor,
		_lineOrientation,
		_lineStyle,
		_lineWeight,
		_imageSizeMode,
		_imageWidth,
		_imageHeight,
		_lockColumnHeaders,
		_lockRowHeaders,
		_longName,
		_marginBottom,
		_marginLeft,
		_marginRight,
		_marginTop,
		_maxHeight,
		_maxPieLabelRadius,
		_maxWidth,
		_mergeCells,
		_mergeColumnCells,
		_multiSliderAlignment,
		_name,
		_newRowOrColumn,
		_nestedLabel,
		_nestedLabelBracket,
		_oldHeight,
		_oldWidth,
		_openInNewWindow,
		_overflowMode,
        _orientationBehavior,
		_pageBreakBetweenGroups,
		_pageHeight,
		_pageOrientation,
		_pageScaling,
		_pageWidth,
		_paperType,
		_pictureAlignment,
		_pictureMode,
		_prefetch,
		_printFooterOn,
		_printHeaderOn,
		
		_repeatHorizontally,
		_repeatSection,
		_restartPageNumbering,
		_rowHeight,
		_rowScenario,
		
		_scalePageHeaderFooter,
		_scalePagesTall,
		_scalePagesWide,
		_scalePercentage,
		
		_secondaryDataProviders,
		_sectionCaption,
		_sectionExpanded,
		_sectionHorizontalOverflow,
		_selectorMouseOverAction,
		_selectorItemSpacing,
		_selectorItemWidthMode,
		_selectorMultiselect,
		_selectorOrientation,
		_selectorStyle,
		_selectorMetricConditionType,
		
		_showBookmarks,
		_showColHeaders,
		_showFloatingToolbars,
		_showGaugeLabel,
		_showInHTMLViewMode,
		_showRowHeaders,
		_showTitleBar,
        _showAsCollapsiblePanel,
		_showWidgetTitleBar,
		_singleSliderAlignment,
		_sliderFitToContents,
		_suppressLastLevel,
		_templateBanding,
		_templateShowColHeaders,
		_templateShowRowHeaders,
		_titleOverlapViewMode,
		_title,
		_titleBarDisplay,
		_tooltipText,
		_top,
		_topLeftCornerRadius,
		_topRightCornerRadius,
		_transitionDuration,
		_transitionID,
		_transitionUse,
		_url,
		_useGaugeDefaultSeriesColors,
		_useMasterPageHeaderFooter,
		_usePageWidthAsLayoutWidth,
		_viewTitle,
		_widgetClassName,
		_widgetEnable,
		_widgetIsApp,
		_widgetProperties,
		_widgetSWFPath,
		_width,
		_widthMode,
		_windowState,
		_zIndex,
		
		//Newly Added To make the collection complete for Properties.mm
		_enableHorizontalSwipeToChange,
		_executionFullScreenMode,
		_graphType,
		_longNames,
		_selectorSelectionColor,
		_supportedOrientations,
		_widgetProps,
		_widgetNameForIPad,
		_maxCategories,
		_maxSeries,
		_legendDisplay,
		_titleDisplay,
		_depthEffect,
		_displayPageByBar,
		_displaySelectorDocked,
		_useAsInformationWindow,
        _informationWindowPlacement,
        _infoWindowMode,
        _infoWindowPosition,
        _tabCloseToDismissInfoWindow,
        _displayInPartialScreen,
        _initialSlidingInWidth,
        _initialSlidingInHeight,
        _themeColorType,
        _informationWindowTarget,
        _ctlAutoSubmit,
		_showControlTitleBar,
		_textOverflow,
		_selectorInclude,
		_visProps,
		_widgetNameForIPhone,
		_enableGraphTooltips,
        _enableZoom,
		_layoutBarPosition,
		_layoutBarStyle,
		_fitPDFContentHeight,
		
		//Newly added for IVE
		_analysisSection,
		_filterDisplay,
		_displayText, 
		_displayConfirmation,
        _offlineSubmission,
        _txnSubmitInOrder,
        _selectorAutoPick,
		//newly added 
		_displayRepromptIcon,
		_useLayoutAsInformationWindow,	
		_useAsFilterPanel,
        _useAsMasterPanel,
        _useAsFilterSummaryPanel,
        _layoutXML,
        _useSegments,
        _styleForSwitchingPanel,
        _resetToFirstPanelWhenTargeted, // TQMS 578048. By yzhi.
        _copyMode,//newly added for copy mode
        _enableCopyForText,
		_isNavigationDocument,
        _showingBarsTemporarily,//793295
		//ending
		_propertyEnd,
		
		//renamed
		_backGroundAlpha = _backgroundAlpha,
		_backGroundColor = _backgroundColor,
		_fontFamily2 = _fontFamily,
		_backGroundStyle = _backgroundStyle,
        
        _transparency,
        _blur,
	}_GridProperty;

	class PropertyPackage
	{
	public:
		PropertyPackage(bool iIsForGridOnly);
		~PropertyPackage();
	
		CComVariant* GetDefaultPropertyValue(Int32 iPropertyId);
		CComVariant* GetDefaultPropertyValue(_GridProperty iPropertyId);
	
		bool IsPropertyUseDefault(CComVariant* ipVariant);
		bool Find(_GridProperty iPropertyId);
		
		bool mIsGridOnly;
        static size_t InitialMemeryUsage();
	private:
		//std::map<_GridProperty, CComVariant> msDefaultMap;
		std::vector<CComVariant> msDefaultMap;
		std::map<CComVariant*, bool> msDefaultAddressPool;
	};
	
		//some one already named
		//Attention: all the named ones should be put at the heading part
		//Caller should adjust the starting index of the second section, make sure it exceeds all the named ones.
		/*_unDefined = -10000,
		_backgroundAlpha = DssGraphAreaAlpha,
		_backgroundColor = DssGraphAreaColor,
		_borderStyleBottom = DssGridBorderStyleBottom,
		_borderStyleLeft = DssGridBorderStyleLeft,
		_borderStyleRight = DssGridBorderStyleRight,
		_borderStyleTop = DssGridBorderStyleTop,
		_fontFamily = DssGraphFontName,
		_fontWeight = DssGridFontStyleBold,
		_fontSize = DssGraphFontSize,
		_numberFormat = - 616,
		_textAlign = DssGraphFontAlign,
		*/

}

#endif
