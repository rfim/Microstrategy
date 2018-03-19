//==============================================================================================
//	FILENAME	:	Legend.h
//	AUTHOR		:	Xin Liu
//	CREATION	:	5/20/08
//	Copyright (C) MicroStrategy, Inc. 2008
//==============================================================================================


#ifndef MSICHART_LEGEND_H
#define	MSICHART_LEGEND_H

#include "GraphObjectManager.h"
#include "../Dataset.h"
#include "GraphCollectionObject.h"
#include "StraightLineObject.h"

namespace MsiChart
{
	/**
	Indicates the legend position in the whole chart.
	*/
	enum EnumLegendLayout
	{
		LL_LegendFreeFloat,
		LL_LegendOnRightSide,
		LL_LegendOnLeftSide,
		LL_LegendBelowChart
	}; 

	enum EnumLegendType
	{
		LT_UNDEFINED,
		LT_ITEMD,
		LT_RANGE,
	};
	

	/**
	Indicates the relative position between legend marker and legend text.
	*/
	enum EnumLegendMarkersStyle
	{
		LMS_MarkerstoLeftofText,
		LMS_MarkerstoRightofText,
		LMS_TextCenteronMarkers,
		LMS_MarkersAboveText,
		LMS_MarkersBelowText
	};

	typedef enum
	{
		LegendRows,
		LegendLocked,
		LegendFancyWidthInset,
		LegendFancyHeightInset,
		LegendLegendElementWidth,
		LegendLegendElementHeight,
	}EnumLegendDetails;


	/**This class delegates the chart legend.
	It is responsible for managing several TextObjects delegating the legend texts and several MarkerObjects delegating the legend area and the legend markers.
	*/
	class Legend: public GraphObjectManager
	{
	public:
		typedef MSynch::SmartPtr<Legend> Ptr;

		Legend(const TripleId& irTripleId, GraphObjectManager* ipManager, Dataset::Ptr iDatasetPtr, Rect2D iArea);
		~Legend();

		virtual void Draw();
		virtual void GenerateMapAndList();
		virtual void GetOptions(Int32 iPropertyId, CComVariant* opValue) const;
		virtual void GetOptions(Int32 iSeriesId, Int32 iPropertyId, CComVariant* opValue) const;

		/**
		 This function sets FormatLine of the lines behind the legend markers. These kind of lines only exist in Line and Scatter chart.
		 @return True means FormatLine is set successfully and false means not.
		 */
		virtual bool SetFormatLineOfLegendMarkerLine(FormatLine::Ptr iFormatLine, Int32 iSeriesId, bool iIsLineVertical = false);
		
		/**
		This function returns the legend area.
		@return The legend area.
		*/
		const Rect2D& GetLegendArea();

		/**
		 This function tells if the items are listed across groups or series.
		 @return True means the items are from each group.
		 */
		virtual bool IsByGroup() const; //For grid chart
		
		/**
		 This function is used to update the format fill for the indexed legend marker (if it has format fill) and format line.
		 @param iIndex, the index of the target legend marker.
		 @param iFormatFill, the newly assigned format fill
		 @param iFormatLine, the newly assigned format line.	
		 */
		virtual void UpdateFormatForLegendMarker(Int32 iIndex, FormatFill::Ptr iFormatFill, FormatLine::Ptr iFormatLine, bool iIsLineAssignedToLegnedMarkerLine = false, bool iIsLineVertical = false);

		/**
		This function is to calculate the frame area.
		@param iWidthMargin The width margin between frame and legend.
		@param iHeightMargin The height margin between frame and legend.
		@param iorArea Uutput is the area for frame.
		*/
		void CalculateFrame(Int32 iWidthMargin, Int32 iHeightMargin, Rect2D& orArea);

		/**
		This method will return how many items thie legend have. For Range legend it will return 0;
		@return the item number.
		*/
		virtual Int32 GetItemCount(); 

		EnumLegendType GetType();


	protected:
		Dataset::Ptr mDatasetPtr;				///< Pointer to the dataset.
		Rect2D mLegendArea;						///< Indicates the legend area.
		Rect2D mArea;							///< Indicates the area excluding title, subtitle, footnote.
		EnumLegendType mType;					///< Legend Type
		/**
		 This function is to return the legend position.
		 */
		virtual Int32 hGetLegendPosition();		 	
	};

	inline EnumLegendType Legend::GetType()
	{
		return mType;
	}
}

#endif
