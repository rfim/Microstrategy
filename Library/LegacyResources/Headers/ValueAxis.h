//==============================================================================================
//	FILENAME	:	ValueAxis.h
//	AUTHOR		:	Liang Liu
//	CREATION	:	2008-06-04
//	Copyright (C) MicroStrategy Incorporated 2008
//	All Rights Reserved
//==============================================================================================
#ifndef MsiChart_ValueAxis_h
#define MsiChart_ValueAxis_h

#include "Axis.h"
#include "NumberObject.h"

namespace MsiChart
{

	struct ValueAxisInfo
	{
		TripleId mId;
		unsigned char mAxisLabelsLocation; 
		unsigned char mAxisDirection;	
		
		ValueAxisInfo()
		{
			mId = TripleId(DssGraphY1Body, 0, -3);
			mAxisDirection = AD_Y;
			mAxisLabelsLocation = ALL_LEFT;
		}
		
		void ToX(bool iIsLabelAoveAxis)
		{
			mId = TripleId(DssGraphX1Body, 0, -3);
			mAxisDirection = AD_X;
			mAxisLabelsLocation = iIsLabelAoveAxis ? ALL_TOP : ALL_BOTTOM;
		}
		
		void ToY1(bool iIsSeriesAxis = false)
		{
			mId = TripleId(DssGraphY1Body, 0, -3); //maybe some other ID?
			mAxisDirection = AD_Y;
			mAxisLabelsLocation = ALL_LEFT;
		}

		void ToY2(bool iIsSeriesAxis = false)
		{
			mId = TripleId(DssGraphY2Body, 0, -3); //maybe some other ID?
			mAxisDirection = AD_Y;
			mAxisLabelsLocation = ALL_RIGHT;
		}

	};




	struct ValueAxisOptions
	{
		ValueAxisOptions();			
		bool mUseLogarithmicScale;
		bool mAlwaysIncludeZero;
		bool mDrawFromZero;
		bool mUseMinValue;
		bool mUseMaxValue;
		bool mShowMinLabel;
		bool mShowMaxLabel;
		bool mDrawCustomLine; 
		bool mDrawSecondCustomLine;
		bool mUseSmartScaleBreak;
        bool mUseCustomMinMax;
        bool mDisableFractionalGrids;   ///< whether or not fractional grids should be disabeld.
        bool   mCondenseLabel;
        bool   mHasCustomAxisOrigin;
		unsigned char mOffScaleType;
		double mAxisMinValue; 
		double mAxisMaxValue;
		double mAxisCustomValue; 
		double mAxisSecondCustomValue;
        double mCustomAxisOriginValue;
	}; 

	/**
	Creates and maintains axis title, axis labels, axis line and grid lines. 
	This class also provide conversion from value to coordinate.	
	*/
	class ValueAxis: public Axis
	{
	public:
		typedef MSynch::SmartPtr<ValueAxis> Ptr;
		
		/**        
        @param iLargerGridInterval Indicates whether larger grid interval is desired. 
		@param iMin Indicate the minimum value in current graph. 
		@param iMax Indicate the maximum value in current graph. 
		@param iIsPercent Indicate whether current value type is percent.
		*/
		ValueAxis(const TripleId& irTripleId, 
			GraphObjectManager* ipManager, 
			double iMin, 
			double iMax,		
			bool iLargerGridInterval = false, 			 
			bool iIsPercent = false,
			unsigned char iAxisDirection = AD_Y,
			bool iIsShown = true,
			Int32 iPrimaryMetricIndex = -1); 
		virtual ~ValueAxis();
			
		// Override methods
		virtual void GenerateMapAndList();
		virtual void GetOptions(Int32 iSeriesId, Int32 iPropertyId, CComVariant* opValue) const;
		
