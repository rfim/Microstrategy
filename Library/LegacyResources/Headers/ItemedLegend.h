/*
 *  ItemedLegend.h
 *  IPhoneChart
 *
 *  Created by dong shi on 12/14/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MSICHART_ITEMEDLEGEND_H
#define	MSICHART_ITEMEDLEGEND_H

#include "Legend.h"
#include "FormatFill.h"
#include "GraphObject.h"
#include "Rect2D.h"



namespace MsiChart
{
	/**
	 The legend options.
	 */
	struct GraphOptionsLegend
	{
		GraphOptionsLegend(bool iShowLegend, unsigned char iLegendPosition, unsigned char iLegendMarkersStyle, unsigned char iLegendBoxStyle)
		: mShowLegend(iShowLegend),
		mShowSeriesTitles(true),
		mLegendPosition(iLegendPosition), 
		mLegendMarkersStyle(iLegendMarkersStyle), 
		mLegendBoxStyle(iLegendBoxStyle),
		mLegendItemsNum(0),
		mRecalculate(false),
		mReposition(false),
		mAxisLabelsAutoFit(true),
		mIsByGroup(false),
		mIsColorBy(false)
		{}
		bool mShowLegend;
		bool mShowSeriesTitles;
		unsigned char mLegendPosition;
		unsigned char mLegendMarkersStyle;
		unsigned char mLegendBoxStyle;
		unsigned char mLegendItemsNum;
		bool mVertical;
		bool mRecalculate;
		bool mReposition;
		bool mAxisLabelsAutoFit;
		bool mIsByGroup;			 ///< To Support GraphMatrix.	
		bool mIsColorBy;			 ///< To Support GraphMatrix.
	}; 



	class ItemedLegend: public Legend
	{
	public:
		typedef MSynch::SmartPtr<ItemedLegend> Ptr;
		
		ItemedLegend(const TripleId& irTripleId, GraphObjectManager* ipManager, Dataset::Ptr iDatasetPtr, Rect2D iArea);
		~ItemedLegend();
		
		virtual void Draw();
		virtual void GenerateView();
		virtual void GenerateMapAndList();
		virtual void GetOptions(Int32 iPropertyId, CComVariant* opValue) const;
		virtual void GetOptions(Int32 iSeriesId, Int32 iPropertyId, CComVariant* opValue) const;
		virtual Int32 GetItemCount();
		
		/**
		 This function sets FormatLine of the lines behind the legend markers. These kind of lines only exist in Line and Scatter chart.
		 @return True means FormatLine is set successfully and false means not.
		 */
		bool SetFormatLineOfLegendMarkerLine(FormatLine::Ptr iFormatLine, Int32 iSeriesId, bool iIsLineVertical = false);
		
		/**
		 This function tells if the items are listed across groups or series.
		 @return True means the items are from each group.
		 */
		bool IsByGroup() const; //For grid chart

		void UpdateFormatForLegendMarker(Int32 iIndex, FormatFill::Ptr iFormatFill, FormatLine::Ptr iFormatLine, bool iIsLineAssignedToLegnedMarkerLine = false, bool iIsLineVertical = false);

		
	protected:
		/**
		 This function is to return the legend position.
		 */
		Int32 hGetLegendPosition();		 	
		
	private:	
		std::vector<StraightLineObject::Ptr> mLegendMarkerLines;	///< The legend marker lines behind the legend markers which only exist in Line chart.
		GraphOptionsLegend mGraphOptionsLegend;	///< The struct holds all the properties related to Legend.
		
		Int32 mItemMargin;						///< The item margin which determines the distance between items.
		Int32 mEdgeMargin;
		Int32 mLegendMarkerSize;				///< The legend marker size in pixel.
		bool mIsExpanded;				///< Indicates whether the graph size is automatically adjusted or not. "Expand background frame to accommodate the recalculated legend size" option.
		Int32 mLegendItemCount;
		
		std::map<Int32, Int32> mLegendItemIndexToStockMetricIndex;
		MBase::String	mStockMetricValue[3];
		unsigned char	mStockMetricLayout[3];
		
		/**
		 This function calculates the value of mItemMargin and mLegendMarkerSize according to the input font info.
		 @param iFormatFontPtr The input FormatFont.
		 */
		void hCalculateLegendLayoutParas(FormatFont::Ptr iFormatFontPtr);
		
		/**
		 This function loads all the properties related to Legend.
		 */
		void hLoadProperties();
		
		/**
		 This function calculates the legend layout and generates GraphObjects.
		 @param irArea The area excluding Title, Subtitle, Footnote is provided for Legend.
		 @param orLegendArea The output legend area.
		 @param iLegendTextCollectionPtr The collection of legend texts.
		 */
		void hCalculateLegendLayoutInDevice(const Rect2D &irArea, Rect2D &orLegendArea, GraphCollectionObject::Ptr iLegendTextCollectionPtr);
		
		/**
		 This function generates several TextObjects delegating legend texts. It calculates a legend text area that could accommodate any of legend texts. 
		 @param irArea The area excluding Title, Subtitle, Footnote is provided for Legend.
		 @param orTextArea The output maximum text area.
		 @param iLegendTextCollectionPtr The collection of legend texts. 
		 */
		void hGenerateLegendText(const Rect2D& irArea, Rect2D &orTextArea, GraphCollectionObject::Ptr iGraphCollectionObjectPtr);
		
		/**
		 This function determines the placement order of each legend item according to the graph type.
		 @return True means reverse order while false means normal order
		 */
		bool hIsReversedOrder();
		
		/**
		 This function is only used in the layout mode "Manual + Re-calculate legend size automatically when data changes + Re-position chart frame and legend"
		 to determine the legend position according to the legend location got from GraphStyle.
		 @return A enumation of EnumLegendLayout.
		 */
		unsigned char hCalculateLegendPosition();
		
		
		/**
		 This function is used to compute the number of the items listed in legend
		  @return number of the items.
		 */
		Int32 hGetItemCount();
	};

	inline bool ItemedLegend::IsByGroup() const
	{
		return mGraphOptionsLegend.mIsByGroup;
	}

	inline Int32 ItemedLegend::hGetLegendPosition()
	{
		return mGraphOptionsLegend.mLegendPosition;
	}
	
	
}
#endif