//====================================================================
//	Filename:		CategoryPlotManager.h
//	AUTHOR		:	Xin Liu
//	CREATION	:	12/18/08
//	Copyright (C) MicroStrategy, Inc. 2008
//==============================================================================================

#ifndef MSICHART_CATEGORYPLOTMANAGER_H
#define	MSICHART_CATEGORYPLOTMANAGER_H

#include "PlotManager.h"
#include "CategoryPlot.h"
#include "ValueAxis.h"
#include "CategoryAxis.h"
#include "ABLPlot.h"

namespace MsiChart
{

	/**
	CategoryPlotManager class is used to support Chart Areas for Area, Bar and Line charts.
	*/
	class CategoryPlotManager: public PlotManager
	{
	public:
		typedef MSynch::SmartPtr<CategoryPlotManager> Ptr;

		CategoryPlotManager(const TripleId& irTripleId, GraphObjectManager* ipManager, const Rect2D& irPlotArea);
		virtual ~CategoryPlotManager();

		//Re-implemention the virtual functions inherited from GraphObjectManager
		virtual GraphObject::Ptr OnPointDetection(const Point2D& irCursor) const; 
		virtual void OnRectangleSelection(const Rect2D& irRect, std::vector<GraphObject::Ptr>& iorSelectedObjects) const;
		virtual void Draw();
		virtual void GenerateView();
		virtual void GenerateImageMap(ImageMap& iorImageMap); 
		virtual void GenerateMapAndList(); 	
	//	virtual void GetOptions(Int32 iSeriesId, Int32 iPropertyId, VARIANT* opValue) const;
		
		virtual Int32 GetDataAreaSplitX() const;
		
		virtual void OnZooming(Int32 iOldStart, Int32 iOldEnd, Int32 iNewStart, Int32 iNewEnd, bool iIsVertical);

		virtual bool GetToolTipData(Int32 iSeriesID, Int32 iGroupID, double& orValue, double& orPercentValue, Int32 iOffset = 0);

	protected:
		/**
		This function loads some of ABLOptions, which used to initiate CategoryPlot, ValueAxis and CategoryAxis correctly.
		*/
		virtual void LoadProperties();

		/**
		This function reserve area for category axes.
		@param irDataArea The input area.
		@param orDataArea The output residual area.
		*/
		void ReserveAreaCategoryAxis(const Rect2D& irDataArea, Rect2D& orDataArea);

		/**
		This function determines whether there are mutiple value axes or mutiple category axes.
		*/
		bool IsMutipleValueAxis() const;
        
        /**
         This function determines whether should rotate data label in vertical bar chart.
         */
        void RotateDataLabel();

	private:
		//private methods
	//	void hLoadProperties();

		/**
		This function reserve area for value axes.
		@param irDataArea The input area.
		@param orDataArea The output residual area.
		*/
		void hReserveAreaValueAxis(const Rect2D& irDataArea, Rect2D& orDataArea);

		/**
		This function checks whether the graph type is kind of bipolar types.
		@param iGraphType The graph type.
		@return True means it is a bipolar type while false means not.
		*/
		bool hIsBipolar(Int32 iGraphType) const;

		/**
		This function sets data area for one Y1 axis or a couple of Y1 axis and Y2 axis.
		@param iChartArea The data area.
		@param iAxisId Used to specifies the Y1 axis and Y2 axis
		*/
		void hSetDataAreaValueAxis(const Rect2D& irChartArea, Int32 iAxisId);

		/**
		A factory method used to create an ABLPlot, GanttPlot or BoxPlot instance.
		@param iChartAreaId Spcecifies the chart area id.
		@return A CategoryPlot pointer to an ABLPlot, GanttPlot or BoxPlot instance.
		*/
		CategoryPlot::Ptr hCreateCategoryPlot(Int32 iChartAreaId);

		void hGenerateAxisLines();

		void hGenerateInterlacedGrids(const std::vector<Rect2D>& irChartAreas);

		void hGenerateGridLines(const std::vector<Rect2D>& irChartAreas);
        
        void hSetDataArea(const Rect2D& irChartArea, std::vector<Rect2D>& orChartAreas);
        
        void hPrepareDataAreaWithLabelRotation(Rect2D& irChartArea, std::vector<Rect2D>& orChartAreas);

	protected:
		//data members
		EnumDSSGraphType		mGraphType;
		EnumLayout		  		mLayout;
		EnumOrientation			mOrientation;
		bool					mUseDepth;
		bool					mDualAxes;
		bool					mSplitDualAxes;
		Int32					mDepth;
		Int32					mDepthDirection;
		Int32					mSplitAxisPosition;
		Int32					mDataAreaSplitX;
		Rect2D					mDataArea;
		std::vector<CategoryPlot::Ptr> mCategoryPlots;	///< CategoryPlot objects
		std::vector<CategoryAxis::Ptr> mVecO1Axis;		///< Category axes
		std::vector<ValueAxis::Ptr> mVecY1Axis;			///< Y1 value axes
		std::vector<ValueAxis::Ptr> mVecY2Axis;			///< Y2 value axes

	};

	inline bool CategoryPlotManager::IsMutipleValueAxis() const
	{
		return (mOrientation == mChartAreaOptions.mOrientation);
	}
}

#endif
