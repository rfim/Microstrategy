//=======================================================================
//	Filename:		Kernel/SourceCode/ChartEngine/PrivateSource/PiePlot.h
//	Date:			2008-05-21
//	Authors:		Bing Huang

//	Copyright (C) 1997-2008 MicroStrategy Incorporated
//	All rights reserved
//=======================================================================

#ifndef MsiChart_PiePlot_h
#define MsiChart_PiePlot_h

#include "PDCHeader/PDCvector"

#include "NumberObject.h"
#include "PolyLineObject.h"
#include "PieSliceObject.h"
#include "Plot.h"

namespace MsiChart
{
	typedef enum
	{
		PL_PIE,
		PL_RING
	}EnumPieLayout;

	typedef enum
	{
		LOPie_ABSOLUTE,
		LOPie_CLUSTER,
		LOPie_STACKED,
		LOPie_PERCENT
	}EnumGMPieLayout;
    
	typedef enum
	{
		PDP_ON_SLICE,
		PDP_OUTSIDE_SLICE
	}EnumPieDataLabelsPosition;

	typedef enum
	{
		RR_NORMAL_ONSLICE,
		RR_SMART_ONSLICE,
		RR_NORMAL,
		RR_NORMAL_OUTSIDE,
		RR_SMART_OUTSIDE
	}EnumRadiusRatio;

	struct GraphPieDataLabels
	{
		GraphPieDataLabels() : mShowDataValues(true), mDataValuesPosition(PDP_OUTSIDE_SLICE), 
			mShowDataLabels(false), mDataLabelsPosition(PDP_ON_SLICE), mShowLeaderLines(false)
		{
		}
		bool						mShowDataValues;
		bool						mShowDataLabels;
		bool						mShowLeaderLines;
		EnumPieDataLabelsPosition	mDataValuesPosition;		
		EnumPieDataLabelsPosition	mDataLabelsPosition;		
		ChartAdvFormat				mNumberFormat;
	};
	
	struct GraphSeriesPie
	{
		typedef MSynch::SmartPtr<GraphSeriesPie> Ptr;
		
		GraphSeriesPie() : mDetachSlice(0), mDeleteSlice(false)
		{
		}

		Int32	mDetachSlice;
		bool	mDeleteSlice;
		GraphPieDataLabels	mDataLabels;
	};

	struct GraphOptionsPie
	{
		GraphOptionsPie() : mLayout(PL_PIE), mPieTilt(0), mPieDepth(0), mPieRotation(90),    // Rotation = 90, slice start at 12 o'clock
			mPiesPerRow(2), mPieSizeProportional(false), mPieTiltOn(false),  
			mIs2DCircular(true), mShowPieName(true), mShowDataValuesAsPercentage(false),
			mRingPieFactor(70), mShowRingPieTotal(false), mSmartLabelPlacement(true),
			mShowLabelsOnSide(false), mPieRestore(false), mShowDataValues(true), 
			mShowDataLabels(false), mDataValuesPosition(PDP_OUTSIDE_SLICE), mDataLabelsPosition(PDP_ON_SLICE), mGMPieLayout(LOPie_ABSOLUTE)
		{
		}

		EnumPieLayout	mLayout;
		Int32			mPieTilt;
		Int32			mPieDepth;		
		Int32			mPieRotation;
		Int32			mPiesPerRow;
		Int32			mRingPieFactor;
		bool			mPieTiltOn;
		bool			mPieSizeProportional;
		bool			mIs2DCircular;
		bool			mShowPieName;
		bool			mShowDataValuesAsPercentage;
		bool			mShowRingPieTotal;
		bool			mSmartLabelPlacement;
		bool			mShowLabelsOnSide;
		bool			mPieRestore;
		bool			mShowDataValues;
		bool			mShowDataLabels;
		EnumPieDataLabelsPosition	mDataValuesPosition;		
		EnumPieDataLabelsPosition	mDataLabelsPosition;		
		ChartAdvFormat	mGraphNumberFormat;
        EnumGMPieLayout mGMPieLayout;
	};

	//pre-declaration
	class GraphCollectionObject;
	class PieSliceObject;

	struct TextLabelAndValue
	{
		typedef MSynch::SmartPtr<TextLabelAndValue> Ptr;

		TextLabelAndValue(const PieSliceObject* ipSlice, TextObject::Ptr iLabelPtr, TextObject::Ptr iValuePtr, 
			bool iLabelOnSlice, bool iValueOnSlice, Int32 iSeriesIndex) 
			: mpSlice(ipSlice), mSeriesIndex(iSeriesIndex)
		{
			mValueLabelPtr[0] = iLabelPtr;
			mValueLabelPtr[1] = iValuePtr;
			mValueLabelOnSlice[0] = iLabelOnSlice;
			mValueLabelOnSlice[1] = iValueOnSlice;
		}

