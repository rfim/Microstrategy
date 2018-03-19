//==============================================================================================
//	FILENAME	:	Point2D.h
//	AUTHOR		:	Liang Liu
//	CREATION	:	2008-09-25
//	Copyright (C) MicroStrategy Incorporated 2008
//	All Rights Reserved
//==============================================================================================

#ifndef MsiChart_Point2D_h
#define MsiChart_Point2D_h

#include "PDCHeader/PDCvector"
#include "PDCHeader/PDCcmath"

#ifdef _VE_CE
#pragma warning(disable : 4244)
#pragma warning(disable : 4305)
#endif

namespace MsiChart
{
	/**
	This is a template class for 2D point.
	*/
	template <typename T = double>
	class Point
	{
	public:
		/**
		Constructs from X and Y coordinates.
		@param iX X-coordinate of the point.
		@param iY Y-coordinate of the point.
		*/
		Point(T iX = T(), T iY = T()): x(iX), y(iY){}
		/**
		Copy constructor
		@param irPoint Another Point<T> object.
		*/
		Point(const Point<T>& irPoint):x(irPoint.x), y(irPoint.y){}
		/**
		Constructs from a Point<U> object.
		@param irPoint A Point<U> object.
		*/
		template <typename U>
		Point(const Point<U>& irPoint);

		/**
		Set new coordinates for current point.
		@param iX X-coordinate of the point.
		@param iY Y-coordinate of the point.
		*/
		void Reset(T iX, T iY);

		/**
		Computes the square of the distance between two points.
		@irPoint Another point.
		@return The square of the distance between current point and irPoint.
		*/
		double DistanceSquared(const Point<T>& irPoint) const;
		/**
		Computes the distance between two points.
		@irPoint Another point.
		@return The distance between current point and irPoint.
		*/
		double Distance(const Point<T>& irPoint) const;

		/**
		Check whether current point is inside a polygon or not.
		Reference: http://local.wasp.uwa.edu.au/~pbourke/geometry/insidepoly/
		@param irPolygon Specifies a polygon.
		@return Return true if current point is inside irPolygon. Otherwise, return false.
		*/
		bool IsInPolygon(const std::vector<Point<T> >& irPolygon) const;
		
		/**
		Check whether current point is strictly inside a polygon or not.
		A point is strictly inside a polygon if and only if (x+1,y),(x,y+1),(x-1,y) and (x,y-1) are inside that polygon.
		@param irPolygon Specifies a polygon.
		@return Return true if current point is strictly inside irPolygon. Otherwise, return false.
		*/
		bool IsStrictlyInPolygon(const std::vector<Point<T> >& irPolygon) const;
		/**
		Check whether two points coincide with each other.
		@irPoint Specifies another point.
		@return Return true if the coordinates of current point is not the same as that of irPoint.
		Otherwise, return false.
		*/
        
		bool operator != (const Point<T>& irPoint) const;
        bool operator == (const Point<T>& irPoint) const;
		/**
		Return the central symmetric point of current point.
		@return Point<T>(-x, -y).
		*/
		Point<T> operator - () const;

		/**
		Rotate a point relative to another point.
		@irAnchor Specifies a reference point.
		@iRadian Specifies a rotation angle.
		*/
		void Rotate(const Point<T>& irAnchor, double iRadian);

		/**
		Rotate a point relative to another point.
		@irAnchor Specifies a reference point.
		@iRadian Specifies a rotation angle.
		@return The rotated point.
		*/
		Point<T> RotateAndClone(const Point<T>& irAnchor, double iRadian);

		/**
		Computes the midpoint of two point.
		@irPoint Specifies another point.
		@return The midpoint of current point and irPoint.
		*/
		Point<T> MiddleTo(const Point<T>& irPoint) const;

		/**
		Computes a shifted point based on current point and an offset.
		@irOffset Specifies an offset based on current point.
		@return Point<T>(x+irOffset.x,y+irOffset.y).
		*/
		Point<T> Shifted(const Point<T>& irOffset) const;

		/**
		 *	Swap current point's coordinate
		 */
		void Swap();

		/**
		 *	Swap point value with another one
		 */
		void Swap(Point<T>& iorPoint);

		/**
		Check whether current point is on a segment or not.
		@param irSegmentStart,irSegmentEnd Specifies a line segment.
		@iExclude Indicates whether the two end points need to be excluded or not.
		*/
		bool IsOnSegment(const Point<T>& irSegmentStart, const Point<T>& irSegmentEnd, bool iExclude) const;

