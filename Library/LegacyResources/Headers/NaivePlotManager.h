//====================================================================
//	Filename:		NaivePlotManager.h
//	AUTHOR		:	Xin Liu
//	CREATION	:	12/18/08
//	Copyright (C) MicroStrategy, Inc. 2008
//==============================================================================================

#ifndef MSICHART_NAIVEPLOTMANAGER_H
#define	MSICHART_NAIVEPLOTMANAGER_H

#include "PlotManager.h"
#include "Plot.h"

namespace MsiChart
{

	/**
	NaivePlotManager class is used to support chart types that don't have axis along the frame, such as Pie, Polar, Radar, Gauges.
	*/
	class NaivePlotManager: public PlotManager
	{
	public:
		typedef MSynch::SmartPtr<NaivePlotManager> Ptr;

		NaivePlotManager(const TripleId& irTripleId, GraphObjectManager* ipManager, const Rect2D& irPlotArea);
		virtual ~NaivePlotManager();

		//Re-implemention the virtual functions inherited from GraphObjectManager
		virtual GraphObject::Ptr OnPointDetection(const Point2D& irCursor) const; 
		virtual void OnRectangleSelection(const Rect2D& irRect, std::vector<GraphObject::Ptr>& iorSelectedObjects) const;
		virtual void Draw();
		virtual void GenerateView();
		virtual void GenerateImageMap(ImageMap& iorImageMap); 
		virtual void GenerateMapAndList();
		virtual Int32 GetDataAreaSplitX() const;
		virtual bool GetToolTipData(Int32 iSeriesID, Int32 iGroupID, double& orValue, double& orPercentValue, Int32 iOffset = 0);

	private:

		Int32 hGetGraphMajorType(Int32 iGraphType);

		std::vector<Plot::Ptr> mPlots;	///< Plot objects


	};
}

#endif
