//=======================================================================
//	Filename:		Kernel/SourceCode/ChartEngine/PrivateSource/BoxPlot.h
//	Date:			2009-03-02
//	Authors:		Bing Huang

//	Copyright (C) 1997-2008 MicroStrategy Incorporated
//	All rights reserved
//=======================================================================

#ifndef MsiChart_BoxPlot_h
#define MsiChart_BoxPlot_h

#include "CategoryPlot.h"
#include "CategoryAxis.h"
#include "ValueAxis.h"

namespace MsiChart
{
	//enumerations and structs
	struct BoxSeriesOptions
	{
		typedef MSynch::SmartPtr<BoxSeriesOptions> Ptr;

		BoxSeriesOptions() : mShowDataLabels(false), mMarkerShape(0), mMarkerSize(50)
		{}

		bool			mShowDataLabels;
		unsigned char	mMarkerShape;
		Int32			mMarkerSize;
		ChartAdvFormat				mGraphNumberFormat;
		
	};

	class BoxPlot : public CategoryPlot
	{
	public:
		typedef MSynch::SmartPtr<BoxPlot> Ptr;

		BoxPlot(const TripleId& irTripleId, GraphObjectManager* ipManager, const Rect2D& irDeviceRect);
		
		//virtual functions from GraphObjectManager
		virtual void GenerateMapAndList();
		virtual void GetOptions(Int32 iSeriesId, Int32 iPropertyId, CComVariant* opValue) const;
		
		//pure virtual functions from CategoryPlot
		virtual void Init();
		virtual void GetMinMaxValue(EnumValueAxis iValueAxis, double& iorMin, double& iorMax, bool& iorAreInitialized);
		virtual void AddValueAxis(ValueAxis::Ptr iValueAxisPtr);
		virtual void SetCategoryAxis(CategoryAxis::Ptr iCategoryAxisPtr);
				
	protected:

	private:
		/**
		Load Box chart properties.
		*/	
		void hLoadProperties();

		/**
		Load series level properties.
		@param iSeriesIndex The input series index which is the index of mSeriesId.
		*/
		void hLoadSeriesProperties(Int32 iSeriesIndex);

		/**
		Calculate the bar/marker width and spaces between markers and markers in different gorups.
		*/
		void hCalBarMarkerWidthAndSpaces();

		/**
		Get the start position and the size in category axis direction of a riser defined by series ID and group ID.
		Since boxplot will always be vertical chart, the direction is actually X direction.
		@param iSeriesIndex The input series index in mSeriesId.
		@param iGroupId The input group ID.
		@param orStart The output start position.
		@param orSize The output size.
		*/
		void hGetRiserStartPosAndSize(Int32 iSeriesIndex, Int32 iGroupId, Int32& orStart, Int32& orSize);

		/**
		*/
		//bool hGetRiserRect(Int32 iSeriesIndex, Int32 iGroupId, Rect2D& orRect);

		/**
		Create labels for the first five number if exist and add them to map and collection.
		@param iGroupId The input group ID.
		@param iFiveNumbers The input numbers.
		@param iIsNull Indicate whether the corresponding number in iFiveNumbers is NULL or not.
		@param irTextCollection The input collection.
		*/
		void hCreateFiveNumberLabels(Int32 iGroupId, double iFiveNumbers[], bool iIsNull[], std::vector<GraphCollectionObject::Ptr>& irTextCollection);

		/**
		Create the box plot boday for one group.
		@param iGroupId The input group ID.
		@param irRiserCollection The input riser collection objects.
		@param irLineCollection The input line collection objects
		@param irTextCollection The input text collection objects.
		*/
		void hCreateBoxPlotBodyForGroup(Int32 iGroupId, std::vector<GraphCollectionObject::Ptr>& irRiserCollection, std::vector<GraphCollectionObject::Ptr>& irLineCollection, std::vector<GraphCollectionObject::Ptr>& irTextCollection);

		/**
		Create markers and their labels, add markers to map, list and collection, add labels to map and collection.
		@param irRiserCollection The input riser collection objects.
		@param irTextCollection The input text collection objects.
		*/
		void hCreateMarkersAndLabels(std::vector<GraphCollectionObject::Ptr>& irRiserCollection, std::vector<GraphCollectionObject::Ptr>& irTextCollection);

		/**
		As the name, Create labels, add to map and collection, but not list.
		@param iSeriesIndex The input series index in mSeriesId.
		@param iGroupId The input group ID.
		@param irValue The input value.
		@param irTopRight The top or right point according to the new center point.
		@param irBottomLeft The bottom or left point according to the new center point.
		@param iIsReverse Indicate whether reverse, two things will affect this: Axis reverse and data negative or positive.
		@param iCollectionPtr The input collection.
		*/
		void hCreatelabels(Int32 iSeriesIndex, Int32 iGroupId, const double& irValue, const Point2D& irTopRight, const Point2D& irBottomLeft, bool iIsReverse, GraphCollectionObject::Ptr iCollectionPtr);

		/**
		For BoxPlot the first five number are used to create the main body.
		This function will prepare the five numbers.
		@param iGroupId The input group ID.
		@param ioFiveNumbers The input and output five numbers.
		@param ioIsNull The array indicate whether the data is NULL or not.
		*/
		void hGetFiveNumber(Int32 iGroupId, double ioFiveNumbers[], bool ioIsNull[]);

		/**
		As the name, create one rectangle object and add to collection, map and list.
		@param iStartX The start position in X direction.
		@param iSizeX The width of the rectangle.
		@param irStartYValue The start value of Y direction.
		@param irEndYValue The end value of Y direction ,combined with irStartYValue to get the Y direction information.
		@param iSeriesId The input series ID.
		@param iGroupId The input group ID.
		@param iCollectionPtr The input collection.
		*/
		void hCreateOneRectangle(Int32 iStartX, Int32 iSizeX, const double& irStartYValue, const double& irEndYValue, Int32 iSeriesId, Int32 iGroupId, GraphCollectionObject::Ptr iCollectionPtr);

		/**
		Create a straight line object and add to collection, map and list.
		@param irTripleId The input triple ID.
		@param irStart The start point of the line.
		@param irEnd The end point of the line.
		@param iCollectionPtr The input collection.
		*/
		void hCreateLine(const TripleId& irTripleId, const Point2D& irStart, const Point2D& irEnd, GraphCollectionObject::Ptr iCollectionPtr);

		CategoryAxis::Ptr mCategoryAxisPtr;
		ValueAxis::Ptr mValueAxisPtr;
		EnumDSSBoxPlotTailShape mTailShape;
		double mBarWidth;
		double mMarkerWidth;
		double mSpaceBetweenMarker;
		double mSpaceBetweenGroupBar;
		double mSpaceBetweenGroupMarker;
		Int32 mSeriesCount;
		Int32 mGroupCount;
		Int32 mOverlap;
		Int32 mGapWidth;
		bool mShowLabel;
		bool mShowValue;
		EnumDSSGraphDataTextPosition			mTextPosition;
		ChartAdvFormat							mTotalNumberFormat;
		std::vector<BoxSeriesOptions::Ptr>	mBoxSeriesOptions;
	};

}

#endif