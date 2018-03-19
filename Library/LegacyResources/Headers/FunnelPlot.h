//=======================================================================
//	Filename:		Kernel/SourceCode/ChartEngine/PrivateSource/FunnelPlot.h
//	Date:			2009-04-03
//	Authors:		Bing Huang

//	Copyright (C) 1997-2008 MicroStrategy Incorporated
//	All rights reserved
//=======================================================================

#ifndef MsiChart_FunnelPlot_h
#define MsiChart_FunnelPlot_h

#include "NumberObject.h"
#include "Plot.h"

namespace MsiChart
{
	typedef enum
	{
		FDP_ON_SLICE,
		FDP_OUTSIDE_SLICE
	}EnumFunnelDataLabelsPosition;

	struct GraphFunnelDataLabels
	{
		GraphFunnelDataLabels() : mShowDataValues(true), mDataValuesPosition(FDP_OUTSIDE_SLICE), 
			mShowDataLabels(true), mDataLabelsPosition(FDP_OUTSIDE_SLICE), mShowLeaderLines(true)
		{
		}
		bool							mShowDataValues;
		bool							mShowDataLabels;
		bool							mShowLeaderLines;
		EnumFunnelDataLabelsPosition	mDataValuesPosition;		
		EnumFunnelDataLabelsPosition	mDataLabelsPosition;		
		ChartAdvFormat					mNumberFormat;
	};
	
	struct FunnelSeriesOption
	{
		typedef MSynch::SmartPtr<FunnelSeriesOption> Ptr;
		
		FunnelSeriesOption() : mDetachSlice(0)
		{
		}

		Int32	mDetachSlice;
		GraphFunnelDataLabels	mDataLabels;
	};

	struct FunnelOption
	{
		FunnelOption() : mFunnelTilt(0), mFunnelWidth(0), mFunnelHeight(0), mFunnelExplode(0),
			mFunnelsPerRow(2), mIs2DFunnel(true), mShowFunnelName(true), mShowDataValuesAsPercentage(false),
			mSmartLabelPlacement(true), mShowDataValues(true), 
			mShowDataLabels(false), mDataValuesPosition(FDP_ON_SLICE), mDataLabelsPosition(FDP_ON_SLICE)
		{
		}

		Int32			mFunnelTilt;
		Int32			mFunnelWidth;		
		Int32			mFunnelHeight;
		Int32			mFunnelExplode;
		Int32			mFunnelsPerRow;
		bool			mIs2DFunnel;
		bool			mShowFunnelName;
		bool			mShowDataValuesAsPercentage;
		bool			mSmartLabelPlacement;
		bool			mShowLabelsOnSide;
		bool			mShowDataValues;
		bool			mShowDataLabels;
		EnumFunnelDataLabelsPosition	mDataValuesPosition;		
		EnumFunnelDataLabelsPosition	mDataLabelsPosition;		
		ChartAdvFormat	mGraphNumberFormat;
	};

	//pre-declaration
	class GraphCollectionObject;
	struct ChartAdvFormat;
	class FunnelObject;

	struct FunnelTextLabelAndValue
	{
		typedef MSynch::SmartPtr<FunnelTextLabelAndValue> Ptr;

		FunnelTextLabelAndValue(const FunnelObject* ipFunnel, TextObject::Ptr iLabelPtr, TextObject::Ptr iValuePtr, 
			bool iLabelOnSlice, bool iValueOnSlice, Int32 iSeriesIndex) 
			: mpFunnel(ipFunnel), mSeriesIndex(iSeriesIndex)
		{
			mValueLabelPtr[0] = iLabelPtr;
			mValueLabelPtr[1] = iValuePtr;
			mValueLabelOnSlice[0] = iLabelOnSlice;
			mValueLabelOnSlice[1] = iValueOnSlice;
		}

		const FunnelObject* mpFunnel;
		TextObject::Ptr mValueLabelPtr[2];
		bool mValueLabelOnSlice[2];	
		Int32 mSeriesIndex;
	};


	
	class FunnelPlot : public Plot
	{
	public:
		typedef MSynch::SmartPtr<FunnelPlot> Ptr;

		FunnelPlot(const TripleId& irTripleId, GraphObjectManager* ipManager, const Rect2D& irDataArea);
		
		//virtual functions from GraphObjectManager
		virtual void GenerateMapAndList();
		virtual void GetOptions(Int32 iSeriesId, Int32 iPropertyId, CComVariant* opValue) const;
		virtual void Draw();
		virtual void GenerateView();
		
