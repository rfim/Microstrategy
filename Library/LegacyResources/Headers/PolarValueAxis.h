//==============================================================================================
//	FILENAME	:	PolarValueAxis.h
//	AUTHOR		:	Liang Liu
//	CREATION	:	2009-02-17
//	Copyright (C) MicroStrategy Incorporated 2009
//	All Rights Reserved
//==============================================================================================
#ifndef MsiChart_PolarValueAxis_h
#define MsiChart_PolarValueAxis_h

#include "ValueAxis.h"

namespace MsiChart
{
	extern const Int32 gLabelMarginFactor;

	/**
	This struct keeps some useful information which is used in Polar and Radar chart.
	*/
	struct PolarInfo
	{
		PolarInfo(const Rect2D& irFrameRect);
		/**
		Load properties.
		@param iChartContextPtr This is used to access ChartContext::GetProperty().
		@param iChartAreaId Specifies the chart area ID.
		*/
		void LoadProperties(ChartContext::Ptr iChartContextPtr, Int32 iChartAreaId);
		/**
		Get options.
		@param iPropertyId Specifies the property ID.
		@param opValue The output value.
		*/
		bool GetOptions(Int32 iPropertyId, VARIANT* opValue) const;

		float mStartAngle;		///< Indicates the start angle (in radian) of angle axis.		
		/// If this is true, spider grid lines are used. Otherwise, circular grid lines are used.
		bool mUseSpiderLines;			
		bool mShowDataLines;
		bool mConnectFirstLast;	///< Indicates whether the first data point should be connected to the last data point.
		bool mUseRemoteAxis;	///< Indicates whether remote axis is used or not.
		bool mIsClockwise;		///< Indicates whether the angle axis goes clockwise or not.
		bool mIsLogarithmicX;		///< Indicates whether X axis uses logarithmic scale or not.
		bool mShowXMajor;
		bool mShowMarkers;
		unsigned char mMarkerShape;	///< Indicates the marker shape.
		Point2D mCenter;
		Int32 mRadius;
		Rect2D mFrameRect;			
		std::vector<float> mXTickAngles;	///< Maintains the tick angles of X axis.
		std::vector<double> mXTicks;	///< Maintains the ticks of X axis.
	};
	/**
	Creates and maintains axis title, axis labels, axis line and grid lines. 
	This class also provide conversion from value to coordinate.	
	*/
	class PolarValueAxis: public ValueAxis
	{
	public:
		typedef MSynch::SmartPtr<PolarValueAxis> Ptr;
		
		/**        
        @param irTripleId,ipManager,iMin,iMax,iLargerGridInterval See ValueAxis::ValueAxis().
		@param irPolarInfo Reference to Polar information struct.
		*/
		PolarValueAxis(const TripleId& irTripleId, 
			GraphObjectManager* ipManager, 
			double iMin, 
			double iMax,
			const PolarInfo& irPolarInfo,
			bool iLargerGridInterval = false); 
		virtual ~PolarValueAxis();
			
		// Override methods
		virtual void GenerateMapAndList();		
		/**
		Reserve area for axis labels.
		@param iorArea This data area will be adjusted by subtracting the area for axis labels or outer ticks.
		@param iIsReversed If this parameter is false, iorArea will be adjusted by substracting label area or tick area.
		Otherwise, it will be adjusted by adding label area or tick area.
		*/
		virtual void ReserveLabelArea(Rect2D& iorArea, bool iIsReversed);	

		/**
		This method is used to update label location according to a reference rectangle.
		@param iorLabelArea Indicates the reference rectangle of axis labels. It will be adjusted by adding label area.
		*/
		void UpdateLabelLocation(Rect2D& iorLabelArea);
		/**		
		Axis title is created in ReserveTitleArea(). But the data area may be changed later.
		The location of axis title need to be updated using this method.
		@param irTitleArea Indicates the reference rectangle of axis title.
		*/
		void UpdateTitleLocation(const Rect2D& irTitleArea);

		/**
		Get standard coordinates from polar coordinates.
		@iAngle Specifies an angle in radian.
		@iRadius Specifies a radius.	
		@return Return the standard coordinates acoording the input parameters.
		*/
		Point2D GetPointByAngleAndRadius(double iAngleRad, double iRadius) const;

		/**
        This function provides conversion from data value to an angle in radian. 
        @param iValue Input data value. 		
        @return Return the corresponding angle of iValue in radian.
		*/
		float ValueToAngle(double iValue) const; 	
		/**
        This function provides conversion from data value to an angle in radian. 
        @param iValue Input data value. 		
        @param orAngle Output the corresponding angle of iValue in radian.
		@return Return true if the value should be shown. Otherwise, return false. If the return value is false, orAngle is not valid.
		*/
		bool ValueToAngle(double iValue, float& orAngle) const;
		/**
        This function provides conversion from data value to a radius. 
        @param iValue Input data value. 	
		@param iTruncate If this is true, iValue will be treated as mMaxTick if iValue>mMaxTick and mMinTick if iValue<mMinTick.
        @return Return the corresponding radius.
		*/
		float ValueToRadius(double iValue, bool iTruncate = false) const;	
		/**
        This function provides conversion from data value to a radius. 
        @param iValue Input data value. 			
        @return orRaidus Output the corresponding radius.
		@return Return true if the value should be shown. Otherwise, return false. If the return value is false, orRadius is not valid.
		*/
		bool ValueToRadius(double iValue, float& orRadius) const;

