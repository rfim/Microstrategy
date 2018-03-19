//====================================================================
//	Filename:		Kernel/SourceCode/ChartEngine/PrivateSource/StockPlotManager.h
//	AUTHOR		:	Xin Liu
//	CREATION	:	04/15/09
//	Copyright (C) MicroStrategy, Inc. 2009
//==============================================================================================

#ifndef MSICHART_STOCKPLOTMANAGER_H
#define	MSICHART_STOCKPLOTMANAGER_H

#include "CategoryPlotManager.h"
#include "StockPlot.h"
#include "ValueAxis.h"
#include "CategoryAxis.h"

namespace MsiChart
{

	/**
	StockPlotManager class is used to support Chart Areas for Stock charts.
	*/
	class StockPlotManager: public CategoryPlotManager
	{
	public:
		typedef MSynch::SmartPtr<StockPlotManager> Ptr;

		StockPlotManager(const TripleId& irTripleId, GraphObjectManager* ipManager, const Rect2D& irPlotArea);
		virtual ~StockPlotManager();

		//Re-implemention the virtual functions inherited from GraphObjectManager
		virtual GraphObject::Ptr OnPointDetection(const Point2D& irCursor) const; 
		virtual void OnRectangleSelection(const Rect2D& irRect, std::vector<GraphObject::Ptr>& iorSelectedObjects) const;
		virtual void Draw();
		virtual void GenerateView();
		virtual void GenerateImageMap(ImageMap& iorImageMap); 
		virtual void GenerateMapAndList(); 	
		//	virtual void GetOptions(Int32 iSeriesId, Int32 iPropertyId, VARIANT* opValue) const;
		virtual bool GetToolTipData(Int32 iSeriesID, Int32 iGroupID, double& orValue, double& orPercentValue, Int32 iOffset = 0); 

	protected:
		/**
		Overwrite CategoryPlotManager::LoadProperties().
		This function loads some of ABLOptions, which used to initiate CategoryPlot, ValueAxis and CategoryAxis correctly.
		*/
	//	virtual void LoadProperties();

	private:
		/**
		This function reserve area for value axes.
		@param irDataArea The input area.
		@param orDataArea The output residual area.
		*/
		void hReserveAreaValueAxis(const Rect2D& irDataArea, Rect2D& orDataArea);

		/**
		This function sets data area for one Y1 axis or a combination of Y1, Y2, Y3 and Y4.
		@param iChartArea The data area.
		@param iAxisId Used to specifies the Y1 axis and Y2 axis
		*/
		void hSetDataAreaValueAxis(const Rect2D& irChartArea, Int32 iAxisId);

		void hLoadProperties();

		void hGenerateAxisLines();

		void hGenerateInterlacedGrids(const std::vector<Rect2D>& irChartAreas);

		void hGenerateGridLines(const std::vector<Rect2D>& irChartAreas);

		//private data members
		std::vector<ValueAxis::Ptr> mVecY3Axis;			///< Y3 value axes
		std::vector<ValueAxis::Ptr> mVecY4Axis;			///< Y4 value axes
		bool mShowMetric1;
		bool mShowMetric2;
		bool mShowMetric3;

	};
}

#endif