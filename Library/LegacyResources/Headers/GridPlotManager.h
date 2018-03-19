/*
 *  GridPlotManager.h
 *  SimpleChart
 *
 *  Created by Liu Xin on 1/15/10.
 *  Copyright 2010 MSTR. All rights reserved.
 *
 */

//====================================================================
//	Filename:		Kernel/SourceCode/IPhoneChartGrid/SimpleChart/CategoryPlotManager.h
//	AUTHOR		:	Xin Liu
//	CREATION	:	12/18/08
//	Copyright (C) MicroStrategy, Inc. 2010
//==============================================================================================


////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                            //
//                                                                                            //
//                                                                                            //
//                          Warning : this class is obsolete.                                 //
//                                                                                            //
//                                                                                            //
//                                                                                            //
////////////////////////////////////////////////////////////////////////////////////////////////



#ifndef MSICHART_GridPlotManager_H
#define	MSICHART_GridPlotManager_H

#include "PlotManager.h"
#include "ValueAxis.h"
#include "CategoryAxis.h"

namespace MsiChart
{
	
	typedef enum EnumRowColumnScenario
	{
		RCS_FIT_TO_WINDOW = 1,
		RCS_FIT_TO_CONTENT,
		RCS_FIXED
	}EnumRowColumnScenario;
	
	typedef struct ColumnTreeGenerationInfo
	{
		Int32 mCurrentLeafCount;
		Int32 mCurrentStartX;
		std::string mCurrentString;
	}ColumnTreeGenerationInfo;
	
	typedef struct ColumnTextsForEachLevel
	{
		std::vector<TextObject::Ptr> mTexts;
		std::vector<Int32> mLeafCounts;
	}ColumnTextsForEachLevel;
	
	
	/*typedef struct 
	{
		Int32 mItemCount;	// store how many labels with the same text will be generated
		Int32 mStartX;		// store the start position of the next label
		TextObject::Ptr mPreTextPtr; //store the text information
	};*/
	
	
	/**
	 GridPlotManager class is used to support Grid mini charts
	 */
	class GridPlotManager: public PlotManager
	{
	public:
		typedef MSynch::SmartPtr<GridPlotManager> Ptr;
		
		GridPlotManager(const TripleId& irTripleId, GraphObjectManager* ipManager, const Rect2D& irPlotArea);
		virtual ~GridPlotManager();
		
		//Re-implemention the virtual functions inherited from GraphObjectManager
		virtual GraphObject::Ptr OnPointDetection(const Point2D& irCursor) const; 
		virtual void Draw();
		virtual void GenerateImageMap(ImageMap& iorImageMap); 
		virtual void GenerateMapAndList(); 	
		
		virtual Int32 GetDataAreaSplitX() const;
		
		Int32 GetDataAreaSplitY() const;
		
		virtual void OnZooming(Int32 iOldStart, Int32 iOldEnd, Int32 iNewStart, Int32 iNewEnd, bool iIsVertical);
		
		virtual bool GetToolTipData(Int32 iSeriesID, Int32 iGroupID, double& orValue, double& orPercentValue, Int32 iOffset = 0);
		
	protected:
		/**
		 This function loads some of ABLOptions, which used to initiate CategoryPlot, ValueAxis and CategoryAxis correctly.
		 */
		virtual void LoadProperties();
		
	private:
		//private methods
		void hGenerateHeadersAndGrids();
		void hGenerateRowHeadersAndExtraColumn();
		void hGenerateColumnHeadersAndGrids();
		void hGenerateOneRisers(Int32 iRowIndex, Int32 iColIndex, double iMin, double iMax, Int32 iOriginalX, double iValue, const Rect2D& irGridRect);
		
		Int32 hGetColumnEstimateMaximumWidth(std::vector<TextObject::Ptr>& irTexts) const;
		Int32 hGenerateExtraColumn(Int32& iorStartX);

		
	protected:
		//data members
		EnumDSSGraphType		mGraphType;
		Int32					mDataAreaSplitX;
		Int32					mDataAreaSplitY;
		Rect2D					mDataArea;
		
		EnumRowColumnScenario mRowScenario;
		EnumRowColumnScenario mColScenario;
				
		Int32 mRowHeight; //currently we support only one height for all rows, decided by the font size
		std::vector<Int32> mColumnWidth;
		std::vector<Int32> mHeaderHeight; // we still need a vector to get all the head height to arrange the layout
		
		
		Int32 mRowHeaderStartY;
		Int32 mColHeaderStartX;
		
		bool mMergeCells;
		bool mMergeColumnCells;
		bool mLongNames;
		bool mTemplateShowRowHeaders;
		bool mTemplateShowColHeaders;
		bool mLockRowHeaders;
		bool mLockColumnHeaders;
		bool mTitleOverlapViewMode;

		bool mIsShowExtraColumn;
		
		Int32 mRowCount;
		Int32 mColumnCount;
		
		Int32 mRowTreeDepth;
		Int32 mColTreeDepth;
		
		Int32 mRowTreeActualDepth;
		
		bool mShowRisers;
	};
}

#endif

