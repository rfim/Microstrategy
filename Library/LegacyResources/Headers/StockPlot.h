//====================================================================
//	Filename:		Kernel/SourceCode/ChartEngine/PrivateSource/StockPlot.h
//	AUTHOR		:	Xin Liu
//	CREATION	:	4/08/09
//	Copyright (C) MicroStrategy, Inc. 2009
//==============================================================================================

#ifndef MSICHART_STOCKPLOT_H
#define	MSICHART_STOCKPLOT_H


#include "CategoryPlot.h"
#include "NumberObject.h"
#include "ValueAxis.h"
#include "CategoryAxis.h"

namespace MsiChart
{
	enum EnumStockValueAxisIndex
	{
		SVAI_Y2,
		SVAI_Y3,
		SVAI_Y4,
	};

	struct StockMetricOptions
	{
		StockMetricOptions()
			: mStockMetricShow(false)
			, mStockMetricLayout(DssGraphMetricVerticalLine)
	//		, mStockMetricValue(L"")
			, mOffsetInDataset(-1)
		{}

		bool			mStockMetricShow;
		unsigned char	mStockMetricLayout;
	//	MBase::String	mStockMetricValue;
		Int32			mOffsetInDataset;
	};

	struct StockOptions
	{
		StockOptions()
			: mGapWidth(40)
			, mStockLayout(DssGraphStockLayoutDefault)
			, mShowOpen(false)
			, mShowClose(false)
			, mTextPosition(DssGraphDTPositionOutMax)
			, mStockDataValueType(DssGraphDataValueHigh)
			, mStockRiserDataGroup(0)
		{}

		Int32			mGapWidth;		///< Gap width between groups
		unsigned char	mStockLayout;
		bool			mShowOpen;
		bool			mShowClose;
		EnumDSSGraphDataTextPosition	mTextPosition;
		EnumDSSGraphStockDataValueType	mStockDataValueType;
		Int32			mStockRiserDataGroup;
		StockMetricOptions	mStockMetricOptions[3];
	};

	struct StockSeriesOptions
	{
		typedef MSynch::SmartPtr<StockSeriesOptions> Ptr;

		StockSeriesOptions()
			: mShowDataLabels(false)
		{}

		bool						mShowDataLabels;	
		GraphTrendLineOptions		mTrendLineOptions;
		ChartAdvFormat				mGraphNumberFormat;
	};

	class StockPlot: public CategoryPlot
	{
	public:
		typedef MSynch::SmartPtr<StockPlot> Ptr;

		StockPlot(const TripleId& irTripleId, GraphObjectManager* ipManager, const Rect2D& irDataArea);
		~StockPlot();

		//Re-implemention the virtual functions inherited from GraphObjectManager
	//	virtual GraphObject::Ptr OnPointDetection(const Point2D& irCursor) const; 
	//	virtual void OnRectangleSelection(const Rect2D& irRect, std::vector<GraphObject::Ptr>& iorSelectedObjects) const;
	//	virtual void Draw();
	//	virtual void GenerateImageMap(ImageMap& iorImageMap); 
		virtual void GenerateMapAndList(); 
		virtual void GetOptions(Int32 iSeriesId, Int32 iPropertyId, CComVariant* opValue) const;

		//Re-implemention the virtual functions inherited from CategoryPlot
		virtual void Init();
		virtual void GetMinMaxValue(EnumValueAxis iValueAxis, double& iorMin, double& iorMax, bool& iorAreInitialized);
		virtual void AddValueAxis(ValueAxis::Ptr iValueAxisPtr);
		virtual void SetCategoryAxis(CategoryAxis::Ptr iCategoryAxisPtr);

	private:
		/**
		Load plot level properties.
		*/	
		void hLoadProperties();

		/**
		Load series level properties.
		@param iInfoIndex The index for mLineSeiresOptions.
		*/
		void hLoadSeriesProperties(Int32 iSeriesId);

		/**
		*/
		void hInitializeCollections();

		/**
		*/
		void hGenerateOneSeries(Int32 iSeriesIndex);

		/**
		*/
		void hGenerateOneRiser(Int32 iSeriesIndex, Int32 iGroupId);

		void hGenerateOneRiserForY2Y3Y4(Int32 iSeriesIndex, Int32 iGroupId, EnumStockValueAxisIndex iValueAxis
			, Int32 iRiserStartX, Int32 iRiserSizeX, GraphCollectionObject* ipCollection);

		void hGetRiserStartPosAndSize(Int32 iGroupId, Int32& orStart, Int32& orSize, Int32& orLeftGapWidth, Int32 &orRightGaphWidth);

		Int32 hGetSideRiserHeight();

		void hGetDataPointsPolyLine(Int32 iSeriesIndex, std::vector<std::vector<Point2D> >& orVecDataPolyLine, bool iIncludeBase = false);

		void hGetDataPointsPolyLineForY2Y3Y4(Int32 iSeriesIndex, std::vector<std::vector<Point2D> >& orVecDataPolyLine
			, EnumStockValueAxisIndex iValueAxis, bool iIncludeBase = false);

		/**
		Generate one data label.
		@param iSeriesIndex Specifies the series index.
		@param iGroupId Specifies the group id.
		@param irValue The value of the data label.
		@param iDataLabelCollectionPtr The data labels collection.
		@return A text object delegating the data label.
		*/
		TextObject::Ptr hCreateDataLabel(Int32 iSeriesIndex, Int32 iGroupId, double iValue, GraphCollectionObject::Ptr iDataLabelCollectionPtr);

		void hGetTrendlineData(Int32 iSeriesIndex, std::vector<double>& orVecX, std::vector<double>& orVecY);

		void hGenerateTrendline();

		Int32 hGetSeriesIdForY2Y3Y4Risers(Int32 iSeriesIndex, EnumStockValueAxisIndex iValueAxis);

		//private members
		ValueAxis::Ptr mY1AxisPtr;			///< Y1 value axis.
		ValueAxis::Ptr mY2AxisPtr;			///< Y2 value axis.
		ValueAxis::Ptr mY3AxisPtr;			///< Y3 value axis.
		ValueAxis::Ptr mY4AxisPtr;			///< Y4 value axis.
		CategoryAxis::Ptr mO1AxisPtr;		///< Category axis.

		StockOptions mStockOptions;
		std::vector<StockSeriesOptions::Ptr> mStockSeriesOptions;

		Int32 mSeriesCount;
		Int32 mGroupCount;
		bool mIsLineRiser;
		std::vector<GraphCollectionObject::Ptr> mRiserCollections;
		std::vector<GraphCollectionObject::Ptr> mDataLabelCollections;
		std::map<Int32, GraphCollectionObject::Ptr> mY2Y3Y4RiserCollections;
	};
}

#endif