		const PieSliceObject* mpSlice;
		TextObject::Ptr mValueLabelPtr[2];
		bool mValueLabelOnSlice[2];	
		Int32 mSeriesIndex;
	};

	struct TextHeightAndCount
	{
		//typedef MSynch::SmartPtr<TextHeightAndCount> Ptr;

		TextHeightAndCount(Int32 iTextHeight, Int32 iLineCount, TextObject::Ptr iTextPtr) : mTextHeight(iTextHeight)
			, mLineCount(iLineCount), mTextPtr(iTextPtr)
		{}

		//friend bool GreaterThan(const TextHeightAndCount& irTextHeightAndCountA, const TextHeightAndCount& irTextHeightAndCountB);

		Int32 mTextHeight;
		Int32 mLineCount;
		TextObject::Ptr mTextPtr;
	};

	bool GreaterThan(const TextHeightAndCount& irTextHeightAndCountA, const TextHeightAndCount& irTextHeightAndCountB);
	
	class PiePlot : public Plot
	{
	public:
		typedef MSynch::SmartPtr<PiePlot> Ptr;

		PiePlot(const TripleId& irTripleId, GraphObjectManager* ipManager, const Rect2D& irDeviceRect);
		
		//virtual functions from GraphObjectManager
		virtual void GenerateMapAndList();
		virtual void GetOptions(Int32 iSeriesId, Int32 iPropertyId, CComVariant* opValue) const;
		virtual void Draw();
		virtual void GenerateView();
		
		//override functions
		virtual GraphObject::Ptr OnPointDetection(const Point2D& irCursor) const; 
		virtual void OnRectangleSelection(const Rect2D& irRect, std::vector<GraphObject::Ptr>& iorSelectedObjects) const;

		virtual bool GetToolTipData(Int32 iSeriesID, Int32 iGroupID, double& orValue, double& orPercentValue, Int32 iOffset = 0);
		virtual bool GetGroupSpace(std::vector<Int32>& orVect, Int32 iGroupIndex);		
				
		void GetOptions(Int32 iPropertyId, CComVariant* opValue) const;

		/**
		Get the number of pies in one row.
		@return The mPiesPerRowInChart member variable.
		*/
		Int32 GetPiesPerRow() const;

		/**
		Get the number of rows in chart.
		@return The mNumberRows member variable.
		*/
		Int32 GetNumberOfRows() const;

		/**
		Get the data value position for one specified series, only on or outside slice.
		This function mainly called by PieSliceObject.
		@param iSeriesIndex The input series index. 
		@return The data value position.
		*/
		const EnumPieDataLabelsPosition GetValuePosition(Int32 iSeriesIndex) const;

		/**
		Get the data label position for one specified series, only on or outside slice.
		This function mainly called by PieSliceObject.
		@param iSeriesIndex The input series index. 
		@return The data label position.
		*/
		const EnumPieDataLabelsPosition GetLabelPosition(Int32 iSeriesIndex) const;

		double GetSmartLabelRadius(Int32 iGroupId) const;
		
		/// Provide position information for tooltip display on iPad.
		bool GetDataPoint(Int32 iSeriesId, Int32 iGroupId, Point2D& orDataPoint);

	protected:
		virtual void SetGraphType(EnumDSSGraphType iType);

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

		/**
		Get the drawing order for one group.
		@param iGroupId The input group ID.
		@param irAngles The input angles.
		@param orDrawingOrderIndex The drawing order of this group, in series index.
		@param oDividedIndex The output divided index.
		@param oDividedAngle The output divided angle.
		*/
		void hGetDrawingOrder(std::vector<Int32>& irAngles, std::vector<Int32>& orDrawingOrderIndex, Int32& orDividedIndex, Int32& orDividedAngle) const;

		/**
		Load plot level properties.
		*/
		void hLoadProperties();

		/**
		Load series level properties.
		@param iSeriesIndex The input series index. 
		*/
		void hLoadSeriesProperties(Int32 iSeriesIndex);

		virtual bool hDoInitialize();

		virtual void hGenerateMapAndListForGroup(Int32 iGroupId, std::vector<GraphCollectionObject::Ptr>& iorSliceCollection, std::vector<GraphCollectionObject::Ptr>& iorTextLabelCollection, 
		std::vector<GraphCollectionObject::Ptr>& iorTextValueCollection);

