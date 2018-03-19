//=======================================================================
//	Filename:		Kernel/SourceCode/ChartEngine/PrivateSource/GanttPlot.h
//	Date:			2009-02-05
//	Authors:		Bing Huang

//	Copyright (C) 1997-2008 MicroStrategy Incorporated
//	All rights reserved
//=======================================================================

#ifndef MsiChart_GanttPlot_h
#define MsiChart_GanttPlot_h

#include "CategoryPlot.h"
#include "CategoryAxis.h"
#include "ValueAxis.h"

namespace MsiChart
{
	//enumerations and structs
	struct GanttSeriesOptions
	{
		typedef MSynch::SmartPtr<GanttSeriesOptions> Ptr;

		GanttSeriesOptions() : mShowDataLabels(false)
		{}

		bool						mShowDataLabels;	
		GraphTrendLineOptions		mTrendLineOptions;
		ChartAdvFormat				mGraphNumberFormat;
	};

	class GanttPlot : public CategoryPlot
	{
	public:
		typedef MSynch::SmartPtr<GanttPlot> Ptr;

		GanttPlot(const TripleId& irTripleId, GraphObjectManager* ipManager, const Rect2D& irDeviceRect);
		
		//virtual functions from GraphObjectManager
		virtual void GenerateMapAndList();
		virtual void GetOptions(Int32 iSeriesId, Int32 iPropertyId, CComVariant* opValue) const;
		virtual void Draw();
		virtual void GenerateView();
		
		//override functions
		virtual GraphObject::Ptr OnPointDetection(const Point2D& irCursor) const; 
		virtual void OnRectangleSelection(const Rect2D& irRect, std::vector<GraphObject::Ptr>& iorSelectedObjects) const;

		//pure virtual functions from CategoryPlot
		virtual void Init();
		virtual void GetMinMaxValue(EnumValueAxis iValueAxis, double& iorMin, double& iorMax, bool& iorAreInitialized);
		virtual void AddValueAxis(ValueAxis::Ptr iValueAxisPtr);
		virtual void SetCategoryAxis(CategoryAxis::Ptr iCategoryAxisPtr);
				
	protected:

	private:
		/**
		Load gantt chart properties.
		*/	
		void hLoadProperties();

		/**
		Load series level properties.
		@param iSeriesIndex The input series index which is the index of mSeriesId.
		*/
		void hLoadSeriesProperties(Int32 iSeriesIndex);

		/**
		Create risers and labels for all groups and series.
		@param irRiserCollection The input riser collection objects.
		@param irTextCollection The input text collection objects.
		*/
		void hCreateRisersAndLabels(std::vector<GraphCollectionObject::Ptr>& irRiserCollection, std::vector<GraphCollectionObject::Ptr>& irTextCollection);

		/**
		Calculate the bar width and spaces between bars and space between groups.
		*/
		void hCalBarWidthAndSpaces();

		/**
		Get the start position and the size in category axis direction of a riser defined by series ID and group ID.
		Since gantt will always be horizontal chart, the direction is actually Y direction.
		@param iSeriesIndex The input series index in mSeriesId.
		@param iGroupId The input group ID.
		@param orStart The output start position.
		@param orSize The output size.
		*/
		void hGetRiserStartPosAndSize(Int32 iSeriesIndex, Int32 iGroupId, Int32& orStart, Int32& orSize);

		/**
		Used to get trend line X data.
		We use a new method to get the X data instead of obtaining from Category Axis to improve the accuracy.
		@param iSeriesIndex The input series index in mSeriesId.
		@param iGroupId The input group ID.
		@param orPosition The output position in double.
		*/
		void hGetLogicalMiddlePosition(Int32 iSeriesIndex, Int32 iGroupId, double& orPosition) const;

		/**
		Get the rectangle for a riser specified by series and group ID.
		@param iSeriesIndex The input series index in mSeriesId.
		@param iGroupId The input group ID.
		@param orRect The output rectangle.
		*/
		bool hGetRiserRect(Int32 iSeriesIndex, Int32 iGroupId, Rect2D& orRect);

		/**
		Create TrendLine for all series.
		*/
		void hCreateAllSeriesTrendLine();

		/**
		Get the trend line point of a riser specified by series and group ID.
		@param iSeriesIndex The input series index in mSeriesId.
		@param iGroupId The input group ID.
		@param orPoint The output point.
		*/
		bool hGetTrendLinePoint(Int32 iSeriesIndex, Int32 iGroupId, Point<double>& orPoint);

		/**
		Get the two data values related to one riser.
		@param iSeriesIndex The input series index in mSeriesId.
		@param iGroupId The input group ID.
		@param iorStart The start value of the riser.
		@param iorLength The length of the riser.
		@return true if get two values successfully, false if failed.
		*/
		bool hGetStartAndLength(Int32 iSeriesIndex, Int32 iGroupId, double& iorStart, double& iorLength) const;

		CategoryAxis::Ptr mCategoryAxisPtr;
		ValueAxis::Ptr mValueAxisPtr;
		double mBarWidth;
		double mSpaceBetweenRiser;
		double mSpaceBetweenGroup;
		double mGroupSize;
		Int32 mSeriesCount;
		Int32 mGroupCount;
		Int32 mOverlap;
		Int32 mGapWidth;
		bool mUseDepth;
		Int32 mDepth;
		Int32 mDepthDirection;
		bool mShowLabel;
		bool mShowValue;
		EnumDSSGraphDataTextPosition			mTextPosition;
		ChartAdvFormat							mTotalNumberFormat;
		std::vector<GanttSeriesOptions::Ptr>	mGanttSeriesOptions;
	};

}

#endif
