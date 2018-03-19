//==============================================================================================
//	FILENAME	:	Point3D.h
//	AUTHOR		:	Liang Liu
//	CREATION	:	2009-05-18
//	Copyright (C) MicroStrategy Incorporated 2009
//	All Rights Reserved
//==============================================================================================

#ifndef MsiChart_Point3D_h
#define MsiChart_Point3D_h

#include "PDCHeader/PDCvector"
#include "PDCHeader/PDCcmath"

namespace MsiChart
{
	/**
	This is a template class for 3D Point.
	*/
	template <typename T = double>
	class Point3D
	{
	public:
		/**
		Constructs from X, Y and Z coordinates.
		@param iX X-coordinate of the Point.
		@param iY Y-coordinate of the Point.
		@param iZ Z-coordinate of the Point.
		*/
		Point3D(T iX = T(), T iY = T(), T iZ = T()): x(iX), y(iY), z(iZ){}
		/**
		Copy constructor
		@param irPoint Another Point3D<T> object.
		*/
		Point3D(const Point3D<T>& irPoint):x(irPoint.x), y(irPoint.y), z(irPoint.z){}
		/**
		Constructs from a Point3D<U> object.
		@param irPoint A Point3D<U> object.
		*/
		template <typename U>
		Point3D(const Point3D<U>& irPoint);

		/**
		Set new coordinates for current Point.
		@param iX X-coordinate of the Point.
		@param iY Y-coordinate of the Point.
		@param iZ Z-coordinate of the Point.
		*/
		void Reset(T iX, T iY, T iZ);

		/**
		Computes the square of the distance between two Points.
		@irPoint Another Point.
		@return The square of the distance between current Point and irPoint.
		*/
		double DistanceSquared(const Point3D<T>& irPoint) const;
		/**
		Computes the distance between two Points.
		@irPoint Another Point.
		@return The distance between current Point and irPoint.
		*/
		double Distance(const Point3D<T>& irPoint) const;			
		
		/**
		Check whether two Points coincide with each other.
		@irPoint Specifies another Point.
		@return Return true if the coordinates of current Point is not the same as that of irPoint.
		Otherwise, return false.
		*/
		bool operator != (const Point3D<T>& irPoint) const;

		/**
		Return the central symmetric Point of current Point.
		@return Point3D<T>(-x, -y, -z).
		*/
		Point3D<T> operator - () const;

		/**
		Computes the midPoint of two 3D Points.
		@irPoint Specifies another 3D Point.
		@return The midPoint of current Point and irPoint.
		*/
		Point3D<T> MiddleTo(const Point3D<T>& irPoint) const;

		/**
		Computes a shifted 3D point based on current point and an offset.
		@irOffset Specifies an offset based on current Point.
		@return Point3D<T>(x+irOffset.x,y+irOffset.y,z+irOffset.z).
		*/
		Point3D<T> Shifted(const Point3D<T>& irOffset) const;
	
		/**
		Swap Point3D value with another one.
		@param iorPoint Specifies another 3D point.

		 */
		void Swap(Point3D<T>& iorPoint);

		T x;	///< X-coordinate of current Point3D.
		T y;	///< Y-coordinate of current Point3D.
		T z;	///< Z-coordinate of current Point3D.
	};


	template<typename T>
	template<typename U> Point3D<T>::Point3D(const Point3D<U>& iPoint)
	{
		x = iPoint.x;
		y = iPoint.y;
		z = iPoint.z;
	}

	template<typename T>
	inline void Point3D<T>::Reset(T iX, T iY, T iZ)
	{
		x = iX;
		y = iY;
		z = iZ;
	}

	template<typename T>
	double Point3D<T>::DistanceSquared(const Point3D<T>& irPoint) const
	{
		return (x - irPoint.x) * (x - irPoint.x)
			+ (y - irPoint.y) * (y - irPoint.y)
			+ (z - irPoint.z) * (z - irPoint.z);
	}

	template<typename T>
	double Point3D<T>::Distance(const Point3D<T>& irPoint) const
	{
		return std::sqrt(DistanceSquared(irPoint));
	}

	template<typename T>
	bool Point3D<T>::operator != (const Point3D<T>& irPoint) const
	{
		return ((x != irPoint.x) || (y != irPoint.y) || (z != irPoint.z));
	}

	template<typename T>
	Point3D<T> Point3D<T>::operator - () const
	{
		return Point3D<T>(-x, -y, -z);
	}

	template<typename T>
	Point3D<T> Point3D<T>::MiddleTo(const Point3D<T> &irPoint) const
	{
		return Point3D<T>((x + irPoint.x) / 2, (y + irPoint.y) / 2, (z + irPoint.z) / 2);
	}

	template<typename T>
	Point3D<T> Point3D<T>::Shifted(const Point3D<T>& irOffset) const
	{
		return Point3D<T>(x + irOffset.x, y + irOffset.y, z + irOffset.z);
	}

	template<typename T>
	void Point3D<T>::Swap(Point3D<T>& iorPoint)
	{
		std::swap(x, iorPoint.x);
		std::swap(y, iorPoint.y);
		std::swap(z, iorPoint.z);
	}
	typedef Point3D<Int32> SPoint3D;
	typedef Point3D<float> FPoint3D;
	typedef Point3D<double> DPoint3D;
}

#endif
