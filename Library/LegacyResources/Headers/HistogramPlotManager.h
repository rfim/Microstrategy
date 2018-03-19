//====================================================================
//	Filename:		Kernel/SourceCode/ChartEngine/PrivateSource/HistogramPlotManager.h
//	AUTHOR		:	Xin Liu
//	CREATION	:	2/13/09
//	Copyright (C) MicroStrategy, Inc. 2009
//==============================================================================================

#ifndef MSICHART_HISTOGRAMPLOTMANAGER_H
#define	MSICHART_HISTOGRAMPLOTMANAGER_H

#include "PlotManager.h"
#include "HistogramPlot.h"

namespace MsiChart
{

	/**
	HistogramPlotManager class is used to support Chart Areas for Histogram charts.
	*/
	class HistogramPlotManager: public PlotManager
	{
	public:
		typedef MSynch::SmartPtr<HistogramPlotManager> Ptr;

		HistogramPlotManager(const TripleId& irTripleId, GraphObjectManager* ipManager, const Rect2D& irPlotArea);
		virtual ~HistogramPlotManager();

		//Re-implemention the virtual functions inherited from GraphObjectManager
		virtual GraphObject::Ptr OnPointDetection(const Point2D& irCursor) const; 
		virtual void OnRectangleSelection(const Rect2D& irRect, std::vector<GraphObject::Ptr>& iorSelectedObjects) const;
		virtual void Draw();
		virtual void GenerateView();
		virtual void GenerateImageMap(ImageMap& iorImageMap); 
		virtual void GenerateMapAndList();

		//From PlotManager
		virtual bool GetToolTipData(Int32 iSeriesID, Int32 iGroupID, double& orValue, double& orPercentValue, Int32 iOffset = 0); 

	protected:
		/**
		This function loads some of Histogram options, which used to initiate HistogramPlot and ValueAxis.
		*/
		virtual void LoadProperties();

	private:

		/**
		This function determines whether there are mutiple value axes or mutiple category axes.
		*/
		bool hIsMutipleY1ValueAxis() const;

		/**
		This function sets data area for Y1 axis.
		@param iChartArea The data area.
		@param iValueAxisPtr Specifies the value axis.
		*/
		void hSetDataAreaValueAxis(const Rect2D& irChartArea, ValueAxis::Ptr iValueAxisPtr);

		/**
		This function reserve area for X or Y1 value axes.
		@param irVecAxis Specifies X axes or Y1 axes.
		@param irDataArea The input area.
		@param orDataArea The output residual area.
		*/
		void hReserveAreaValueAxis(const std::vector<ValueAxis::Ptr> irVecAxis, const Rect2D& irDataArea, Rect2D& orDataArea, bool iIsVertical);

		void hGenerateAxisLines();

		void hGenerateInterlacedGrids(const std::vector<Rect2D>& irChartAreas);

		void hGenerateGridLines(const std::vector<Rect2D>& irChartAreas);


		//private data members
		bool mUseDepth;
		Int32 mDepth;
		Int32 mDepthDirection;
		EnumOrientation mOrientation;
		std::vector<HistogramPlot::Ptr> mHistogramPlots;	///< HistogramPlot objects
		std::vector<ValueAxis::Ptr> mVecXAxis;			///< X value axes
		std::vector<ValueAxis::Ptr> mVecY1Axis;			///< Y1 value axes

		


	};

	inline bool HistogramPlotManager::hIsMutipleY1ValueAxis() const
	{
		return (mOrientation == mChartAreaOptions.mOrientation);
	}
}

#endif