		/**
		Adjust the device data area, trim space for ticks and depth axis line.
		@param iorDeviceDataArea This data area will be adjusted by subtracting tick area and depth line area.
		@param iIsReversed If this parameter is false, iorDeviceDataArea will be adjusted by substracting tick 
		area and depth axis line area. Otherwise, it will be adjusted by adding tick area and depth line area.
		*/
		void AdjustDeviceDataArea(Rect2D& iorDeviceDataArea, bool iIsReversed = false); 			
		/**
        This function provides conversion from data value to position. 
        @param iValue Input data value. 
		@param iTruncate If this is true, iValue will be treated as mMaxTick if iValue>mMaxTick and mMinTick if iValue<mMinTick.
        @return If current ValueAxis is vertical, return y-coordinate of iValue. 
		If current ValueAxis is horizontal, return x-coordinate of iValue.
		*/
		Int32 ValueToPosition(double iValue, bool iTruncate = true) const; 
		/**
        This function provides conversion from data value to position.
		This function support DssGraphOffScaleType property which is not supported in ValueToPosition().
        @param iValue Input data value. 	
        @param orPosition If current ValueAxis is vertical, output the y-coordinate of iValue. 
		If current ValueAxis is horizontal, output the x-coordinate of iValue.
		@return Return whether this value should be shown or not. If the return value is false, orPosition is not valid.
		*/
		bool GetPositionByValue(double iValue, Int32& orPosition) const; 
		/**
        This function provides conversion from data value to position.
		This function support DssGraphOffScaleType property which is not supported in ValueToPosition().
        @param iValue Input data value. 	
        @param orPosition If current ValueAxis is vertical, output the y-coordinate of iValue. 
		If current ValueAxis is horizontal, output the x-coordinate of iValue.
		@return Return whether this value should be shown or not. If the return value is false, orPosition is not valid.
		*/
		bool GetFloatPositionByValue(double iValue, float& orPosition) const;
	
		/**
		 This function provides conversion from data value to position in 3D chart.
		 This function support DssGraphOffScaleType property which is not supported in ValueToPositionFor3D().
		 @param iValue Specifies a logical coordinate.
		 @param orPosition Output 3D absolute coordinate along current axis.        
		 @return Return whether this value should be shown or not. If the return value is false, orPosition is not valid.
		 */
		bool Get3DPositionByValue(double iValue, double& orPosition) const;
		
		/**
        This function provides conversion from data value to position. 
        @param iValue Input data value. 		
        @return If current ValueAxis is vertical, return y-coordinate of iValue. 
		If current ValueAxis is horizontal, return x-coordinate of iValue.
		*/
		float ValueToFloatPosition(double iValue) const; 
		/**
        This function provides conversion from data value to bubble size. 
		@param iDialogSize Specifies the bubble size in dialog.
        @param iValue Input data value. 
        @return Return the corresponding bubble size.
		*/
		Int32 ValueToBubbleSize(Int32 iDialogSize, double iValue) const;			
		/**
		Get the position of base. 
		This method is supposed to be used by BarSeriesRender and AreaSeriesRender. 
		@return If current ValueAxis is vertical, return y-coordinate of base. 
		If current ValueAxis is horizontal, return x-coordinate of base.
		*/
		Int32 GetBasePosition();
		/**
		 Get the position of base for 3D chart. 	
		 @return Return y-coordinate of base. 
		 */
		double GetBasePositionFor3D();
		/**
		Get the minimum tick value in current ValueAxis. 
		@return The minimum tick value. 
		*/
		double GetMinTick();  		
		/**
		Get the maximum tick value in current ValueAxis. 
		@return The maximum tick value. 
		*/
		double GetMaxTick();	
		/**
		Get the value of base. 
		This value maybe useful in 3D chart. For 2D chart, use GetBasePosition().		
		@return The value of Base. 
		*/
		double GetBaseValue(); 
		/**
		Check whether the base value is zero or minimum. 
		@return Return mOptions.mDrawFromZero. 
		*/
		bool IsBaseAtZero();
		/**
		Generate custom line if necessary.
		Since custom line will be drawn after risers are drawn, 
		it cannot be put into the GraphObjectList of current ValueAxis.
		@param irDataArea Specify the data area of current chart area.
		@param iorGraphObjectList If a custom line is generated, it will be put into iorGraphObjectList.
		*/
		void GenerateCustomLine(const Rect2D& irDataArea, std::vector<GraphObject::Ptr>& iorGraphObjectList);
        void GenerateOneCustomLine(double iValue, Int32 iLineObjectId, const Rect2D& irDataArea, std::vector<GraphObject::Ptr>& iorGraphObjectList);
        
