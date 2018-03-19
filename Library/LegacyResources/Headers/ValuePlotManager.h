//====================================================================
//	Filename:		ValuePlotManager.h
//	AUTHOR		:	Fangyao Ma
//	CREATION	:	02/09/09
//	Copyright (C) MicroStrategy, Inc. 2009
//==============================================================================================

#ifndef MSICHART_VALUEPLOTMANAGER_H
#define	MSICHART_VALUEPLOTMANAGER_H

#include "PlotManager.h"
#include "ValuePlot.h"
#include "ValueAxis.h"
#include "ValuePiePlot.h"

namespace MsiChart
{
	/// Define value axis type
	typedef enum 
	{
		VAT_X = 0,
		VAT_Y1 = 1,
		VAT_Y2 = 2,
		VAT_Z = 3,
	}EnumValueAxisType;

	/**
	 *	ValuePlotManager class is used to support Chart Areas for Scatter & Bubble graph.
	 */
	class ValuePlotManager: public PlotManager
	{
	public:
		typedef MSynch::SmartPtr<ValuePlotManager> Ptr;

		ValuePlotManager(const TripleId& irTripleId, GraphObjectManager* ipManager, const Rect2D& irPlotArea);
	
		//Re-implemention the virtual functions inherited from GraphObjectManager
		virtual GraphObject::Ptr OnPointDetection(const Point2D& irCursor) const; 
		virtual void OnRectangleSelection(const Rect2D& irRect, std::vector<GraphObject::Ptr>& iorSelectedObjects) const;
		virtual void Draw();
		virtual void GenerateView();
		virtual void GenerateImageMap(ImageMap& iorImageMap); 
		virtual void GenerateMapAndList(); 	
	protected:
		/**
		This function loads some properties which used to initiate ValuePlot, get Series distribution.
		*/
		virtual void LoadProperties();

	private:
		/**
		This function reserve space for vertical area.
		@param irDataArea The input area.
		@param orDataArea The output residual area.
		*/
		void hReserveVerticalArea(const Rect2D& irDataArea, Rect2D& orDataArea);

		/**
		This function reserve space for horizontal area.
		@param irDataArea The input area.
		@param orDataArea The output residual area.
		*/
		void hReserveHorizontalArea(const Rect2D& irDataArea, Rect2D& orDataArea);

		bool hIsDualAxis();

		/**
		 *	This function checks whether there should be multiple Y axis
		 *	@return true: indicates there will be multiple Y(Y1,Y2) axis
		 */
		bool hIsMultipleYAxis();

		/**
		 *	Get the minimum and maximum value for value axis
		 *	@param iIndex: indicates which value axis
		 *	@param orMin: Minimum value for this axis
		 *	@param orMax: Maximum value for this axis
		 */
		void hGetMinMaxValueWithChartArea(Int32 iChartAreaID, EnumValueAxisType iVAType, double& orMin, double& orMax);

		/**
		This function sets data area for one Y1 axis or a couple of Y1 axis and Y2 axis.
		@param iChartArea The data area.
		@param iAxisId Used to specifies the Y1 axis and Y2 axis
		*/
		void hSetDataAreaValueAxis(const Rect2D& irChartArea, Int32 iAxisId);


		/**
		 *	This function get the corresponding ValueAxis according to the ChartAreaID and the ValueAxisType(X, Y1, Y2)
		 *	@param iChartAreaID specifies the ChartArea ID
		 *	@param iVAType specifies the ValueAxis type
		 */
		ValueAxis::Ptr hGetValueAxisPtr(Int32 iChartAreaID, Int32 iVAType);

		/**
		 *	This function checks whether the data is enough to renderring the specific kind of graph
		 *	@return: true indicates the 
		 */
		bool hCheckDataset();

		/*
		* Generate the axis line for all axes
		*/
		void hGenerateAxisLines();

		/*
		* Generate the grid line for all axes
		*/
		void hGenerateGridLines(const std::vector<Rect2D>& irChartAreas);

		/*
		* Generate the interlaced grids for all axes
		*/
		void hGenerateInterlacedGrids(const std::vector<Rect2D>& irChartAreas);	
		

		//private data members
		std::vector<Plot::Ptr> mVecValuePlot;	///< ValuePlot objects
		std::vector<ValueAxis::Ptr> mVecXAxis;		///< X axes
		std::vector<ValueAxis::Ptr> mVecY1Axis;		///< Y1 value axes
		std::vector<ValueAxis::Ptr> mVecY2Axis;		///< Y2 value axes
		std::vector<ValueAxis::Ptr> mVecZAxis;		///< Z value axes

		std::vector<Int32> mVecSeriesVAIndex;
	};
}

#endif