		/**
		Adjust big font outside.
		Adjust mIsAllLabelsOnSlice flag.
		*/
		bool hAdjustBigFontOutSideForGroup(Int32 iGroupId);

		void hGetAngleInfoForGroup(Int32 iGroupId, Int32& orValidSeriesNumber, Int32& orValidSeriesIndex, std::vector<Int32>& orAngles);

		virtual const PieSliceObject* hCreateSlice(Int32 iSeriesIndex, Int32 iGroupId, Int32 iStartAngle, Int32 iEndAngle, bool iIsOneSeries, GraphCollectionObject::Ptr iSliceCollectionPtr);

		void hCreateLabelsAndValues(Int32 iSeriesIndex, Int32 iGroupId, const PieSliceObject* ipSlice, GraphCollectionObject::Ptr iLabelCollectionPtr, GraphCollectionObject::Ptr iValueCollectionPtr);

		bool hGetDisplayValue(Int32 iSeriesIndex, Int32 iGroupId, double& orValue);

		TextObject::Ptr hCreateValue(Int32 iSeriesIndex, Int32 iGroupId, double& irValue, GraphCollectionObject::Ptr ioTextValueCollectionPtr);

		TextObject::Ptr hCreateLabel(Int32 iSeriesIndex, Int32 iGroupId, GraphCollectionObject::Ptr ioTextLabelCollectionPtr);

		void hCreateRingPieTotal();

		void hCreateGroupLabel();

		virtual void hGetInitRadius();

		void hSmartAndSideAdjustForGroup(Int32 iGroupId);

		void hSmartAndSide(Int32 iGroupId);

		void hSmartLabels(Int32 iGroupId);

		void hSideOutsideSlice(Int32 iGroupId);

		void hSmartLabelsOnSlice(Int32 iGroupId);

		void hSmartLabelsOutsideSlice(Int32 iGroupId);

		void hLabelsOnSide(Int32 iGroupId);

		void hLabelsOnSliceSide(Int32 iGroupId);

		void hSmartAdjustY(Int32 iGroupId);

		void hLabelsOnSideForLR(Int32 iGroupId, bool iIsLeft);

		void hGetRectsAndIndexLR(Int32 iGroupId, bool iIsLeft, std::vector<Rect2D>& orRect, std::vector<Int32>& orIndex, Int32& orHeightTotal);

		void hSmartLabelsOnSliceLR(Int32 iGroupId, bool iIsLeft, double iRadiusA, double iRadiusB);
		
		void hSmartLabelsOutsideSliceLR(Int32 iGroupId, bool iIsLeft, double iRadiusA, double iRadiusB, std::vector<Rect2D>& irRect, std::vector<Int32>& irIndex, Int32 iHeightTotal);

		void hLabelsOnSliceSideForLR(Int32 iGroupId, bool iIsLeft);

		void hSmartAdjustYForLR(Int32 iGroupId, bool iIsLeft);

		bool hIsRectInEllipse(const Rect2D& irRect, const Point2D& irCenter, double& irRadiusA, double& irRadiusB);

		bool hIsPointInEllipse(const Point2D& irPoint, const Point2D& irCenter, double& irRadiusA, double& irRadiusB);

		Int32 hGetDeltaXFromEllipseY(Int32 iY, const Point2D& irCenter, double& irRadiusA, double& irRadiusB);		

		virtual void hUpdateAllRadius(bool iIsProportional);
		
		void hSetOnSliceHeightLimitationForGroup(Int32 iGroupId, bool iIsLeft);

		void hAdjustAllLabelsPosition();

		void hMoveLabel(Int32 iGroupId, TextLabelAndValue::Ptr iTextValueAndLabelPtr);

		void hMoveOutsideLabel(Point2D& irPoint, Int32 iXSign, Int32 iYSign, TextObject::Ptr iTextPtr);

		void hGetOffScale(Int32 iGroupId, TextLabelAndValue::Ptr iTextValueAndLabelPtr, Int32& orHorizontalOffScale, Int32& orVerticalOffScale);

		void hGetRectForLabelAndValue(TextLabelAndValue::Ptr iTextPtr, bool iIsInside, bool iIsRotateRect, bool iIsVerticalAligned, Rect2D& orRect);

		void hAdjustToOutside(Int32 iGroupId, TextLabelAndValue::Ptr iTextPtr);

		void hCreateLeaderLines(GraphCollectionObject::Ptr ioLeaderLineCollectionPtr);

		void hCreateLeaderLinesForGroupLR(Int32 iGroupId, bool iIsLeft, GraphCollectionObject::Ptr ioLeaderLineCollectionPtr);

