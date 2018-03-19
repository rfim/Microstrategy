//==============================================================================================
//	FILENAME	:	CurveLineObject.h
//	AUTHOR		:	Xin Liu, Bing Huang
//	CREATION	:	11/19/2008
//	Copyright (C) MicroStrategy, Inc. 2008
//==============================================================================================

#ifndef MSICHART_CURVELINEROBJECT_H
#define MSICHART_CURVELINEROBJECT_H

#include "PDCHeader/PDCvector"
#include "GraphObject.h"
#include "BezierCurve.h"

namespace MsiChart
{	
	/**
	This class delegates curve lines. We use Bezier curve here.
	*/
	class CurveLineObject: public GraphObject
	{
	public:
		typedef MSynch::SmartPtr<CurveLineObject> Ptr;

		/**	Constructor.
		@param irPoints Specifies the data points in the curve line. 	
		@param iIsClosePath Indicates whether the path is closed.
		*/
		CurveLineObject(const TripleId& irTripleId, GraphObjectManager* ipManager, 
			const std::vector<Point2D>& irPoints, GraphCollectionObject* ipParentObject = NULL, bool iHasLineFormat = true, bool iIsVertical = true, bool iIsClosePath = false);

		/** Constructor.
		@param iBezierCurvePtr Specifies the input bezier curve line.
		@param iIsClosePath Indicates whether the path is closed.
		*/
		CurveLineObject(const TripleId& irTripleId, GraphObjectManager* ipManager, 
			BezierCurve::Ptr iBezierCurvePtr, GraphCollectionObject* ipParentObject = NULL, bool iHasLineFormat = true, bool iIsClosePath = false);
		~CurveLineObject();

		// virtual functions from GraphObject
		virtual void Draw() const;
		virtual void DrawOutLine();
		virtual bool PointInObject(const Point2D& irPoint);
		virtual bool ObjectInRectangle(const Rect2D& irRect);	
		virtual void GenerateImageMap(ImageMap& iorImageMap);

		/**
		This function adds one bezier curve line to the curve line object.
		@param iBezierCurvePtr Specifies the input bezier curve line.
		*/
		void AddBezierCurveLine(BezierCurve::Ptr iBezierCurvePtr);

		/**
		This is a helper method for creating a cairo path.
		Move to the start point of the curve line.
		@param iIsLinePath If this is true, the curve line will be shifted by (0.5, 0,5).
		*/
		void MoveToStart(bool iIsLinePath = false) const;

		/**
		This is a helper method for creating a cairo path.
		Draw a curve path to the end point of the curve line.
		@param iIsLinePath If this is true, the curve line will be shifted by (0.5, 0,5).
		*/
		void CurveToEnd(bool iIsLinePath = false) const;	

		/**
		This is a helper method for creating a cairo path.
		Move to the start point of the curve line.
		@param iIsLinePath If this is true, the curve line will be shifted by (0.5, 0,5).
		*/
		void LineToStart(bool iIsLinePath = false) const;

		/**
		Get start point.
		@return Return the start point of the curve line.
		*/
		FPoint2D GetStartPoint() const;

		/**
		Get end point.
		@return Return the end point of the curve line.
		*/
		FPoint2D GetEndPoint() const;

		/**
		Evaluate at a single point from a given independent value.
		@param iIndependentValue Specifies a value of indenpendent variable.
		@return Return the corresponding value of dependent variable.		
		*/
		double Evaluate(Int32 iIndependentValue) const;

		/**
		Get the minimum and maximum values on the curve line;
		@param orMin Output the minimum value on the curve line;
		@param orMax Output the maximum value on the curve line;
		*/
		void GetMinMax(Int32& orMin, Int32& orMax) const;

		/**
		Get an approximate polyline of the curve line.	
		@param orPolygon The output polygon.
		*/
		void GetApproximatePolyline(std::vector<Point2D>& orPolyline) const;

		/**
		Create a new Curve line which is reversed and shifted version of current curve line.
		Reversed:	1. the order of bezier curves in mBezierCurves is reversed.
					2. each bezier curve in mBezierCurves itself is reversed.
		Shifted:	The curve line is moved with offset "irOffset".
		@param irOffset The input offset.
		@return The newly created curve line.
		*/
		CurveLineObject::Ptr CloneReverseShift(const Point2D& irOffset);

		/**
		Clone a new Curve line which has the same content as current curve line.
		@return The newly created curve line.
		*/
		CurveLineObject::Ptr Clone() const;

		/**
		Reverse the current curve line object.
		*/
		void Reverse();

