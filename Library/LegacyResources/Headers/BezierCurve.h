//==============================================================================================
//	FILENAME	:	BezierCurve.h
//	AUTHOR		:	Liang Liu
//	CREATION	:	2009-01-04
//	Copyright (C) MicroStrategy Incorporated 2009
//	All Rights Reserved
//==============================================================================================

#ifndef MsiChart_BezierCurve_h
#define MsiChart_BezierCurve_h

#include "Rect2D.h"
#include "Synch/Defines/SmartPtr.h"

struct _cairo;
typedef struct _cairo cairo_t;

namespace MsiChart
{
#ifdef _VE_CE
	class ChartContext;
#endif
	/**
	This class mainly provides some basic operations for explicit cubic Bezier curve. 
	Here, "explicit" means that the x-coordinates (vertical) or y-coordinates (horizontal) of the control points are
	evenly spaced. This class is the foundation class for curve lines.
	\n Reference: http://www.tsplines.com/resources/class_notes/Bezier_curves.pdf
	*/
	class BezierCurve
	{
	public:
		typedef MSynch::SmartPtr<BezierCurve> Ptr;		
		/**
		@param irP0,irP1,irP2,irP3 Specifies four control points. For P1 or P2, only one coordinate needs to be provided 
		because the other coordinate can be derived from P0 and P3. For example, if iIsVertical is true, then 
		P1.x=(2*P0.x+P3.x)/3 and P2.x=(P0.x+2*P3.x)/3.
		@param iIsVertical Indicates whether current Bezier curve is vertical or not.
		@param iIsXAlwaysIndependent Indicates whether x-coordinates are always independent or not.
		Internally, x-coordinates always corresponds to independent variable for convience.
		*/
		BezierCurve(const FPoint2D& irP0, float iP1, float iP2, const FPoint2D& irP3, 
			bool iIsVertical, bool iIsXAlwaysIndependent = false);	

		/**
		Get an approximate polygon of current BezierCurve.	
		@param iLineWidth Specifies the line width of current BezierCurve.
		@param orPolygon The output polygon.
		*/
		virtual void GetApproximatePolygon(Int32 iLineWidth, std::vector<Point2D>& orPolygon) const;

		/**
		Get an approximate polyline of current BezierCurve.	
		@param iLineWidth Specifies the line width of current BezierCurve.
		@param orPolygon The output polygon.
		*/
		void GetApproximatePolyline(std::vector<Point2D>& orPolyline) const;

		/**
		Reverse the bezier curve.
		*/
		void Reverse();

#ifdef _VE_CE
		/**
		This is a helper method for creating a cairo path.
		Move to the start point, namely P0.
		@param ipContext is a pointer to the ChartContext.
		@param iTuning If this is true, then:
		1. When mIsVertical is true, the first and the last control points will be shifted by (0.5, 0);
		2. When mIsVertical is false, the first and the last control points will be shifted by (0, 0.5);		
		*/
		void MoveToStart(ChartContext* ipContext, bool iTuning = false) const;
		/**
		This is a helper method for creating a cairo path.
		Draw a curve path from P0 to P3.
		@param ipContext is a pointer to the ChartContext.
		@param iTuning If this is true, then:
		1. When mIsVertical is true, the first and the last control points will be shifted by (0.5, 0);
		2. When mIsVertical is false, the first and the last control points will be shifted by (0, 0.5);	
		*/
		void CurveToEnd(ChartContext* ipContext, bool iTuning = false) const;
		/**
		This is a helper method for creating a cairo path.
		Move to the start point, namely P0.
		@param ipCGPath core graphic path.
		@param iTuning If this is true, then:
		1. When mIsVertical is true, the first and the last control points will be shifted by (0.5, 0);
		2. When mIsVertical is false, the first and the last control points will be shifted by (0, 0.5);	
		*/
		void LineToStart(ChartContext* ipContext, bool iTuning = false) const;
#else
		/**
		This is a helper method for creating a cairo path.
		Move to the start point, namely P0.
		@param ipCGPath core graphic path.
		@param iTuning If this is true, then:
		1. When mIsVertical is true, the first and the last control points will be shifted by (0.5, 0);
		2. When mIsVertical is false, the first and the last control points will be shifted by (0, 0.5);		
		*/
		void MoveToStart(CGPath* ipCGPath, bool iTuning = false) const;
		/**
		This is a helper method for creating a cairo path.
		Draw a curve path from P0 to P3.
		@param ipCGPath core graphic path.
		@param iTuning If this is true, then:
		1. When mIsVertical is true, the first and the last control points will be shifted by (0.5, 0);
		2. When mIsVertical is false, the first and the last control points will be shifted by (0, 0.5);	
		*/
		void CurveToEnd(CGPath* ipCGPath, bool iTuning = false) const;		
		/**
		This is a helper method for creating a cairo path.
		Move to the start point, namely P0.
		@param ipCGPath core graphic path.
		@param iTuning If this is true, then:
		1. When mIsVertical is true, the first and the last control points will be shifted by (0.5, 0);
		2. When mIsVertical is false, the first and the last control points will be shifted by (0, 0.5);	
		*/
		void LineToStart(CGPath* ipCGPath, bool iTuning = false) const;
#endif