		/**
		This method is used to reserve title and label area according to a reference rectangle.
		@param iorArea Indicates the reference rectangle of axis title and labels. It will be adjusted by substracting 
		title and label area.
		@param iLowMargin Specifies the existing margin on the low end. Here, "Low" means bottom or left.
		@param iHighMargin Specifies the existing margin on the high end. Here, "High" means top or right.
		*/
		void ReserveTitleAndLabels(Rect2D& iorArea, Int32 iLowMargin = 1000, Int32 iHighMargin = 1000);
		/**
		This method is used to update title and label area according to a reference rectangle.
		@param iorArea Indicates the reference rectangle of axis title. It will be adjusted by adding title and label area.
		*/
		virtual void UpdateTitleAndLabelArea(Rect2D& iorArea);	

		/**
		Get the interval of major grid.
		@return Return the interval of major grid.
		*/
		double GetStep() const;

		/**
		Get the number format of value axis labels.
		@param orFormat Output the number formatting of value axis labels.
		*/
		void GetNumberFormat(ChartAdvFormat& orFormat);

		virtual double GetLogicalCoord(Int32 iAbsoluteCoord);
		virtual float GetAbsoluteCoord(double iLogicalCoord);	
		virtual void GetMinMaxLogicalCoord(double& orMin, double& orMax) const;
		virtual double GetSlopeFactor(double iLogicalCoord) const;

		/**
		Get all the ticks which we will try to display.
		*/
		virtual void GetTicks(std::vector<double>& orTicks) const;

		/**
		Check whether current ValueAxis use logarithmic scale or not.
		@return Return true if current ValueAxis use logarithmic scale.
		*/
		virtual bool IsLogarithmic() const;
		
		virtual void OnZooming(Int32 iOldStart, Int32 iOldEnd, Int32 iNewStart, Int32 iNewEnd);
		
		/// Restore from zoom state.
		void OnZoomRestore();
		
		/**
		 Some styles of grid lines have outer ticks. This method returns the length of outer tick.
		 @return Return the length of outer tick. If no grid line has outer tick, return 0.
		 */
		Int32 GetOuterTickLength() const;

		virtual void GenerateLabelsFor3D(const std::vector<Point2D>& irGridPoints, const DVector2D& irUnitOffsetDirection,   
										 GraphCollectionObject* ipLabelCollectioin, std::vector<GraphObject::Ptr>& iorGraphObjectList);
		
		virtual void GetGridLinePositionsFor3D(std::vector<double>& orGridLinePositions, EnumGraphObjectType iGraphObjectType) const;
		
		virtual double GetPositionAtTick(double iTickValue);
 		virtual double GetIntervalSpace();
		virtual void TrimAxisPadding(Rect2D& iorRect, bool iUseFixedMargin = true);


		bool IsZeroBaseLineQualified();	
		void SetTrimWithPadding(bool iIsTrimmed);
		bool GetTrimWithPadding();
		virtual Int32 LayoutAxis(bool iIsRotated, Int32& orMaxLabelSpace);	
        virtual Int32 GetUniformAxisExtraPadding();
        void GetMaxMinDataInput(double &orMaximum, double &orMinimum);
        
        int GetPrimaryMetricIndex() const;
        
        bool IsCustomMinMax() const;
        bool IsTickExist(double iValue);
        bool HasCustomAxisOrigin();
        double GetCustomAxisOriginValue() const;
        Int32 GetLabelObjectId()const;
	protected:
		/**
		Load related properties of ValueAxis from ChartContext.
		*/
		void LoadProperties();	
		/**
		Create axis line. This is a helper method of GenerateAxisLine().
		*/
		virtual void CreateAxisLine();

		/**
		This is a helper method for derived classes. Get position from a given value.
		@param iValue Specifies a value.
		@param iStartPosition,iStopPosition Specifies a range.
		@return A corresponding position of iValue.
		*/
		float ValueToPosition(double iValue, float iStartPosition, float iStopPosition) const;	

		/**
		This is a helper method for derived classes. Get position from a given value.
		@param iValue Specifies a value.
		@param iStartPosition,iStopPosition Specifies a range.
		@param orPosition Output a corresponding position of iValue.
		@return Return whether this value should be shown or not. If the return value is false, orPosition is not valid.
		*/
		bool ValueToPosition(double iValue, float iStartPosition, float iStopPosition, float& orPosition) const;
			