		//override functions
		virtual GraphObject::Ptr OnPointDetection(const Point2D& irCursor) const; 
		virtual void OnRectangleSelection(const Rect2D& irRect, std::vector<GraphObject::Ptr>& iorSelectedObjects) const;

		virtual bool GetToolTipData(Int32 iSeriesID, Int32 iGroupID, double& orValue, double& orPercentValue, Int32 iOffset = 0);
				
	private:
		
		/**
		Initialize member variables
		*/
		bool hInit();
		/**
		Load plot level properties.
		*/
		void hLoadProperties();

		/**
		Load series level properties.
		@param iSeriesIndex The input series index. 
		*/
		void hLoadSeriesProperties(Int32 iSeriesIndex);

		/**
		Get the data value position information, including smart labels information.
		@param iSeriesIndex The input series index. 
		@return The position information for data values. 
		*/
		EnumDSSGraphDataTextPosition hGetValuePosition(Int32 iSeriesIndex) const;

		/**
		Parse and Set the data value position information, including smart labels information.
		@param iSeriesIndex The input series index. 
		@param iPosition The input position information. 
		*/
		void hSetValuePosition(Int32 iSeriesIndex, EnumDSSGraphDataTextPosition iPosition);

		/**
		Get the data label position information, including smart labels information.
		@param iSeriesIndex The input series index. 
		@return The position information for data labels. 
		*/
		EnumDSSGraphDataTextPosition hGetLabelPosition(Int32 iSeriesIndex) const;

		/**
		Parse and Set the data label position information, including smart labels information.
		@param iSeriesIndex The input series index. 
		@param iPosition The input position information. 
		*/
		void hSetLabelPosition(Int32 iSeriesIndex, EnumDSSGraphDataTextPosition iPosition);	

		void hCreateGroupLabel();

		void hGenerateForGroup(Int32 iGroupId, std::vector<GraphCollectionObject::Ptr>& irFunnelCollection, std::vector<GraphCollectionObject::Ptr>& irLabelCollection, 
			std::vector<GraphCollectionObject::Ptr>& irValueCollection);
		void hCreateLabelsAndValues(Int32 iGroupId, Int32 iSeriesIndex, double& irValue, const FunnelObject* ipFunnel, GraphCollectionObject::Ptr iLabelCollectionPtr, GraphCollectionObject::Ptr iValueCollectionPtr);
		void hCreateLeaderLinesForGroup(Int32 iGroupId, std::vector<GraphCollectionObject::Ptr>& irLeaderLineCollection);
		void hFunnelSmartLabelForGroup(Int32 iGroupId);
		void hFunnelSmartOnSliceForGroup(Int32 iGroupId);
		void hFunnelSmartOutSliceForGroup(Int32 iGroupId);

		void hMoveLabel(Int32 iGroupId, FunnelTextLabelAndValue::Ptr iTextPtr);
		void hGetRectForLabelAndValue(FunnelTextLabelAndValue::Ptr iTextPtr, bool iIsInside, Rect2D& orRect);
		void hAdjustToOutside(Int32 iGroupId, FunnelTextLabelAndValue::Ptr iTextPtr);
		void hSmartYDistance(std::vector<Int32>& iorDistance) const;
		void hFunnelSmartOutSliceForGroupLeftOrRight(Int32 iGroupId, bool iIsLeft);
		void hRelativeMoveOutsideLabel(FunnelTextLabelAndValue::Ptr iTextPtr, Int32 iYOffset);

		//initialize in hLoadProperties() & hLoadSeriesProperties.
		FunnelOption mFunnelOption;
		std::vector<FunnelSeriesOption::Ptr> mFunnelSeriesOptions;

		Int32 mSeriesCount;
		Int32 mGroupCount;

		Int32 mFunnelsPerRowInChart;
		Int32 mNumberRows;

		double mScale;
		double mExplodeRatio;
		double mCotangent;
		
		std::vector<double> mGroupTotals;

		std::vector<Rect2D> mGroupRects;
		std::vector<Rect2D> mFunnelRects;

		Int32 mMaxGroupLabelsHeight;
		Int32 mFunnelHeight;
		Int32 mUpmostUnExplodeWidth;
		Int32 mUpmostFunnelWidth;
		Int32 mDownmostFunnelWidth;
		Int32 mTotalFunnelHeight;	
		Int32 mFunnelHeightMargin;
		Int32 mSpaceBetweenFunnels;

		std::vector<TextObject::Ptr> mGroupLabels;

		std::vector<FunnelTextLabelAndValue::Ptr> mTextForOneGroup;

	};
}

#endif
