/*
 *  FromatPropertySetDefinition.cpp
 *  DashboardViewer
 *
 *  Created by Luo Ming on 09-12-30.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "DSSFromatPropertyDefinition.h"

DSSFromatPropertyDefinition::DSSFromatPropertyDefinition()
{
	// Populate our format definition with the default values

	/*			
		  <prs n="FormattingNumber">
			<pr n="Category" dt="3" pdv="9"/>
			<pr n="DecimalPlaces" dt="3" pdv="0"/>
			<pr n="ThousandSeparator" dt="11" pdv="-1"/>
			<pr n="CurrencySymbol" dt="8" pdv="$"/>
			<pr n="CurrencyPosition" dt="3" pdv="0"/>
			<pr n="Format" dt="8" pdv="General"/>
			<pr n="NegativeNumbers" dt="3" pdv="3"/>
		  </prs>
	*/
	
	DSSFormatPropertySetMap* FormattingNumber = new DSSFormatPropertySetMap();
	FormattingNumber->SetPropertyIDType("Category", 1, 3, "9");
	FormattingNumber->SetPropertyIDType("DecimalPlaces", 2, 3, "0");
	FormattingNumber->SetPropertyIDType("ThousandSeparator", 3, 11, "-1");
	FormattingNumber->SetPropertyIDType("CurrencySymbol", 4, 8, "$");
	FormattingNumber->SetPropertyIDType("CurrencyPosition", 5, 3, "0");
	FormattingNumber->SetPropertyIDType("Format", 6, 8, "General");
	FormattingNumber->SetPropertyIDType("NegativeNumbers", 7, 3, "3");
	FormattingNumber->setName("FormattingNumber");

	mFormatPropertySetsMap["FormattingNumber"] = (FormattingNumber);
	
	/*
		<prs n="FormattingAlignment">
			<pr n="Horizontal" dt="3" pdv="1"/>
			<pr n="Vertical" dt="3" pdv="1"/>
			<pr n="TextWrap" dt="11" pdv="0"/>
			<pr n="TextDirection" dt="3" pdv="0"/>
		  </prs>
	*/
	
	DSSFormatPropertySetMap* FormattingAlignment = new DSSFormatPropertySetMap();
	FormattingAlignment->SetPropertyIDType("Horizontal", 1, 3, "1");
	FormattingAlignment->SetPropertyIDType("Vertical", 2, 3, "1");
	FormattingAlignment->SetPropertyIDType("TextWrap", 3, 11, "0");
	FormattingAlignment->SetPropertyIDType("TextDirection", 4, 3, "0");
	FormattingAlignment->setName("FormattingAlignment");
	
	mFormatPropertySetsMap["FormattingAlignment"] = (FormattingAlignment);
	
	/*
		<prs n="FormattingFont">
			<pr n="Name" dt="8" pdv="Arial"/>
			<pr n="Bold" dt="11" pdv="0"/>
			<pr n="Italic" dt="11" pdv="0"/>
			<pr n="Size" dt="3" pdv="10"/>
			<pr n="Strikeout" dt="11" pdv="0"/>
			<pr n="Underline" dt="11" pdv="0"/>
			<pr n="Color" dt="3" pdv="0"/>
			<pr n="Script" dt="3" pdv="0"/>
		  </prs>
	*/
	
	DSSFormatPropertySetMap* FormattingFont = new DSSFormatPropertySetMap();
	FormattingFont->SetPropertyIDType("Name", 1, 8, "Arial");
	FormattingFont->SetPropertyIDType("Bold", 2, 11, "0");
	FormattingFont->SetPropertyIDType("Italic", 3, 11, "0");
	FormattingFont->SetPropertyIDType("Size", 4, 3, "10");
	FormattingFont->SetPropertyIDType("Strikeout", 5, 11, "0");
	FormattingFont->SetPropertyIDType("Underline", 6, 11, "0");
	FormattingFont->SetPropertyIDType("Color", 7, 3, "0");
	FormattingFont->SetPropertyIDType("Script", 8, 3, "@@0x04e6");
    FormattingFont->SetPropertyIDType("HighlightColor", 9, 3, "0");
	FormattingFont->setName("FormattingFont");
	
	mFormatPropertySetsMap["FormattingFont"] = (FormattingFont);
	
	/*
		<prs n="FormattingBorder">
			<pr n="HInsideStyle" dt="3" pdv="1"/>
			<pr n="VInsideStyle" dt="3" pdv="1"/>
			<pr n="TopStyle" dt="3" pdv="1"/>
			<pr n="LeftStyle" dt="3" pdv="1"/>
			<pr n="BottomStyle" dt="3" pdv="1"/>
			<pr n="RightStyle" dt="3" pdv="1"/>
			<pr n="HInsideColor" dt="3" pdv="0"/>
			<pr n="VInsideColor" dt="3" pdv="0"/>
			<pr n="TopColor" dt="3" pdv="0"/>
			<pr n="LeftColor" dt="3" pdv="0"/>
			<pr n="BottomColor" dt="3" pdv="0"/>
			<pr n="RightColor" dt="3" pdv="0"/>
			<pr n="Border3DStyle" dt="3" pdv="0"/>
			<pr n="Border3DWeight" dt="3" pdv="2"/>
			<pr n="Border3DBottomColor" dt="3" pdv="0"/>
			<pr n="Border3DLeftColor" dt="3" pdv="0"/>
			<pr n="Border3DRightColor" dt="3" pdv="0"/>
			<pr n="Border3DTopColor" dt="3" pdv="0"/>
		  </prs>
	*/
	
	DSSFormatPropertySetMap* FormattingBorder = new DSSFormatPropertySetMap();
	FormattingBorder->SetPropertyIDType("HInsideStyle", 1, 3, "1");
	FormattingBorder->SetPropertyIDType("VInsideStyle", 2, 3, "1");
	FormattingBorder->SetPropertyIDType("TopStyle", 3, 3, "1");
	FormattingBorder->SetPropertyIDType("LeftStyle", 4, 3, "1");
	FormattingBorder->SetPropertyIDType("BottomStyle", 5, 3, "1");
	FormattingBorder->SetPropertyIDType("RightStyle", 6, 3, "1");
	FormattingBorder->SetPropertyIDType("HInsideColor", 7, 3, "0");
	FormattingBorder->SetPropertyIDType("VInsideColor", 8, 3, "0");
	FormattingBorder->SetPropertyIDType("TopColor", 9, 3, "0");
	FormattingBorder->SetPropertyIDType("LeftColor", 10, 3, "0");
	FormattingBorder->SetPropertyIDType("BottomColor", 11, 3, "0");
	FormattingBorder->SetPropertyIDType("RightColor", 12, 3, "0");
	FormattingBorder->SetPropertyIDType("Border3DStyle", 13, 3, "0");
	FormattingBorder->SetPropertyIDType("Border3DWeight", 14, 3, "2");
	FormattingBorder->SetPropertyIDType("Border3DBottomColor", 15, 3, "0");
	FormattingBorder->SetPropertyIDType("Border3DLeftColor", 16, 3, "0");
	FormattingBorder->SetPropertyIDType("Border3DRightColor", 17, 3, "0");
	FormattingBorder->SetPropertyIDType("Border3DTopColor", 18, 3, "0");
	FormattingBorder->setName("FormattingBorder");
	
	mFormatPropertySetsMap["FormattingBorder"] = (FormattingBorder);
	
	/*
		<prs n="FormattingPatterns">
			<pr n="FillColor" dt="3" pdv="16777215"/>
			<pr n="PatternColor" dt="3" pdv="8650752"/>
			<pr n="PatternStyle" dt="3" pdv="1"/>
			<pr n="FillStyle" dt="3" pdv="0"/>
			<pr n="ApplyToGraphThreshold" dt="11" pdv="0"/>
			<pr n="GradientColor" dt="3" pdv="16777215"/>
			<pr n="GradientAngle" dt="2" pdv="0"/>
			<pr n="GradientXOffset" dt="2" pdv="0"/>
			<pr n="GradientYOffset" dt="2" pdv="0"/>
            <pr n="SelectorSelectionColor" dt="3" pdv="-1"/>
            <pr n="SelectorSelectionFillStyle" dt="3" pdv="0"/>
            <pr n="SeriesFillStyle" dt="3" pdv="4"/>
            <pr n="SeriesFillColor" dt="3" pdv="16777215"/>
            <pr n="SeriesGradientColor" dt="3" pdv="16777215"/>
            <pr n="SeriesGradientAngle" dt="2" pdv="0"/>
            <pr n="SeriesGradientXOffset" dt="2" pdv="0"/>
            <pr n="SeriesGradientYOffset" dt="2" pdv="0"/>
            <pr n="SeriesPatternStyle" dt="3" pdv="1"/>
            <pr n="SeriesPatternColor" dt="3" pdv="8650752"/>
		  </prs>
	*/
	
	DSSFormatPropertySetMap* FormattingPatterns = new DSSFormatPropertySetMap();
	FormattingPatterns->SetPropertyIDType("FillColor", 1, 3, "16777215");
	FormattingPatterns->SetPropertyIDType("PatternColor", 2, 3, "8650752");
	FormattingPatterns->SetPropertyIDType("PatternStyle", 3, 3, "1");
	FormattingPatterns->SetPropertyIDType("FillStyle", 4, 3, "0");
	FormattingPatterns->SetPropertyIDType("ApplyToGraphThreshold", 5, 11, "0");
	FormattingPatterns->SetPropertyIDType("GradientColor", 6, 3, "16777215");
	FormattingPatterns->SetPropertyIDType("GradientAngle", 7, 2, "0");
	FormattingPatterns->SetPropertyIDType("GradientXOffset", 8, 2, "0");
	FormattingPatterns->SetPropertyIDType("GradientYOffset", 9, 2, "0");
   
	FormattingPatterns->SetPropertyIDType("SelectorSelectionColor", 10, 3, "-1");
    FormattingPatterns->SetPropertyIDType("SelectorSelectionFillStyle", 11, 3, "0");
    FormattingPatterns->SetPropertyIDType("SeriesFillStyle", 12, 3, "4");
    FormattingPatterns->SetPropertyIDType("SeriesFillColor", 13, 3, "16777215");
    FormattingPatterns->SetPropertyIDType("SeriesGradientColor", 14, 3, "16777215");
    FormattingPatterns->SetPropertyIDType("SeriesGradientAngle", 15, 2, "0");
    FormattingPatterns->SetPropertyIDType("SeriesGradientXOffset", 16, 2, "0");
    FormattingPatterns->SetPropertyIDType("SeriesGradientYOffset", 17, 2, "0");
    FormattingPatterns->SetPropertyIDType("SeriesPatternStyle", 18, 3, "1");
    FormattingPatterns->SetPropertyIDType("SeriesPatternColor", 19, 3, "8650752");
    FormattingPatterns->SetPropertyIDType("HighlightColor", 20, 3, "-1");
    FormattingPatterns->SetPropertyIDType("FillEffect", 21, 3, "0");
	FormattingPatterns->setName("FormattingPatterns");
	
	mFormatPropertySetsMap["FormattingPatterns"] = (FormattingPatterns);
	
	/*
		  <prs n="FormattingBanding">
			<pr n="Enabled" dt="11" pdv="0"/>
			<pr n="RowsColumns" dt="3" pdv="0"/>
			<pr n="Group1" dt="3" pdv="0"/>
			<pr n="Group2" dt="3" pdv="0"/>
			<pr n="ColorGroup1" dt="3" pdv="-1"/>
			<pr n="ColorGroup2" dt="3" pdv="13026246"/>
		  </prs>
	*/
	
	DSSFormatPropertySetMap* FormattingBanding = new DSSFormatPropertySetMap();
	FormattingBanding->SetPropertyIDType("Enabled", 1, 11, "0");
	FormattingBanding->SetPropertyIDType("RowsColumns", 2, 3, "0");
	FormattingBanding->SetPropertyIDType("Group1", 3, 3, "0");
	FormattingBanding->SetPropertyIDType("Group2", 4, 3, "0");
	FormattingBanding->SetPropertyIDType("ColorGroup1", 5, 3, "-1");
	FormattingBanding->SetPropertyIDType("ColorGroup2", 6, 3, "13026246");
	FormattingBanding->setName("FormattingBanding");
	
	mFormatPropertySetsMap["FormattingBanding"] = (FormattingBanding);
	
	/*
		<prs n="FormattingLine">
			<pr n="LineStyle" dt="3" pdv="1"/>
			<pr n="LineColor" dt="3" pdv="0"/>
			<pr n="LineOrientation" dt="3" pdv="0"/>
			<pr n="LineWeight" dt="3" pdv="1"/>
			<pr n="LineColor2" dt="3" pdv="0"/>
		  </prs>
	*/
	
	DSSFormatPropertySetMap* FormattingLine = new DSSFormatPropertySetMap();
	FormattingLine->SetPropertyIDType("LineStyle", 1, 3, "1");
	FormattingLine->SetPropertyIDType("LineColor", 2, 3, "0");
	FormattingLine->SetPropertyIDType("LineOrientation", 3, 3, "0");
	FormattingLine->SetPropertyIDType("LineWeight", 4, 3, "1");
	FormattingLine->SetPropertyIDType("LineColor2", 5, 3, "0");
	FormattingLine->setName("FormattingLine");
	
	mFormatPropertySetsMap["FormattingLine"] = (FormattingLine);
	
	/*
		<prs n="FormattingSize">
			<pr n="Width" dt="5" pdv="1"/>
			<pr n="Height" dt="5" pdv="0.5"/>
			<pr n="CanGrow" dt="11" pdv="-1"/>
			<pr n="CanShrink" dt="11" pdv="-1"/>
			<pr n="MaxHeight" dt="5" pdv="0"/>
			<pr n="HeightMode" dt="3" pdv="0"/>
			<pr n="WidthMode" dt="3" pdv="0"/>
			<pr n="AutoResizeInDesign" dt="11" pdv="-1"/>
			<pr n="LockAspectRatio" dt="11" pdv="-1"/>
			<pr n="CanGrowHorizontal" dt="11" pdv="-1"/>
			<pr n="CanShrinkHorizontal" dt="11" pdv="-1"/>
			<pr n="MaxWidth" dt="5" pdv="0"/>
			<pr n="OldWidth" dt="5" pdv="0"/>
			<pr n="OldHeight" dt="5" pdv="0"/>
			<pr n="LayoutXML" pri="16" dt="8" pdv=""/>	 
	 <PROPERTYDEF NAME="ImageSizeMode"           TYPE="integer"   DEFAULT="1"  GROUPS="Format" />
		  </prs>
	*/
	
	DSSFormatPropertySetMap* FormattingSize = new DSSFormatPropertySetMap();
	FormattingSize->SetPropertyIDType("Width", 1, 5, "1");
	FormattingSize->SetPropertyIDType("Height", 2, 5, "0.5");
	FormattingSize->SetPropertyIDType("CanGrow", 3, 11, "-1");
	FormattingSize->SetPropertyIDType("CanShrink", 4, 11, "-1");
	FormattingSize->SetPropertyIDType("MaxHeight", 5, 5, "0");
	FormattingSize->SetPropertyIDType("HeightMode", 6, 3, "0");
	FormattingSize->SetPropertyIDType("WidthMode", 7, 3, "0");
	FormattingSize->SetPropertyIDType("AutoResizeInDesign", 8, 11, "-1");
	FormattingSize->SetPropertyIDType("LockAspectRatio", 9, 11, "-1");
	FormattingSize->SetPropertyIDType("CanGrowHorizontal", 10, 11, "-1");
	FormattingSize->SetPropertyIDType("CanShrinkHorizontal", 11, 11, "-1");
	FormattingSize->SetPropertyIDType("MaxWidth", 12, 5, "0");
	FormattingSize->SetPropertyIDType("OldWidth", 13, 5, "0");
	FormattingSize->SetPropertyIDType("OldHeight", 14, 5, "0");
	FormattingSize->SetPropertyIDType("TextOverflow", 15, 11, "0");
	FormattingSize->SetPropertyIDType("LayoutXML", 16, 8, "");
	FormattingSize->SetPropertyIDType("ImageSizeMode", 17, 3, "1");
	FormattingSize->SetPropertyIDType("ImageWidth", 18, 5, "1");
	FormattingSize->SetPropertyIDType("ImageHeight", 19, 5, "0.5");
	FormattingSize->setName("FormattingSize");
	
	mFormatPropertySetsMap["FormattingSize"] = (FormattingSize);
	
	/*
		  <prs n="FormattingAppearance">
			<pr n="Name" dt="8" pdv=""/>
			<pr n="Visible" dt="11" pdv="-1"/>
			<pr n="HideDuplicates" dt="11" pdv="0"/>
			<pr n="SpecialEffect" dt="3" pdv="0"/>
			<pr n="Tooltip" dt="8" pdv=""/>
			<pr n="TooltipText" dt="8" pdv=""/>
			<pr n="HasRoundedCorners" dt="11" pdv="0"/>
			<pr n="DropShadowEffect" dt="3" pdv="0"/>
			<pr n="DropShadowDepth" dt="3" pdv="3"/>
			<pr n="LightDirection" dt="3" pdv="135"/>
			<pr n="FormatVersion" dt="3" pdv="0"/>
			<pr n="TopLeftCornerRadius" dt="3" pdv="5"/>
			<pr n="TopRightCornerRadius" dt="3" pdv="5"/>
			<pr n="BottomLeftCornerRadius" dt="3" pdv="5"/>
			<pr n="BottomRightCornerRadius" dt="3" pdv="5"/>
		  </prs>
	*/
	
	DSSFormatPropertySetMap* FormattingAppearance = new DSSFormatPropertySetMap();
	FormattingAppearance->SetPropertyIDType("Name", 1, 8, "");
	FormattingAppearance->SetPropertyIDType("Visible", 2, 11, "-1");
	FormattingAppearance->SetPropertyIDType("HideDuplicates", 3, 11, "0");
	FormattingAppearance->SetPropertyIDType("SpecialEffect", 4, 3, "0");
	FormattingAppearance->SetPropertyIDType("Tooltip", 5, 8, "");
	FormattingAppearance->SetPropertyIDType("TooltipText", 6, 8, "");
	FormattingAppearance->SetPropertyIDType("HasRoundedCorners", 7, 11, "0");
	FormattingAppearance->SetPropertyIDType("DropShadowEffect", 8, 3, "0");
	FormattingAppearance->SetPropertyIDType("DropShadowDepth", 9, 3, "3");
	FormattingAppearance->SetPropertyIDType("LightDirection", 10, 3, "135");
	FormattingAppearance->SetPropertyIDType("FormatVersion", 11, 3, "0");
	FormattingAppearance->SetPropertyIDType("TopLeftCornerRadius", 12, 3, "5");
	FormattingAppearance->SetPropertyIDType("TopRightCornerRadius", 13, 3, "5");
	FormattingAppearance->SetPropertyIDType("BottomLeftCornerRadius", 14, 3, "5");
	FormattingAppearance->SetPropertyIDType("BottomRightCornerRadius", 15, 3, "5");
    FormattingAppearance->SetPropertyIDType("IgnoreInExport", 16, 11, "0");
    FormattingAppearance->SetPropertyIDType("Docked", 17, 11, "1");
    FormattingAppearance->SetPropertyIDType("ShowOnlyInRepeatingCopies", 18, 11, "0");
    FormattingAppearance->SetPropertyIDType("ExpandAllPanels", 19, 3, "0");
    FormattingAppearance->SetPropertyIDType("RepeatPanelSpacing", 20, 5, "0.5");
    FormattingAppearance->SetPropertyIDType("SplitPanelAcrossPages", 21, 11, "1");
    FormattingAppearance->SetPropertyIDType("DisplayStyle", 22, 3, "0");
    FormattingAppearance->SetPropertyIDType("Transparency", 23, 3, "0");
    FormattingAppearance->SetPropertyIDType("Blur", 24, 11, "0");
	FormattingAppearance->setName("FormattingAppearance");
	
	mFormatPropertySetsMap["FormattingAppearance"] = (FormattingAppearance);
	
	/*
	<prs n="FormattingCrosstab">
	  <pr n="GridPosition" dt="3" pdv="1"/>
	  <pr n="GridAreaPercent" dt="3" pdv="50"/>
	  <pr n="OverflowMode" dt="3" pdv="0"/>
	  <pr n="ShowRowHeaders" dt="11" pdv="-1"/>
	  <pr n="ShowColHeaders" dt="11" pdv="-1"/>
	  <pr n="ExportOverflowMode" dt="3" pdv="0"/>
	  <pr n="Prefetch" dt="11" pdv="0"/>
	</prs>
	*/
	
	DSSFormatPropertySetMap* FormattingCrosstab = new DSSFormatPropertySetMap();
	FormattingCrosstab->SetPropertyIDType("GridPosition", 1, 3, "1");
	FormattingCrosstab->SetPropertyIDType("GridAreaPercent", 2, 3, "50");
	FormattingCrosstab->SetPropertyIDType("OverflowMode", 3, 3, "0");
	FormattingCrosstab->SetPropertyIDType("ShowRowHeaders", 4, 11, "-1");
	FormattingCrosstab->SetPropertyIDType("ShowColHeaders", 5, 11, "-1");
	FormattingCrosstab->SetPropertyIDType("ExportOverflowMode", 6, 3, "0");
	FormattingCrosstab->SetPropertyIDType("Prefetch", 7, 11, "0");
	FormattingCrosstab->SetPropertyIDType("ShowExportOptions", 8, 11, "0");
	FormattingCrosstab->setName("FormattingCrosstab");
	
	mFormatPropertySetsMap["FormattingCrosstab"] = (FormattingCrosstab);
	
	/*			
		<prs n="FormattingPadding">
		  <pr n="LeftPadding" dt="5" pdv="1"/>
		  <pr n="RightPadding" dt="5" pdv="1"/>
		  <pr n="TopPadding" dt="5" pdv="1"/>
		  <pr n="BottomPadding" dt="5" pdv="1"/>
		  <pr n="LineSpacing" dt="5" pdv="0"/>
		</prs>
	
	*/

	DSSFormatPropertySetMap* FormattingPadding = new DSSFormatPropertySetMap();
	FormattingPadding->SetPropertyIDType("LeftPadding", 1, 5, "1");
	FormattingPadding->SetPropertyIDType("RightPadding", 2, 5, "1");
	FormattingPadding->SetPropertyIDType("TopPadding", 3, 5, "1");
	FormattingPadding->SetPropertyIDType("BottomPadding", 4, 5, "1");
	FormattingPadding->SetPropertyIDType("LineSpacing", 5, 5, "0");
	FormattingPadding->setName("FormattingPadding");
	
	mFormatPropertySetsMap["FormattingPadding"] = (FormattingPadding);
	
	/*
		  <prs n="FormattingPicture">
			<pr n="PictureMode" dt="3" pdv="0"/>
			<pr n="PictureAlignment" dt="3" pdv="2"/>
		  </prs>
	*/
	
	DSSFormatPropertySetMap* FormattingPicture = new DSSFormatPropertySetMap();
	FormattingPicture->SetPropertyIDType("PictureMode", 1, 3, "0");
	FormattingPicture->SetPropertyIDType("PictureAlignment", 2, 3, "2");
    FormattingPicture->SetPropertyIDType("NormalURL", 3, 8, "");
    FormattingPicture->SetPropertyIDType("HighlightURL", 4, 8, "");
	FormattingPicture->setName("FormattingPicture");
	
	mFormatPropertySetsMap["FormattingPicture"] = (FormattingPicture);
	
	/*
		<prs n="FormattingOrder">
			<pr n="ZIndex" dt="3" pdv="1"/>
		  </prs>
	*/
	
	DSSFormatPropertySetMap* FormattingOrder = new DSSFormatPropertySetMap();
	FormattingOrder->SetPropertyIDType("ZIndex", 1, 3, "1");
	FormattingOrder->setName("FormattingOrder");
	
	mFormatPropertySetsMap["FormattingOrder"] = (FormattingOrder);
	
	/*
		<prs n="FormattingView">
			<pr n="ShowTitleBar" dt="11" pdv="0"/>
			<pr n="WindowState" dt="2" pdv="0"/>
			<pr n="Title" dt="8" pdv=""/>
			<pr n="TitleBarDisplay" dt="3" pdv="0"/>
		  </prs>
	*/
	
	DSSFormatPropertySetMap* FormattingView = new DSSFormatPropertySetMap();
	FormattingView->SetPropertyIDType("ShowTitleBar", 1, 11, "0");
	FormattingView->SetPropertyIDType("WindowState", 2, 2, "0");
	FormattingView->SetPropertyIDType("Title", 3, 8, "");
	FormattingView->SetPropertyIDType("TitleBarDisplay", 4, 3, "0");
	FormattingView->SetPropertyIDType("ShowWidgetTitleBar", 5, 11, "0");
	FormattingView->SetPropertyIDType("UseAsInformationWindow", 6, 11, "0");
	FormattingView->SetPropertyIDType("EnableHorizontalSwipeToChange", 7, 11, "0");
    FormattingView->SetPropertyIDType("StyleForSwitchingPanel", 8, 2, "0");
    FormattingView->SetPropertyIDType("InformationWindowPlacement", 9, 2, "1");
    FormattingView->SetPropertyIDType("InfoWindowMode", 10, 2, "1");
    FormattingView->SetPropertyIDType("InfoWindowPosition", 11, 2, "4");
    FormattingView->SetPropertyIDType("TabCloseToDismissInfoWindow", 12, 11, "0");
    FormattingView->SetPropertyIDType("CopyMode", 13, 2, "2");
    FormattingView->SetPropertyIDType("DisplayInPartialScreen", 14, 11, "0");
    FormattingView->SetPropertyIDType("ShowAsCollapsiblePanel", 15, 11,"0");
    FormattingView->SetPropertyIDType("InitialSlidingInWidth", 16, 5, "0");
    FormattingView->SetPropertyIDType("InitialSlidingInHeight", 17, 5, "0");
	FormattingView->setName("FormattingView");
	
	mFormatPropertySetsMap["FormattingView"] = (FormattingView);


	DSSFormatPropertySetMap* FormattingLayout = new DSSFormatPropertySetMap();
	FormattingLayout->SetPropertyIDType("UseMasterPageHeaderFooter", 1, 11, "-1");
	FormattingLayout->SetPropertyIDType("TabColor", 2, 3, "16777215");
	FormattingLayout->SetPropertyIDType("DefaultDatasetID", 3, 8, "");
	FormattingLayout->SetPropertyIDType("TabBackStyle", 4, 3, "1");
	FormattingLayout->SetPropertyIDType("SupportedOrientations", 5, 3, "3");
	FormattingLayout->SetPropertyIDType("Autowire", 6, 11, "0");
	FormattingLayout->SetPropertyIDType("FitPDFContentHeight", 7, 11, "0");
	FormattingLayout->SetPropertyIDType("DisplayPageByBar", 8, 11, "-1");
	FormattingLayout->SetPropertyIDType("DisplayRePromptIcon", 9, 11, "-1");
	FormattingLayout->SetPropertyIDType("UseAsInfoWindow", 10, 11, "0");
	FormattingLayout->SetPropertyIDType("UseDeviceWidthForDocLayoutWidth", 11, 11, "0");
	FormattingLayout->SetPropertyIDType("ChangeGroupByOnSwipe", 12, 11, "-1");
	FormattingLayout->SetPropertyIDType("UseAsFilterPanel", 13, 11, "0");
	FormattingLayout->SetPropertyIDType("UseAsMasterPanel", 14, 11, "0");
	FormattingLayout->SetPropertyIDType("UseSegments", 15, 11, "0");
    FormattingLayout->SetPropertyIDType("UseAsFilterSummaryPanel", 16, 11, "0");
    FormattingLayout->SetPropertyIDType("CtlAutoSubmit", 17, 11, "-1");
	FormattingLayout->SetPropertyIDType("FilterSummaryPanelShowTooltip", 18, 11, "-1");
	FormattingLayout->SetPropertyIDType("ResetToFirstPanelWhenTargeted", 19, 11, "0");
    
    FormattingLayout->SetPropertyIDType("UseAsNavBar", 20, 11);
    FormattingLayout->SetPropertyIDType("NavBarLayout", 21, 3);
    FormattingLayout->SetPropertyIDType("UseAsTableControl", 22, 11,"0");
    FormattingLayout->SetPropertyIDType("TableControlDefinition", 23, 8,"");
    
	FormattingLayout->setName("FormattingLayout");
	
	mFormatPropertySetsMap["FormattingLayout"] = (FormattingLayout);


	DSSFormatPropertySetMap* FormattingHTML = new DSSFormatPropertySetMap();
	FormattingHTML->SetPropertyIDType("HTMLType", 1, 2, "1");
	FormattingHTML->SetPropertyIDType("URL", 2, 8, "");
	FormattingHTML->SetPropertyIDType("HTMLText", 3, 8, "");
	FormattingHTML->setName("FormattingHTML");
	
	mFormatPropertySetsMap["FormattingHTML"] = (FormattingHTML);
	
	
	DSSFormatPropertySetMap* FilterDetailsFormatProperties = new DSSFormatPropertySetMap();
	FilterDetailsFormatProperties->SetPropertyIDType("FilterDetailsFormat", 1, 8, "");
	FilterDetailsFormatProperties->setName("FilterDetailsFormatProperties");
	
	mFormatPropertySetsMap["FilterDetailsFormatProperties"] = (FilterDetailsFormatProperties);
	
	
	/*
		<prs n="FormattingSelector">
			<pr n="Style" dt="2" pdv="0"/>
			<pr n="Multiselect" dt="11" pdv="0"/>
			<pr n="Orientation" dt="2" pdv="0"/>
			<pr n="AutoSubmit" dt="11" pdv="-1"/>
			<pr n="ItemWidthMode" dt="2" pdv="1"/>
			<pr n="MouseOverAction" dt="2" pdv="0"/>
			<pr n="ItemSpacing" dt="2" pdv="-1"/>
			<pr n="DisplaySelectorDocked" dt="11" pdv="0"/>   
			<pr n="Include" dt="11"  pdv="-1"/>   
			<pr n="MetricConditionType" dt="3"  pdv="0"/>
 			<pr n="DisplayText" dt="8" pdv=""/>
			<pr n="DisplayConfirmation" dt="11"  pdv="0"/>
			<pr n="SubmitMessage"  dt="8" pdv="Your transaction has been submitted."/>
		  </prs>
	*/
	
	DSSFormatPropertySetMap* FormattingSelector = new DSSFormatPropertySetMap();
	FormattingSelector->SetPropertyIDType("Style", 1, 2, "0");
	FormattingSelector->SetPropertyIDType("Multiselect", 2, 11, "0");
	FormattingSelector->SetPropertyIDType("Orientation", 3, 2, "0");
	FormattingSelector->SetPropertyIDType("AutoSubmit", 4, 11, "-1");
	FormattingSelector->SetPropertyIDType("ItemWidthMode", 5, 2, "1");
	FormattingSelector->SetPropertyIDType("MouseOverAction", 6, 2, "0");
	FormattingSelector->SetPropertyIDType("ItemSpacing", 7, 2, "-1");
	FormattingSelector->SetPropertyIDType("DisplaySelectorDocked", 8, 11, "0");
	FormattingSelector->SetPropertyIDType("Include", 9, 11, "-1");
	FormattingSelector->SetPropertyIDType("MetricConditionType", 10, 3, "0");
	FormattingSelector->SetPropertyIDType("DisplayText", 11, 8, "");
	FormattingSelector->SetPropertyIDType("DisplayConfirmation", 12, 11, "0");
	FormattingSelector->SetPropertyIDType("SubmitMessage", 13, 8, "Your transaction has been submitted.");
	FormattingSelector->SetPropertyIDType("ShowElementCount", 14, 11, "0");
	FormattingSelector->SetPropertyIDType("ShowSummary", 15, 11, "0");
    FormattingSelector->SetPropertyIDType("SearchOnServer", 16, 11, "-1");
    FormattingSelector->SetPropertyIDType("ShowSearchBox", 17, 2, "0");
    FormattingSelector->SetPropertyIDType("ClearDirectTargetSelectors", 18, 11, "0");
    FormattingSelector->SetPropertyIDType("ExportAsShownOnWeb", 19, 2, "0");
    FormattingSelector->SetPropertyIDType("MaxSelectionCount", 20, 2, "-1");
    FormattingSelector->SetPropertyIDType("MinSelectionCount", 21, 2, "-1");
    FormattingSelector->SetPropertyIDType("NoSelectionBehavior", 22, 2, "0");
    FormattingSelector->SetPropertyIDType("DisplaySummary", 23, 11, "0");
    FormattingSelector->SetPropertyIDType("InformationWindowTarget", 24, 8, "");
    FormattingSelector->SetPropertyIDType("ValidateElements", 25, 3, "0");
    FormattingSelector->SetPropertyIDType("ActionForDirectTargetSelectors", 26, 3, "0");
    FormattingSelector->SetPropertyIDType("OrientationBehavior", 27, 2, "0");
    FormattingSelector->SetPropertyIDType("IndenpendentNodeSelector", 28, 2, "0");
    FormattingSelector->SetPropertyIDType("OfflineSubmission", 29, 11, "1");
    FormattingSelector->SetPropertyIDType("TXNSubmitInOrder", 30, 11,"0");
    FormattingSelector->SetPropertyIDType("AutoPickBasedOnSourceTemplateControl", 31, 11, "0");
	FormattingSelector->setName("FormattingSelector");
	
	mFormatPropertySetsMap["FormattingSelector"] = (FormattingSelector);
	
	/*
	<prs n="Visualization">
	  <pr n="VisualizationsEnabled" dt="11" pdv="0"/>
	  <pr n="ViewMode" dt="3" pdv="0"/>
	  <pr n="VisualizationList" dt="8" pdv=""/>
	  <pr n="SelectedVisualization" dt="8" pdv=""/>
	  <pr n="FileName" dt="8" pdv=""/>
	  <pr n="CustomProps" dt="8" pdv=""/>
	  <pr n="VisProps" dt="8" pdv=""/>
	</prs>
	*/
	
	DSSFormatPropertySetMap* Visualization = new DSSFormatPropertySetMap();
	Visualization->SetPropertyIDType("VisualizationsEnabled", 1, 11, "0");
	Visualization->SetPropertyIDType("ViewMode", 2, 3, "0");
	Visualization->SetPropertyIDType("VisualizationList", 3, 8, "");
	Visualization->SetPropertyIDType("SelectedVisualization", 4, 8, "");
	Visualization->SetPropertyIDType("FileName", 5, 8, "");
	Visualization->SetPropertyIDType("CustomProps", 6, 8, "");
	Visualization->SetPropertyIDType("VisProps", 7, 8, "");
	Visualization->SetPropertyIDType("IncludeMDProperties", 8, 2, "0");
	Visualization->SetPropertyIDType("IPhoneVisualization", 9, 8, "");
	Visualization->SetPropertyIDType("IPadVisualization", 10, 8, "");
	Visualization->SetPropertyIDType("APhoneVisualization", 11, 8, "");
	Visualization->SetPropertyIDType("ATabletVisualization", 12, 8, "");
	Visualization->setName("Visualization");
	
	mFormatPropertySetsMap["Visualization"] = (Visualization);
	
	/*
	<prs n="FormattingTransitions">
	  <pr n="Enable" dt="11" pdv="0"/>
	  <pr n="Transition" dt="8" pdv=""/>
	  <pr n="Duration" dt="3" pdv="-1"/>
	</prs>
	*/
	
	DSSFormatPropertySetMap* FormattingTransitions = new DSSFormatPropertySetMap();
	FormattingTransitions->SetPropertyIDType("Enable", 1, 11, "0");
	FormattingTransitions->SetPropertyIDType("Transition", 2, 8, "");
	FormattingTransitions->SetPropertyIDType("Duration", 3, 3, "-1");
	FormattingTransitions->setName("FormattingTransitions");
	
	mFormatPropertySetsMap["FormattingTransitions"] = (FormattingTransitions);
	
	/*
		<prs n="FormattingPage">
			<pr n="PageHeight" dt="5" pdv="11"/>
			<pr n="PageWidth" dt="5" pdv="8.5"/>
			<pr n="Orientation" dt="2" pdv="0"/>
			<pr n="PaperType" dt="3" pdv="1"/>
			<pr n="MarginTop" dt="5" pdv="0.75"/>
			<pr n="MarginBottom" dt="5" pdv="0.75"/>
			<pr n="MarginLeft" dt="5" pdv="0.75"/>
			<pr n="MarginRight" dt="5" pdv="0.75"/>
			<pr n="Scaling" dt="2" pdv="0"/>
			<pr n="ScalePercentage" dt="2" pdv="100"/>
			<pr n="ScalePagesWide" dt="2" pdv="1"/>
			<pr n="ScalePagesTall" dt="2" pdv="1"/>
			<pr n="BlackAndWhite" dt="11" pdv="0"/>
			<pr n="PrintHeaderOn" dt="3" pdv="0"/>
			<pr n="PrintFooterOn" dt="3" pdv="0"/>
			<pr n="ExpandToFit" dt="2" pdv="0"/>
			<pr n="SectionHorizontalOverflow" dt="2" pdv="0"/>
			<pr n="ScalePageHeaderFooter" dt="11" pdv="0"/>
			<pr n="DefaultGridStyle" dt="8" pdv=""/>
			<pr n="GraphQualityDPI" dt="3" pdv="288"/>
			<pr n="GraphFormat" dt="3" pdv="0"/>
			<pr n="IncludeBookmarks" dt="11" pdv="-1"/>
			<pr n="EmbedFonts" dt="11" pdv="0"/>
			<pr n="EditableViewMode" dt="11" pdv="0"/>
			<pr n="ShowBookmarks" dt="11" pdv="-1"/>
			<pr n="UsePageWidthAsLayoutWidth" dt="11" pdv="-1"/>
			<pr n="DefaultViewMode" dt="3" pdv="-1"/>
			<pr n="AvailableViewModes" dt="3" pdv="-1"/>
		  </prs>
	*/
	
	DSSFormatPropertySetMap* FormattingPage = new DSSFormatPropertySetMap();
	FormattingPage->SetPropertyIDType("PageHeight", 1, 5, "11");
	FormattingPage->SetPropertyIDType("PageWidth", 2, 5, "8.5");
	FormattingPage->SetPropertyIDType("Orientation", 3, 2, "0");
	FormattingPage->SetPropertyIDType("PaperType", 4, 3, "1");
	FormattingPage->SetPropertyIDType("MarginTop", 5, 5, "0.75");
	FormattingPage->SetPropertyIDType("MarginBottom", 6, 5, "0.75");
	FormattingPage->SetPropertyIDType("MarginLeft", 7, 5, "0.75");
	FormattingPage->SetPropertyIDType("MarginRight", 8, 5, "0.75");
	FormattingPage->SetPropertyIDType("Scaling", 9, 2, "0");
	FormattingPage->SetPropertyIDType("ScalePercentage", 10, 2, "100");
	FormattingPage->SetPropertyIDType("ScalePagesWide", 11, 2, "1");
	FormattingPage->SetPropertyIDType("ScalePagesTall", 12, 2, "1");
	FormattingPage->SetPropertyIDType("BlackAndWhite", 13, 11, "0");
	FormattingPage->SetPropertyIDType("PrintHeaderOn", 14, 3, "0");
	FormattingPage->SetPropertyIDType("PrintFooterOn", 15, 3, "0");
	FormattingPage->SetPropertyIDType("ExpandToFit", 16, 2, "0");
	FormattingPage->SetPropertyIDType("SectionHorizontalOverflow", 17, 2, "0");
	FormattingPage->SetPropertyIDType("ScalePageHeaderFooter", 18, 11, "0");
	FormattingPage->SetPropertyIDType("DefaultGridStyle", 19, 8, "");
	FormattingPage->SetPropertyIDType("GraphQualityDPI", 20, 3, "288");
	FormattingPage->SetPropertyIDType("GraphFormat", 21, 3, "0");
	FormattingPage->SetPropertyIDType("IncludeBookmarks", 22, 11, "-1");
	FormattingPage->SetPropertyIDType("EmbedFonts", 23, 11, "0");
	FormattingPage->SetPropertyIDType("EditableViewMode", 24, 11, "0");
	FormattingPage->SetPropertyIDType("ShowBookmarks", 25, 11, "-1");
	FormattingPage->SetPropertyIDType("UsePageWidthAsLayoutWidth", 26, 11, "-1");
	FormattingPage->SetPropertyIDType("DefaultViewMode", 27, 3, "-1");
	FormattingPage->SetPropertyIDType("AvailableViewModes", 28, 3, "-1");
	FormattingPage->setName("FormattingPage");
	
	mFormatPropertySetsMap["FormattingPage"] = (FormattingPage);
	
	/*
	<prs n="FormattingNavigation">
	  <pr n="IsHyperlink" dt="11" pdv="0"/>
	  <pr n="Hyperlink" dt="8" pdv=""/>
	  <pr n="OpenInNewWindow" dt="11" pdv="-1"/>
	</prs>
	*/
	
	DSSFormatPropertySetMap* FormattingNavigation = new DSSFormatPropertySetMap();
	FormattingNavigation->SetPropertyIDType("IsHyperlink", 1, 11, "0");
	FormattingNavigation->SetPropertyIDType("Hyperlink", 2, 8, "");
	FormattingNavigation->SetPropertyIDType("OpenInNewWindow", 3, 11, "-1");
	FormattingNavigation->SetPropertyIDType("Hyperlinks", 4,	8, "");
	FormattingNavigation->SetPropertyIDType("ResolveHyperlinks",	5, 8, "");
	FormattingNavigation->setName("FormattingNavigation");
	
	mFormatPropertySetsMap["FormattingNavigation"] = (FormattingNavigation);
	
	
	DSSFormatPropertySetMap* WatermarkProperties = new DSSFormatPropertySetMap();
	WatermarkProperties->SetPropertyIDType("WatermarkType", 1, 3, "0");
	WatermarkProperties->SetPropertyIDType("Text", 2, 8, "");
	WatermarkProperties->SetPropertyIDType("FontName", 3, 8, "Arial");
	WatermarkProperties->SetPropertyIDType("FontBold", 4, 11, "0");
	WatermarkProperties->SetPropertyIDType("FontItalic",	5, 11, "0");
	WatermarkProperties->SetPropertyIDType("FontSize", 6, 3,	"10");
	WatermarkProperties->SetPropertyIDType("FontStrikeout", 7, 11, "0");
	WatermarkProperties->SetPropertyIDType("FontUnderline", 8, 11, "0");
	WatermarkProperties->SetPropertyIDType("FontColor", 9, 3, "0");
	WatermarkProperties->SetPropertyIDType("FontScript",	10,	3, "0");
	WatermarkProperties->SetPropertyIDType("AutoFontSize", 11, 11, "-1");
	WatermarkProperties->SetPropertyIDType("TextWashout", 12, 11, "-1");
	WatermarkProperties->SetPropertyIDType("TextOrientation", 13, 3, "0");
	WatermarkProperties->SetPropertyIDType("ImageSource", 14, 8, "");
	WatermarkProperties->SetPropertyIDType("ImageScale",	15, 3, "-1");
	WatermarkProperties->setName("WatermarkProperties");
	
	mFormatPropertySetsMap["WatermarkProperties"] = (WatermarkProperties);

	
	DSSFormatPropertySetMap* FormattingIncrementalFetch = new DSSFormatPropertySetMap();
	FormattingIncrementalFetch->SetPropertyIDType("EnableIncrementalFetch", 1, 11, "0");
	FormattingIncrementalFetch->SetPropertyIDType("MaxElements",	2, 3, "25");
	FormattingIncrementalFetch->SetPropertyIDType("EnableIFOnGrid", 3, 11, "0");
	FormattingIncrementalFetch->SetPropertyIDType("MaxRows",	4, 2, "50");
	FormattingIncrementalFetch->SetPropertyIDType("MaxColumns", 5, 2, "10");
	FormattingIncrementalFetch->SetPropertyIDType("EnableIFOnGraph",	6, 11, "0");
	FormattingIncrementalFetch->SetPropertyIDType("PreLoadAllPanels", 7,	2, "2");
	FormattingIncrementalFetch->SetPropertyIDType("EnableIFOnGridColumns", 8, 11, "0");
	FormattingIncrementalFetch->setName("FormattingIncrementalFetch");
	
	mFormatPropertySetsMap["FormattingIncrementalFetch"] = (FormattingIncrementalFetch);
	
	/*
		  <prs n="FormattingPosition">
			<pr n="Top" dt="5" pdv="0"/>
			<pr n="Left" dt="5" pdv="0"/>
		  </prs>
	*/
	
	DSSFormatPropertySetMap* FormattingPosition = new DSSFormatPropertySetMap();
	FormattingPosition->SetPropertyIDType("Top", 1, 5, "0");
	FormattingPosition->SetPropertyIDType("Left", 2, 5, "0");
	FormattingPosition->SetPropertyIDType("Locked", 3, 11, "0");
	FormattingPosition->setName("FormattingPosition");
	
	mFormatPropertySetsMap["FormattingPosition"] = (FormattingPosition);
	
	/*
	<prs n="FormattingSection">
	  <pr n="RepeatSection" dt="11" pdv="0"/>
	  <pr n="ForceNewPage" dt="3" pdv="0"/>
	  <pr n="NewRowOrColumn" dt="3" pdv="0"/>
	  <pr n="KeepSectionTogether" dt="11" pdv="0"/>
	  <pr n="Expanded" dt="11" pdv="-1"/>
	  <pr n="Caption" dt="8" pdv=""/>
	  <pr n="RepeatHorizontally" dt="11" pdv="0"/>
	  <pr n="ShowInHTMLViewMode" dt="11" pdv="-1"/>
	  <pr n="HideIfEmpty" dt="11" pdv="0"/>
	</prs>
	*/
	
	DSSFormatPropertySetMap* FormattingSection = new DSSFormatPropertySetMap();
	FormattingSection->SetPropertyIDType("RepeatSection", 1, 11, "0");
	FormattingSection->SetPropertyIDType("ForceNewPage", 2, 3, "0");
	FormattingSection->SetPropertyIDType("NewRowOrColumn", 3, 3, "0");
	FormattingSection->SetPropertyIDType("KeepSectionTogether", 4, 11, "0");
	FormattingSection->SetPropertyIDType("Expanded", 5, 11, "-1");
	FormattingSection->SetPropertyIDType("Caption", 6, 8, "");
	FormattingSection->SetPropertyIDType("RepeatHorizontally", 7, 11, "0");
	FormattingSection->SetPropertyIDType("ShowInHTMLViewMode", 8, 11, "-1");
	FormattingSection->SetPropertyIDType("HideIfEmpty", 9, 11, "0");
	FormattingSection->SetPropertyIDType("FitExcelRowHeight", 10, 11, "0");
    FormattingSection->SetPropertyIDType("DockSection", 11, 11, "0");
	FormattingSection->setName("FormattingSection");
	
	mFormatPropertySetsMap["FormattingSection"] = (FormattingSection);
	
	/*
	<prs n="FormattingGroup">
	  <pr n="KeepGroupTogether" dt="3" pdv="0"/>
	  <pr n="RestartPageNumbering" dt="11" pdv="0"/>
	  <pr n="PageBreakBetweenGroups" dt="11" pdv="0"/>
	  <pr n="IsDataHorizontal" dt="11" pdv="0"/>
	  <pr n="IsChildrenHorizontal" dt="11" pdv="0"/>
	</prs>
	*/
	
	DSSFormatPropertySetMap* FormattingGroup = new DSSFormatPropertySetMap();
	FormattingGroup->SetPropertyIDType("KeepGroupTogether", 1, 3, "0");
	FormattingGroup->SetPropertyIDType("RestartPageNumbering", 2, 11, "0");
	FormattingGroup->SetPropertyIDType("PageBreakBetweenGroups", 3, 11, "0");
	FormattingGroup->SetPropertyIDType("IsDataHorizontal", 4, 11, "0");
	FormattingGroup->SetPropertyIDType("IsChildrenHorizontal", 5, 11, "0");
	FormattingGroup->SetPropertyIDType("UseOneSheetPerPage", 6, 11, "0");
	FormattingGroup->setName("FormattingGroup");
	
	mFormatPropertySetsMap["FormattingGroup"] = (FormattingGroup);

	DSSFormatPropertySetMap* RWDocumentProperties = new DSSFormatPropertySetMap();
	RWDocumentProperties->SetPropertyIDType("ZoomFactor", 1, 5, "1");
	RWDocumentProperties->SetPropertyIDType("DefaultPageByExportMode", 2, 3, "0");
	RWDocumentProperties->SetPropertyIDType("PromptUserForPagebyExport", 3, 11, "-1");
	RWDocumentProperties->SetPropertyIDType("AllowToggleThresholds", 4, 3, "0");
	RWDocumentProperties->SetPropertyIDType("ExecutionFullScreenMode", 5, 11, "0");
	RWDocumentProperties->SetPropertyIDType("CtlAutoSubmit", 6, 11, "-1");
	RWDocumentProperties->SetPropertyIDType("DefaultsVersion", 7, 3, "0");
	RWDocumentProperties->SetPropertyIDType("EmbeddedFonts", 8, 8, "");
	RWDocumentProperties->SetPropertyIDType("CalculateDocWidth", 9, 11, "-1");
	RWDocumentProperties->SetPropertyIDType("ShowFloatingToolbars", 10, 11, "-1");
	RWDocumentProperties->SetPropertyIDType("EnableDocumentCaching", 11, 3, "-2147483648");
	RWDocumentProperties->SetPropertyIDType("ExportRange", 12, 3, "0");
	RWDocumentProperties->SetPropertyIDType("ExcelEmbedImages", 13, 11, "0");
	RWDocumentProperties->SetPropertyIDType("ExcelGraphFormat", 14, 3, "0");
	RWDocumentProperties->SetPropertyIDType("CreateDocumentCache", 15, 11, "0");
	RWDocumentProperties->SetPropertyIDType("ZoomType", 16, 3, "0");
	RWDocumentProperties->SetPropertyIDType("ShowTabsForSingleLayout", 17, 11, "0");
	RWDocumentProperties->SetPropertyIDType("PreLoadAllPanels", 18, 3, "0");
	RWDocumentProperties->SetPropertyIDType("SelectorBehavior", 19, 3, "0");
	RWDocumentProperties->SetPropertyIDType("layoutBarStyle", 20, 3, "0");
	RWDocumentProperties->SetPropertyIDType("layoutBarPosition", 21, 3, "1");
	RWDocumentProperties->SetPropertyIDType("ViewModeInteractivityLevel", 22, 3, "1");
	RWDocumentProperties->SetPropertyIDType("enableGraphTooltips", 23, 11, "0");
	RWDocumentProperties->SetPropertyIDType("SaveExportResultForRWD", 24, 3, "1");
	RWDocumentProperties->SetPropertyIDType("ShowHiddenObjects", 25, 3, "1");
	RWDocumentProperties->SetPropertyIDType("AllowMobileTemporaryCaching", 26, 11, "-1");
	RWDocumentProperties->SetPropertyIDType("ExcelEnableShapes", 27, 11, "0");
	RWDocumentProperties->SetPropertyIDType("ApplyGBToAllLayouts", 28, 3, "0");
	RWDocumentProperties->SetPropertyIDType("EnablePartialCSS", 29, 11, "-1");
	RWDocumentProperties->SetPropertyIDType("LayoutTabPosition", 30, 3, "0");
	RWDocumentProperties->SetPropertyIDType("IsNavigationDocument", 31, 11, "0");
    RWDocumentProperties->SetPropertyIDType("ShowingBarsTemporarily", 32, 11, "0");//793295
	RWDocumentProperties->setName("RWDocumentProperties");
	
	mFormatPropertySetsMap["RWDocumentProperties"] = (RWDocumentProperties);

	/*
	<prs n="FormattingWidget">
	  <pr n="RSLPath" dt="8" pdv=""/>
	  <pr n="ClassName" dt="8" pdv=""/>
	  <pr n="Enable" dt="11" pdv="0"/>
	  <pr n="WidgetProps" dt="8" pdv=""/>
	  <pr n="NonFlashRender" dt="3" pdv="0"/>
	  <pr n="IsApp" dt="11" pdv="0"/>
	  <pr n="SecondaryDataProviders" dt="8" pdv=""/>
	</prs>
	*/
	
	DSSFormatPropertySetMap* FormattingWidget = new DSSFormatPropertySetMap();
	FormattingWidget->SetPropertyIDType("RSLPath", 1, 8, "");
	FormattingWidget->SetPropertyIDType("ClassName", 2, 8, "");
	FormattingWidget->SetPropertyIDType("Enable", 3, 11, "0");
	FormattingWidget->SetPropertyIDType("WidgetProps", 4, 8, "");
	FormattingWidget->SetPropertyIDType("NonFlashRender", 5, 3, "0");
	FormattingWidget->SetPropertyIDType("IsApp", 6, 11, "0");
	FormattingWidget->SetPropertyIDType("SecondaryDataProviders", 7, 8, "");
	FormattingWidget->SetPropertyIDType("DhtmlRenderer", 8, 3, "0");
	FormattingWidget->setName("FormattingWidget");
	
	mFormatPropertySetsMap["FormattingWidget"] = (FormattingWidget);
	
	DSSFormatPropertySetMap* FlashCompatibility = new DSSFormatPropertySetMap();
	FlashCompatibility->SetPropertyIDType("CanStagger", 1, 3, "1");
	FlashCompatibility->SetPropertyIDType("CanDropLabelsCategory", 2, 3, "0");
	FlashCompatibility->SetPropertyIDType("CanDropLabelsLinear", 3, 3, "1");
	FlashCompatibility->SetPropertyIDType("AddButtonBarPadding", 4, 3, "0");
	FlashCompatibility->SetPropertyIDType("ShowGaugeLabel", 5, 3, "0");
	FlashCompatibility->SetPropertyIDType("UseGaugeDefaultSeriesColors", 6, 3, "0");
	FlashCompatibility->SetPropertyIDType("MaxPieLabelRadius", 7, 5, "0.3");
	FlashCompatibility->SetPropertyIDType("SliderAutomaticStyle", 8, 3, "0");
	FlashCompatibility->SetPropertyIDType("MultiSliderAlignment", 9, 3, "0");
	FlashCompatibility->SetPropertyIDType("SingleSliderAlignment", 10, 3, "0");
	FlashCompatibility->SetPropertyIDType("EnableGridPadding", 11, 3, "0");
	FlashCompatibility->SetPropertyIDType("SliderFitToContents", 12, 3, "1");
	FlashCompatibility->setName("FlashCompatibility");
				
	mFormatPropertySetsMap["FlashCompatibility"] = (FlashCompatibility);
	
	DSSFormatPropertySetMap* FormattingAnalysis = new DSSFormatPropertySetMap();
	FormattingAnalysis->SetPropertyIDType("AnalysisSection", 1, 3, "1");
	FormattingAnalysis->SetPropertyIDType("Collapsed", 2, 11, "0");
	FormattingAnalysis->SetPropertyIDType("DatasetObjectsVisible", 3, 11, "0");
	FormattingAnalysis->SetPropertyIDType("DatasetObjectsWidth", 4, 5, "2");
	FormattingAnalysis->SetPropertyIDType("DropZonesVisible", 5, 11, "0");
	FormattingAnalysis->SetPropertyIDType("DropZonesWidth", 6, 5, "2");
	FormattingAnalysis->SetPropertyIDType("FilterDisplay", 7, 3, "1");
	FormattingAnalysis->SetPropertyIDType("FilterMode", 8, 3, "1");
    FormattingAnalysis->SetPropertyIDType("Theme", 9, 3,"0");
	
	mFormatPropertySetsMap["FormattingAnalysis"] = (FormattingAnalysis);
	
	DSSFormatPropertySetMap* FormattingTransactions = new DSSFormatPropertySetMap();
	FormattingTransactions->SetPropertyIDType("InputControlStyle", 1, 3, "-1");
	FormattingTransactions->SetPropertyIDType("InputControlSetting", 2, 8, "");
	FormattingTransactions->SetPropertyIDType("ShowChangeIndication", 3, 11, "-1");
    FormattingTransactions->SetPropertyIDType("HasMetricForBadge", 4, 11, "-1");
    FormattingTransactions->SetPropertyIDType("SetBadgeToValueOnField", 5, 11, "-1");
	mFormatPropertySetsMap["FormattingTransactions"] = (FormattingTransactions);
    
    DSSFormatPropertySetMap* Mobile = new DSSFormatPropertySetMap();
    Mobile->SetPropertyIDType("View", 1, 3, "1");
    Mobile->SetPropertyIDType("FontSize", 2, 3, "-1");
    Mobile->SetPropertyIDType("ShowTitle", 3, 3, "-1");
    Mobile->SetPropertyIDType("ShowCellInfo", 4, 3, "-1");
    Mobile->SetPropertyIDType("HiddenColumns", 5, 8, "");
    Mobile->SetPropertyIDType("ColumnFitMode", 6, 3, "1");
    Mobile->SetPropertyIDType("CustomWidths", 7, 8, "");
    Mobile->SetPropertyIDType("LockedColumns", 8, 8, "");
    Mobile->SetPropertyIDType("AllowZoom", 9, 11, "-1");
    Mobile->SetPropertyIDType("EnableBookStylePagebyNavigation", 10, 11, "0");
    Mobile->SetPropertyIDType("EnableZoom", 11, 11, "0");
    Mobile->SetPropertyIDType("RefreshFrequency", 12, 3, "-1");
    Mobile->SetPropertyIDType("EnableCopyForText", 13,11, "0");
    Mobile->SetPropertyIDType("EnableCopyForGridCell", 14,11, "0");
    Mobile->SetPropertyIDType("EnableExport", 15, 11, "0");
    Mobile->SetPropertyIDType("DocumentThemeColor", 16,3, "0");
	Mobile->SetPropertyIDType("EnableSwipingToSwitchLayout", 17, 11, "-1");
    Mobile->setName("Mobile");
    mFormatPropertySetsMap["Mobile"] = (Mobile);

    	DSSFormatPropertySetMap* GeneralReportProperties = new DSSFormatPropertySetMap();
	GeneralReportProperties->SetPropertyIDType("CanBeScheduled", 1, 11, "-1");
	GeneralReportProperties->SetPropertyIDType("EnableReportCaching", 2, 3, "1");
	GeneralReportProperties->SetPropertyIDType("AllowAllObjectsBrowsing", 3, 11, "-1");
	GeneralReportProperties->SetPropertyIDType("StartBrowsingFolderID", 4, 8, "");
	GeneralReportProperties->SetPropertyIDType("LoadIntelligentCubeImmediately", 5, 11, "-1");
	GeneralReportProperties->SetPropertyIDType("SupportedNodes", 6, 8, "All");
	GeneralReportProperties->SetPropertyIDType("LoadIntelligentCubesImmediately", 7, 3, "-1");
	GeneralReportProperties->SetPropertyIDType("AllowMobileTemporaryCaching", 8, 11, "-1");
	GeneralReportProperties->setName("GeneralReportProperties");
	mFormatPropertySetsMap["GeneralReportProperties"] = (GeneralReportProperties);
    
    DSSFormatPropertySetMap* GraphInfo = new DSSFormatPropertySetMap();
    GraphInfo->SetPropertyIDType("BinaryDefinition", 1, 13, "");
    GraphInfo->SetPropertyIDType("ResizingMode", 2, 3, "2");
    GraphInfo->SetPropertyIDType("MaxSeries", 3, 3, "10");
    GraphInfo->SetPropertyIDType("MaxCategories", 4, 3, "50");
    GraphInfo->SetPropertyIDType("SaveDimensionMode", 5, 3, "1");
    GraphInfo->SetPropertyIDType("IsChartAvailable", 6, 11, "-1");
    GraphInfo->SetPropertyIDType("GraphTypesAvailable", 7, 3, "-1");
    GraphInfo->SetPropertyIDType("Height", 8, 3, "400");
    GraphInfo->SetPropertyIDType("Width", 9, 3, "700");
    GraphInfo->SetPropertyIDType("GraphType", 10, 3, "14");
    GraphInfo->SetPropertyIDType("SeriesByCols", 11, 11, "-1");
    GraphInfo->SetPropertyIDType("XRatio", 12, 3, "7");
    GraphInfo->SetPropertyIDType("YRatio", 13, 3, "4");
    GraphInfo->SetPropertyIDType("SuppressLastLevel", 14, 11, "0");
    GraphInfo->SetPropertyIDType("GraphFormat", 15, 3, "1");
    GraphInfo->SetPropertyIDType("ShowStyleSheet", 16, 11, "-1");
    GraphInfo->SetPropertyIDType("IncludeSubtotal", 17, 11, "0");
    GraphInfo->SetPropertyIDType("TitleDisplay", 18, 11, "-1");
    GraphInfo->SetPropertyIDType("LegendDisplay", 19, 11, "-1");
    GraphInfo->SetPropertyIDType("SubtitleDisplay", 20, 11, "0");
    GraphInfo->SetPropertyIDType("FootnoteDisplay", 21, 11, "0");
    GraphInfo->SetPropertyIDType("DatalabelDisplay", 22, 11, "0");
    GraphInfo->SetPropertyIDType("DepthEffect", 23, 11, "0");
    GraphInfo->SetPropertyIDType("ChartBackColor", 24, 3, "16777215");
    GraphInfo->SetPropertyIDType("TypesApplyTo", 25, 3, "0");
    GraphInfo->SetPropertyIDType("RecalculateChartFrame", 26, 11, "-1");
    GraphInfo->SetPropertyIDType("RecalculateLegend", 27, 11, "-1");
    GraphInfo->SetPropertyIDType("DesktopResizing", 28, 3, "0");
    GraphInfo->SetPropertyIDType("FixedWidth", 29, 3, "7000");
    GraphInfo->SetPropertyIDType("FixedHeight", 30, 3, "4000");
    GraphInfo->SetPropertyIDType("CustomPalette", 31, 8, "");
    GraphInfo->SetPropertyIDType("ConcatenateLabels", 32, 11, "0");
    GraphInfo->SetPropertyIDType("AutosizeHorizonal", 33, 11, "0");
    GraphInfo->SetPropertyIDType("AutosizeVertical", 34, 11, "0");
    GraphInfo->SetPropertyIDType("UseDefaultFont", 35, 11, "0");
    GraphInfo->SetPropertyIDType("DefaultFontName", 36, 8, "");
    GraphInfo->SetPropertyIDType("DefaultFontCharset", 37, 3, "");
    GraphInfo->SetPropertyIDType("NestedLabel", 38, 11, "0");
    GraphInfo->SetPropertyIDType("NestedLabelBracket", 39, 11, "0");
    GraphInfo->SetPropertyIDType("LayoutMode", 40, 3, "2");
    GraphInfo->SetPropertyIDType("EnableTooltips", 41, 11, "-1");
    GraphInfo->SetPropertyIDType("UseCurvedLines", 42, 11, "-1");
    GraphInfo->SetPropertyIDType("UseFontAntialiasing", 43, 3, "1");
    GraphInfo->SetPropertyIDType("EnhancedSeriesEffect", 44, 3, "0");
    GraphInfo->SetPropertyIDType("FlashSeriesEffect", 45, 3, "0");
    GraphInfo->SetPropertyIDType("RepeatGraph", 46, 3, "0");
    GraphInfo->SetPropertyIDType("RepeatGraphSpacing", 47, 5, "0.5");
    GraphInfo->SetPropertyIDType("UseMaxCategoriesAsMin", 48, 11, "0");
    GraphInfo->SetPropertyIDType("UseZerosForGraphNodes", 49, 3, "0");
    GraphInfo->SetPropertyIDType("EnhancedSeriesEffectStyles", 50, 3, "1");
    GraphInfo->SetPropertyIDType("ApplyMetricFormatting", 51, 11, "-1");
    
    mFormatPropertySetsMap["GraphInfo"] = (GraphInfo);
    
    DSSFormatPropertySetMap* TemplateFormatting = new DSSFormatPropertySetMap();
        
    TemplateFormatting->SetPropertyIDType("SelectedAutostyle", 1, 8, "(Custom)");
    TemplateFormatting->SetPropertyIDType("ApplyMetricProperties", 2, 8, "0000001");
    TemplateFormatting->SetPropertyIDType("Banding", 3, 8, "&lt;EN&gt;0&lt;/EN&gt;&lt;IH&gt;1&lt;/IH&gt;&lt;BR&gt;1&lt;/BR&gt;&lt;C1&gt;-1&lt;/C1&gt;&lt;C2&gt;13026246&lt;/C2&gt;&lt;BH&gt;0&lt;/BH&gt;&lt;G1&gt;1&lt;/G1&gt;&lt;G2&gt;1&lt;/G2&gt;&lt;RH&gt;&lt;/RH&gt;&lt;CH&gt;&lt;/CH&gt;&lt;IC1&gt;1&lt;/IC1&gt;&lt;IC2&gt;0&lt;/IC2&gt;");
    TemplateFormatting->SetPropertyIDType("PagesVisible", 4, 2, "0");
    TemplateFormatting->SetPropertyIDType("Indented", 5, 2, "0");
    TemplateFormatting->SetPropertyIDType("LockRowHeaders", 6, 2, "0");
    TemplateFormatting->SetPropertyIDType("LockColumnHeaders", 7, 2, "0");
    TemplateFormatting->SetPropertyIDType("LongNames", 8, 2, "0");
    TemplateFormatting->SetPropertyIDType("MergeCells", 9, 2, "0");
    TemplateFormatting->SetPropertyIDType("CustomAutostyle", 10, 8, "&lt;NA&gt;(Custom)&lt;/NA&gt;&lt;VS&gt;&lt;TF&gt;&lt;NA&gt;Arial&lt;/NA&gt;&lt;SI&gt;10&lt;/SI&gt;&lt;FL&gt;1000&lt;/FL&gt;&lt;CO&gt;0&lt;/CO&gt;&lt;CS&gt;0&lt;/CS&gt;&lt;/TF&gt;&lt;TA&gt;1&lt;/TA&gt;&lt;DL&gt;&lt;EST&gt;0&lt;/EST&gt;&lt;ESB&gt;0&lt;/ESB&gt;&lt;LHB&gt;10+N0+N0+N0+N0+&lt;/LHB&gt;&lt;LHF&gt;&lt;NA&gt;Arial&lt;/NA&gt;&lt;SI&gt;10&lt;/SI&gt;&lt;FL&gt;1000&lt;/FL&gt;&lt;CO&gt;16777215&lt;/CO&gt;&lt;CS&gt;0&lt;/CS&gt;&lt;/LHF&gt;&lt;LHA&gt;220&lt;/LHA&gt;&lt;LBH&gt;1&lt;/LBH&gt;&lt;LBV&gt;2&lt;/LBV&gt;&lt;SRBC&gt;0&lt;/SRBC&gt;&lt;SRBA&gt;0&lt;/SRBA&gt;&lt;SHBC&gt;0&lt;/SHBC&gt;&lt;SHBA&gt;0&lt;/SHBA&gt;&lt;/DL&gt;&lt;TRBC&gt;0&lt;/TRBC&gt;&lt;TRBA&gt;0&lt;/TRBA&gt;&lt;THB&gt;10+N0+N0+N0+N0+&lt;/THB&gt;&lt;THA&gt;240&lt;/THA&gt;&lt;THBC&gt;0&lt;/THBC&gt;&lt;THBA&gt;0&lt;/THBA&gt;&lt;OA&gt;1&lt;/OA&gt;&lt;OB&gt;10+N0+N0+N0+N0+&lt;/OB&gt;&lt;OBC&gt;8388608&lt;/OBC&gt;&lt;OBA&gt;1&lt;/OBA&gt;&lt;/VS&gt;&lt;HS&gt;&lt;TF&gt;&lt;NA&gt;Arial&lt;/NA&gt;&lt;SI&gt;10&lt;/SI&gt;&lt;FL&gt;1000&lt;/FL&gt;&lt;CO&gt;0&lt;/CO&gt;&lt;CS&gt;0&lt;/CS&gt;&lt;/TF&gt;&lt;TA&gt;1&lt;/TA&gt;&lt;DL&gt;&lt;EST&gt;0&lt;/EST&gt;&lt;ESB&gt;0&lt;/ESB&gt;&lt;LHB&gt;10+N0+N0+N0+N0+&lt;/LHB&gt;&lt;LHF&gt;&lt;NA&gt;Arial&lt;/NA&gt;&lt;SI&gt;10&lt;/SI&gt;&lt;FL&gt;1000&lt;/FL&gt;&lt;CO&gt;16777215&lt;/CO&gt;&lt;CS&gt;0&lt;/CS&gt;&lt;/LHF&gt;&lt;LHA&gt;270&lt;/LHA&gt;&lt;LBH&gt;7&lt;/LBH&gt;&lt;LBV&gt;2&lt;/LBV&gt;&lt;SRBC&gt;0&lt;/SRBC&gt;&lt;SRBA&gt;0&lt;/SRBA&gt;&lt;SHA&gt;230&lt;/SHA&gt;&lt;SHBC&gt;0&lt;/SHBC&gt;&lt;SHBA&gt;0&lt;/SHBA&gt;&lt;/DL&gt;&lt;TRB&gt;10+N0+N0+N0+N0+&lt;/TRB&gt;&lt;TRBC&gt;0&lt;/TRBC&gt;&lt;TRBA&gt;0&lt;/TRBA&gt;&lt;THB&gt;10+N0+N0+N0+N0+&lt;/THB&gt;&lt;THA&gt;230&lt;/THA&gt;&lt;THBC&gt;0&lt;/THBC&gt;&lt;THBA&gt;0&lt;/THBA&gt;&lt;OA&gt;1&lt;/OA&gt;&lt;OB&gt;10+N0+N0+N0+N0+&lt;/OB&gt;&lt;OBC&gt;8388608&lt;/OBC&gt;&lt;OBA&gt;1&lt;/OBA&gt;&lt;/HS&gt;&lt;GB&gt;20+N0+N0+N0+N0+&lt;/GB&gt;&lt;LC&gt;128&lt;/LC&gt;&lt;LBC&gt;14483455&lt;/LBC&gt;&lt;HC&gt;65535&lt;/HC&gt;&lt;EC&gt;12632256&lt;/EC&gt;&lt;RGS&gt;1&lt;/RGS&gt;&lt;RAF&gt;1&lt;/RAF&gt;&lt;RA&gt;240&lt;/RA&gt;&lt;RF&gt;&lt;NA&gt;Arial&lt;/NA&gt;&lt;SI&gt;10&lt;/SI&gt;&lt;FL&gt;0000&lt;/FL&gt;&lt;CO&gt;0&lt;/CO&gt;&lt;CS&gt;0&lt;/CS&gt;&lt;/RF&gt;&lt;RBC&gt;16777215&lt;/RBC&gt;&lt;TBC&gt;12632256&lt;/TBC&gt;&lt;TBA&gt;1&lt;/TBA&gt;");
    TemplateFormatting->SetPropertyIDType("WordWrap", 11, 11, "-1");
    TemplateFormatting->SetPropertyIDType("MinimumColumnWidth", 12, 5, "5.86");
    TemplateFormatting->SetPropertyIDType("OutlineLevelsExpanded" , 13, 11, "-1");
    TemplateFormatting->SetPropertyIDType("BandingOptions", 14, 3, "0");
    TemplateFormatting->SetPropertyIDType("SelectedAutostyleID", 15, 8, "");
    TemplateFormatting->SetPropertyIDType("IgnoreOldStyle" , 16, 11, "0");
    TemplateFormatting->SetPropertyIDType("ColumnHeadersWidth", 17, 8, "");
    TemplateFormatting->SetPropertyIDType("ColumnValuesWidth", 18, 8, "");
    TemplateFormatting->SetPropertyIDType("OverwriteExistingTemplateFormatting", 19, 11, "-1");
    TemplateFormatting->SetPropertyIDType("OutlineLevelExpanding", 20, 3, "-1");
    TemplateFormatting->SetPropertyIDType("MergeColumnCells", 21, 11, "-1");
    TemplateFormatting->SetPropertyIDType("KeepCurrentElementOnSave", 22, 11, "-1");
    TemplateFormatting->SetPropertyIDType("ShowRowHeaders", 23, 11, "-1");
    TemplateFormatting->SetPropertyIDType("ShowColumnHeaders", 24, 11, "-1");
    TemplateFormatting->SetPropertyIDType("AllowToggleThresholds", 25, 3, "-1");
    TemplateFormatting->SetPropertyIDType("TitleOverlapViewMode", 26, 2, "0");
    TemplateFormatting->SetPropertyIDType("ShowMetricObjectInGraphZone", 27, 11, "0");
    TemplateFormatting->SetPropertyIDType("PreviousIDAsAutostyle", 28, 8, "");
    
    mFormatPropertySetsMap["Template Formatting"] = TemplateFormatting;
    
    DSSFormatPropertySetMap* ColumnWidthProperties = new DSSFormatPropertySetMap();
    ColumnWidthProperties->SetPropertyIDType("ColumnScenario", 1, 3, "1");
    ColumnWidthProperties->SetPropertyIDType("HeaderWidth", 2, 3, "-1");
    ColumnWidthProperties->SetPropertyIDType("GridWidth", 3, 3, "-1");
    ColumnWidthProperties->SetPropertyIDType("ExtraWidth", 4, 3, "-1");
    ColumnWidthProperties->SetPropertyIDType("RowScenario", 5, 3, "2");
    ColumnWidthProperties->SetPropertyIDType("RowHeight", 6, 3, "-1");
    mFormatPropertySetsMap["ColumnWidthProperties"] = ColumnWidthProperties;
    
	std::string lPropertySetNameArray[37] = {"", "FormattingNumber", "FormattingAlignment", "FormattingFont", "FormattingBorder", "FormattingPatterns", "FormattingBanding", "FormattingLine", "FormattingSize", "FormattingAppearance", "FormattingCrosstab", "FormattingPadding", "FormattingPicture", "FormattingOrder", "FormattingView", "FormattingLayout", "FormattingHTML", "FilterDetailsFormatProperties", "FormattingSelector", "Visualization", "FormattingTransitions", "FormattingPage", "FormattingNavigation", "WatermarkProperties", "FormattingIncrementalFetch", "FormattingPosition", "FormattingSection", "FormattingGroup", "RWDocumentProperties", "FormattingWidget", "FormattingAnalysis", "FormattingTransactions", "Mobile", "GeneralReportProperties", "GraphInfo", "Template Formatting", "ColumnWidthProperties"};
	mpPropertySetNameVector	= new std::vector<std::string>(lPropertySetNameArray, lPropertySetNameArray+37);
	//It's a 1 dimensional Array, index is PropertySetID, item is PropertySetName, PropertySetID is an int in Definition binary
	
	//integer 3
	//bool 11
	//double 5
	//string 8
    //short 2
}

