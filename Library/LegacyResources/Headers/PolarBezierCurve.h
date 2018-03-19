//==============================================================================================
//	FILENAME	:	PolarPolarBezierCurve.h
//	AUTHOR		:	Liang Liu
//	CREATION	:	2009-03-23
//	Copyright (C) MicroStrategy Incorporated 2009
//	All Rights Reserved
//==============================================================================================

#ifndef MsiChart_PolarBezierCurve_h
#define MsiChart_PolarBezierCurve_h

#include "Rect2D.h"
#include "Synch/Defines/SmartPtr.h"
#include "BezierCurve.h"

struct _cairo;
typedef struct _cairo cairo_t;

namespace MsiChart
{
	/**
	This class provides some basic operations for cubic Bezier curve used in Polar coordinate. 	
	*/
	class PolarBezierCurve : public BezierCurve
	{
	public:
		typedef MSynch::SmartPtr<PolarBezierCurve> Ptr;		
		/**		
		*/
		PolarBezierCurve(const Point2D& irCenter, double iTheta0, double iRou0, double iDRou0,
			double iTheta3, double iRou3, double iDRou3);	
		PolarBezierCurve(FPoint2D& irP0, FPoint2D& irP3);
		PolarBezierCurve(FPoint2D& irP0, FPoint2D& irP1, FPoint2D& irP2, FPoint2D& irP3);

		/**
		Get an approximate polygon of current PolarBezierCurve.	
		@param iLineWidth Specifies the line width of current PolarBezierCurve.
		@param orPolygon The output polygon.
		*/
		virtual void GetApproximatePolygon(Int32 iLineWidth, std::vector<Point2D>& orPolygon) const;

	private:
		/**
		Get an approximate polyline of current PolarBezierCurve.	
		@param iLineWidth Specifies the line width of current PolarBezierCurve.
		@param orPolygon The output polygon.
		*/
		void hGetApproximatePolyline(std::vector<Point2D>& orPolyline) const;	

		/**
		Evaluate at a single point from a given independent value.
		@param iIndependentValue Specifies a value of indenpendent variable.
		@return Return a point on current curve.		
		*/
		Point2D hEvaluate(float iT) const;		
	};	

	
}

#endif