		/// Generate tick labels.
		void GenerateTickLabels();
		/**
		This is a helper method for hGenerateTickLabels().
		*/
		void hCreateTickLabels();
		/**
		This is a helper method for hCreateTickLabels(). This method will create one tick label.
		@param irTripleId The TripleId of axis labels.
		@param iTickValue Specifies the value of the label to be created.
		*/
		virtual void hCreateOneTickLabel(const TripleId& irTripleId, double iTickValue);
		/**
		This is a helper method for GenerateMapAndList().
		Axis labels are created in ReserveTitleAndLabels(). But the data area may be changed later.
		The location of each label need to be updated using this method.	
		*/
		void hUpdateLabelLocation();
		/**
		This is a helper method for hUpdateLabelLocation(). This method will update the location of one tick label.		
		@param iGraphObjectIndex The index of current label in mGraphObjectList.
		@param iLabelLocation Should be a value in EnumAxisLabelsLocation. 
		For convience, when HasTwoGridLineParts() is true, ALL_LEFT_RIGHT is treated as ALL_LEFT, 
		and ALL_BOTTOM_TOP is treated as ALL_BOTTOM. 
		@param iTickValue Specifies the tick value of current label.	
		*/
		void hUpdateOneLabelLocation(Int32 iGraphObjectIndex, Int32 iLabelLocation, double iTickValue);
		/**
		This is a helper method for hGenerateTickLabels().
		This method will generate tick values and put them into mTicks.
		*/
		void hComputeTicks();
		/**
		Calculate the general step between adjacent tick values.
		@param iMaximum Specifies the maximum value.
		@param iMinimum Specifies the minimum value.
		@param iIncludeZero Indicates whether zero is always included or not.
		@return The general step between adjacent tick values.
		*/
		virtual double hComputeStep(double iMaximum, double iMinimum, bool iIncludeZero);		
		/**
		Reserve area for axis labels or outer ticks.
		@param iorArea This data area will be adjusted by subtracting the area for axis labels or outer ticks.
		@param iIsLabel Indicates whether label area or tick area need to be reserved.
		@param iIsReversed If this parameter is false, iorArea will be adjusted by substracting label area or tick area.
		Otherwise, it will be adjusted by adding label area or tick area.
		*/
		void hReserveLabelOrTickArea(Rect2D& iorArea, bool iIsLabel, bool iIsReversed);
		
		/**
		Get the maximum width or height of axis labels.
		@param iIsWidth Indicates whether the width or the height is needed.
		@return If iIsWidth is true, return the maximum width of axis labels. 
		Otherwise, return the maximum height of axis labels. 
		*/
		Int32 hGetMaxLabelWidthOrHeight(bool iIsWidth) const;		
		/**
		New feature. Generate interlaced grids.
		*/
		void hGenerateInterlacedGrids();
		/**
		This is a helper method of hGenerateInterlacedGrids().
		This method will generate one grid section acoording to two given values.
		@param irTripleId Specifies the TripleId of current grid section.
		@param iValue1,iValue2 Specifies the range of current grid section.
		@param ipGridLineSet Points to a GraphCollectionObject which indicates the grid section collection.
		*/
		void hGenerateGridSectionByValue(const TripleId& irTripleId, double iValue1, double iValue2, GraphCollectionObject* ipGridSet);
		/**
		Generate major grid lines.
		*/
		void hGenerateMajorGrid();	
		/**
		Generate minor grid lines.
		*/
		void hGenerateMinorGrid();		
		/**
		This is a helper method of hGenerateMajorGrid() and hGenerateMinorGrid().
		This method will generate one grid line acoording to a given value.
		@param irTripleId Specifies the TripleId of current grid line.
		@param iValue Specifies the data value of current grid line.
		@param ipGridLineSet Points to a GraphCollectionObject which indicates the grid line collection.
		@param iIsMajor Indicates whether current grid line is a major grid line or a minor grid line.
		*/
		void hGenerateGridByValue(const TripleId& irTripleId, double iValue, GraphCollectionObject* ipGridLineSet, bool iIsMajor);	