		/**
		Check whether current point is on a vertical/horizontal segment or not.
		@param irSegmentStart,irSegmentEnd Specifies a line segment.
		@return If it is on a vertical/horizontal segment, return true. Otherwise return false.
		*/
		bool IsOnVerOrHorSegment(const Point<T>& irSegmentStart, const Point<T>& irSegmentEnd) const;

#ifndef _VE_CE
		/**
		 Get the CGPoint version of current point.
		 @return Return current point in CGPoint format.
		 */
		CGPoint GetCGPoint() const;
#endif
		
		/**
		 Get the distance between current point and an input polygon.
		 @param irPolygon Specifies a polygon.
		 @return Return the distance between current point and an input polygon.
		 If current point is inside the input polygon, return 0.
		 */
		double DistanceToPolygon(const std::vector<Point<T> >& irPolygon) const;
		
		/**
		 Get the distance between current point and an input line segment.
		 @param irLineStart,irLineEnd Specifies a line segment.
		 @return Return the distance between current point and an line segment.	
		 */
		double DistanceToLine(const Point<T>& irLineStart, const Point<T>& irLineEnd) const;


		T x;	///< X-coordinate of current point.
		T y;	///< Y-coordinate of current point.
	};


	template<typename T>
	template<typename U> Point<T>::Point(const Point<U>& iPoint)
	{
		x = static_cast<T>(iPoint.x);
		y = static_cast<T>(iPoint.y);
	}

	template<typename T>
	inline void Point<T>::Reset(T iX, T iY)
	{
		x = iX;
		y = iY;
	}

	template<typename T>
	double Point<T>::DistanceSquared(const Point<T>& irPoint) const
	{
		return (x - irPoint.x) * (x - irPoint.x)
			+ (y - irPoint.y) * (y - irPoint.y);
	}

	template<typename T>
	double Point<T>::Distance(const Point<T>& irPoint) const
	{
		return std::sqrt(DistanceSquared(irPoint));
	}

	template<typename T>
	bool Point<T>::operator != (const Point<T>& irPoint) const
	{
		return ((x != irPoint.x) || (y != irPoint.y));
	}
    
    template<typename T>
    bool Point<T>::operator == (const Point<T>& irPoint) const
    {
        return ((x == irPoint.x) && (y == irPoint.y));
    }

	template<typename T>
	Point<T> Point<T>::operator - () const
	{
		return Point<T>(-x, -y);
	}

	//	Reference: http://local.wasp.uwa.edu.au/~pbourke/geometry/insidepoly/
	template<typename T>
	bool Point<T>::IsInPolygon(const std::vector<Point<T> >& irPolygon) const
	{
		unsigned Int32 lNumberOfEdges = (Int32)irPolygon.size();
		if (lNumberOfEdges < 3) // A polygon should have at least 3 edges.
			return false;

        //[consider to do] Li en: special case, across at the end point of the edge.
		bool lResult = false;
		for (unsigned Int32 i = 0, j = lNumberOfEdges - 1; i < lNumberOfEdges; j = i++)
		{
			if (((irPolygon[i].y <= y && y < irPolygon[j].y) ||
				(irPolygon[j].y <= y && y < irPolygon[i].y)) &&
				x <= (irPolygon[j].x - irPolygon[i].x) * (y - irPolygon[i].y) * 1.0 / (irPolygon[j].y - irPolygon[i].y) + irPolygon[i].x)
			{
				lResult = !lResult;
			}
		}
		return lResult;
	}

	template<typename T>
	bool Point<T>::IsStrictlyInPolygon(const std::vector<Point<T> >& irPolygon) const
	{		
		if (!Point<T>(x + 0.1f, y).IsInPolygon(irPolygon))
			return false;
		if (!Point<T>(x - 0.1f, y).IsInPolygon(irPolygon))
			return false;
		if (!Point<T>(x, y + 0.1f).IsInPolygon(irPolygon))
			return false;
		if (!Point<T>(x, y - 0.1f).IsInPolygon(irPolygon))
			return false;
		return true;		
	}

	template<typename T>
	void Point<T>::Rotate(const Point<T>& irAnchor, double iRadian)
	{
		double lXDist = x - irAnchor.x;
		double lYDist = y - irAnchor.y;

		x = static_cast<T>(irAnchor.x + cos(iRadian) * lXDist - sin(iRadian) * lYDist);
		y = static_cast<T>(irAnchor.y + sin(iRadian) * lXDist + cos(iRadian) * lYDist);
	}

	template<typename T>
	Point<T> Point<T>::RotateAndClone(const Point<T>& irAnchor, double iRadian)
	{
		Rotate(irAnchor, iRadian);
		return *this;
	}

	// specialize for Int32
	template<>
	void Point<Int32>::Rotate(const Point<Int32>& irAnchor, double iRadian);

	//template<>
	//Point<Int32> Point<Int32>::RotateAndClone(const Point<Int32>& irAnchor, double iRadian);

