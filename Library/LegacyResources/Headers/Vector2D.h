//==============================================================================================
//	FILENAME	:	TVector2D.h
//	AUTHOR		:	Liang Liu
//	CREATION	:	2008-09-25
//	Copyright (C) MicroStrategy Incorporated 2008
//	All Rights Reserved
//==============================================================================================

#ifndef MsiChart_Vector2D_h
#define MsiChart_Vector2D_h

#include "Point2D.h"
#include "PDCHeader/PDCcmath"

namespace MsiChart
{
	/**
	This is a template class for 2D vector.
	*/
	template <class T = float>
	class TVector2D
	{	
	public:		
		/** 
		Constructs from two double values.
		@param iX X component of the vector.
		@param iY Y component of the vector.
		*/
		TVector2D(double iX, double iY) : x(static_cast<T>(iX)), y(static_cast<T>(iY)){}
		/** 
		Constructs from a Point2D object.
		@param irPoint A 2D vector from Origin to irPoint will be constructed.
		*/
		TVector2D(const Point2D& irPoint) : x(irPoint.x), y(irPoint.y){} 
		/** 
		Constructs from two Point2D objects.
		A 2D vector from irStart to irEnd will be constructed.
		@param irStart Specifies the start point.
		@param irEnd Specifies the end point.
		*/
		TVector2D(const Point2D& irStart, const Point2D& irEnd) 
			: x(static_cast<T>(irEnd.x - irStart.x)), y(static_cast<T>(irEnd.y - irStart.y)){} 		

		/** 
		Constructs from two FPoint2D objects.
		A 2D vector from irStart to irEnd will be constructed.
		@param irStart Specifies the start point.
		@param irEnd Specifies the end point.
		*/
		TVector2D(const FPoint2D& irStart, const FPoint2D& irEnd) 
			: x(static_cast<T>(irEnd.x - irStart.x)), y(static_cast<T>(irEnd.y - irStart.y)){} 	

		/**
		Set new values for current vector.
		@param iX X component of the vector.
		@param iY Y component of the vector.
		*/
		void Reset(T iX, T iY);

		/** 
		Get the square of length of a vector
		@return Return the square of length of current vector, namely x*x+y*y.
		*/
		T LengthSquared() const;
		/**
		Get the length of a vector
		@return Return the length of current vector, namely (x*x+y*y)^(1/2).
		*/
		T Length() const;
		/**
		Compute the included angle of two vectors.
		@param irVector Specifies another vector.
		@return The included angle between current vector and irVector
		*/
		double IncludedAngle(const TVector2D<T>& irVector) const;
		/**
		Compute the inner product of two vectors
		@param irVector Specifies another vector.
		@return The inner product.
		*/
		T InnerProduct(const TVector2D<T>& irVector) const;
		/**
		Compute the cross product of two vectors
		@param irVector Specifies another vector.
		@return The cross product.
		*/
		T CrossProduct(const TVector2D<T>& irVector) const;	

		/// Normalize the current vector.
		void Normalize();
	
	public:
		T x;	///< X component of current vector.
		T y;	///< Y component of current vector.
	};	
	typedef TVector2D<float> Vector2D;
	typedef TVector2D<double> DVector2D;

	template <class T>
	inline void TVector2D<T>::Reset(T iX, T iY)
	{
		x = iX;
		y = iY;
	}

	//get the square length of a vector
	template <class T> 
	inline T TVector2D<T>::LengthSquared() const
	{
		return x * x + y * y;
	}	
	
	//A . B
	template <class T> 
	inline T TVector2D<T>::InnerProduct(const TVector2D<T>& irVector) const
	{
		return (x * irVector.x + y * irVector.y);
	}
	
	//A X B
	template <class T> 
	inline T TVector2D<T>::CrossProduct(const TVector2D<T>& irVector) const
	{
		return (x * irVector.y - y * irVector.x);
	}

	template <class T> 
	double TVector2D<T>::IncludedAngle(const TVector2D<T>& irVector) const
	{
		double lTemp = InnerProduct(irVector) / (std::sqrt(LengthSquared() * irVector.LengthSquared()));
		lTemp = (lTemp > 1.0f) ? 1.0f : lTemp;	
		lTemp = (lTemp < -1.0f) ? -1.0f : lTemp;	
		return std::acos(lTemp);
	}

	template <class T> 
	T TVector2D<T>::Length() const
	{
		return std::sqrt(LengthSquared());
	}

	template <class T> 
	void TVector2D<T>::Normalize()
	{
		double lLength = Length();
		x = static_cast<T>(x / lLength);
		y = static_cast<T>(y / lLength);		
	}
}
#endif