DSSFromatPropertyDefinition::~DSSFromatPropertyDefinition()
{
	FormatPropertySetsMap::iterator it;
	for ( it=mFormatPropertySetsMap.begin() ; it != mFormatPropertySetsMap.end(); it++ )
		delete (*it).second;
	if (mpPropertySetNameVector)
	{
		delete mpPropertySetNameVector;
		mpPropertySetNameVector = NULL;
	}
}

/*std::string DSSFromatPropertyDefinition::getPropertyValue(std::string iPropertySetName, std::string iPropertyName)
{
	FormatPropertySetsMap::iterator iter = mFormatPropertySetsMap.find(iPropertySetName);
	
	if (iter!=mFormatPropertySetsMap.end())
		return ((*iter).second)->getPropertyValue(iPropertyName);
	
	return "";
}*/

std::string DSSFromatPropertyDefinition::getPropertyNameFromID(std::string iPropertySetName, int iPropertyID)
{
	FormatPropertySetsMap::iterator iter = mFormatPropertySetsMap.find(iPropertySetName);
	
	if (iter!=mFormatPropertySetsMap.end())
		return (*iter).second->getPropertyName(iPropertyID);

	return "";
}

EnumVariantType DSSFromatPropertyDefinition::getPropertyTypeFromID(std::string iPropertySetName, int iPropertyID)
{
	FormatPropertySetsMap::iterator iter = mFormatPropertySetsMap.find(iPropertySetName);
	
	if (iter!=mFormatPropertySetsMap.end())
		return (*iter).second->getPropertyType(iPropertyID);
	
	return VT_EMPTY;
}

std::string DSSFromatPropertyDefinition::getPropertySetNameFromID(int iPropertySetID)
{
	//test start
	if (!mpPropertySetNameVector)
		return "";
	if (iPropertySetID >= mpPropertySetNameVector->size())
		return "";
	//test end
	
	return (*mpPropertySetNameVector)[iPropertySetID];
}

DSSFormatPropertySetMap* DSSFromatPropertyDefinition::getFormatPropertySet(std::string iPropertySetName)
{
	FormatPropertySetsMap::iterator iter = mFormatPropertySetsMap.find(iPropertySetName);
	
	if (iter!=mFormatPropertySetsMap.end())
		return (*iter).second;
	
	return NULL;
}