	template<typename T>
	Point<T> Point<T>::MiddleTo(const Point<T> &irPoint) const
	{
		return Point<T>((x + irPoint.x) / 2, (y + irPoint.y) / 2);
	}

	template<typename T>
	Point<T> Point<T>::Shifted(const Point<T>& irOffset) const
	{
		return Point<T>(x + irOffset.x, y + irOffset.y);
	}

	template<typename T>
	void Point<T>::Swap()
	{
		std::swap(x, y);
	}

	template<typename T>
	void Point<T>::Swap(Point<T>& iorPoint)
	{
		std::swap(x, iorPoint.x);
		std::swap(y, iorPoint.y);
	}

	template<typename T>
	bool Point<T>::IsOnSegment(const Point<T>& iSegmentStart, const Point<T>& iSegmentEnd, bool iExclude) const
	{		
		const float lTolerance = iExclude ? 0.01f : -0.01f;
		if(fabs(iSegmentStart.x - iSegmentEnd.x) < 0.01f)//vertical segment, so compare Y
		{
			if((y - iSegmentStart.y) > lTolerance && (iSegmentEnd.y - y) > lTolerance)
				return true;
			if((y - iSegmentEnd.y) > lTolerance && (iSegmentStart.y - y) > lTolerance)
				return true;
		}
		else//not vertical, so compare X
		{
			if((x - iSegmentStart.x) > lTolerance && (iSegmentEnd.x - x) > lTolerance)
				return true;
			if((x - iSegmentEnd.x) > lTolerance && (iSegmentStart.x - x) > lTolerance)
				return true;
		}
		return false;
	}

	template<typename T>
	bool Point<T>::IsOnVerOrHorSegment(const Point<T>& irSegmentStart, const Point<T>& irSegmentEnd) const
	{
		if (y == irSegmentStart.y && y == irSegmentEnd.y)
		{
			return ((x - irSegmentStart.x) >= 0 && (irSegmentEnd.x - x) >= 0) ||
				((x - irSegmentStart.x) <= 0 && (irSegmentEnd.x - x) <= 0);
		}

		if (x == irSegmentStart.x && x == irSegmentEnd.x)
		{
			return ((y - irSegmentStart.y) >= 0 && (irSegmentEnd.y - y) >= 0) ||
				((y - irSegmentStart.y) <= 0 && (irSegmentEnd.y - y) <= 0);
		}
		return false;
	}
	
#ifndef _VE_CE
	template<typename T>
	CGPoint Point<T>::GetCGPoint() const
	{
		return CGPointMake(x, y);
	}
#endif

	template<typename T>
	double Point<T>::DistanceToPolygon(const std::vector<Point<T> >& irPolygon) const
	{
		if (IsInPolygon(irPolygon))
			return 0;
		
		const size_t lNumOfEdges = irPolygon.size();
		double lDistance = DistanceToLine(irPolygon[0], irPolygon[lNumOfEdges - 1]);		
		for (size_t i = 0; i + 1 < lNumOfEdges; ++i)
		{
			double lNewDistance = DistanceToLine(irPolygon[i], irPolygon[i + 1]);
			if (lDistance > lNewDistance)
			{
				lDistance = lNewDistance;
			}
		}
		return lDistance;
	}
	
	template<typename T>
	double Point<T>::DistanceToLine(const Point<T>& irLineStart, const Point<T>& irLineEnd) const
	{
		double lPointToStart2 = DistanceSquared(irLineStart);
		double lPointToEnd2 = DistanceSquared(irLineEnd);
		double lStartToEnd2 = irLineStart.DistanceSquared(irLineEnd);
		
		const float lTolerance = 0.01;
		if (lPointToStart2 < lTolerance || lPointToEnd2 < lTolerance)
			return 0;
		if (lStartToEnd2 < lTolerance)
			return std::sqrt(lPointToStart2);
		
		if (lPointToStart2 >= lPointToEnd2 + lStartToEnd2)
			return std::sqrt(lPointToEnd2);
		
		if (lPointToEnd2 >= lPointToStart2 + lStartToEnd2)
			return std::sqrt(lPointToStart2);
		
		Point<T> lStartToPoint(x - irLineStart.x, y - irLineStart.y);
		Point<T> lStartToEnd(irLineEnd.x - irLineStart.x, irLineEnd.y - irLineStart.y);
		double lCrossProduct = lStartToPoint.x * lStartToEnd.y - lStartToPoint.y * lStartToEnd.x;
		return (lCrossProduct < 0 ? -lCrossProduct : lCrossProduct) / std::sqrt(lStartToEnd2);
		
	}

	///

	typedef Point<Int32> Point2D;
	typedef Point<float> FPoint2D;
#ifdef _VE_CE
	typedef Point<double> DPoint2D;
#endif
}

#endif
