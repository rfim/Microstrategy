//==============================================================================================
//	FILENAME	:	ChartManager.h
//	AUTHOR		:	Xin Liu
//	CREATION	:	5/22/08
//	Copyright (C) MicroStrategy, Inc. 2008
//==============================================================================================

#ifndef MSICHART_CHARTMANAGER_H
#define	MSICHART_CHARTMANAGER_H

#include "PDCHeader/PDCvector"
#include "../Dataset.h"
#include "GraphObjectManager.h"
#include "Plot.h"
#include "PlotManager.h"
#include "Legend.h"
#include "TextObject.h"



namespace MsiChart
{
	const Int32 gDefaultMarginForTimeSeries = 3;	///< The default margin value for time series chart.
	const Int32 gDefaultMargin = 10;	///< The default margin value.	

	/**The root of the GraphObjectManager tree.
	ChartManager is reponsible for creating and manipulating several TextObjects delegating Title, Subtitle and Footnote in its graph object map and list. 
	It also contains a Legend object manager and a Plot object manager as its data member. According to the graph type which is provided to the ChartManager
	object, it creates different Plot object. When generating the manager tree, ChartManager object calculates the plot area and pass the plot area.
	information to Plot object. 
	*/
	class ChartManager: public GraphObjectManager
	{
	public:
		typedef MSynch::SmartPtr<ChartManager> Ptr;

		ChartManager(const TripleId& iTripleId, Dataset::Ptr iDatasetPtr, ChartContext::Ptr iChartContextPtr);
		~ChartManager();

		virtual GraphObject::Ptr OnPointDetection(const Point2D& irCursor) const; 
		virtual void OnRectangleSelection(const Rect2D& irRect, std::vector<GraphObject::Ptr>& iorSelectedObjects) const;
		virtual void GenerateMapAndList();
		virtual void Draw();
		virtual void GenerateView();
		virtual void GenerateImageMap(ImageMap& iorImageMap);

		virtual void GetOptions(Int32 iPropertyId, CComVariant* opValue) const;
		virtual void GetOptions(Int32 iSeriesId, Int32 iPropertyId, CComVariant* opValue) const;

		/**
		This function is to get the displayed data value used for tooltip.
		@param iSeriesID Specifies the series id.
		@param iGroupID Specifies the group id.
		@param orValue Outputs the dislayed data value.
		@param orPercentValue Outputs the displayed percentage data value.
		@param iOffset Distinguishes multi-data points, such as scatter where the flags would have different values for XVALUE, YVALUE and so on for other charts
		*/
		bool GetToolTipData(Int32 iSeriesID, Int32 iGroupID, double& orValue, double& orPercentValue, Int32 iOffset);
		/* obsolete functions
		void SetGraphType(Int32 iGraphType);
		void GetGraphType(Int32& orGraphType);
		*/

		void SetDataset(Dataset::Ptr iDataSetPtr);
		Int32 GetDataAreaSplitX() const;

		/**
		 Handles zooming event.
		 @param iOldStart, iOldEnd Specifies an old range.
		 @param iNewStart, iNewEnd Specifies a new range.
		 @param iIsVertical Indicate whether to zoom vertically or horizontally.
		 */
		void OnZooming(Int32 iOldStart, Int32 iOldEnd, Int32 iNewStart, Int32 iNewEnd, bool iIsVertical);


		//void hCalculatePlotArea();	

		/**
		This function loads those properties used in ChartManager from ChartContext.
		*/
		void LoadProperties();

		/**
		This function calculates the layout of the whole graph, including the positions of Title, Subtitle, Footnote, Legend and Plot. It also generates GraphObjects.
		@param iorArea Input is the whole image area, and the output is the area left for Plot.
		*/
		void CalculateLayout(Rect2D& iorArea);

		/**
		Check the series count.
		If dual axes chart, series count should be 2 or larger.
		Else series count shouldbe 1 or larger.
		*/
		bool CheckDataset();
		
		/**
		 Create background object.
		 @return Return background object.
		 */
		RectangleObject::Ptr CreateBackground();

		void SetHighlightLayerObject(LayerObject::Ptr iHighlightLayerObjectPtr);
		LayerObject::Ptr GetHighlightLayerObject();

		
		/**
		 Set the <SeriesID, GroupId> pairs that indicates the highlight objects in document opening
		 */
		void SetHighlightSelectedObjects(std::vector<std::pair<Int32, Int32> >& iSelectedSeriesAndColumns);
        void ClearHighlightSelectedObjects();
		
		/// 471270 When there's curve polygon, need to draw legend and category labels on top layer.
		void DrawPostGraphObjects();

		/** Get size infomation for a specific group or legend
		*/	
		bool GetGroupSpace(std::vector<Int32>& orVect, Int32 iGroupIndex, bool iIsLegend);

	private:
		/**
		This function is a helper function used to calculate the frame area.
		@param iorArea Input is the area excluding Title, Subtitle and Footnote, and the output is the area for frame.
		@param iWidthMargin The width margin between frame and legend.
		@param iHeightMargin The height margin between frame and legend.
		*/
	//	void hAdjustFrame(Rect2D& iorArea, Int32 iWidthMargin, Int32 iHeightMargin);

		Int32 mGraphType;				///< The graph type.

		std::vector<PlotManager::Ptr> mPlotManagers;
		Dataset::Ptr mDatasetPtr;		///< The dataset.
		Legend::Ptr mLegendPtr;			///< The Legend instance.
		LayerObject::Ptr mHighlightLayerObjectPtr;	///< the init highlight layer object		
		
		bool mIsTitleShown;				///< Indicates whether Title is shown or not.
		bool mIsSubtitleShown;			///< Indicates whether Subtitle is shown or not.
		bool mIsFootnoteShown;			///< Indicates whether Footnote is shown or not.
		bool mIsLegendShown;			///< Indicates whether Legend is shown or not.
		bool mIsExpanded;				///< Indicates whether the graph size is automatically adjusted or not. "Expand background frame to accommodate the recalculated legend size" option.

		bool mIsInitialized;			///< Indicates whether the GraphObjectManager tree and all the GraphObjects have been constructed or not.

		Int32 mLeftMargin;
		Int32 mRightMargin;
		Int32 mTopMargin;
		Int32 mBottomMargin;
		
		bool mAxisLabelsAutoFit;
		
		std::vector<std::pair<Int32, Int32> > mSelectedSeriesAndColumns;
		
		std::vector<GraphObject::Ptr> mPostGraphObjectList;		
		
	protected:
	   Legend::Ptr hGetLegend(Rect2D&  iorArea);	
	};
}

#endif
