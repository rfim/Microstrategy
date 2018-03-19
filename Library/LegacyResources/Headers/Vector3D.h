//==============================================================================================
//	FILENAME	:	Vector3D.h
//	AUTHOR		:	Liang Liu
//	CREATION	:	2009-05-18
//	Copyright (C) MicroStrategy Incorporated 2009
//	All Rights Reserved
//==============================================================================================

#ifndef MsiChart_Vector3D_h
#define MsiChart_Vector3D_h

#include "Point3D.h"
#include "PDCHeader/PDCcmath"

namespace MsiChart
{
	/**
	This is a template class for 3D vector.
	*/
	template <class T = float>
	class Vector3D
	{	
	public:		
		/** 
		Constructs from two double values.
		@param iX X component of the vector.
		@param iY Y component of the vector.
		@param iZ Z component of the vector.
		*/
		Vector3D(double iX = T(), double iY = T(), double iZ = T()) : x(static_cast<T>(iX)), y(static_cast<T>(iY)), z(static_cast<T>(iZ)){}
		/** 
		Constructs from a Point3D object.
		@param irPoint A 3D vector from Origin to irPoint will be constructed.
		*/
		Vector3D(const Point3D<T>& irPoint) : x(irPoint.x), y(irPoint.y), z(irPoint.z){} 
		/** 
		Constructs from two Point3D objects.
		A 3D vector from irStart to irEnd will be constructed.
		@param irStart Specifies the start point.
		@param irEnd Specifies the end point.
		*/
		template <typename U>
		Vector3D(const Point3D<U>& irStart, const Point3D<U>& irEnd) 
			: x(static_cast<T>(irEnd.x - irStart.x)), 
			  y(static_cast<T>(irEnd.y - irStart.y)),
			  z(static_cast<T>(irEnd.z - irStart.z)){} 		

		/** 
		Get the square of length of a vector
		@return Return the square of length of current vector, namely x*x+y*y+z*z.
		*/
		double LengthSquared() const;
		/**
		Get the length of a vector
		@return Return the length of current vector, namely (x*x+y*y+z*z)^(1/2).
		*/
		double Length() const;
		/**
		Compute the included angle of two vectors.
		@param irVector Specifies another vector.
		@return The included angle between current vector and irVector
		*/
		double IncludedAngle(const Vector3D<T>& irVector) const;
		/**
		Compute the inner product of two vectors
		@param irVector Specifies another vector.
		@return The inner product.
		*/
		double InnerProduct(const Vector3D<T>& irVector) const;
		/**
		Compute the normalized inner product of two vectors
		@param irVector Specifies another vector.
		@return The inner product.
		*/
		double NormalizedInnerProduct(const Vector3D<T>& irVector) const;
		/**
		Compute the cross product of two vectors
		@param irVector Specifies another vector.
		@param orCrossProduct Output the cross product.
		*/
		void CrossProduct(const Vector3D<T>& irVector, Vector3D<T>& orCrossProduct) const;	

		/**
		Compute the cross product of two vectors
		@param irVector Specifies another vector.
		@return The cross product.
		*/
		Vector3D<T> CrossProduct(const Vector3D<T>& irVector) const;	

		/// Normalize the current vector.
		void Normalize();

		/**
		Swap Vector3D value with another one.
		@param iorVector Specifies another 3D vector.
		*/
		void Swap(Vector3D<T>& iorVector);
	
		/**
		Get the negative vector of current vector.
		@return Vector3D<T>(-x, -y, -z).
		*/
		Vector3D<T> operator - () const;
		/**
		Get the difference between two vectors.
		@param irVector Specifies another vector.
		@return Return Vector3D<T>(x-irVector.x, y-irVector.y, z-irVector.z).
		*/
		Vector3D<T> operator - (const Vector3D<T>& irVector) const;
		/**
		Get the sum between two vectors.
		@param irVector Specifies another vector.
		@return Return Vector3D<T>(x+irVector.x, y+irVector.y, z+irVector.z).
		*/
		Vector3D<T> operator + (const Vector3D<T>& irVector) const;		

		/**
		Scale current vector.
		@param iFactor Specifies the scaling factor.
		@return Return the scaled vector.
		*/
		Vector3D<T> MultiplyBy(double iFactor) const;	
	
	public:
		T x;	///< X component of current vector.
		T y;	///< Y component of current vector.
		T z;	///< Z component of current vector.
	};	

	//get the square length of a vector
	template <class T> 
	inline double Vector3D<T>::LengthSquared() const
	{
		return x * x + y * y + z * z;
	}	
	
	//A . B
	template <class T> 
	inline double Vector3D<T>::InnerProduct(const Vector3D<T>& irVector) const
	{
		return (x * irVector.x + y * irVector.y + z * irVector.z);
	}

	//A . B / (||A|| * ||B||)
	template <class T> 
	inline double Vector3D<T>::NormalizedInnerProduct(const Vector3D<T>& irVector) const
	{
		return InnerProduct(irVector) / (std::sqrt(LengthSquared() * irVector.LengthSquared()));
	}
	
	//A X B
	template <class T> 
	inline void Vector3D<T>::CrossProduct(const Vector3D<T>& irVector, Vector3D<T>& orCrossProduct) const
	{
		orCrossProduct.x = y * irVector.z - z * irVector.y;
		orCrossProduct.y = z * irVector.x - x * irVector.z;
		orCrossProduct.z = x * irVector.y - y * irVector.x;
	}

	//A X B
	template <class T> 
	inline Vector3D<T> Vector3D<T>::CrossProduct(const Vector3D<T>& irVector) const
	{
		return Vector3D<T>(y * irVector.z - z * irVector.y, z * irVector.x - x * irVector.z, x * irVector.y - y * irVector.x);
	}

	template <class T> 
	double Vector3D<T>::IncludedAngle(const Vector3D<T>& irVector) const
	{
		double lTemp = InnerProduct(irVector) / (std::sqrt(LengthSquared() * irVector.LengthSquared()));
		lTemp = (lTemp > 1.0f) ? 1.0f : lTemp;	
		lTemp = (lTemp < -1.0f) ? -1.0f : lTemp;	
		return std::acos(lTemp);
	}

	template <class T> 
	double Vector3D<T>::Length() const
	{
		return std::sqrt(LengthSquared());
	}

	template <class T> 
	inline void Vector3D<T>::Normalize()
	{
		double lLength = Length();
		x = x / lLength;
		y = y / lLength;
		z = z / lLength;
	}

	template<typename T>
	void Vector3D<T>::Swap(Vector3D<T>& iorVector)
	{
		std::swap(x, iorVector.x);
		std::swap(y, iorVector.y);
		std::swap(z, iorVector.z);

	}
	template <class T>
	inline Vector3D<T> Vector3D<T>::operator - () const
	{
		return Vector3D<T>(-x, -y, -z);
	}
	template <class T>
	inline Vector3D<T> Vector3D<T>::operator - (const Vector3D<T>& irVector) const
	{
		return Vector3D<T>(x - irVector.x, y - irVector.y, z - irVector.z);
	}
	template <class T>
	inline Vector3D<T> Vector3D<T>::operator + (const Vector3D<T>& irVector) const
	{
		return Vector3D<T>(x + irVector.x, y + irVector.y, z + irVector.z);
	}
	template <class T>
	Vector3D<T> Vector3D<T>::MultiplyBy(double iFactor) const
	{
		return Vector3D<T>(iFactor * x, iFactor * y, iFactor * z);
	}
	
	typedef Vector3D<float> FVector3D;
	typedef Vector3D<double> DVector3D;
}
#endif
