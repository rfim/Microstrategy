//==============================================================================================
//	FILENAME	:	NeedleObject.h
//	AUTHOR		:	Xin Liu
//	CREATION	:	3/05/09
//	Copyright (C) MicroStrategy, Inc. 2009
//==============================================================================================

#ifndef MSICHART_NEEDLEOBJECT_H
#define	MSICHART_NEEDLEOBJECT_H

#include "ShapeObject.h"

namespace MsiChart
{
	/**
	This class delegates needle objects only used in Gauge chart.
	*/
	class NeedleObject: public ShapeObject
	{
	public:
		typedef MSynch::SmartPtr<NeedleObject> Ptr;

		/**
		Constructor
		@param irRect The rectangle area.
		@param iDepth The depth value.
		@param iAngle The depth direction.
		*/
		NeedleObject(const TripleId& irTripleId, 					
						GraphObjectManager* ipManager,		// manager who create the object 				
						const Point2D& irBasePoint, Int32 iSize, float iAngle,
						EnumDSSGraphGaugeNeedleStyle iNeedleStyle = DssGraphGaugeNeedleStyleSimple,
						GraphCollectionObject* ipParentObject = NULL);

		virtual ~NeedleObject();
		
		//virtual functions from GraphObject
		virtual void Draw() const;
		virtual bool ObjectInRectangle(const Rect2D& irRect);
		virtual Point2D GetDataPoint();
	//	virtual void DrawOutLine();
	//	virtual bool PointInObject(const Point2D& irPoint);
	//	virtual void GenerateImageMap(ImageMap& iorImageMap);


		//pure virtual function from ShapeObject
		virtual void GetBoundingRect(Rect2D& orBoundingRect) const; 		
	
	protected:
		virtual void GetClockwisePolygon(std::vector<Point2D>& orPolygon) const; 

	private:
		/**
		This function calculates the polygon that defines the needle object.
		*/
		void hGetClockwisePolygon();

		/**
		This function gets a list of points that defines a needle style in local coordinate.
		@param opPointNumber The number of points.
		@param opPoints Ths list of points.
		*/
		void hGetNeedleStyle(Int32* opPointNumber, const Int32** opPoints) const;

		/**
		This fucntion gets the local needle style index according to the needle style defined in CDSSGraphStype.idl
		*/
		unsigned char hConvertToLocalNeedleStyleIndex(EnumDSSGraphGaugeNeedleStyle iNeedleStyle) const;
		
		
		//data members
		EnumDSSGraphGaugeNeedleStyle mNeedleStyle;	///< The needle style.
		Point2D	mBasePoint;				///< The base point the needle.
		Int32	mSize;					///< The needle size.
		float	mAngle;					///< The angle of the needle.
		std::vector<Point2D> mPolygon;	///< The polygon that defines the needle object.
		std::vector<Point<float> > mFloatPolygon;	///< The polygon that defines the needle object with float cooridates.
	};

}

#endif