		bool hGetLeaderLinePoint(TextLabelAndValue::Ptr iTextValueAndLabelPt, Point2D& orPoint);

		bool hAdjustRadiusForOutsideLabels();

		bool hMoveLabelInEllipse(Int32 iGroupId, TextLabelAndValue::Ptr iTextValueAndLabelPtr, Rect2D& irBoundingRect, bool& orIsValidDirection, bool& orUpDirection);

		void hMoveLabelInAngularBisector(Int32 iGroupId, TextLabelAndValue::Ptr iTextValueAndLabelPtr, Rect2D& irBoundingRect, Int32 iYOffset);

		bool hRectangleEqual(const Rect2D& irRectA, const Rect2D& irRectB);

		void hSetHeightLimit(Int32 iGroupId, TextLabelAndValue::Ptr iTextValueAndLabelPtr);

		bool hGetOnSliceBound(Int32 iGroupId, TextLabelAndValue::Ptr iTextValueAndLabelPtr, Rect2D& irBoundingRect, bool iIsOuter, bool iIsMoveInEllipse, Point2D& orBound);

		//void hGetOutsideLabelRotation(std::vector<GraphCollectionObject::Ptr>& irTextLabelCollection, std::vector<GraphCollectionObject::Ptr>& irTextValueCollection);

		/**
		Given a vector of the distance among labels, we will get a solution to reduce the
		overlapping between labels.
		@param iorDistance The input and output parameters, the negative value indicates
		an overlapping occurs. 
		*/
		void hSmartDistance(std::vector<Int32>& iorDistance) const;

		//all other members are initialized in hDoInitialize()
    protected:
		std::vector<double> mGroupTotals;
		double mMaxGroupTotal;
        double mMinGroupTotal;
		Int32 mInitRadius; // no proportional
		Int32 mGroupCount;
		GraphOptionsPie mPieOption;

		//initialize in hLoadProperties() & hLoadSeriesProperties.
		std::vector<GraphSeriesPie::Ptr> mSeriesOptions;

        //vector for slice object
		std::vector<std::vector<PieSliceObject::Ptr> > mSlices;

        
        Int32 mDepthInChart;

//    private:        
		//member variables

		


		//different the mPiesPerRow in GraphOptionsPie, it is the actually pies in per row
		Int32 mPiesPerRowInChart;
		Int32 mNumberRows;

		//the cairo scale value.
		double mScale;		

		//rectangles
		std::vector<Rect2D> mGroupRects;
		std::vector<Rect2D> mPieRects;

		//center and radius
		std::vector<Point<Int32> > mPieCenters;
		std::vector<Int32> mRadius;


		std::vector<TextObject::Ptr> mGroupLabels;
		Int32 mMaxGroupLabelsHeight;
		std::vector<Int32> mGroupLabelY;
		
		///// vector use in program for each group /////
		//// initialized in hGenerateMapAndListForGroup() ////


		//vector for leader lines
		//std::vector<PolyLineObject::Ptr> mLeaderLines;

		//text object for all the groups
		//we have to store all the know whether we are going to make the pie size smaller or
		//bigger after checking whether all labels should be on slice.
		//one for left side, one for right side
		//the Int32 value indicate the series index
		
		std::vector<std::vector<TextLabelAndValue::Ptr> > mLeftTexts;
		std::vector<std::vector<TextLabelAndValue::Ptr> > mRightTexts;

		//std::vector<Int32> mAngles;

		bool mIsAllLabelsOnSlice;

		///used for the rotation handling
		//bool mIsAllOutsideLabelSameRotated;

		Int32 mMaxExplode;
	};
	
	inline Int32 PiePlot::GetPiesPerRow() const
	{
		return mPiesPerRowInChart;
	}

	inline Int32 PiePlot::GetNumberOfRows() const
	{
		return mNumberRows;
	}

	inline const EnumPieDataLabelsPosition PiePlot::GetValuePosition(Int32 iSeriesIndex) const
	{
		return mSeriesOptions[iSeriesIndex]->mDataLabels.mDataValuesPosition;
	}

	inline const EnumPieDataLabelsPosition PiePlot::GetLabelPosition(Int32 iSeriesIndex) const
	{
		return mSeriesOptions[iSeriesIndex]->mDataLabels.mDataLabelsPosition;
	}

	inline double PiePlot::GetSmartLabelRadius(Int32 iGroupId) const
	{
		return 0.6 * mInitRadius + 0.6 * mRadius[iGroupId];
	}
	
}

#endif