		/**
		Get start point.
		@return Return the start point, namely mP0.
		*/
		FPoint2D GetStartPoint() const;

		/**
		Get end point.
		@return Return the end point, namely mP3.
		*/
		FPoint2D GetEndPoint() const;

		/**
		Evaluate at a single point from a given independent value.
		@param iIndependentValue Specifies a value of indenpendent variable.
		@return Return the corresponding value of dependent variable.		
		*/
		double Evaluate(double iIndependentValue) const;
		/**
		Evaluate the derivative at a single point.
		@param iIndependentValue Specifies a value of indenpendent variable.
		@return The evaluation resul, namely y=(((P0'*s+2*P1')*s+P2')*t*t.
		*/
		double EvaluateDerivative(double iIndependentValue) const;

		/**
		Evaluate second derivative at a given independent value.
		@param iIndependentValue Specifies a value of indenpendent variable.
		@return Return the second derivative.		
		*/
		double Evaluate2ndDerivative(double iIndependentValue) const;
		
		/**
		Evaluate third derivative of current BezierCurve.
		@return Return the third derivative.		
		*/
		double Evaluate3rdDerivative() const;

		/**
		Get the start value of independent variable of Bezier curve;
		@return Return the start value of independent variable.
		*/
		float GetStartIndependent() const;
		/**
		Get the end value of independent variable of Bezier curve;
		@return Return the end value of independent variable.
		*/
		float GetEndIndependent() const;

		/**
		Get the minimum and maximum dependent values on current Bezier curve;
		@param orMinY Output the minimum dependent value on current Bezier curve;
		@param orMaxY Output the maximum dependent value on current Bezier curve;
		*/
		void GetMinMax(Int32& orMinY, Int32& orMaxY) const;

		/**
		Get the minimum independent on current Bezier curve;
		@return Return the minimum independent on current Bezier curve;
		*/
		float GetMinX() const;
		/**
		Get the maximum independent on current Bezier curve;
		@return Return the maximum independent on current Bezier curve;
		*/
		float GetMaxX() const;	

		/**
		Clone another BezierCurve which is identical to current BezierCurve.
		@return Return the cloned BezierCurve.
		*/
		BezierCurve::Ptr Clone() const;

		/**
		This method is used for DepthCurveLine.
		Clone a reverse shift BezierCurve based on current BezierCurve.
		*/
		BezierCurve::Ptr CloneReverseShift(const Point2D& irOffset) const;

		/**
		Check whether current BezierCurve is used in vertical graph or not.
		@return Return true if current graph is vertical.
		*/
		bool IsVertical() const;
		