		/**
		This is a helper method of GenerateCustomLine().	
		 @param iorGraphObjectList If a custom line is generated, it will be put into iorGraphObjectList.
		 @param iCustomValue Specifies a custom line value.
		 @param iCustomLineId Specifies an object ID for custom line.
		*/	
		void hGenerateOneCustomLine(std::vector<GraphObject::Ptr>& iorGraphObjectList, 
									double iCustomValue, Int32 iCustomLineId);
		/**
		Get a straight line from a given value.
		@param iValue Specifies a value.
		@param orLowEnd The left or bottom end of the output straight line.
		@param orHighEnd The right or top end of the output straight line.
		*/
		void hGetLineByValue(double iValue, Point2D& orLowEnd, Point2D& orHighEnd) const;
		/**
		Check whether any axis line need to be drawn for current ValueAxis.
		@return Return true if at least one axis line need to be drawn for current Value Axis.
		*/
		bool hHasAxisLine() const;	
		/**
		Check whether current graph type is dual axis without split.
		@return Return true if current graph type is dual axis without split.
		Otherwise, return false.
		*/
		bool hIsDualAxisWithoutSplit() const;
		/** This is a helper method of hComputeTicks(). 	
		This method is called when mIsPercent is true.
		*/
		void hComputeTicksForPercent();
		/** This is a helper method of hComputeTicks(). 	
		This method is called when hIsHistogramX() is true.
		*/
		void hComputeTicksForHistogramX();
		/** This is a helper method of hComputeTicks(). 	
		This method is called when mIsPercent is false and mUseLogarithmicScale is true.
		*/
		void hComputeTicksForLogScale();
		// Four combinations of mUseMaxValue and mUseMinValue.
		/** This is a helper method of hComputeTicks(). 
		This method is called when when mUseLogarithmicScale, mIsPercent, mUseMaxValue and mUseMinValue are false.
		*/
		void hComputeTicksForFlexMinMax();
		/** This is a helper method of hComputeTicks(). 
		This method is called when the following conditions hold simultaneously. 		
		1. mIsPercent, mUseLogarithmicScale and mUseMinValue are false.
		2. mUseMaxValue is true.
		*/
		void hComputeTicksForFixMax();
		/** This is a helper method of hComputeTicks(). 
		This method is called when the following conditions hold simultaneously. 
		1. mIsPercent, mUseLogarithmicScale and mUseMaxValue are false.
		2. mUseMinValue is true.
		*/
		void hComputeTicksForFixMin();
		/** This is a helper method of hComputeTicks(). 
		This method is called when the following conditions hold simultaneously. 
		1. mIsPercent and mUseLogarithmicScale are false.
		2. mUseMaxValue and mUseMinValue are true.
		*/
		void hComputeTicksForFixMinMax();			
		/**
		Hide overlapped labels.
		@param iStartIndex The start index of labels in mGraphObjectList.
		@param iEndIndex The end index of labels in mGraphObjectList.
		[iStartIndex, iEndIndex) specifies a bunch of labels in mGraphObjectList.
		*/
		void HideOverlappedLabels(Int32 iStartIndex, Int32 iEndIndex);
        
        
        Int32 hComputeLabelSkipStepForCustomPercentLabels(Int32 iStartIndex, Int32 iEndIndex);
        void HideOverlappedLabelsForCustomPercentLabels(Int32 iStartIndex, Int32 iEndIndex);
		/**
		Check whether there are too many ticks for manual major grid interval.
		@return Return true if there are too many ticks for manual major grid interval. Otherwise, return false.
		*/
		bool hHasTooManyTicks() const;
		/**
		Check whether current axis is the X axis in Histogram graph.
		@return Return true if current axis is the X axis in Histogram graph.
		*/
		bool hIsHistogramX() const;

		bool hIsPolarXOrHistogramX() const;

