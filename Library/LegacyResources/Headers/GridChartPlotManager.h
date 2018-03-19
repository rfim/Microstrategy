/*
 *  GridChartPlotManager.h
 *  IPhoneChart
 *
 *  Created by dong shi on 12/3/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef GRIDCHARTPLOTMANAGER_H
#define GRIDCHARTPLOTMANAGER_H
#include "PlotManager.h"
#include "CategoryAxis.h"
#include "GridChartPlot.h"
#include "GridPiePlot.h"

namespace MsiChart
{
	struct GridChartOptions
	{
		bool mIsAutoFitMode;
		bool mIsColorUniform;
		double mMaxDataVal;	//resued for uniform color value when color is uniform for all marker.
		double mMinDataVal;
		double mMaxColVal;
		double mMinColVal;
		
		unsigned Int32 mMarkerShape;
		Int32 mLowColor;
		Int32 mHighColor;
		Int32 mColorIndex;
		Int32 mSizeIndex;
		Int32 mUniformColorMode;
		
		
		GridChartOptions() : mIsAutoFitMode(true), mIsColorUniform(false), mMaxColVal(0), mMinColVal(0),
			mMaxDataVal(100000.0), mMinDataVal(0.0), mColorIndex(0), mSizeIndex(0), mLowColor(0), mHighColor(0), mUniformColorMode(2),
			mMarkerShape(DssGraphMarkerShapeCircle)
		{
			
		}
		
	};
	
	class GridChartPlotManager : public PlotManager
	{
	public:
		typedef MSynch::SmartPtr<GridChartPlotManager> Ptr;

		GridChartPlotManager(const TripleId& irTripleId, GraphObjectManager* ipManager, const Rect2D& irPlotArea);
		virtual ~GridChartPlotManager();
		
		//Re-implemention the virtual functions inherited from GraphObjectManager
		virtual GraphObject::Ptr OnPointDetection(const Point2D& irCursor) const; 
		virtual void OnRectangleSelection(const Rect2D& irRect, std::vector<GraphObject::Ptr>& iorSelectedObjects) const;
		virtual void Draw();
		virtual void GenerateView();
		virtual void GenerateImageMap(ImageMap& iorImageMap); 
		virtual void GenerateMapAndList(); 	
		
		GridChartOptions* GetPlotOption();

	protected:
		bool hIsReuseYAxis();
		void hLoadProperties();
		GridBasePlot::Ptr hCreatePlot(Int32 iChartAreaIndex);
		void hCreateAxisAtChartArea(Int32 iIndex);
		void hReserveAxisArea(const Rect2D& irRect, Rect2D& iorRect, bool iIsForReusedAxis);
		void hAssignChartAreas(const Rect2D& irDataArea, const std::vector<Rect2D>& irChartAreas);
		void hPopulatePlotOptions();	
		
	private:
		std::vector<GridBasePlot::Ptr> mPlots;
		std::vector<CategoryAxis::Ptr> mXAxisSet;
		std::vector<CategoryAxis::Ptr> mYAxisSet;
		bool mIsReuseYAxis;
		GridChartOptions mPlotOption;
	};
	
	inline 	GridChartOptions* GridChartPlotManager::GetPlotOption()
	{
		return &mPlotOption;
	}

}

#endif