		/**
		This is a helper method for DepthCurveLineObject.
		Check whether current depth Bezier curve need to be drawn before another depth Bezier curve. 
		@irOffset Specifies the depth direction for depth curve line.
		@iBezierCurvePtr Specifies another BezierCurve.
		@return Return true if current depth curve need to be drawn before the input curve.
		*/
		bool IsBefore(const Point2D& irOffset, BezierCurve::Ptr iBezierCurvePtr) const;
		/**
		This is a helper method for DepthCurveLineObject.
		Split current BezierCurve at tangent points along the direction specified by irOffset.
		@param irOffset Specifies the depth direction for depth curve line.
		@param orSplitCurves After splitting, the current Bezier curve will become the first piece of Bezier curve.
		The other split Bezier curves will be put into orSplitCurves.
		*/
		void SplitAtTangentPoints(const Point2D& irOffset, std::vector<BezierCurve::Ptr>& orSplitCurves);
		/**
		This is a helper method for DepthCurveLineObject.
		Split current BezierCurve at the intersections with another BezierCurve.
		@param iBezierCurvePtr Specifies another BezierCurve.
		@param orSplitCurves After splitting, the current Bezier curve will become the first piece of Bezier curve.
		The other split Bezier curves will be put into orSplitCurves.
		*/
		void SplitAtIntersections(BezierCurve::Ptr iBezierCurvePtr, std::vector<BezierCurve::Ptr>& orSplitCurves);

	protected:
		/**
		@param irP0,irP1,irP2,irP3 Specifies four control points.
		*/
		BezierCurve(const FPoint2D& irP0, const FPoint2D& irP1, const FPoint2D& irP2, const FPoint2D& irP3);	

		FPoint2D mP0;
		FPoint2D mP1;
		FPoint2D mP2;
		FPoint2D mP3;
	private:
		/**
		Split current BezierCurve into two small Bezier curves.
		@param iTao Specifies the split position. 0<iTao<1.
		@return The current BezierCurve will become the first split curve. The second split curve will be returned. 
		*/
		BezierCurve::Ptr hSplit(double iTao);			

		/**
		Solve Quadratic Equation A*x*x+2*B*x+C=0. The positive roots will be put into orPositiveRoots.
		@param iA,iB,iC Specify a quadratic equation.
		@param orPositiveRoots Output the positive roots.
		*/
		void hSolveQuadraticEquation(double iA, double iB, double iC, std::vector<double>& orPositiveRoots) const;
		
		/**
		Solve Cubic Equation A*x*x*x+3*B*x*x+3*C*x+D=0. The positive roots will be put into orPositiveRoots.
		@param iA,iB,iC Specify a quadratic equation.
		@param orPositiveRoots Output the positive roots.
		*/
		void hSolveCubicEquation(double iA, double iB, double iC, double iD, std::vector<double>& orPositiveRoots) const;

		/**
		Evaluate at a single point.
		@param iS Specifies a point on current Bezier curve. iS=(1-t)/t.
		@return The evaluation resul, namely y=(((P0*s+3*P1)*s+3*P2)*s+P3)*t*t*t.
		*/
		double hEvaluate(double iS) const;
		
		/**
		Evaluate the derivative at a single point.
		@param iS Specifies a point on current Bezier curve. iS=(1-t)/t.
		@return The evaluation resul, namely y=(((P0'*s+2*P1')*s+P2')*t*t.
		*/
		double hEvaluateDerivative(double iS) const;

		/**
		Get tangent positions.
		@param iSlope Specifies a direction.
		@param Output the positions ((1-t)/t) in decending order.
		*/
		void hGetTangentPositions(double iSlope, std::vector<double>& orPositions) const;

		/**
		Split current Bezier curve at given positions.
		@param iorPositions Specifies the split positions ((1-t)/t)) in decending order. After this method is called,
		iorPositions will become empty.
		@param orSplitCurves After splitting, the current Bezier curve will become the first piece of Bezier curve.
		The other split Bezier curves will be put into orSplitCurves.
		*/
		void hSplitAtPositions(std::vector<double>& iorPositions, std::vector<BezierCurve::Ptr>& orSplitCurves);