		bool hCheckValue(double& iorValue, double& orMin, double& orMax) const;
		
		
		double hLengthToValue(double iLength);				//< Map a length to a value.
		void hAdjustAxisRangeForGM();						//< Enlarge the axis range so all the element/markers would be included in graph area
		void hAdjustTickForGM();							//< Decide flex min/max or fix min/max will be used by checking the tick values
		//double hGetMaxOffsetSpaceForGMMarker();				//< Get max offset space according to graph type and radius computation forum.
		bool hGetNumberFormatFromMetric();					//< Get NumberFormat From Primary Metric
		TextObject* hGetAxisLabelByTickValue(double iValue); //< Get the corresponding label to one tick value. 
		void hGetOneTickToLabel(bool iIsOnHighSide, double iValue, TextObject* ipObject);//< Build the label from tick value to label object
		void hBuildTickToLabelMap(bool iIsOnHighSide, Int32 iStartIndexInset);//< Build the label from tick value to label object
		bool IsAxisLabelAbbreviated(const TripleId& irTripleId);   			  //< Scan all the labels and check if any abbreviation happens for the labels under this axis.
		void hSetAxisLabelStyle();							//< Notify chart context if abbreviation of the labels should be used.
		void hUpdateShownTicksToGMContext();				//< Notify GM context which labels will be shown.
        void hSetAxisOrigin();
        /**
         * To pick the first label that satisfies the 5 pixels paddings.
         * @param iStartIndex, the start index of the label set.
         * @param iEndIndex, the end index of the label set.
         * @param iorIsPaddingPossible, if at least one such label could be found.
         * @return the index to the first label that satisfy the requirement.
         */
		Int32 hPickStartLabelIndex(Int32 iStartIndex, Int32 iEndIndex, bool& iorIsPaddingPossible);				        

        /**
         * To compute the label step for skipping in order that no (or minimal) overlap happens between labels.
         * @param iSkipFromHighEnd, if we begin skipping from the high end of the axis, this case happens in percent chart in Graph matrix.
         * @param iNeedDistanceCheck, need to check the distance between adjacent labels, used in graph matrix.
         * @param iStartIndex, the start index of the label set.
         * @param iEndIndex, the end index of the label set.
         * @param iRefLabelIndex, the index to the ref label, this is where we start.
         * @param iAnchorStyle, 
         * @param ipRefLabel, the refLabel itself.
         * @param iExpDirection
         * @return the label step.
         */
        Int32 hComputeLabelSkipStep(bool iSkipFromHighEnd, bool iNeedDistanceCheck, Int32 iStartIndex, Int32 iEndIndex, Int32 iRefLabelIndex, Int32 iAnchorStyle, TextObject* ipRefLabel, REC_EXP_DIRECTION iExpDirection);

        /** 
         * To hide labels with the specific step.
         * @param iSkipFromHighEnd, if we begin skipping from the high end of the axis, this case happens in percent chart in Graph matrix.
         * @param iStartIndex, the start index of the label set.
         * @param iEndIndex, the end index of the label set.
         * @param iRefLabelIndex, the index to the ref label, this is where we start.
         * @param the skip step.
         */
        void hHideLabelsWithStep(bool iSkipFromHighEnd, Int32 iStartIndex, Int32 iEndIndex, Int32 iReflabeIndex, Int32 iLabelStep);
        
        /**
         * To hide the labels from high end by checking the 5 pixel padding rule
         * @param iLastIndex, where to start off hiding.
         */
		Int32 hHideLabelsFromHighEnd(Int32 iLastIndex, bool iIsFromHigh);						
		bool hIsLabelFitWithPadding(TextObject* ipTextLable, Int32 iPadding); 
		Int32 hGetEvenSpacedPadding(bool iIsHighSide, double iBaseLength);
		void hFilterRepeatedLabels();
        double hScaleStep(double iStep);
        
        int hGetMaxScreenPixels() const;
    
        void hInitLargerGridInterval();
        bool hIsPercentChart();
        
		//---------Data members----------------------------------
		ValueAxisOptions mOptions;	///< Options of Value Axis. 
		/// Indicates whether larger grid interval is desired. 
		bool mIsShown;
		bool mLargerGridInterval; 
		/// Indicate whether current value type is percent.
		bool mIsPercent;		
		double mMinimum;	///< The minimum value suggested by renderer. 		
		double mMaximum;	///< The maximum value suggested by renderer. 		
		double mMaxTick;	///< The maximum tick value.		
		double mMinTick;	///< The minimum tick value. 
		double mStep;		///< A desired data interval which corresponds to a desired geometric grid interval. 
		Int32 mLabelStartIndex;		///< The start index of labels in mGraphObjectList.
		std::vector<double> mTicks;	///< A list of desired tick values.	
		
