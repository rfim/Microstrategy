//====================================================================
//	Filename:		PlotManager.h
//	AUTHOR		:	Xin Liu
//	CREATION	:	12/04/08
//	Copyright (C) MicroStrategy, Inc. 2008
//==============================================================================================

#ifndef MSICHART_PLOTMANAGER_H
#define	MSICHART_PLOTMANAGER_H
#include "../Dataset.h"
#include "Legend.h"

#include "GraphObjectManager.h"
#include "CategoryPlot.h"

namespace MsiChart
{

	/**
	Chart Areas options.
	*/
	struct ChartAreaOptions
	{
		ChartAreaOptions()
			: mUseChartArea(false)
			, mChartAreaNumber(1)
			, mOrientation(OR_VERTICAL)
		{}

		bool mUseChartArea;				///< Indicates whether Chart Areas is enabled or not
		Int32 mChartAreaNumber;			///< Chart areas count
		EnumOrientation mOrientation;	///< Chart areas orientation
		std::vector<double> mVecPercentageHeightOrWidth;		///< Space division of chart areas
		std::vector<std::vector<Int32> > mSeriesDistribution;	///< Series distribution of all series
        std::vector<std::vector<Int32> > mGroupDistribution;	///< Group distribution of all groups
	};


	/**
	PlotManagger is introduced to support Chart Areas feature. Since it is closely related to the graph type, we have to have different PlotManager to
	support Chart Areas for all kinds of graph types. PlotManager serves as the base class and is used to handle all the chart areas options.
	*/
	class PlotManager: public GraphObjectManager
	{
	public:
		typedef MSynch::SmartPtr<PlotManager> Ptr;

		PlotManager(const TripleId& irTripleId, GraphObjectManager* ipManager, const Rect2D& irPlotArea);
		virtual ~PlotManager();

		//Re-implemention the virtual functions inherited from GraphObjectManager
		//virtual GraphObject::Ptr OnPointDetection(const Point2D& irCursor) const; 
		//virtual void OnRectangleSelection(const Rect2D& irRect, std::vector<GraphObject::Ptr>& iorSelectedObjects) const;
		//virtual void Draw();
		//virtual void GenerateImageMap(ImageMap& iorImageMap); 
		//virtual void GenerateMapAndList(); 	
		virtual void GetOptions(Int32 iSeriesId, Int32 iPropertyId, CComVariant* opValue) const;

		/**
		Set dataset to PlotManager.
		@param iLegendPtr The input dataset pointer. 
		*/
		void SetDataset(Dataset::Ptr iDataSetPtr);

		/**
		Set legend to PlotManager.
		@param iLegendPtr The input legend pointer. 
		*/
		void SetLegend(Legend::Ptr iLegendPtr);

		/**
		This function calculates how to divide the whole data area into chart areas.
		@param irDataArea Specifies the input whole data area.
		@param orChartAreas The output chart areas
		*/
		void CalculateChartAreas(const Rect2D& irDataArea, std::vector<Rect2D>& orChartAreas);

		/**
		This function is to get the displayed data value used for tooltip. 
		PlotManager just provides the default implementation, the derived classes may have to overwrite it, such as Histogram, ABL with percentage, etc.
		@param iSeriesID Specifies the series id.
		@param iGroupID Specifies the group id.
		@param orValue Outputs the dislayed data value.
		@param orPercentValue Outputs the displayed percentage data value.
		@param iOffset Distinguishes multi-data points, such as scatter where the flags would have different values for XVALUE, YVALUE and so on for other charts
		*/
		virtual bool GetToolTipData(Int32 iSeriesID, Int32 iGroupID, double& orValue, double& orPercentValue, Int32 iOffset = 0); 

		void SetMarginInfo(Int32 iLeftMargin, Int32 iRightMargin, Int32 iTopMargin, Int32 iBottomMargin);
	
		virtual Int32 GetDataAreaSplitX() const;
		
		/**
		 Handles zooming event.
		 @param iOldStart, iOldEnd Specifies an old range.
		 @param iNewStart, iNewEnd Specifies a new range.
		 @param iIsVertical Indicate whether to zoom vertically or horizontally.
		 */
		virtual void OnZooming(Int32 iOldStart, Int32 iOldEnd, Int32 iNewStart, Int32 iNewEnd, bool iIsVertical);	
		void GetPlotArea(Rect2D& orRect);

	protected:

		/**
		This function loads values of the chart areas options from ChartContext.
		*/
		void LoadProperties();

		/**
		This function determines whether there is intersection between two rectangle and calculates the intersection part if there is.
		@param irRectA The first input rectangle.
		@param irRectB The second input rectangle.
		@param orRect The intersection rectangle.
		@param True means there is intersection while false means not.
		*/
		bool RectangleIntersection(const Rect2D& irRectA, const Rect2D& irRectB, Rect2D& orRect) const;

		/**
		This function calculates the union rectangle of the two rectangles.
		@param irRectA The first input rectangle.
		@param irRectB The second input rectangle.
		@param orRect The union rectangle.
		*/
		void RectangleUnion(const Rect2D& irRectA, const Rect2D& irRectB, Rect2D& orRect) const;


		Int32 FindChartAreaIdBySeriesId(Int32 iSeriesId) const;

		Legend::Ptr mLegendPtr;				///< The pointer to Legend
		Dataset::Ptr mDatasetPtr;			///< The pointer to Dataset
		Int32 mGraphType;					///< The graph type
		Rect2D mPlotArea;					///< The whole plot area
		ChartAreaOptions mChartAreaOptions;	///< The chart areas option
		bool mRecalculateFrame;				///< Indicates whether the frame should be recalculated and repositioned.

		Int32 mLeftMargin;
		Int32 mRightMargin;
		Int32 mTopMargin;
		Int32 mBottomMargin;
	};
}

#endif