		/**
		Get the minimum independent on current curve;
		@return Return the minimum independent on current curve;
		*/
		float GetMinX() const;
		/**
		Get the maximum independent on current curve;
		@return Return the maximum independent on current Bezier curve;
		*/
		float GetMaxX() const;	

		/**
		This is a helper method for DepthCurveLineObject.
		Check whether current depth curve line need to be drawn before another depth curve line. 
		@irOffset Specifies the depth direction for depth curve line.
		@iCurveLinePtr Specifies another Curve Line.
		@return Return true if current depth curve need to be drawn before the input curve.
		*/
		bool IsBefore(const Point2D& irOffset, CurveLineObject::Ptr iCurveLinePtr) const;

		/**
		This is a helper method for DepthCurveLineObject.
		Split current CurveLine at tangent points along the direction specified by irOffset.
		@param irOffset Specifies the depth direction for depth curve line.
		@param orSplitCurves After splitting, the current depth curve lines will become the first piece of curve lines.
		The other split depth curve lines will be put into orSplitCurves.
		*/
		void SplitAtTangentPoints(const Point2D& irOffset, std::vector<CurveLineObject::Ptr>& orSplitCurves);

		/**
		This is a helper method for DepthCurveLineObject.
		Split current Curve Line at the intersections with another Curve Line.
		@param iCurveLinePtr Specifies another Curve Line.
		@param orSplitCurves After splitting, the current curve line will become the first piece of curve line.
		The other split curve lines will be put into orSplitCurves.
		*/
		void SplitAtIntersections(CurveLineObject::Ptr iCurveLinePtr, std::vector<CurveLineObject::Ptr>& orSplitCurves);

	protected:
		//pure virtual functions from GraphObject
		virtual void GetClockwisePolygon(std::vector<Point2D>& orPolygon) const;

	private:
		/**
		This function generates the bezier curves according to the input data points.
		@param irPoints Specifies the data points.
		*/
		void hGenerateBezierCurveLines(const std::vector<Point2D>& irPoints);

		/**
		This is a helper method for SplitAtTangentPoints().
		@param irOffset Specifies the depth direction for depth curve line.
		@param orSplitCurves After splitting, the current depth curve lines will become the first piece of curve lines.
		The other split depth curve lines will be put into orSplitCurves.
		*/
		void hSplitAtTangentPointRecursive(const Point2D& irOffset, std::vector<CurveLineObject::Ptr>& orSplitCurves);

		/**
		This is a helper method for SplitAtIntersections().
		It splits the curves only if the intersection points with input curve are inside some bezier curves.
		@param iCurveLinePtr The input curve line.
		@param orSplitCurves After splitting, the current depth curve lines will become the first piece of curve lines.
		The other split depth curve lines will be put into orSplitCurves.
		*/
		void hSplitInsideBezierRecursive(CurveLineObject::Ptr iCurveLinePtr, std::vector<CurveLineObject::Ptr>& orSplitCurves);

		/**
		This is a helper method for SplitAtIntersections().
		It splits the curves only if the intersection points with input curve happened to 
		be the boundary points of some bezier curves.
		@param iCurveLinePtr The input curve line.
		@param orSplitCurves After splitting, the current depth curve lines will become the first piece of curve lines.
		The other split depth curve lines will be put into orSplitCurves.
		*/
		void hSplitEndPointBezierRecursive(CurveLineObject::Ptr iCurveLinePtr, std::vector<CurveLineObject::Ptr>& orSplitCurves);

		/**
		Create a new curve line, first part is iBezierCurvePtr if not NULL, second part is from mBezierCurves[iIndex] to mBezierCurves.end().
		Meanwhile, it will erase the elements in mBezierCurves from mBezierCurves[iIndex] to mBezierCurves.end()
		@param iBezierCurvePtr The input Bezier curve.
		@param iIndex The input index.
		@return The newly created curve line object.
		*/
		CurveLineObject::Ptr hCreateCurveLine(const BezierCurve::Ptr iBezierCurvePtr, Int32 iIndex);

		/**
		Tridiagonal matrix algorithm. Please refer to http://en.wikipedia.org/wiki/Tridiagonal_matrix_algorithm for more details.
		*/
		void hTridiagonalSolve(std::vector<double>& irB, std::vector<double>& irC, std::vector<double>& irD, std::vector<double>& orX);

		std::vector<BezierCurve::Ptr>	mBezierCurves;
		bool mIsVertical;
		bool mIsClosePath;
	};
}

#endif
