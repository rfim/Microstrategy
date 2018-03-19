/*
 *  GraphMatrixPlotManager.h
 *  SimpleChart
 *
 *  Created by Liu Xin on 1/15/10.
 *  Copyright 2010 MSTR. All rights reserved.
 *
 */

////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                            //
//                                                                                            //
//                                                                                            //
//                          Warning : this class is obsolete.                                 //
//                                                                                            //
//                                                                                            //
//                                                                                            //
////////////////////////////////////////////////////////////////////////////////////////////////


//====================================================================
//	Filename:		Kernel/SourceCode/IPhoneChartGrid/SimpleChart/CategoryPlotManager.h
//	AUTHOR		:	Xin Liu
//	CREATION	:	1/15/10
//	Copyright (C) MicroStrategy, Inc. 2010
//==============================================================================================

#ifndef MSICHART_GraphMatrixPlotManager_H
#define	MSICHART_GraphMatrixPlotManager_H

#include "PlotManager.h"
#include "ValueAxis.h"
#include "CategoryAxis.h"

namespace MsiChart
{
	
	/**
	 GraphMatrixPlotManager class is used to support Grid mini charts
	 */
	class GraphMatrixPlotManager: public PlotManager
	{
	public:
		typedef MSynch::SmartPtr<GraphMatrixPlotManager> Ptr;
		
		GraphMatrixPlotManager(const TripleId& irTripleId, GraphObjectManager* ipManager, const Rect2D& irPlotArea);
		virtual ~GraphMatrixPlotManager();
		
		//Re-implemention the virtual functions inherited from GraphObjectManager
		virtual GraphObject::Ptr OnPointDetection(const Point2D& irCursor) const; 
		virtual void Draw();
		virtual void GenerateView();
		virtual void GenerateImageMap(ImageMap& iorImageMap); 
		virtual void GenerateMapAndList(); 	
		
		virtual Int32 GetDataAreaSplitX() const;
		
		virtual void OnZooming(Int32 iOldStart, Int32 iOldEnd, Int32 iNewStart, Int32 iNewEnd, bool iIsVertical);
		
		virtual bool GetToolTipData(Int32 iSeriesID, Int32 iGroupID, double& orValue, double& orPercentValue, Int32 iOffset = 0);
		
	protected:
		/**
		 This function loads some of ABLOptions, which used to initiate CategoryPlot, ValueAxis and CategoryAxis correctly.
		 */
		virtual void LoadProperties();
		
	private:
		//private methods
		void hGenerateHeaders();
		void hGenerateRowHeaders();
		void hGenerateColumnHeaders();
		void hGenerateGrids();
		void hGenerateLabels(Rect2D& iorRect);
		void hGeneratePies();
		
		void hGenerateLine(CategoryAxis::Ptr lCategoryAxisPtr, ValueAxis::Ptr lValueAxisPtr, const Rect2D& irRect);
		void hGenerateRiser(CategoryAxis::Ptr lCategoryAxisPtr, ValueAxis::Ptr lValueAxisPtr, const Rect2D& irRect);
		Int32 hGetXValue(Int32 iGroupIndex, Int32 iGroupCount, const Rect2D& irRect);

		
	protected:
		//data members
		EnumDSSGraphType		mGraphType;
		Int32					mDataAreaSplitX;
		Rect2D					mDataArea;
		Int32 mRowHeaderWidth;
		Int32 mRowHeaderHeight;
		Int32 mColumnHeaderWidth;
		Int32 mColumnHeaderHeight;
		Int32 mLabelRectWidth;
		Int32 mRowHeaderCount;
		Int32 mColumnHeaderCount;
		std::vector<CategoryPlot::Ptr> mCategoryPlots;	///< CategoryPlot objects
		std::vector<CategoryAxis::Ptr> mVecO1Axis;		///< Category axes
		std::vector<ValueAxis::Ptr> mVecY1Axis;			///< Y1 value axes		
		
		std::vector<PiePlot::Ptr> mPiePlots;
		std::vector<GraphObject::Ptr> mPieFrames;
		std::vector<Legend::Ptr> mLegends;
		
		std::vector<GraphObject::Ptr> mDataLines;
	};
}

#endif

