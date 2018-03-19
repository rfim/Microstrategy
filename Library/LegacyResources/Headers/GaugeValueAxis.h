//==============================================================================================
//	FILENAME	:	GaugeValueAxis.h
//	AUTHOR		:	Liang Liu
//	CREATION	:	2009-03-02
//	Copyright (C) MicroStrategy Incorporated 2009
//	All Rights Reserved
//==============================================================================================
#ifndef MsiChart_GaugeValueAxis_h
#define MsiChart_GaugeValueAxis_h

#include "ValueAxis.h"

namespace MsiChart
{
	// The distance between the center and the tip of grid line will be mRadius - mRadius / gGridLineMarginFactor.
	extern const Int32 gGridLineMarginFactor;

	// The length of grid line will be mRadius / gGridLineLengthFactor.
	extern const Int32 gGridLineLengthFactor;

	/**
	Creates and maintains axis labels and grid lines for Gauge chart. 
	This class also provide conversion from value to angle.	
	*/
	class GaugeValueAxis: public ValueAxis
	{
	public:
		typedef MSynch::SmartPtr<GaugeValueAxis> Ptr;
		
		/**        
        @param irTripleId,ipManager,iMin,iMax See ValueAxis::ValueAxis().
		@param iStartAngle,iStopAngle Indicates the angle range of current Gauge axis.
		@param iRadius Indicates the radius of curreng Gauge axis.
		*/
		GaugeValueAxis(const TripleId& irTripleId, 
			GraphObjectManager* ipManager, 
			double iMin, 
			double iMax,
			Int32 iStartAngle,
			Int32 iStopAngle,
			float iRadius,
			bool iUseOldStyle,
			bool iDrawLabelsOutside); 
		virtual ~GaugeValueAxis();
			
		// Override methods
		virtual void GenerateMapAndList();

		/**
		Generate grid lines. 
		This method can be called multiple times. Each time, the caller can pass a different parameter. All the grid lines will
		be put into one collection so that they can be selected together. In this way, one GaugeValueAxis can be share by
		multiple Gauges.
		@irCenter The Center of current Gauge.
		*/
		void GenerateGridLines(const Point2D& irCenter);
		/**
		Generate axis labels. 
		This method can be called multiple times. Each time, the caller can pass a different parameter. All the labels will
		be put into one collection so that they can be selected together. In this way, one GaugeValueAxis can be share by
		multiple Gauges.
		@irCenter The Center of current Gauge.
		*/
		void GenerateLabels(const Point2D& irCenter);
				
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

	protected:	
		/**
		This is a helper method for hCreateTickLabels(). This method will create one tick label.
		@param irTripleId The TripleId of axis labels.
		@param iTickValue Specifies the value of the label to be created.
		*/
		virtual void hCreateOneTickLabel(const TripleId& irTripleId, double iTickValue);
		
		/// Inherited from ValueAxis. Build relation between step and radius.
		virtual double hComputeStep(double iMaximum, double iMinimum, bool iIncludeZero);

		float mStartAngle;		///< Indicates the start angle of current Gauge axis.		
		float mStopAngle;		///< Indicates the stop angle of current Gauge axis.				
		float mRadius;			///< Indicates the radius of current Gauge.
		Point2D mCenter;		///< Indicates the center of current Gauge.
		bool mUseOldStyle;		///< Indicates whether to use old style or not.
		bool mDrawLabelsOutside;///< Indicates whether to draw labels outside or not. This option is disabled when mUseOldStyle is false.

	private:
		/**
		This is a helper method of GenerateGridLines().
		@param irTripleId The triple ID of current grid line.
		@param iTickValue Specifies the corresponding value of current grid line.
		@param ipGridLineSect Specifies the GraphCollectionObject in which current grid line needs to be put.
		*/
		void hCreateOneGridLine(const TripleId& irTripleId, double iTickValue, GraphCollectionObject* ipGridLineSet);
		
		/**
		Get standard coordinates from polar coordinates.
		@iAngleRad Specifies an angle in radian.
		@iRadius Specifies a radius.
		@return Return the standard coordinates acoording the input parameters and mCenter.
		*/
		Point2D hGetPointByAngleAndRadius(double iAngleRad, double iRadius) const;	

		/**
		This is a helper method of hCreateOneTickLabel().
		@param iorLabelCenter The label center which need to be adjusted. Here, "Center" only means horizontal direction.
		@param iLabelWidth Specifies the current label width.
		@iAngle Specifies the angle of current label.
		*/
		void hAdjustLabelCenterByAngle(Point2D& iorLabelCenter, Int32 iLabelWidth, float iAngle) const;
	};	
	
}
#endif
