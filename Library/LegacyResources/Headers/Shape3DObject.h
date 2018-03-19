//==============================================================================================
//	FILENAME	:	Shape3DObject.h
//	AUTHOR		:	Liang Liu
//	CREATION	:	2009-05-18
//	Copyright (C) MicroStrategy Incorporated 2009
//	All Rights Reserved
//==============================================================================================
#ifndef MsiChart_Shape3DObject_h
#define MsiChart_Shape3DObject_h

#include "PDCHeader/PDCvector"
#include "ShapeObject.h"
#include "Vector3D.h"

namespace MsiChart
{	
	typedef std::vector<DPoint3D> Polygon3D;

	/// This is the base class of 3D graph objects.
	class Shape3DObject: public ShapeObject
	{
	public:
		typedef MSynch::SmartPtr<Shape3DObject> Ptr;

		/** Constructor
		@param irTripleId,ipManager,ipParentObject,iHasFill Required by base class.		
		*/
		Shape3DObject(const TripleId& irTripleId, GraphObjectManager* ipManager,
			GraphCollectionObject* ipParentObject = NULL, bool iHasFill = true); 
		virtual ~Shape3DObject();

		/** This function add a new 3D polygon to the current Shape3DObject.		
		@param irPolygon Specifies a new 3D polygon.
		@param irPlaneNormal Specifies the normal direction of the input polygon.
		*/
		void AddPolygon(const Polygon3D& irPolygon, const Vector3D<double>& irPlaneNormal); 	

		// Inherit from GraphObject class.
		virtual void Draw() const;		
		virtual void DrawOutLine();
		virtual bool PointInObject(const Point2D& irPoint);
		virtual bool ObjectInRectangle(const Rect2D& irRect);		
		virtual void GenerateImageMap(ImageMap& iorImageMap);
		virtual void GetBoundingRect(Rect2D& orBoundingRect) const;
		virtual Int32 DistanceToPoint(const Point2D& irPoint);	
		
		/**
		Get the highest point (based on the y coordinate after projection) among all the vertices.
		@return Return the highest point.
		*/
		Point2D GetHighestPoint() const;

		/**
		Check whether current Shape3DObject is valid or not.
		*/
		bool IsValid() const;

	protected:
		// Inherit from GraphObject class.
		virtual void GetClockwisePolygon(std::vector<Point2D>& orPolygon) const;

		// Inherit from ShapeObject class.
		virtual void FillBevel() const;
		virtual void AddCompensativeLayer() const;

		//-----------Data member--------------------------
		//std::vector<Polygon3D> m3DPolygons;	///< Represents a vector of 3D polygons. 		
		std::vector<Polygon2D> m2DPolygonList;	///< Maintains a vector of projected polygons.
		std::vector<float> mLightFactorList;	///< Maintains the light factors for the projected polygons.
		mutable Int32 mPolygonIndex;			///< Indicates an index in m2DPolygonList. This index is useful for GetBoundingRect() and GetClockwisePolygon().
		bool mIsValid;							///< Indicates whether the current Shape3DObject has a valid location.
	};	

	inline bool Shape3DObject::IsValid() const
	{
		return mIsValid;
	}
}
#endif