		/**
		This is a helper method of SplitAtIntersections().
		@param ipBezierCurve Specifies another BezierCurve.
		@param orIntersections Output the x-coordinates of the intersection points.
		*/
		void hGetIntersections(const BezierCurve* ipBezierCurve, std::vector<double>& orIntersections) const;

		/**
		Get a sub curve based on current BezierCurve.
		@param iValueX specifies a position on current BezierCurve.
		@param iisRightPart Indicates whether the right sub-curve or the left sub-curve is needed.
		@return Return a sub-curve of current BezierCurve.
		*/
		BezierCurve::Ptr hGetSubCurve(float iValueX, bool iIsRightPart) const;		

		/**
		If mP0.x < mP3.x, do nothing. Otherwise, do the following manipulation.
		1. Swap mP0 and mP3.
		2. Swap mP1 and mP2.
		*/
		void hMakeXAscending();

		/**
		Get Parameter Tao from x-coordinate.
		@param specifies a position on current BezierCurve.
		@return Return the corresponding parameter of iValueX. t=(X-P0.x)/(P3.x-P0.x).
		*/
		float hGetTaoFromX(float iValueX) const;

		/**
		Make delta BezierCurve by calculate the difference between current BezierCurve and an input BezierCurve.
		The current BezierCurve will become the delta BezierCurve.
		@param iBezierCurvePtr Specifies another BezierCurve.
		*/
		void hMakeDeltaBezier(BezierCurve::Ptr iBezierCurvePtr);

		/**
		Get the x-coordinates which have zero y-coordinates. 		
		@param orXRoots Output the root of current BezierCurve equation.
		If all the points on current BezierCurve have zero y-coordinates, no x-coordinates will be put into orXRoots.
		*/
		void hGetXRoots(std::vector<double>& orXRoots) const;
	
		/**
		Check whether a line segment has intersection with current BezierCurve.
		@param irStart,irEnd Specify a line segment.
		@return Return true if the input line segment has intersection with current BezierCurve.
		Otherwise, return false.
		*/
		bool hHasIntersection(const FPoint2D& irStart, const FPoint2D& irEnd) const;

		/**
		Calculate the cubic root of a given value.
		@param iValue Specifies a double value.
		@return Return the cubic root of iValue.
		*/
		double hCubicRoot(double iValue) const;	
		
		/** 
		Check whether all the control points have the same Y value.
		@return Return true if all the control points have the same Y value.
		*/
		bool hHaveSameY() const;

		bool mIsVertical;
	};	

	inline FPoint2D BezierCurve::GetStartPoint() const
	{		
		return mIsVertical ? mP0 : FPoint2D(mP0.y, mP0.x);
	}

	inline FPoint2D BezierCurve::GetEndPoint() const
	{
		return mIsVertical ? mP3 : FPoint2D(mP3.y, mP3.x);
	}

	inline bool BezierCurve::IsVertical() const
	{
		return mIsVertical;
	}
	
	inline float BezierCurve::GetMinX() const
	{
		return mP0.x < mP3.x ? mP0.x : mP3.x;
	}

	inline float BezierCurve::GetMaxX() const
	{
		return mP0.x < mP3.x ? mP3.x : mP0.x;
	}
	inline float BezierCurve::hGetTaoFromX(float iValueX) const
	{
		return (iValueX - mP0.x) / (mP3.x - mP0.x);
	}

	inline float BezierCurve::GetStartIndependent() const
	{
		return mP0.x;
	}
	
	inline float BezierCurve::GetEndIndependent() const
	{
		return mP3.x;
	}

	inline bool BezierCurve::hHaveSameY() const
	{
		return mP0.y == mP1.y &&  mP1.y == mP2.y && mP2.y == mP3.y;
	}
}

#endif