		bool mIsStagger;			///< Used in Histogram chart. 
		ChartAdvFormat mNumberFormat;	///< Stores the number format of tick labels.
		Rect2D mLabelArea;
		Int32 mGraphMajorType;			///< Keeps the current graph major type.
		Int32 mLabelObjectId;
		bool mIsTrimmedWithPadding;		///< Padding trimmed
		mutable bool mIsMaxZoom;
        std::vector<Int32> mMetricList; ///< To indicate which metric this axis controls
		Int32 mPrimaryMetricIndex;		///< The index of the metric if all our data comes under it.
		Int32 mLabelSpan;				///< The span between two labels for time series widget.
		std::map<double, TextObject*> mLowTicksToLabel;	///< A map from tick value to its corresponding label
		std::map<double, TextObject*> mHighTicksToLabel;	///< A map from tick value to its corresponding label
		/*
		   since we may ajust the mMinmum and mMaxmum, we use those two values to
		   store the origin value pass in ctor
		*/
		double mOriginMinimum;   
		double mOriginMaxmum;   
        double mStepScale;
		  
	private:
		virtual void hGenerateGridsByArea(bool iIsGridLine);
		bool hIsAutoXAxis() const;
		void hApplyAutoXAxisLayout();
		bool hIsManualMinShown() const;
		void hGetLabelMargins(Int32& orLowMargin, Int32& orHighMargin);
        void hAdjustAxisInputScale();
        
        /**
         * This method increases the max tick of value axis so that
         * there is a gap of iReservedHeight height between the new max tick and the previous max tick.
         * This method should be called before NumberObject of tick labels are generated.
         * When custom max value is defined this method return false.
         * Currently it is used in TimeSeries to reserve area for tooptip but you can use it for other purpose.
         */
        bool hReserveAreaAtTop(Int32 iReservedHeight);
	};	

	inline bool ValueAxis::IsBaseAtZero()
	{
		return mOptions.mDrawFromZero && (!mOptions.mUseLogarithmicScale);
	}

	inline bool ValueAxis::hHasTooManyTicks() const
	{
		return mMajorGrid.mUseManualGrid && mMajorGrid.mGridInterval < (mMaxTick - mMinTick) / 30;
	}

	inline bool ValueAxis::hIsHistogramX() const
	{
		return mGraphMajorType == GMT_HISTOGRAM && mTripleId.mObjectId == DssGraphX1Body;
	}

	inline bool ValueAxis::hIsPolarXOrHistogramX() const
	{
		return (mGraphMajorType == GMT_HISTOGRAM || mGraphMajorType == GMT_POLAR) && mTripleId.mObjectId == DssGraphX1Body;
	}

	inline double ValueAxis::GetStep() const
	{
		return mStep;
	}
	inline bool ValueAxis::IsLogarithmic() const
	{
		return mOptions.mUseLogarithmicScale;
	}

	inline bool ValueAxis::hIsManualMinShown() const
	{
		return mOptions.mUseMinValue && mOptions.mShowMinLabel;
	}
	inline bool ValueAxis::IsZeroBaseLineQualified()
	{
        // Dual axis support, just a work round, we need to generate invalid valueaxis to complete the workflow, so here we need to judge the invalidation
        if (!mAxisInfo.mIsValid) {
            return false;
        }
        
        //[2013/2/19] if custom min/max enabled in Graph matrix
        return mMaxTick * mMinTick < 0;
		//return mMaximum * mMinimum < 0;
	}
	inline void ValueAxis::SetTrimWithPadding(bool iIsTrimmed)
	{
		mIsTrimmedWithPadding = iIsTrimmed;
	}		

	inline bool ValueAxis::GetTrimWithPadding()
	{
		return mIsTrimmedWithPadding;
	}
    
    inline int ValueAxis::GetPrimaryMetricIndex() const
    {
        return mPrimaryMetricIndex;
    }
    
    inline bool ValueAxis::IsCustomMinMax() const
    {
        return mOptions.mUseCustomMinMax;
    }
    inline Int32 ValueAxis::GetLabelObjectId() const
    {
        return mLabelObjectId;
    }
}
#endif