		/**
		Get center point.
		@return Return the center point.
		*/
		Point2D GetCenter() const;		

		virtual double GetLogicalCoord(Int32 iAbsoluteCoord);
		virtual float GetAbsoluteCoord(double iLogicalCoord);

		/**
		Map radius by angle. If spider grid lines are used, the value in radius axis need to be mapped by angle.
		@param iAngleRad Specifies an angle, namely X-Value.
		@param iRadius Specifies a radius, namely Y-Value.
		@return Return the mapped radius.
		*/
		double MapRadiusByAngle(double iAngleRad, double iRadius) const;
		/**
		Map radius and derivative of radius by angle when spider grid lines are used.
		@param iAngleRad Specifies an angle, namely X-Value.
		@param iorRou Specifies a radius, namely Y-Value.
		@param iroDRou Specifies the derivative of radius at iAngleRad.
		*/
		void MapRouAndDRouByAngle(double iAngleRad, double& iorRou, double& iorDRou) const;
		/**
		Get absolute slope from logical slope.
		@param iLogicalSlope Specifies a slope in logical coordinate.
		@return Return a corresponding slope in absolute coordinate.
		*/
		double GetAbsoluteSlope(double iLogicalSlope, double iLogicalX, double iLogicalY) const;
		/**
		Set frame path. This method is used for clipping.
		*/
		void SetFramePath() const;

		/**
		Get all the ticks of current X axis. The min and max tick 
		@param orTicks Output the ticks.
		*/
		void GetXTicks(std::vector<double>& orTicks) const;

		/**
		Check whether spider grid lines are used or not.
		@return Return true if spider grid lines are used.
		*/
		bool IsSpiderGrid() const;

		/**
		Get a reference to PolarInfo struct.
		@return Return a reference to current PolarInfo struct object.
		*/
		const PolarInfo& GetPolarInfo() const;

		/** Generate Frame object if current Axis is an X axis.
		@return Return the generated frame object.
		*/
		GraphObject::Ptr GenerateFrame();

		/**
		Generate interlaced grid interval.
		*/		
		void GenerateInterlacedGrids();	

	protected:		
		/**
		This is a helper method for UpdateLabelLocation(). This method will update the location of one tick label.		
		@param iGraphObjectIndex The index of current label in mGraphObjectList.		
		and ALL_BOTTOM_TOP is treated as ALL_BOTTOM. 
		@param iTickValue Specifies the tick value of current label.	
		*/
		void hUpdateOneLabelLocation(Int32 iGraphObjectIndex, double iTickValue);

		const PolarInfo& mrPolarInfo; ///< Keeps a reference to the polar information struct.

	private:
		/// Generate axis line object if current Axis is a Y axis.
		void hGenerateAxisLine();	

		//void hGenerateRemoteLines();			
		/**
		This is a helper method of hUpdateOneLabelLocation().
		@param iorLabelCenter The label center which need to be adjusted. Here, "Center" only means horizontal direction.
		@param iLabelWidth Specifies the current label width.
		@iAngle Specifies the angle of current label.
		*/
		void hAdjustLabelCenterByAngle(Point2D& iorLabelCenter, Int32 iLabelWidth, float iAngle) const;
		/**
		This is a helper method of hGenerateMajorGrid() and hGenerateMinorGrid().
		This method will generate one grid line acoording to a given value.
		@param iValue Specifies the data value of current grid line.
		@param ipGridLineSet Points to a GraphCollectionObject which indicates the grid line collection.
		@param iIsMajor Indicates whether current grid line is a major grid line or a minor grid line.
		*/
		void hGenerateGridByValue(double iValue, GraphCollectionObject* ipGridLineSet, bool iIsMajor);	
		/**
		Generate major grid lines.
		*/
		void hGenerateMajorGrid();	
		/**
		Generate minor grid lines.	
		*/
		void hGenerateMinorGrid();	

		void hGenerateRemoteLines(GraphCollectionObject* ipGridLineSet);	

		void hGenerateRemoteLineByValue(double iValue, GraphCollectionObject* ipGridLineSet);

		float hGetYAxisAngle() const;

		bool hIsXAxis() const;

		bool hIsY2Axis() const;	

		void hNormalizeAngle(float& iorAngle) const;		
	};		

	inline bool PolarValueAxis::hIsXAxis() const
	{
		return mTripleId.mObjectId == DssGraphX1Body || mTripleId.mObjectId == DssGraphO1Body;
	}
	inline bool PolarValueAxis::hIsY2Axis() const
	{
		return mTripleId.mObjectId == DssGraphY2Body;
	}
	inline bool PolarValueAxis::IsSpiderGrid() const
	{
		return mrPolarInfo.mUseSpiderLines;
	}

	inline const PolarInfo& PolarValueAxis::GetPolarInfo() const
	{
		return mrPolarInfo;
	}
}
#endif
